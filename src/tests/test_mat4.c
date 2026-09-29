#include <stdio.h>
#include "../math/mat4.h"
#include "test_utils.h"
#include <math.h>

int main()
{
    /*
     * Identity Test
     */

    float identityResult[16];

    mat4Identity(identityResult);

    float identityExpected[16] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};

    if (compareMat4(identityResult, identityExpected))
        printf("Identity Test PASSED\n");
    else
        printf("Identity Test FAILED\n");

    /*
     * Multiply Test
     */

    float a[16];
    float b[16];
    float multiplyResult[16];

    mat4Identity(a);
    mat4Identity(b);

    mat4Multiply(multiplyResult, a, b);

    if (compareMat4(multiplyResult, identityExpected))
        printf("Multiply Test PASSED\n");
    else
        printf("Multiply Test FAILED\n");

    /*
     * Translate Test
     */

    float translateResult[16];

    mat4Identity(translateResult);

    mat4Translate(
        translateResult,
        translateResult,
        5.0f,
        2.0f,
        -3.0f);

    float translateExpected[16] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            5, 2, -3, 1};

    if (compareMat4(translateResult, translateExpected))
        printf("Translate Test PASSED\n");
    else
        printf("Translate Test FAILED\n");

    /*
     * Scale Test
     */

    float scaleResult[16];

    mat4Identity(scaleResult);

    mat4Scale(
        scaleResult,
        scaleResult,
        2.0f,
        3.0f,
        4.0f);

    float scaleExpected[16] =
        {
            2, 0, 0, 0,
            0, 3, 0, 0,
            0, 0, 4, 0,
            0, 0, 0, 1};

    if (compareMat4(scaleResult, scaleExpected))
        printf("Scale Test PASSED\n");
    else
        printf("Scale Test FAILED\n");

    /*
     * RotateX Test
     */

    float rotateXResult[16];

    mat4Identity(rotateXResult);

    mat4RotateX(
        rotateXResult,
        rotateXResult,
        3.14159265f / 2.0f);

    float rotateXExpected[16] =
        {
            1, 0, 0, 0,
            0, 0, 1, 0,
            0, -1, 0, 0,
            0, 0, 0, 1};

    if (compareMat4(rotateXResult, rotateXExpected))
        printf("RotateX Test PASSED\n");
    else
        printf("RotateX Test FAILED\n");

    /*
     * RotateY Test
     */

    float rotateYResult[16];

    mat4Identity(rotateYResult);

    mat4RotateY(
        rotateYResult,
        rotateYResult,
        3.14159265f / 2.0f);

    float rotateYExpected[16] =
        {
            0, 0, -1, 0,
            0, 1, 0, 0,
            1, 0, 0, 0,
            0, 0, 0, 1};

    if (compareMat4(rotateYResult, rotateYExpected))
        printf("RotateY Test PASSED\n");
    else
        printf("RotateY Test FAILED\n");

    /*
     * RotateZ Test
     */

    float rotateZResult[16];

    mat4Identity(rotateZResult);

    mat4RotateZ(
        rotateZResult,
        rotateZResult,
        3.14159265f / 2.0f);

    float rotateZExpected[16] =
        {
            0, 1, 0, 0,
            -1, 0, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};

    if (compareMat4(rotateZResult, rotateZExpected))
        printf("RotateZ Test PASSED\n");
    else
        printf("RotateZ Test FAILED\n");

    /*
     * Perspective Test
     */

    float perspectiveResult[16];

    mat4Perspective(
        perspectiveResult,
        3.14159265f / 4.0f, // 45°
        16.0f / 9.0f,
        0.1f,
        100.0f);

    printf("Perspective Test:\n");

    for (int i = 0; i < 16; i++)
    {
        printf("%f ", perspectiveResult[i]);
    }

    printf("\n");

    /*
     * LookAt Test
     */

    float eye[3] =
        {
            0.0f,
            0.0f,
            5.0f};

    float center[3] =
        {
            0.0f,
            0.0f,
            0.0f};

    float up[3] =
        {
            0.0f,
            1.0f,
            0.0f};

    float lookAtResult[16];

    mat4LookAt(
        lookAtResult,
        eye,
        center,
        up);

    float lookAtExpected[16] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, -5, 1};

    if (compareMat4(
            lookAtResult,
            lookAtExpected))
    {
        printf("LookAt Test PASSED\n");
    }
    else
    {
        printf("LookAt Test FAILED\n");
    }

    float modelView[16];

    mat4Identity(modelView);

    float normal[9];

    mat4NormalMatrix(normal, modelView);

    float expectedNormal[9] =
        {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1};

    int normalOk = 1;

    for (int i = 0; i < 9; i++)
    {
        if (fabs(normal[i] - expectedNormal[i]) > 0.0001f)
        {
            normalOk = 0;
            break;
        }
    }

    if (normalOk)
    {
        printf("NormalMatrix Test PASSED\n");
    }
    else
    {
        printf("NormalMatrix Test FAILED\n");
    }
    return 0;
}