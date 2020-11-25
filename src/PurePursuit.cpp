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

  ret.push_back((negB + sqrtf(radical))/denom);
  ret.push_back((negB - sqrtf(radical))/denom);
  return ret;
}
