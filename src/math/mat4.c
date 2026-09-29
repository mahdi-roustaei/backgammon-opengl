#include "mat4.h"
#include <math.h>

void mat4Identity(float *out)
{

    for (int i = 0; i < 16; i++)
    {
        out[i] = 0.0f;
    }

    out[0] = 1.0f;
    out[5] = 1.0f;
    out[10] = 1.0f;
    out[15] = 1.0f;
}

void mat4Multiply(float *out, const float *a, const float *b)
{
    float result[16];

    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            float sum = 0.0f;

            for (int k = 0; k < 4; k++)
            {
                sum += a[k * 4 + row] * b[col * 4 + k];
            }

            result[col * 4 + row] = sum;
        }
    }

    for (int i = 0; i < 16; i++)
    {
        out[i] = result[i];
    }
}

void mat4Translate(float *out,
                   const float *in,
                   float x,
                   float y,
                   float z)
{
    float t[16];

    mat4Identity(t);

    t[12] = x;
    t[13] = y;
    t[14] = z;

    mat4Multiply(out, in, t);
}

void mat4Scale(float *out,
               const float *in,
               float x,
               float y,
               float z)
{
    float s[16];

    mat4Identity(s);

    s[0] = x;
    s[5] = y;
    s[10] = z;

    mat4Multiply(out, in, s);
}

void mat4RotateX(float *out,
                 const float *in,
                 float angle)
{
    float r[16];

    mat4Identity(r);

    float c = cosf(angle);
    float s = sinf(angle);

    r[5] = c;
    r[6] = s;

    r[9] = -s;
    r[10] = c;

    mat4Multiply(out, in, r);
}
void mat4RotateY(float *out,
                 const float *in,
                 float angle)
{
    float r[16];

    mat4Identity(r);

    float c = cosf(angle);
    float s = sinf(angle);

    r[0] = c;
    r[2] = -s;

    r[8] = s;
    r[10] = c;

    mat4Multiply(out, in, r);
}
void mat4RotateZ(float *out,
                 const float *in,
                 float angle)
{
    float r[16];

    mat4Identity(r);

    float c = cosf(angle);
    float s = sinf(angle);

    r[0] = c;
    r[1] = s;

    r[4] = -s;
    r[5] = c;

    mat4Multiply(out, in, r);
}
void mat4Perspective(float *out,
                     float fovy,
                     float aspect,
                     float near,
                     float far)
{
    float f = 1.0f / tanf(fovy / 2.0f);

    for (int i = 0; i < 16; i++)
    {
        out[i] = 0.0f;
    }

    out[0] = f / aspect;
    out[5] = f;

    out[10] = (far + near) / (near - far);
    out[11] = -1.0f;

    out[14] = (2.0f * far * near) / (near - far);
}

static void vec3Subtract(float* out,
                         const float* a,
                         const float* b)
{
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
}

static float vec3Dot(const float* a,
                     const float* b)
{
    return a[0]*b[0]
         + a[1]*b[1]
         + a[2]*b[2];
}

static void vec3Cross(float* out,
                      const float* a,
                      const float* b)
{
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}

static void vec3Normalize(float* v)
{
    float len = sqrtf(
        v[0]*v[0] +
        v[1]*v[1] +
        v[2]*v[2]
    );

    if(len == 0.0f)
        return;

    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
}
void mat4LookAt(float* out,
                const float* eye,
                const float* center,
                const float* up)
{
    float f[3];
    float s[3];
    float u[3];

    vec3Subtract(f, center, eye);
    vec3Normalize(f);

    vec3Cross(s, f, up);
    vec3Normalize(s);

    vec3Cross(u, s, f);

    out[0] = s[0];
    out[1] = u[0];
    out[2] = -f[0];
    out[3] = 0.0f;

    out[4] = s[1];
    out[5] = u[1];
    out[6] = -f[1];
    out[7] = 0.0f;

    out[8] = s[2];
    out[9] = u[2];
    out[10] = -f[2];
    out[11] = 0.0f;

    out[12] = -vec3Dot(s, eye);
    out[13] = -vec3Dot(u, eye);
    out[14] = vec3Dot(f, eye);
    out[15] = 1.0f;
}

void mat3Inverse(float* out, const float* m)
{
    float det =
        m[0] * (m[4] * m[8] - m[5] * m[7]) -
        m[3] * (m[1] * m[8] - m[2] * m[7]) +
        m[6] * (m[1] * m[5] - m[2] * m[4]);

    if(det == 0.0f)
    {
        return;
    }

    float invDet = 1.0f / det;

    out[0] = (m[4] * m[8] - m[5] * m[7]) * invDet;
    out[1] = (m[2] * m[7] - m[1] * m[8]) * invDet;
    out[2] = (m[1] * m[5] - m[2] * m[4]) * invDet;

    out[3] = (m[5] * m[6] - m[3] * m[8]) * invDet;
    out[4] = (m[0] * m[8] - m[2] * m[6]) * invDet;
    out[5] = (m[2] * m[3] - m[0] * m[5]) * invDet;

    out[6] = (m[3] * m[7] - m[4] * m[6]) * invDet;
    out[7] = (m[1] * m[6] - m[0] * m[7]) * invDet;
    out[8] = (m[0] * m[4] - m[1] * m[3]) * invDet;
}

void mat3Transpose(float* out, const float* m)
{
    out[0] = m[0];
    out[1] = m[3];
    out[2] = m[6];

    out[3] = m[1];
    out[4] = m[4];
    out[5] = m[7];

    out[6] = m[2];
    out[7] = m[5];
    out[8] = m[8];
}

void mat4NormalMatrix(float* out, const float* modelView)
{
    float upper3x3[9];

    upper3x3[0] = modelView[0];
    upper3x3[1] = modelView[1];
    upper3x3[2] = modelView[2];

    upper3x3[3] = modelView[4];
    upper3x3[4] = modelView[5];
    upper3x3[5] = modelView[6];

    upper3x3[6] = modelView[8];
    upper3x3[7] = modelView[9];
    upper3x3[8] = modelView[10];

    float inverse[9];

    mat3Inverse(inverse, upper3x3);

    mat3Transpose(out, inverse);
}