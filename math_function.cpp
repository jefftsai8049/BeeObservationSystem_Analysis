#include "math_function.h"


float mf::findMax(const float &num1, const float &num2, const float &num3)
{
    float max;
    if(num1>num2)
        max = num1;
    else
        max = num2;

    if(max<num3)
        max = num3;

    return max;
}
