#include "test_utils.h"
#include <math.h>

int compareMat4(const float* a, const float* b)
{
    const float epsilon = 0.0001f;

    for (int i = 0; i < 16; i++)
    {
        if (fabs(a[i] - b[i]) > epsilon)
        {
            return 0;
        }
    }

    return 1;
}