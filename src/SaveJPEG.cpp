#include <stdio.h>  // needed by jpeglib.h
#include <memory>
#include <setjmp.h>
extern "C" {  // stupid JPEG library
  #include <jpeglib.h>
  #include <jerror.h>
}
#include "Debug.h"
#include "Save.h"
#include "Utility.h"

namespace corona {

  static const int JPEG_BUFFER_SIZE = 4096;
  void    JPEG_init_destination(j_compress_ptr cinfo);
  boolean JPEG_empty_output_buffer(j_compress_ptr cinfo);
  void    JPEG_term_destination(j_compress_ptr cinfo);

  // From OpenJPEG.cpp
  extern void JPEG_error_exit(j_common_ptr cinfo);

  struct iwrite_mgr {
    jpeg_destination_mgr pub;
    JOCTET*              buffer;
    boolean              bah;
  };

  struct InternalStruct {
    struct {
      jpeg_error_mgr mgr;
      jmp_buf setjmp_buffer;
    } error_mgr;

    File* file;
    byte buffer[JPEG_BUFFER_SIZE];
  };

  bool SaveJPEG(File* file, Image* source) {
    COR_GUARD("SaveJPG");

    std::auto_ptr<Image> image(CloneImage(source, PF_R8G8B8));
    if (!image.get()) {
      return false;
    }
    const int width  = image->getWidth();

    jpeg_compress_struct JpegInfo;
    JSAMPROW row_pointer[1];
    int Type = 0;

    // Now we can initialize the JPEG compression object.
    jpeg_create_compress(&JpegInfo);
    InternalStruct is;
    is.file = file;
    JpegInfo.client_data = &is;
		
    JpegInfo.err = jpeg_std_error(&is.error_mgr.mgr);
    is.error_mgr.mgr.error_exit = JPEG_error_exit;

    if (setjmp(is.error_mgr.setjmp_buffer)) {
      jpeg_destroy_compress(&JpegInfo);
      return false;
    }

    JpegInfo.dest = (struct jpeg_destination_mgr *)
    (JpegInfo.mem->alloc_small) ((j_common_ptr) &JpegInfo, JPOOL_PERMANENT,
                                 sizeof(struct jpeg_destination_mgr));
    {
      iwrite_mgr* dest = (iwrite_mgr*)JpegInfo.dest;
      dest->pub.init_destination = JPEG_init_destination;
      dest->pub.empty_output_buffer = JPEG_empty_output_buffer;
      dest->pub.term_destination = JPEG_term_destination;
    }	

    // image width and height, in pixels
    JpegInfo.image_width  = image->getWidth();
    JpegInfo.image_height = image->getHeight();
    JpegInfo.input_components = 3;  // # of color components per pixel
    JpegInfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&JpegInfo);
    Type = Type;
    JpegInfo.write_JFIF_header = TRUE;

    jpeg_set_quality(&JpegInfo, 85, TRUE);		// TODO: find way of setting quality

    jpeg_start_compress(&JpegInfo, TRUE);

    unsigned char * data = (unsigned char*)image->getPixels();
    while (JpegInfo.next_scanline < JpegInfo.image_height) {
      // jpeg_write_scanlines expects an array of pointers to scanlines.
      // Here the array is only one element long, but you could pass
      // more than one scanline at a time if that's more convenient.
      row_pointer[0] = &data[JpegInfo.next_scanline * (width * 3)];
      jpeg_write_scanlines(&JpegInfo, row_pointer, 1);
    }

    // Step 6: Finish compression
    jpeg_finish_compress(&JpegInfo);

    // Step 7: release JPEG compression object

    // This is an important step since it will release a good deal of memory.
    jpeg_destroy_compress(&JpegInfo);

    return true;
  }

  void JPEG_init_destination(j_compress_ptr cinfo) {
    iwrite_mgr* dest = (iwrite_mgr*) cinfo->dest;

    /* Allocate the output buffer --- it will be released when done with image */
    dest->buffer = (JOCTET *)
    (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
                                JPEG_BUFFER_SIZE * sizeof(JOCTET));

    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = JPEG_BUFFER_SIZE;
  }

  boolean JPEG_empty_output_buffer(j_compress_ptr cinfo) {
    iwrite_mgr* dest = (iwrite_mgr*) cinfo->dest;

    InternalStruct *is = (InternalStruct*)cinfo->client_data;
    if(is->file->write(dest->buffer,JPEG_BUFFER_SIZE) != JPEG_BUFFER_SIZE)
      ERREXIT(cinfo, JERR_FILE_WRITE);

    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = JPEG_BUFFER_SIZE;

    return TRUE;
  }

  void JPEG_term_destination(j_compress_ptr cinfo) {
    iwrite_mgr* dest = (iwrite_mgr*) cinfo->dest;
    size_t datacount = JPEG_BUFFER_SIZE - dest->pub.free_in_buffer;
    InternalStruct *is = (InternalStruct*)cinfo->client_data;

    /* Write any data remaining in the buffer */
    if (datacount > 0) {
      if(is->file->write(dest->buffer,datacount) != int(datacount))
        ERREXIT(cinfo, JERR_FILE_WRITE);
    }
  }
}
