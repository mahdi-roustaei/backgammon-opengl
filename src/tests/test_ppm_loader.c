#include "../texture/ppm_loader.h"

#include <stdio.h>

static int test_ppm_file(const char *filename)
{
    PpmImage image;

    if (!ppm_load_p3(filename, &image))
    {
        printf("%s: Nicht OK\n", filename);
        return 0;
    }

    printf("%s: OK\n", filename);
    printf("  width:     %d\n", image.width);
    printf("  height:    %d\n", image.height);
    printf("  max value: %d\n", image.max_value);

    ppm_free(&image);
    return 1;
}

int main(void)
{
    int ok = 1;

    ok = test_ppm_file("assets/textures/checker.ppm") && ok;
    ok = test_ppm_file("assets/textures/floor_grid.ppm") && ok;
    ok = test_ppm_file("assets/textures/wall_bricks.ppm") && ok;

    if (ok)
    {
        printf("Alle PPM-Dateien: OK\n");
        return 0;
    }

    printf("Mindestens eine PPM-Datei: Nicht OK\n");
    return 1;
}
