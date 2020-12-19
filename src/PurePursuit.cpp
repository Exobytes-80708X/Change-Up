#include "main.h"
#include <bits/stl_vector.h>
#include <bits/stl_pair.h>

typedef double db;
typedef std::vector<double> vd;
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
  db max_t = -1;
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
  int lineNum = int(currentTime);
  db sub_t = fmod(currentTime,1.0);

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

dpdb shortestRforIntersect(db robX, db robY, db lineX, db lineY, db lineA, db lineB) {
  db x0 = robX - lineX;
  db y0 = robY - lineY;
  db numerator = (lineA*x0 + lineB*y0);
  db denom = (lineA * lineA + lineB * lineB);
  db t = numerator/denom;
  if(t > 1)
    t=1;
  if(t < 0)
    t=0;
  db pointX = lineA*t + lineX;
  db pointY = lineB*t + lineY;
  return std::pair(t,std::pair(pointX,pointY));
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

  db x0;
  db y0;
  db a;
  db b;
  dpdb r_data;
  db d;
  pdb point;

  maxV *= 1000;
  timekP *= 1000;
  anglekP *= 1000;

  while(settleTimer < 200) {
    adaptRadius = minRadius;
    distanceTraveled += calcDistance(prevX,prevY);
    distToEnd = calcDistance(xPts[SIZE-1],yPts[SIZE-1]);
    prevX = robotX;
    prevY = robotY;

    //updateVarLabel(debugLabel3,"RADIUS",debugValue3,adaptRadius,"IN",3);
    data = findFurthestPoint(xPts,yPts,adaptRadius);
    currentTime = data.first;
    //updateVarLabel(debugLabel1,"TIME",debugValue1,currentTime,"",3);
    //updateVarLabel(debugLabel2,"B",debugValue2,b,"IN",3);
    //updateVarLabel(debugLabel3,"X_l",debugValue3,x0,"IN",3);
    //updateVarLabel(debugLabel4,"Y_l",debugValue4,y0,"IN",3);

    if(currentTime == -1) { //if robot radius has no intersection
      db minDistance = 1000000000;
      db minTime = SIZE+1;
      for(int i = 0; i < SIZE-1; i++) {
        x0 = xPts[i];
        y0 = yPts[i];
        a = xPts[i+1] - x0;
        b = yPts[i+1] - y0;
        r_data = shortestRforIntersect(robotX,robotY,x0,y0,a,b);
        point = r_data.second;
        d = calcDistance(robotX,robotY,point.first,point.second);
        if (d < minDistance) {
          minDistance = d;
          minTime = i;
        }
      }
      adaptRadius = minDistance;
      //currentTime = minTime;
    }
    data = findFurthestPoint(xPts,yPts,adaptRadius);
    currentTime = data.first;
    followPoint = data.second;
    followX = followPoint.first;
    followY = followPoint.second;
    //updateVarLabel(debugLabel5,"FOLLOW X",debugValue5,followX,"IN",3);
    //updateVarLabel(debugLabel6,"FOLLOW Y",debugValue6,followY,"IN",3);
    //updateVarLabel(debugLabel6,"TIME",debugValue6,currentTime,"",0);

    if(distToEnd < adaptRadius) { //end point is within radius of robot
        adaptRadius = distToEnd; //shrink radius with distance to endPoint
        followX = xPts[SIZE-1];
        followY = yPts[SIZE-1];

        distError = adaptRadius;
        angleError = calcAngleError(followX,followY);
    }
    else {
      adaptRadius = calcDistance(followX,followY); //shrink adaptRadius based on  distance to closest intersection until it is less than minRadius
      if(adaptRadius < minRadius)
        adaptRadius = minRadius;

        distError = findDistError(xPts,yPts,currentTime,adaptRadius);
        angleError = calcAngleError(followX,followY);
    }

    if(distError < 6.0 && minRadius > 6.0)
      angleError = 0;

    if(distError < 2.0 || fabs(angleError) > 85.0*M_PI/180.0) settleTimer += 10;
    else settleTimer = 0;

    fwdSpeed = distError*timekP*cos(angleError); //the larger the angleError the less it will move forward i.e. if there is a sharp turn it will slow down
    angleSpeed = angleError*anglekP;

    if(fabs(fwdSpeed) > maxV) fwdSpeed = maxV*fwdSpeed/fabs(fwdSpeed);

    derivative = distError - prevDistError;
    prevDistError = distError;

    updateVarLabel(debugLabel1,"RADIUS",debugValue1,adaptRadius,"IN",3);
    updateVarLabel(debugLabel2,"ANGLE ERROR",debugValue2,angleError*180/M_PI,"DEG",3);
    updateVarLabel(debugLabel3,"FWD_SPEED",debugValue3,fwdSpeed,"mV",3);
    updateVarLabel(debugLabel4,"C_TIME",debugValue4,currentTime,"",3);
    updateVarLabel(debugLabel5,"FOLLOW X",debugValue5,followX,"IN",3);
    updateVarLabel(debugLabel6,"FOLLOW Y",debugValue6,followY,"IN",3);

    driveVector(fwdSpeed,angleSpeed,maxV);
    pros::delay(10);
  }
  rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	rightDrive.moveVelocity(0);
  leftDrive.moveVelocity(0);
}
