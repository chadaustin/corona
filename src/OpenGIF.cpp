extern "C" {
  #include <gif_lib.h>
}
#include "Debug.h"
#include "Open.h"
#include "SimpleImage.h"
#include "Utility.h"


namespace corona {

  int InputFunc(GifFileType* giffile, GifByteType* bytes, int size) {
    File* file = (File*)giffile->UserData;
    return file->read(bytes, size);
  }

  Image* OpenGIF(File* file) {
    COR_GUARD("OpenGIF");

    // open GIF file
    GifFileType* giffile = DGifOpen(file, InputFunc);
    if (!giffile) {
      COR_LOG("DGifOpen failed");
      return 0;
    }

    // read GIF image
    if (DGifSlurp(giffile) != GIF_OK) {
      COR_LOG("DGifSlurp failed");
      DGifCloseFile(giffile);
      return 0;
    }

    // validate attributes
    if (giffile->SColorResolution != 8 ||
        giffile->SColorMap == 0 ||
        giffile->SColorMap->BitsPerPixel != 8 ||
        giffile->SColorMap->ColorCount != 256 ||
        giffile->ImageCount < 1)
    {
      COR_LOG("incorrect giffile attributes");
      DGifCloseFile(giffile);
      return 0;
    }

    const int width = giffile->SWidth;
    const int height = giffile->SHeight;
    auto_array<byte> image(new byte[width * height]);
    auto_array<RGB> palette(new RGB[256]);

    memcpy(palette, giffile->SColorMap->Colors, 256 * 3);
    memcpy(image, giffile->SavedImages[0].RasterBits, width * height);

    DGifCloseFile(giffile);
    return new SimpleImage(width, height, PF_I8, image.release(),
                           (byte*)palette.release(), 256, PF_R8G8B8);
  }

}
