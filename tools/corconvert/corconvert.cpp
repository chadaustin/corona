#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <corona.h>
using namespace std;
using namespace corona;

FileFormat getFormat(const char* ext) {
    // get extension's format
    FileFormatDesc** formats = GetSupportedWriteFormats();
    for (size_t i = 0; formats[i]; ++i) {
        size_t extCount = formats[i]->getExtensionCount();
        for (size_t j = 0; j < extCount; ++j) {
            if (stricmp(ext, formats[i]->getExtension(j)) == 0) {
                return formats[i]->getFormat();
            }
        }
    }

    // "No format found"
    return FF_AUTODETECT;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        puts("Usage: corconvert <infile> <outfile>");
        return EXIT_FAILURE;
    }

    const char* infile  = argv[1];
    const char* outfile = argv[2];

    // open the input image
    auto_ptr<Image> image(OpenImage(infile));
    if (!image.get()) {
        printf("Error opening input image: %s\n", infile);
        return EXIT_FAILURE;
    }

    // get output filename's extension
    const char* outext = strrchr(outfile, '/');
    if (!outext) {
        outext = outfile;
    }
    outext = strrchr(outext, '.');
    if (!outext) {
        printf("Could not determine outfile's extension: %s\n", outfile);
        return EXIT_FAILURE;
    }
    ++outext;

    FileFormat format = getFormat(outext);
    if (format == FF_AUTODETECT) {
        printf("Format matching extension %s cannot be written by Corona\n", outext);
        return EXIT_FAILURE;
    }

    // save image to new format
    if (!SaveImage(outfile, format, image.get())) {
        printf("Error saving output image: %s\n", outfile);
        return EXIT_FAILURE;
    }
}
