#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

typedef struct
{
    int vertex_count;
    int texcoord_count;
    int normal_count;
    int face_count;
} ObjInfo;

typedef struct
{
    int vertex_count;
    float *data;
} ObjMesh;

int obj_count_elements(const char *filename, ObjInfo *info);
int obj_load_mesh(const char *filename, ObjMesh *mesh);
void obj_free_mesh(ObjMesh *mesh);

#endif
