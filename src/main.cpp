#include "main.h"
bool isRobotDisabled = true;

void initialize()
{
  imu.reset();
  pros::Task task_1 (thread_sensors, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task task_2 (thread_conveyor, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task task_3 (thread_intake, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task task_GUI (initGUI, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");;
}

void disabled()
{

}

void competition_initialize()
{

}

void intake(int state){
  // state - What intake state to switch to. in = 0, out = 1, stop = 2
  intakeState = state;
}

void shoot(int numBalls){
  switch(numBalls){
    case 1:
      conveyorState = 6;
      break;
    case 2:
      conveyorState = 4;
      break;
    case 3:
      conveyorState = 3;
      break;
  }
  pros::delay(100);
  conveyorState = 0;
}

void eject(int numBalls){
  // switch(numBalls){
  //   case 1:
  //     break;
  //   case 2:
  //     conveyorState = 5;
  //     break;
  //   case 3:
  //     break;
  //}
  //pros::delay(100);
  //conveyorState = 0;

  conveyorState = 2;
  pros::delay(200*numBalls);
  conveyorState = 0;
}
//----------------------------------------------
double quadX(double x,double y,double t){
  return 45*t + x;
}
double quadY(double x,double y,double t){
  return -pow(10*(t-.5),2) + y + 25.0;
}
double quadDerivX(double x,double y,double t){
  return 15;
}
double quadDerivY(double x,double y,double t){
  return -2*(10*(t-.5))*10;
}
double lineX(double x, double y, double slope, double t){
  return x + slope * t;
}
double lineY(double x, double y, double slope, double t){
  return y + slope * t;
}
void followQuad(int numPoints){
  double deltaT = 1.0/numPoints;
  double len = 63.829;
  double t = deltaT;
  double x = robotX;
  double y = robotY;
  for(int i = 1; i <= numPoints; i++){
    double x1 = quadX(x,y,t);
    double y1 = quadY(x,y,t);
    updateVarLabel(debugLabel1,"QUAD X",debugValue1,x1,"IN",3);
    updateVarLabel(debugLabel2,"QUAD Y",debugValue2,y1,"IN",3);
    updateVarLabel(debugLabel3,"t",debugValue3,t,"SECONDS",3);
    updateVarLabel(debugLabel4,"dt",debugValue4,deltaT,"SECONDS",3);
    //face(x1,y1);
    //driveDistance(calcDistance(quadX(x,y,t),quadY(x,y,t)), 10);
    adaptiveDrive(x1, y1, 8);
    t+=deltaT;
    //pros::delay(deltaT*20);
  }
}
void followQuadDeriv(int numPoints){
  double t = 0;
  double deltaT = 1.0/numPoints;
  double x = robotX;
  double y = robotY;
  double s = 0.5;
  for(int i = 0; i <= numPoints; i++){
    double a = lineX(x,y,quadDerivX(x,y,t),s);
    double b = lineY(x,y,quadDerivY(x,y,t),s);
    updateVarLabel(debugLabel1,"QUAD X",debugValue1,a,"IN",3);
    updateVarLabel(debugLabel2,"QUAD Y",debugValue1,b,"IN",3);
    face(a,b);
    driveDistance(calcDistance(a,b), 8);
    t+=deltaT;
    pros::delay(deltaT*20);
  }
}
double findLength(double startT, double endT, int numPoints){
    double t = (endT - startT)/numPoints;
    double total = 0;
    while(startT < endT){
      double p1x = quadX(robotX,robotY,startT);
      double p2x = quadX(robotX,robotY,startT + t);
      double p1y = quadY(robotX,robotY,startT);
      double p2y = quadY(robotX,robotY,startT + t);
      total += calcDistance(p1x,p1y,p2x,p2y);
      startT += t;
    }
    return total;
}
//------------------------------------------------------
void autonomous()
{
  pros::Task task_odometry (thread_Odometry, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::delay(200);
  isRobotDisabled = false;

  switch(auton) {
    case 0: //no auton
    face(90);

    face(0);
    break;

    case 1: //red auton
    break;

    case 2: //blue auton
      followQuad(8);
    break;

    case 3: //skills auton
    intake(inward);
    driveDistance(15,10);
    intake(stop);
    face(-26,-1);
    driveDistance(18,8);
    delayDrive(700, 5500);
    while(!firstBall) pros::delay(10);
    shoot(2);
    intake(inward);
    pros::delay(1000);
    intake(outward);
    driveDistance(-24,8);
    intake(stop);
    face(10,63);
    eject(2);
    intake(inward);
    driveDistance(calcDistance(10,63),10);
    intake(stop);
    face(-10,63);
    intake(inward);
    driveDistance(calcDistance(-10,63),7);
    delayDrive(700,5500);
    intake(stop);
    shoot(2);
    intake(inward);
    pros::delay(150);
    intake(outward);
    pros::delay(500);
    driveDistance(-20,8);
    intake(stop);
    face(3,100);
    intake(inward);
    eject(1);
    driveDistance(calcDistance(3,100),10);
    intake(stop);
    break;

  }
}

void opcontrol()
{
  isRobotDisabled = false;
  pros::Task task_4 (thread_drive, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task task_5 (thread_control, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}
