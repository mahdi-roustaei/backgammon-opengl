#ifndef PPM_LOADER_H
#define PPM_LOADER_H

typedef struct
{
    int width;
    int height;
    int max_value;
    unsigned char *pixels;
} PpmImage;

int ppm_load_p3(const char *filename, PpmImage *image);
void ppm_free(PpmImage *image);

#endif
