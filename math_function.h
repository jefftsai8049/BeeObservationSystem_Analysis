#ifndef MATH_FUNCTION_H
#define MATH_FUNCTION_H

#include <math.h>

namespace mf {

float findMax(const float& num1,const float& num2,const float& num3);

void vectorFindMax(double& maxVal,int& index,const double* val);

void vectorFindMin(double& minVal,int& index,const double* val);


}
#endif // MATH_FUNCTION_H
