#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include <GL/glew.h>

#include "../geometry/obj_loader.h"

typedef struct
{
    GLuint vao;
    GLuint vbo;
    int vertex_count;
} RenderMesh;

int render_mesh_create_from_obj_mesh(RenderMesh *render_mesh, const ObjMesh *obj_mesh);
void render_mesh_draw(const RenderMesh *render_mesh);
void render_mesh_delete(RenderMesh *render_mesh);

#endif
