#if !defined OPERATOR_H
#define OPERATOR_H
#include "structs.h"

number xvcalc_add(number left, number right);
number xvcalc_sub(number left, number right);
number xvcalc_mul(number left, number right);
number xvcalc_div(number left, number right);
number xvcalc_dice(number n_count, number n_faces);

#endif /* OPERATOR_H */