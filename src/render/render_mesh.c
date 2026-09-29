#include "render_mesh.h"

#include <stddef.h>

int render_mesh_create_from_obj_mesh(RenderMesh *render_mesh, const ObjMesh *obj_mesh)
{
    if (render_mesh == NULL || obj_mesh == NULL || obj_mesh->data == NULL || obj_mesh->vertex_count <= 0)
    {
        return 0;
    }

    render_mesh->vao = 0;
    render_mesh->vbo = 0;
    render_mesh->vertex_count = obj_mesh->vertex_count;

    glGenVertexArrays(1, &render_mesh->vao);
    glGenBuffers(1, &render_mesh->vbo);

    if (render_mesh->vao == 0 || render_mesh->vbo == 0)
    {
        render_mesh_delete(render_mesh);
        return 0;
    }

    glBindVertexArray(render_mesh->vao);

    glBindBuffer(GL_ARRAY_BUFFER, render_mesh->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * (size_t)obj_mesh->vertex_count * 8,
        obj_mesh->data,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void *)(0 * sizeof(float))
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void *)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void *)(5 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return 1;
}

void render_mesh_draw(const RenderMesh *render_mesh)
{
    if (render_mesh == NULL || render_mesh->vao == 0 || render_mesh->vertex_count <= 0)
    {
        return;
    }

    glBindVertexArray(render_mesh->vao);
    glDrawArrays(GL_TRIANGLES, 0, render_mesh->vertex_count);
    glBindVertexArray(0);
}

void render_mesh_delete(RenderMesh *render_mesh)
{
    if (render_mesh == NULL)
    {
        return;
    }

    if (render_mesh->vbo != 0)
    {
        glDeleteBuffers(1, &render_mesh->vbo);
    }

    if (render_mesh->vao != 0)
    {
        glDeleteVertexArrays(1, &render_mesh->vao);
    }

    render_mesh->vao = 0;
    render_mesh->vbo = 0;
    render_mesh->vertex_count = 0;
}
