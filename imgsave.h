#ifndef IMGSAVE_H
#define IMGSAVE_H

#include "tiffio.h"

void Imgsave(float** image, char* Filename, int w, int h, int bit, int samplesperpixel){
   TIFF *output_image;
   float min = 0, max = 0;
   char* buf;
   buf = (char *)malloc(sizeof(char) * w * h);

   // Open the TIFF file
   output_image = TIFFOpen(Filename, "w");

   // We need to set some values for basic tags before we can add any data
   TIFFSetField(output_image, TIFFTAG_IMAGEWIDTH, w);
   TIFFSetField(output_image, TIFFTAG_IMAGELENGTH, h);
   TIFFSetField(output_image, TIFFTAG_BITSPERSAMPLE, bit);
   TIFFSetField(output_image, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
   TIFFSetField(output_image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
   TIFFSetField(output_image, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
   TIFFSetField(output_image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);


   for(int x = 0; x < w; x++){
       for(int y = 0; y < h; y++)
       {
           if (max < image[x][y]) max = image[x][y];
           if (min > image[x][y]) min = image[x][y];
       }
   }

   for(int x = 0; x < w; x++)
       for(int y = 0; y < h; y++)
       {
           image[x][y] = (int)(((image[x][y]-min)/(max-min))*255);
           buf[x + y * w] = 255 - image[x][y];
       }

   // Write the information to the file
   TIFFWriteEncodedStrip(output_image, 0, buf, w * h);

   // Close the file
   TIFFClose(output_image);
   free(buf);
}

#endif // IMGSAVE_H
