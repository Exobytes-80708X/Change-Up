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
  return 15*t + x;
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
  double t = 0;
  double deltaT = 1/numPoints;
  double x = robotX;
  double y = robotY;
  for(int i = 0; i < numPoints; i++){
    face(quadX(x,y,t),quadY(x,y,t));
    driveDistance(calcDistance(quadX(x,y,t),quadY(x,y,t)), 10);
    t+=deltaT;
    pros::delay(deltaT*1000);
  }
}
void followQuadDeriv(int numPoints){
  double t = 0;
  double deltaT = 1/numPoints;
  double x = robotX;
  double y = robotY;
  double s = 0.5;
  for(int i = 0; i < numPoints; i++){
    double a = lineX(x,y,quadDerivX(x,y,t),s);
    double b = lineY(x,y,quadDerivY(x,y,t),s);
    face(a,b);
    driveDistance(calcDistance(a,b), 10);
    t+=deltaT;
    pros::delay(deltaT*1000);
  }
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
