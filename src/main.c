#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "math/mat4.h"
#include "graphics/camera.h"
#include "render/shader_program.h"
#include "render/render_mesh.h"
#include "render/image_texture.h"
#include "render/skybox.h"
#include "geometry/obj_loader.h"

static Camera gCamera;
static float gLastMouseX = 400.0f;
static float gLastMouseY = 300.0f;
static int gFirstMouse = 1;
static int gWindowWidth = 800;
static int gWindowHeight = 600;

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    (void)window;
    gWindowWidth = width;
    gWindowHeight = height;
    glViewport(0, 0, width, height);
}

static void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    (void)window;
    if (gFirstMouse)
    {
        gLastMouseX = (float)xpos;
        gLastMouseY = (float)ypos;
        gFirstMouse = 0;
    }
    float xOffset = (float)xpos - gLastMouseX;
    float yOffset = gLastMouseY - (float)ypos;
    gLastMouseX = (float)xpos;
    gLastMouseY = (float)ypos;
    cameraProcessMouse(&gCamera, xOffset, yOffset);
}

static void processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraMoveForward(&gCamera, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraMoveBackward(&gCamera, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraMoveLeft(&gCamera, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraMoveRight(&gCamera, deltaTime);
}

static int loadMeshFromFile(const char* path, RenderMesh* outMesh)
{
    ObjMesh obj;
    if (obj_load_mesh(path, &obj) == 0)
    {
        fprintf(stderr, "OBJ konnte nicht geladen werden: %s\n", path);
        return -1;
    }
    if (render_mesh_create_from_obj_mesh(outMesh, &obj) == 0)
    {
        fprintf(stderr, "RenderMesh konnte nicht erstellt werden: %s\n", path);
        obj_free_mesh(&obj);
        return -1;
    }
    obj_free_mesh(&obj);
    return 0;
}

static void setMat4(GLuint program, const char* name, const float* value)
{
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, value);
}
static void setMat3(GLuint program, const char* name, const float* value)
{
    glUniformMatrix3fv(glGetUniformLocation(program, name), 1, GL_FALSE, value);
}
static void setVec3(GLuint program, const char* name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(program, name), x, y, z);
}
static void setFloat(GLuint program, const char* name, float v)
{
    glUniform1f(glGetUniformLocation(program, name), v);
}
static void setInt(GLuint program, const char* name, int v)
{
    glUniform1i(glGetUniformLocation(program, name), v);
}

static void buildModelMatrix(float* out, float x, float y, float z,
                              float rotY, float scale)
{
    float identity[16], translated[16], rotated[16];
    mat4Identity(identity);
    mat4Translate(translated, identity, x, y, z);
    mat4RotateY(rotated, translated, rotY);
    mat4Scale(out, rotated, scale, scale, scale);
}

static void buildModelMatrixXYZ(float* out, float x, float y, float z,
                                 float rotY, float sx, float sy, float sz)
{
    float identity[16], translated[16], rotated[16];
    mat4Identity(identity);
    mat4Translate(translated, identity, x, y, z);
    mat4RotateY(rotated, translated, rotY);
    mat4Scale(out, rotated, sx, sy, sz);
}

int main(void)
{
    if (!glfwInit())
    {
        fprintf(stderr, "GLFW konnte nicht gestartet werden\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Vollbildmodus auf dem primaeren Monitor */
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    gWindowWidth = mode->width;
    gWindowHeight = mode->height;

    GLFWwindow* window = glfwCreateWindow(gWindowWidth, gWindowHeight,
                                           "CG1 Projekt - Backgammon",
                                           monitor, NULL);
    if (!window)
    {
        fprintf(stderr, "Fenster konnte nicht erstellt werden\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "GLEW Fehler\n");
        return -1;
    }

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Shader laden */
    GLuint phongProgram = shader_create_program_from_files(
        "shaders/phong.vert", "shaders/phong.frag");
    GLuint basicProgram = shader_create_program_from_files(
        "shaders/basic_textured.vert", "shaders/basic_textured.frag");
    GLuint skyboxProgram = shader_create_program_from_files(
        "shaders/skybox.vert", "shaders/skybox.frag");

    if (phongProgram == 0 || basicProgram == 0 || skyboxProgram == 0)
    {
        fprintf(stderr, "Shader-Programme konnten nicht geladen werden\n");
        return -1;
    }

    /* Texturen */
    ImageTexture woodTex, grassTex;
    if (image_texture_load("assets/textures/wood.jpg", &woodTex) == 0) return -1;
    if (image_texture_load("assets/textures/grass.jpg", &grassTex) == 0) return -1;

    /* Skybox laden */
    Skybox skybox;
    if (skybox_load(&skybox,
                    "assets/skybox/posx.jpg", "assets/skybox/negx.jpg",
                    "assets/skybox/posy.jpg", "assets/skybox/negy.jpg",
                    "assets/skybox/posz.jpg", "assets/skybox/negz.jpg") == 0)
    {
        fprintf(stderr, "Skybox konnte nicht geladen werden\n");
        return -1;
    }

    /* Meshes */
    RenderMesh floorMesh, backgammonMesh, cubeMesh;
    if (loadMeshFromFile("assets/models/floor.obj", &floorMesh) != 0) return -1;
    if (loadMeshFromFile("assets/models/backgammon.obj", &backgammonMesh) != 0) return -1;
    if (loadMeshFromFile("assets/models/cube.obj", &cubeMesh) != 0) return -1;

    const float BOARD_SCALE = 0.006f;
    const float TABLE_SIZE = 5.0f;
    const float TABLE_TOP_Y = 1.5f;
    const float LEG_THICKNESS = 0.35f;

    cameraInit(&gCamera, 0.0f, 3.5f, 8.0f);

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float view[16], projection[16];
        cameraGetViewMatrix(&gCamera, view);
        float aspect = (float)gWindowWidth / (float)gWindowHeight;
        mat4Perspective(projection, 45.0f * 3.14159265f / 180.0f, aspect, 0.1f, 100.0f);

        /* === Skybox ZUERST rendern === */
        glUseProgram(skyboxProgram);
        setMat4(skyboxProgram, "view", view);
        setMat4(skyboxProgram, "projection", projection);
        skybox_draw(&skybox, skyboxProgram);

        /* === Phong-Shader fuer opake Objekte === */
        glUseProgram(phongProgram);
        setMat4(phongProgram, "view", view);
        setMat4(phongProgram, "projection", projection);
        setVec3(phongProgram, "viewPos",
                gCamera.position[0], gCamera.position[1], gCamera.position[2]);

        setVec3(phongProgram, "ambientColor", 0.3f, 0.3f, 0.35f);
        setVec3(phongProgram, "specularColor", 0.4f, 0.4f, 0.3f);
        setFloat(phongProgram, "shininess", 24.0f);

        setVec3(phongProgram, "light1Pos", 12.0f, 18.0f, 8.0f);
        setVec3(phongProgram, "light1Color", 1.0f, 0.95f, 0.85f);

        setVec3(phongProgram, "light2Pos", -10.0f, 12.0f, -6.0f);
        setVec3(phongProgram, "light2Color", 0.4f, 0.5f, 0.65f);

        /* Nebelfarbe an Skybox-Stimmung angepasst (sanftes Grau-Blau) */
        setVec3(phongProgram, "fogColor", 0.7f, 0.75f, 0.78f);
        setFloat(phongProgram, "fogStart", 20.0f);
        setFloat(phongProgram, "fogEnd", 50.0f);

        setInt(phongProgram, "diffuseTexture", 0);

        float model[16], modelView[16], normalMat[9];

        /* Rasenboden */
        buildModelMatrix(model, 0.0f, 0.0f, 0.0f, 0.0f, 25.0f);
        mat4Multiply(modelView, view, model);
        mat4NormalMatrix(normalMat, modelView);
        setMat4(phongProgram, "model", model);
        setMat3(phongProgram, "normalMatrix", normalMat);
        image_texture_bind(&grassTex, GL_TEXTURE0);
        render_mesh_draw(&floorMesh);

        /* Backgammon-Brett */
        float rotation = currentFrame * 0.25f;
        buildModelMatrix(model, 0.0f, TABLE_TOP_Y + 0.15f, 0.0f, rotation, BOARD_SCALE);
        mat4Multiply(modelView, view, model);
        mat4NormalMatrix(normalMat, modelView);
        setMat4(phongProgram, "model", model);
        setMat3(phongProgram, "normalMatrix", normalMat);
        image_texture_bind(&woodTex, GL_TEXTURE0);
        render_mesh_draw(&backgammonMesh);

        /* === Glas-Tisch === */
        glUseProgram(basicProgram);
        setMat4(basicProgram, "view", view);
        setMat4(basicProgram, "projection", projection);
        setInt(basicProgram, "diffuseTexture", 0);
        setFloat(basicProgram, "objectAlpha", 0.55f);
        image_texture_bind(&grassTex, GL_TEXTURE0);

        /* Tischplatte */
        buildModelMatrixXYZ(model, 0.0f, TABLE_TOP_Y, 0.0f, 0.0f,
                            TABLE_SIZE, 0.12f, TABLE_SIZE);
        setMat4(basicProgram, "model", model);
        render_mesh_draw(&cubeMesh);

        /* Beine */
        float halfTable = TABLE_SIZE * 0.5f;
        float legPos = halfTable - LEG_THICKNESS * 0.5f;
        float legCorners[4][2] = {
            { -legPos, -legPos },
            {  legPos, -legPos },
            { -legPos,  legPos },
            {  legPos,  legPos }
        };
        for (int i = 0; i < 4; i++)
        {
            buildModelMatrixXYZ(model,
                                legCorners[i][0],
                                TABLE_TOP_Y * 0.5f,
                                legCorners[i][1],
                                0.0f,
                                LEG_THICKNESS, TABLE_TOP_Y, LEG_THICKNESS);
            setMat4(basicProgram, "model", model);
            render_mesh_draw(&cubeMesh);
        }

        /* Querstreben */
        float strutThickness = 0.15f;
        float strutY = 0.4f;
        float strutLen = TABLE_SIZE - LEG_THICKNESS;

        buildModelMatrixXYZ(model, 0.0f, strutY, legPos, 0.0f,
                            strutLen, strutThickness, strutThickness);
        setMat4(basicProgram, "model", model);
        render_mesh_draw(&cubeMesh);

        buildModelMatrixXYZ(model, 0.0f, strutY, -legPos, 0.0f,
                            strutLen, strutThickness, strutThickness);
        setMat4(basicProgram, "model", model);
        render_mesh_draw(&cubeMesh);

        buildModelMatrixXYZ(model, -legPos, strutY, 0.0f, 0.0f,
                            strutThickness, strutThickness, strutLen);
        setMat4(basicProgram, "model", model);
        render_mesh_draw(&cubeMesh);

        buildModelMatrixXYZ(model, legPos, strutY, 0.0f, 0.0f,
                            strutThickness, strutThickness, strutLen);
        setMat4(basicProgram, "model", model);
        render_mesh_draw(&cubeMesh);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    render_mesh_delete(&floorMesh);
    render_mesh_delete(&backgammonMesh);
    render_mesh_delete(&cubeMesh);
    image_texture_delete(&woodTex);
    image_texture_delete(&grassTex);
    skybox_delete(&skybox);
    shader_delete_program(phongProgram);
    shader_delete_program(basicProgram);
    shader_delete_program(skyboxProgram);

    glfwTerminate();
    return 0;
}