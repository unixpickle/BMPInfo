#include <stdio.h>
#include "bmp.h"

int main (int argc, char * argv[]) {
	// create a bitmap (64x64)
	ANBitmapFileRef bmp = ANBitmapFileCreate(64, 64);
	// create a blue pixel
	ANPixel pixel = ANPixelWithComponents(0, 0, 255, 255);
	// set it at the point 63x63 (furthermost corner)
	ANBitmapFileSetPixel(bmp, 63, 63, pixel);
	// write the bitmap to a file
	ANBitmapFileWriteToFile(bmp, "/tmp/test.bmp");
	// free the bitmap
	ANBitmapFileRelease(bmp);
	bmp = NULL;
	
	bmp = ANBitmapFileCreateWithFile ("/tmp/test.bmp");
	ANBitmapFileWriteToFile(bmp, "/cygdrive/c/Users/alex/Desktop/foo.bmp");
	ANBitmapFileRelease(bmp);
	bmp = NULL;
	
	return 0;
}
