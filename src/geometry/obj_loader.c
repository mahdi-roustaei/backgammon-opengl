#include "obj_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    float x;
    float y;
    float z;
} Vec3;

typedef struct
{
    float u;
    float v;
} Vec2;

int obj_count_elements(const char *filename, ObjInfo *info)
{
    FILE *file;
    char line[512];

    if (filename == NULL || info == NULL)
    {
        return 0;
    }

    info->vertex_count = 0;
    info->texcoord_count = 0;
    info->normal_count = 0;
    info->face_count = 0;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "v ", 2) == 0)
        {
            info->vertex_count++;
        }
        else if (strncmp(line, "vt ", 3) == 0)
        {
            info->texcoord_count++;
        }
        else if (strncmp(line, "vn ", 3) == 0)
        {
            info->normal_count++;
        }
        else if (strncmp(line, "f ", 2) == 0)
        {
            info->face_count++;
        }
    }

    fclose(file);
    return 1;
}

int obj_load_mesh(const char *filename, ObjMesh *mesh)
{
    FILE *file;
    ObjInfo info;
    char line[512];

    Vec3 *positions;
    Vec2 *texcoords;
    Vec3 *normals;

    int position_index = 0;
    int texcoord_index = 0;
    int normal_index = 0;
    int output_index = 0;

    if (filename == NULL || mesh == NULL)
    {
        return 0;
    }

    mesh->vertex_count = 0;
    mesh->data = NULL;

    if (!obj_count_elements(filename, &info))
    {
        return 0;
    }

    if (info.vertex_count <= 0 || info.texcoord_count <= 0 || info.normal_count <= 0 || info.face_count <= 0)
    {
        return 0;
    }

    positions = (Vec3 *)malloc(sizeof(Vec3) * (size_t)info.vertex_count);
    texcoords = (Vec2 *)malloc(sizeof(Vec2) * (size_t)info.texcoord_count);
    normals = (Vec3 *)malloc(sizeof(Vec3) * (size_t)info.normal_count);

    mesh->vertex_count = info.face_count * 3;
    mesh->data = (float *)malloc(sizeof(float) * (size_t)mesh->vertex_count * 8);

    if (positions == NULL || texcoords == NULL || normals == NULL || mesh->data == NULL)
    {
        free(positions);
        free(texcoords);
        free(normals);
        obj_free_mesh(mesh);
        return 0;
    }

    file = fopen(filename, "r");
    if (file == NULL)
    {
        free(positions);
        free(texcoords);
        free(normals);
        obj_free_mesh(mesh);
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "v ", 2) == 0)
        {
            sscanf(line, "v %f %f %f",
                   &positions[position_index].x,
                   &positions[position_index].y,
                   &positions[position_index].z);
            position_index++;
        }
        else if (strncmp(line, "vt ", 3) == 0)
        {
            sscanf(line, "vt %f %f",
                   &texcoords[texcoord_index].u,
                   &texcoords[texcoord_index].v);
            texcoord_index++;
        }
        else if (strncmp(line, "vn ", 3) == 0)
        {
            sscanf(line, "vn %f %f %f",
                   &normals[normal_index].x,
                   &normals[normal_index].y,
                   &normals[normal_index].z);
            normal_index++;
        }
        else if (strncmp(line, "f ", 2) == 0)
        {
            int vi[3];
            int ti[3];
            int ni[3];
            int i;

            if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                       &vi[0], &ti[0], &ni[0],
                       &vi[1], &ti[1], &ni[1],
                       &vi[2], &ti[2], &ni[2]) != 9)
            {
                fclose(file);
                free(positions);
                free(texcoords);
                free(normals);
                obj_free_mesh(mesh);
                return 0;
            }

            for (i = 0; i < 3; i++)
            {
                Vec3 p = positions[vi[i] - 1];
                Vec2 t = texcoords[ti[i] - 1];
                Vec3 n = normals[ni[i] - 1];

                mesh->data[output_index++] = p.x;
                mesh->data[output_index++] = p.y;
                mesh->data[output_index++] = p.z;

                mesh->data[output_index++] = t.u;
                mesh->data[output_index++] = t.v;

                mesh->data[output_index++] = n.x;
                mesh->data[output_index++] = n.y;
                mesh->data[output_index++] = n.z;
            }
        }
    }

    fclose(file);
    free(positions);
    free(texcoords);
    free(normals);

    return 1;
}

void obj_free_mesh(ObjMesh *mesh)
{
    if (mesh == NULL)
    {
        return;
    }

    free(mesh->data);
    mesh->data = NULL;
    mesh->vertex_count = 0;
}
