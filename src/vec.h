#ifndef _VEC_H_
#define _VEC_H_

#include <stdint.h>
#include <stdbool.h>

#define VEC2(X, Y) (Vec2) { .x = (X), .y = (Y) }

#define VEC2_DOWN  VEC2( 0,  1)
#define VEC2_UP    VEC2( 0, -1)
#define VEC2_LEFT  VEC2(-1,  0)
#define VEC2_RIGTH VEC2( 1,  0)

typedef struct
{
    int32_t x;
    int32_t y;
} Vec2;

Vec2 vec2_add(Vec2 a, Vec2 b);
bool vec2_equals(Vec2 a, Vec2 b);

#endif // _VEC_H_
