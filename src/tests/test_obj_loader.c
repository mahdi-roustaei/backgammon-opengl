#include "../geometry/obj_loader.h"

#include <stdio.h>

static int test_obj_file(const char *filename)
{
    ObjInfo info;
    ObjMesh mesh;

    if (!obj_count_elements(filename, &info))
    {
        printf("%s count: Nicht OK\n", filename);
        return 0;
    }

    if (!obj_load_mesh(filename, &mesh))
    {
        printf("%s load: Nicht OK\n", filename);
        return 0;
    }

    printf("%s: OK\n", filename);
    printf("  source vertices:  %d\n", info.vertex_count);
    printf("  source texcoords: %d\n", info.texcoord_count);
    printf("  source normals:   %d\n", info.normal_count);
    printf("  source faces:     %d\n", info.face_count);
    printf("  loaded vertices:  %d\n", mesh.vertex_count);
    printf("  floats per vertex: 8\n");

    obj_free_mesh(&mesh);
    return 1;
}

int main(void)
{
    int ok = 1;

    ok = test_obj_file("assets/models/cube.obj") && ok;
    ok = test_obj_file("assets/models/floor.obj") && ok;
    ok = test_obj_file("assets/models/pyramid.obj") && ok;
    ok = test_obj_file("assets/models/wall.obj") && ok;
    ok = test_obj_file("assets/models/glass_panel.obj") && ok;

    if (ok)
    {
        printf("Alle OBJ-Dateien: OK\n");
        return 0;
    }

    printf("Mindestens eine OBJ-Datei: Nicht OK\n");
    return 1;
}
