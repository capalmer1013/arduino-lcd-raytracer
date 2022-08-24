#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include "limits.h"


// Usings


// Constants
const double infinity = INFINITY;
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

double random_double(){
  return random(10000)/10000.0;
}

double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}



// Common Headers

#include "ray.h"
#include "vec3.h"

#endif
