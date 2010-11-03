#include "bmp.h"


// creation

ANBitmapFileRef ANBitmapFileCreate (int width, int height) {
	ANBitmapFileRef bmp = (ANBitmapFileRef)malloc(sizeof(bitmapFile));
	bmp->width = width;
	bmp->height = height;
	// calculate padding
	bmp->linepadding = (4 - (4 % (width * 3)));
	// set some other flags
	memcpy(bmp->title, "BM", 2);
	bmp->headersize = 40;
	bmp->bmpoffset = 54;
	bmp->zerobytes = 0;
	bmp->bmpdatasize = ((bmp->width * 3) + bmp->linepadding) * height;
	bmp->filesize = bmp->bmpdatasize + bmp->headersize;
	bmp->nplanes = 1;
	bmp->bitsperpixel = 24;
	bmp->compression = 0;
	bmp->important = 0;
	bmp->palettesize = 0;
	bmp->pixelsmeter1 = 2835;
	bmp->pixelsmeter2 = 2835;
	bmp->retaincount = 1;
	bmp->bitmapData = (char *)malloc(bmp->bmpdatasize);
	memset(bmp->bitmapData, 0, bmp->bmpdatasize);
	return bmp;
}
ANBitmapFileRef ANBitmapFileCreateCopy (ANBitmapFileRef bmp) {
	ANBitmapFileRef nbmp = (ANBitmapFileRef)malloc(sizeof(bitmapFile));
	memcpy(nbmp, bmp, sizeof(bitmapFile));
	nbmp->bitmapData = (char *)malloc(nbmp->bmpdatasize);
	return nbmp;
}
ANBitmapFileRef ANBitmapFileCreateWithFile (char * file) {
	// load the file, damn this will take a while
	FILE * fp = fopen(file, "r");
	ANBitmapFileRef bmp;
	int width = 0, height = 0;
	int filesize;
	if (!fp) {
		return NULL;
	}

	// read the size and stuff, then create the file
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	
	if (filesize < 56) {
		return NULL;
	}
	
	// seek to the size spot
	fseek(fp, 0x12, SEEK_SET);
	fread(&width, 4, 1, fp);
	fread(&height, 4, 1, fp);
	
	// create the actual file
	bmp = ANBitmapFileCreate(width, height);
	
	
	fseek(fp, 0x2, SEEK_SET);
	fread(&filesize, 4, 1, fp);
	if (filesize != bmp->filesize) {
		return NULL;
	}
	
	fseek(fp, 0x22, SEEK_SET);
	fread(&filesize, 4, 1, fp);
	if (filesize != bmp->bmpdatasize) {
		return NULL;
	}
	
	// read the data here
	fseek(fp, 0x36, SEEK_SET);
	fread(bmp->bitmapData, bmp->bmpdatasize, 1, fp);
	
	fclose(fp);
	return bmp;
}
void ANBitmapFileGetSize (ANBitmapFileRef bmp, int * width, int * height) {
	width[0] = bmp->width;
	height[0] = bmp->height;
}

// pixels

void ANBitmapFileSetPixel (ANBitmapFileRef bmp, int x, int y, ANPixel pixel) {
	int index = ((bmp->height - (y + 1)) * ((bmp->width * 3) + bmp->linepadding));
	index += x * 3;
	char * c = &bmp->bitmapData[index];
	int r, g, b, a;
	ANPixelGetComponents(pixel, &r, &g, &b, &a);
	c[0] = b;
	c[1] = g;
	c[2] = r;
}
void ANPixelGetComponents (ANPixel pixel, int * r, int * g, int * b, int * a) {
	char * buff = (char *)(&pixel);
	r[0] = buff[0];
	g[0] = buff[1];
	b[0] = buff[2];
	a[0] = buff[3];
}
ANPixel ANBitmapFileGetPixel (ANBitmapFileRef bmp, int x, int y) {
	
}
ANPixel ANPixelWithComponents (int red, int green, int blue, int alpha) {
	ANPixel pixel = 0;
	char * buff = (char *)(&pixel);
	buff[0] = red;	
	buff[1] = green;
	buff[2] = blue;
	buff[3] = alpha;
	return pixel;
}

// saving

void ANBitmapFileWriteToFile (ANBitmapFileRef bmp, char * path) {
	if (!bmp) return;
	FILE * fp = fopen(path, "w");
	fwrite(bmp->title, 2, 1, fp);
	fwrite(&(bmp->filesize), 4, 1, fp);
	fwrite(&(bmp->zerobytes), 4, 1, fp);
	fwrite(&(bmp->bmpoffset), 4, 1, fp);
	fwrite(&(bmp->headersize), 4, 1, fp);
	fwrite(&(bmp->width), 4, 1, fp);
	fwrite(&(bmp->height), 4, 1, fp);
	fwrite(&(bmp->nplanes), 2, 1, fp);
	fwrite(&(bmp->bitsperpixel), 2, 1, fp);
	fwrite(&(bmp->compression), 4, 1, fp);
	fwrite(&(bmp->bmpdatasize), 4, 1, fp);
	fwrite(&(bmp->pixelsmeter1), 4, 1, fp);
	fwrite(&(bmp->pixelsmeter2), 4, 1, fp);
	fwrite(&(bmp->palettesize), 4, 1, fp);
	fwrite(&(bmp->important), 4, 1, fp);
	// DONE
	fwrite(bmp->bitmapData, bmp->bmpdatasize, 1, fp);
	fclose(fp);
}

// deletion

ANBitmapFileRef ANBitmapFileRetain (ANBitmapFileRef bmp) {
	bmp->retaincount += 1;
}
void ANBitmapFileRelease (ANBitmapFileRef bmp) {
	bmp->retaincount -= 1;
	if (bmp->retaincount == 0) {
		// free it all
		free(bmp->bitmapData);
		free(bmp);
	}
}
