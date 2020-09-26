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

int ejectBalls = 0;

void eject_thread(void*p)
{
  for(int n = 0; n < ejectBalls; n++) {
    waitForBallToEject();
  }
  ejectBalls = 0;
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
  ejectBalls = numBalls;
  pros::Task sub (eject_thread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");

}
//----------------------------------------------

//------------------------------------------------------
void autonomous()
{
  pros::Task task_odometry (thread_Odometry, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::delay(200);
  isRobotDisabled = false;

  switch(auton) {
    case 0: //no auton
      adaptiveDrive(24,24,10);
      adaptiveDrive(24,48,10);
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
    //face(-26,-1);
    //driveDistance(18,8);
    adaptiveDrive(-20,4,7);
    delayDrive(700, 5500);
    //while(!firstBall) pros::delay(10);
    shoot(2);
    intake(inward);
    pros::delay(900);
    intake(outward);
    driveDistance(-24,8);
    intake(stop);
    //face(14,63);
    eject(2);
    intake(inward);
    adaptiveDrive(14, 55, 10);
    //driveDistance(calcDistance(10,63),10);
    intake(stop);
    face(-10,63);
    intake(inward);
    //adaptiveDrive(-10,63,10);
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
    //face(3,100);
    intake(inward);
    eject(1);
    adaptiveDrive(0, 100, 10);
    //driveDistance(calcDistance(3,100),10);
    intake(stop);
    //face(-20,125);
    //driveDistance(calcDistance(-10,115),10);
    adaptiveDrive(-20,125,10);
    delayDrive(700,5500);
    intake(inward);
    //pros::delay(500);
    shoot(1);
    pros::delay(500);
    intake(outward);
    driveDistance(-30,10);
    face(35,90);
    eject(2);
    intake(inward);
    driveDistance(calcDistance(43,90),10);
    face(43,120);
    driveDistance(20,8);
    delayDrive(700, 5500);
    //pros::delay(500);
    shoot(1);
    pros::delay(500);
    intake(outward);
    driveDistance(-10,10);
    intake(stop);
    face(85,107);
    eject(1);
    intake(inward);
    driveDistance(calcDistance(85,110)+4,10);
    face(110,125);
    intake(stop);
    driveDistance(calcDistance(100,115),8);
    intake(inward);
    delayDrive(600,5500);
    //pros::delay(500);
    shoot(1);
    pros::delay(500);
    intake(outward);
    driveDistance(-20,10);
    intake(inward);
    face(70,55);
    eject(2);
    driveDistance(calcDistance(70,55),10);
    face(200,58);
    delayDrive(1000,7500);
    //pros::delay(500);
    shoot(1);
    pros::delay(500);
    intake(outward);
    driveDistance(-20,10);
    face(82,10);
    eject(1);
    intake(inward);
    driveDistance(calcDistance(82,10),10);
    face(100,-1);
    driveDistance(calcDistance(95,6),10);
    delayDrive(750,5500);
    pros::delay(600);
    shoot(1);
    pros::delay(700);
    intake(outward);
    driveDistance(-30,10);
    face(30,27);
    driveDistance(calcDistance(30,27),10);
    face(30,-1);

    break;

  }
}

void opcontrol()
{
  isRobotDisabled = false;
  pros::Task task_4 (thread_drive, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task task_5 (thread_control, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}
