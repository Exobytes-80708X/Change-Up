#include "main.h"
#include <bits/stl_vector.h>
#include <bits/stl_pair.h>

typedef double db;
typedef std::vector<int> vi;

vi circLineIntersect(db xl, db yl, db a, db b, db r, db xc, db yc){
  vi ret(2);
  db x0 = xl-xc;
  db y0 = yl-yc;
  db radical = pow((a*x0 + b*y0),2.0) - (a*a + b*b)*(x0*x0 + y0*y0 - r*r);
  if(radical < 0)
    return ret;

  db negB = -(a*x0 + b*y0);
  db denom = a*a + b*b;

  db ret1 = (negB + sqrtf(radical))/denom;
  db ret2 = (negB - sqrtf(radical))/denom;
if(ret1 >= 0.0 && ret1 <= 1.0)
    ret.push_back(ret1);
if(ret2 >= 0.0 && ret2 <= 1.0)
    ret.push_back(ret2);
  return ret;
}

void purePursuit(db minRadius, vi xPts, vi yPts)
