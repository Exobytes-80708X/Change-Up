#include "main.h"
#include <bits/stl_vector.h>
#include <bits/stl_pair.h>

typedef double db;
typedef std::vector<db> vd;
typedef std::pair<db,db> pdb;
typedef std::pair<db,pdb> dpdb;

db circLineIntersect(db xl, db yl, db a, db b, db r, db xc, db yc){
  vd ret(2);
  db x0 = xl-xc;
  db y0 = yl-yc;
  db radical = pow((a*x0 + b*y0),2.0) - (a*a + b*b)*(x0*x0 + y0*y0 - r*r);
  if(radical < 0)
    return -1;

  db negB = -(a*x0 + b*y0);
  db denom = a*a + b*b;

  db ret1 = (negB + sqrtf(radical))/denom;
  db ret2 = (negB - sqrtf(radical))/denom;
  if(ret1 >= 0.0 && ret1 <= 1.0)
      ret.push_back(ret1);
  if(ret2 >= 0.0 && ret2 <= 1.0)
      ret.push_back(ret2);

  if(ret.size() == 0)
    return -1;
  if(ret.size() == 1)
    return ret[1];
  return fmax(ret[1],ret[2]);
}

dpdb findFurthestPoint(vd xPts, vd yPts, db r)
{
  int max_index;
  db a;
  db b;
  db x0;
  db y0;
  db sub_t;
  db t;
  db max_t = 0;
  pdb point = std::pair(80708,80708);
  for(int i = 0; i < xPts.size()-1; i++) {
    x0 = xPts[i];
    y0 = yPts[i];
    a = xPts[i+1] - x0;
    b = yPts[i+1] - y0;
    sub_t = circLineIntersect(x0,y0,a,b,r,robotX,robotY);
    if(sub_t != -1) {
      t = sub_t + i;
      if(t > max_t) {
        max_t = t;
        max_index = i;
        point.first = a*sub_t+x0;
        point.second = b*sub_t+y0;
      }
    }
  }
  return std::pair(max_t,point);
}

void purePursuit(db minRadius, db accel, vd xPts, vd yPts, db maxV, db timekP, db anglekP, int timeout)
{
  db END_TIME = xPts.size()-1;
  db currentTime = 0;
  db timeError = END_TIME;
  db angleError;
  db prevTimeError = timeError;

  db derivative;

  db currentSpeed;
  db angleSpeed;

  int settleTimer = 0;
  int timeOutTimer = 0;

  pdb followPoint;
  dpdb data;

  db followX;
  db followY;

  maxV *= 1000;
  timekP *= 1000;
  anglekP *= 1000;

  while(settleTimer < 200) {
    data = findFurthestPoint(xPts,yPts,minRadius);
    followPoint = data.second;
    currentTime = data.first;

    followX = followPoint.first;
    followY = followPoint.second;

    timeError = END_TIME - currentTime;
    angleError = calcAngleError(followX,followY);
    currentSpeed = timeError*timekP;
    angleSpeed = angleError*anglekP;

    derivative = timeError - prevTimeError;
    prevTimeError = timeError;

    driveVector(currentSpeed,angleSpeed,maxV);
    pros::delay(10);
  }
}
