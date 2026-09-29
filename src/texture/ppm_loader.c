#include "ppm_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int read_next_token(FILE *file, char *buffer, int buffer_size)
{
    int c;
    int i = 0;

    if (file == NULL || buffer == NULL || buffer_size <= 1)
    {
        return 0;
    }

    while ((c = fgetc(file)) != EOF)
    {
        if (c == '#')
        {
            while ((c = fgetc(file)) != EOF && c != '\n')
            {
            }
        }
        else if (c != ' ' && c != '\n' && c != '\r' && c != '\t')
        {
            break;
        }
    }

    if (c == EOF)
    {
        return 0;
    }

    do
    {
        if (i < buffer_size - 1)
        {
            buffer[i] = (char)c;
            i++;
        }

        c = fgetc(file);
    }
    while (c != EOF && c != ' ' && c != '\n' && c != '\r' && c != '\t');

    buffer[i] = '\0';
    return 1;
}

int ppm_load_p3(const char *filename, PpmImage *image)
{
    FILE *file;
    char token[64];
    int pixel_count;
    int i;
    int value;

    if (filename == NULL || image == NULL)
    {
        return 0;
    }

    image->width = 0;
    image->height = 0;
    image->max_value = 0;
    image->pixels = NULL;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        return 0;
    }

    if (!read_next_token(file, token, sizeof(token)))
    {
        fclose(file);
        return 0;
    }

    if (strcmp(token, "P3") != 0)
    {
        fclose(file);
        return 0;
    }

    if (!read_next_token(file, token, sizeof(token)))
    {
        fclose(file);
        return 0;
    }
    image->width = atoi(token);

    if (!read_next_token(file, token, sizeof(token)))
    {
        fclose(file);
        return 0;
    }
    image->height = atoi(token);

    if (!read_next_token(file, token, sizeof(token)))
    {
        fclose(file);
        return 0;
    }
    image->max_value = atoi(token);

    if (image->width <= 0 || image->height <= 0 || image->max_value <= 0 || image->max_value > 255)
    {
        fclose(file);
        return 0;
    }

    pixel_count = image->width * image->height * 3;
    image->pixels = (unsigned char *)malloc((size_t)pixel_count);

    if (image->pixels == NULL)
    {
        fclose(file);
        return 0;
    }

    for (i = 0; i < pixel_count; i++)
    {
        if (!read_next_token(file, token, sizeof(token)))
        {
            ppm_free(image);
            fclose(file);
            return 0;
        }

        value = atoi(token);

        if (value < 0 || value > image->max_value)
        {
            ppm_free(image);
            fclose(file);
            return 0;
        }

        image->pixels[i] = (unsigned char)value;
    }

    fclose(file);
    return 1;
}

void ppm_free(PpmImage *image)
{
    if (image == NULL)
    {
        return;
    }

    free(image->pixels);
    image->pixels = NULL;
    image->width = 0;
    image->height = 0;
    image->max_value = 0;
}
