#ifndef MAT4_H
#define MAT4_H

void mat4Identity(float *out);
void mat4Multiply(float *out, const float *a, const float *b);
void mat4Translate(float *out,
                   const float *in,
                   float x,
                   float y,
                   float z);
void mat4Scale(float *out,
               const float *in,
               float x,
               float y,
               float z);
void mat4RotateX(float* out,
                 const float* in,
                 float angle);
void mat4RotateY(float* out,
                 const float* in,
                 float angle);
void mat4RotateZ(float* out,
                 const float* in,
                 float angle);
void mat4Perspective(float* out,
                     float fovy,
                     float aspect,
                     float near,
                     float far);
void mat4LookAt(float* out,
                const float* eye,
                const float* center,
                const float* up);
void mat3Inverse(float* out, const float* in);
void mat3Transpose(float* out, const float* in);
void mat4NormalMatrix(float* out, const float* modelView);

#endif