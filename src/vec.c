#include "vec.h"

Vec2 vec2_add(Vec2 a, Vec2 b)
{
    return VEC2(a.x + b.x, a.y + b.y);
}

bool vec2_equals(Vec2 a, Vec2 b)
{
    return (a.x == b.x && a.y == b.y);
}
