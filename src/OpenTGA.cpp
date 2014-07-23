#include <algorithm>
#include <string.h>
#include "Debug.h"
#include "Open.h"
#include "SimpleImage.h"
#include "Utility.h"


namespace corona {

#define RLEBUFSIZ	512			// size of largest possible RLE packet 

   // (this was fun to write.)
   // data       - pointer to buffer to recieve uncompressed data.
   // bufferSize - buffer size in bytes
   // bpp        - bytes per pixel (8, 16, 24, 32)
   // fp         - file pointer, must be an opened file, ::use fseek to position at valid start of RLE row.
   int ReadRLERow( unsigned char* data, const int& bufferSize, const int& bpp, File* file )
   {
      unsigned int   value;
      unsigned char   byte;   // for reading byte by byte.
      int      i;
      unsigned char*   q;
      int      n( bufferSize );
      static unsigned char rleBuf[RLEBUFSIZ];

      while ( n > 0 )
      {
         if (file->read(&byte, 1) != 1) return -1;
         value = byte;

         if ( value & 0x80 )
         {
            value &= 0x7f;
            value++;
            n -= value * bpp;
            if ( n < 0 )
               return( -1 );
            if (file->read( rleBuf, bpp ) != bpp) return( -1 );
            while ( value > 0 )
            {
               *data++ = rleBuf[0];
               if ( bpp > 1 ) *data++ = rleBuf[1];
               if ( bpp > 2 ) *data++ = rleBuf[2];
               if ( bpp > 3 ) *data++ = rleBuf[3];
               value--;
            }
         }
         else
         {
            value++;
            n -= value * bpp;
            if ( n < 0 )
               return( -1 );

            // Maximum for value is 128 so as long as RLEBUFSIZ
            // is at least 512, and bpp is not greater than 4
            // we can read in the entire raw packet with one operation.
            if (file->read( rleBuf, bpp * value ) != (bpp * value) )
               return( -1 );
            for ( i = 0, q = rleBuf; i < (value * bpp); ++i ) *data++ = *q++;
         }
      }
      return( 0 );
   }

  Image* OpenTGA(File* file) {
    COR_GUARD("OpenTGA");

    // read header
    byte header[18];
    if (file->read(header, 18) != 18) {
      return 0;
    }

    // decode header
    int id_length        = header[0];
    int cm_type          = header[1];
    int image_type       = header[2];
    //int cm_first         = read16_le(header + 3);
    int cm_length        = read16_le(header + 5);
    int cm_entry_size    = header[7];  // in bits
    //int x_origin         = read16_le(header + 8);
    //int y_origin         = read16_le(header + 10);
    int width            = read16_le(header + 12);
    int height           = read16_le(header + 14);
    int pixel_depth      = header[16];
    int image_descriptor = header[17];

    bool mirrored = (image_descriptor & (1 << 4)) != 0;  // left-to-right?
    bool flipped  = (image_descriptor & (1 << 5)) == 0;  // bottom-to-top?

    /*
     * image types
     * 0  = no image data
     * 1  = uncompressed, color-mapped
     * 2  = uncompressed, true-color
     * 3  = uncompressed, black and white
     * 9  = RLE, color-mapped
     * 10 = RLE, true-color
     * 11 = RLE, black and white
     */

    // make sure we support the image
    if ((image_type != 2 && image_type != 10) ||
        (pixel_depth != 24 && pixel_depth != 32)) {
       COR_LOG( "wrong image_type (2 or 10) or pixel_depth (24 or 32)\n" );
       return 0;
    }

    // skip image id
    byte unused[255];
    if (file->read(unused, id_length) != id_length) {
      return 0;
    }

    // skip color map
    if (cm_type != 0) {
      // allocate color map
      int cm_entry_bytes = (cm_entry_size + 7) / 8;
      int cm_size = cm_entry_bytes * cm_length;
      auto_array<byte> color_map(new byte[cm_size]);
      if (file->read(color_map, cm_size) != cm_size) {
        return 0;
      }
    }

    // read image data
    PixelFormat format;
    auto_array<byte> pixels;
    if (pixel_depth == 24) {

      COR_LOG("24-bit image");

      int bytesPerPixel = (pixel_depth/8);
      format = PF_B8G8R8;
      int image_size = width * height * bytesPerPixel;
      pixels = new byte[image_size];

      if (image_type == 10)
      {
         int count = width * bytesPerPixel;
         for (int i = 0; i < height; ++i)
         {
            // expand one scanline of RLE data from the file...
            if (ReadRLERow( &pixels[i*width*bytesPerPixel], count, bytesPerPixel, file ) < 0)
            {
               COR_LOG( "Error decompressing RLE data.\n" );
               return 0;
            }
         }
      }
      else if (image_type == 2)
      {
         if (file->read(pixels, image_size) != image_size) {
         return 0;
         }
      }

    } else if (pixel_depth == 32) {

      COR_LOG("32-bit image");

      int bytesPerPixel = (pixel_depth/8);
      format = PF_B8G8R8A8;
      int image_size = width * height * bytesPerPixel;
      pixels = new byte[image_size];

      if (image_type == 10)
      {
         int count = width * bytesPerPixel;
         for (int i = 0; i < height; ++i)
         {
            // expand one scanline of RLE data from the file...
            if (ReadRLERow( &pixels[i*width*bytesPerPixel], count, bytesPerPixel, file ) < 0)
            {
               COR_LOG( "Error decompressing RLE data.\n" );
               return 0;
            }
         }
      }
      else if (image_type == 2)
      {
         if (file->read(pixels, image_size) != image_size) {
         return 0;
         }
      }

    } else {
      return 0;
    }

    // reverse each row
    if (mirrored) {
      COR_LOG("Image is mirrored");

      const int bpp = pixel_depth / 8;  // bytes per pixel
      for (int y = 0; y < height; ++y) {

	// points to the first pixel of the row
	byte* start = pixels.get() + y * width * bpp;
	// points to the last pixel of the row
	byte* end   = start + (width - 1) * bpp;

	while (start < end) {
	  for (int b = 0; b < bpp; ++b) {
	    std::swap(start[b], end[b]);
	  }
	  start += bpp;
	  end   -= bpp;
	}
      }
    }

    // reverse rows as a whole
    if (flipped) {
      COR_LOG("Image is flipped");

      const int bpp = pixel_depth / 8;  // bytes per pixel
      const int row_size = width * bpp;
      auto_array<byte> temp(new byte[row_size]);  // for the swap

      // points to the beginning of the first row
      byte* start = pixels.get();

      // points to the beginning of the last row
      byte* end   = start + (height - 1) * width * bpp;

      while (start < end) {
	memcpy(temp.get(), start,      row_size);
	memcpy(start,      end,        row_size);
	memcpy(end,        temp.get(), row_size);

	start += row_size;
	end   -= row_size;
      }
    }

    return new SimpleImage(width, height, format, pixels.release());
  }

}
