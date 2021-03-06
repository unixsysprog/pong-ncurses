/*
 * line.h: Definition and functions for line segment used in physics calculations.
 */

#ifndef LINE_H_
#define LINE_H_

#include "physics/vector.h"
#include "physics/ratio.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct line {
    struct vector start;
    struct vector end;
};

struct intersection {
    bool intersects;     // whether or not there's an intersection
    struct vector point; // the point of intersection, if any
};

struct line line_between(struct vector v1, struct vector v2);
struct ratio line_slope(struct line l);

struct intersection intersection_between(struct line l1, struct line l2);

#ifdef __cplusplus
}
#endif

#endif // LINE_H_
