#include "main.h"
#include <bits/stl_vector.h>
#include <bits/stl_pair.h>

typedef double db;
typedef std::vector<db> vd;
typedef std::pair<db,db> pdb;
typedef std::pair<db,pdb> dpdb;

const int UND = 80708;

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
  db max_t = UND;
  pdb point = std::pair(UND,UND);
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

db findDistError(vd xPts, vd yPts, db currentTime,db r)
{
  int SIZE = xPts.size();
  int lineNum = currentTime;
  db sub_t = fmod(currentTime,1);

  db x0 = xPts[lineNum];
  db y0 = yPts[lineNum];
  db a = xPts[lineNum+1] - x0;
  db b = yPts[lineNum+1] - y0;

  db currentX = a*sub_t + x0;
  db currentY = b*sub_t + y0;

  db initDistance = calcDistance(currentX,currentY,xPts[lineNum+1],yPts[lineNum+1]);
  for(int i = lineNum+1; i < SIZE-1; i++) {
    initDistance += calcDistance(xPts[i],yPts[i],xPts[i+1],yPts[i+1]);
  }
  return initDistance + r;
}

void purePursuit(db minRadius, db accel, vd xPts, vd yPts, db maxV, db timekP, db anglekP, int timeout)
{
  int SIZE = xPts.size();
  db END_TIME = SIZE-1;
  db currentTime = 0;
  db distError;
  db angleError;
  db prevDistError = distError;

  db derivative;

  db fwdSpeed;
  db angleSpeed;

  int settleTimer = 0;
  int timeOutTimer = 0;

  pdb followPoint;
  dpdb data;

  db followX;
  db followY;

  db prevX = robotX;
  db prevY = robotY;

  db distanceTraveled = 0.0;
  db distToEnd;

  db adaptRadius = minRadius;

  maxV *= 1000;
  timekP *= 1000;
  anglekP *= 1000;

  while(settleTimer < 200) {
    distanceTraveled += calcDistance(prevX,prevY);
    distToEnd = calcDistance(xPts[SIZE-1],yPts[SIZE-1]);
    prevX = robotX;
    prevY = robotY;

    data = findFurthestPoint(xPts,yPts,adaptRadius);
    currentTime = data.first;

    if(currentTime == UND) { //if robot radius has no intersection
      while(currentTime == UND) {
        adaptRadius += 0.1; //increase adaptRadius by 0.1 inch until it finds an intersection
        data = findFurthestPoint(xPts,yPts,adaptRadius);
        currentTime = data.first;
        pros::delay(10);
      }
    }

    followPoint = data.second;
    followX = followPoint.first;
    followY = followPoint.second;

    if(distToEnd < adaptRadius) { //end point is within radius of robot
        adaptRadius = distToEnd; //shrink radius with distance to endPoint
    }
    else {
      adaptRadius = calcDistance(followX,followY); //shrink adaptRadius based on  distance to closest intersection until it is less than minRadius
      if(adaptRadius < minRadius)
        adaptRadius = minRadius;
    }

    distError = findDistError(xPts,yPts,currentTime,adaptRadius);
    angleError = calcAngleError(followX,followY);

    fwdSpeed = distError*timekP*cos(angleError); //the larger the angleError the less it will move forward i.e. if there is a sharp turn it will slow down
    angleSpeed = angleError*anglekP;

    derivative = distError - prevDistError;
    prevDistError = distError;

    driveVector(fwdSpeed,angleSpeed,maxV);
    pros::delay(10);
  }
}
