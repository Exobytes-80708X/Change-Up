#include "main.h"
bool isRobotDisabled = true;
bool driverControl = false;

void initialize()
{
  imu.reset();
  pros::c::ext_adi_pin_mode(5, 'A', INPUT_ANALOG);
  pros::Task task_GUI (initGUI, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task task_1 (thread_sensors, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task task_2 (thread_subsystems, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}

void disabled()
{

}

void competition_initialize()
{

}

void benchmark_speeds()
{
  double prevX;
  double dX;
  double prevY;
  double dY;

  double dS;

  double max_j;
  double max_a;
  double max_v;

  double current_a;
  double prev_a = 0;
  double dA;

  double current_v;
  double prev_v = 0;
  double dV;

  double current_j;

  double start_time = pros::millis()/1000.0;//std::clock();
  double current_time = start_time;
  double prev_time = current_time;
  double dT = current_time - prev_time;

  while(current_time - start_time <= 3)
  {
    rightDrive.moveVoltage(8000);
    leftDrive.moveVoltage(8000);
    pros::delay(10);
    current_time = pros::millis()/1000.0;//std::clock();
    dT = current_time - prev_time;
    prev_time = current_time;

    dS = calcDistance(prevX,prevY,robotX,robotY);
    prevX = robotX;
    prevY = robotY;

    current_v = dS/dT; //inches per second
    dV = current_v - prev_v;
    prev_v = current_v;

    if(current_v > max_v) max_v = current_v;

    current_a = dV/dT; //inches per second^2
    dA = current_a - prev_a;
    prev_a = current_a;

    if(current_a > max_a) max_v = current_a;

    current_j = dA/dT; //inches per second^3

    if(current_j > max_j) max_j = current_j;

    updateVarLabel(debugLabel1,"MAX VELOCITY",debugValue1,max_v,"IN/S",3);
    updateVarLabel(debugLabel2,"MAX ACCEL",debugValue2,max_a*180/M_PI,"IN/S^2",3);
    updateVarLabel(debugLabel3,"MAX JERK",debugValue3,max_j,"IN/S^3",0);
  }
  rightDrive.moveVoltage(0);
  leftDrive.moveVoltage(0);

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
void shooting_macro(int numOfBalls)
{
  if(!firstBall)
    return;

  topConveyor.move_velocity(600);
  botConveyor.move_velocity(0);
  pros::delay(200);
  countBalls(numOfBalls-1);
}
void autonomous()
{
  pros::Task task_odometry (thread_Odometry, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::delay(200);
  isRobotDisabled = false;
  driverControl = false;

  switch(auton) {
    case 0: //no auton
    //benchmark_speeds();
    //super_macro(2,2);
      shooting_macro(2);
    // intake(inward);
    // countIntakeBalls(2);
    // intake(stop);
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
    adaptiveDrive(-16,4,7);
    delayDrive(700, 5500);
    //while(!firstBall) pros::delay(10);
    // shoot(2);
    // intake(inward);
    // pros::delay(900);
    super_macro(2,2);
    intake(outward);
    driveDistance(-24,8);
    intake(stop);
    face(8,56);
    intake(inward);
    //adaptiveDrive(14, 55, 10);
    eject(2);
    driveDistance(calcDistance(8,56)+6,10);
    driveDistance(-6,10);
    //intake(stop);
    face(-23,56);
    //intake(inward);
    //adaptiveDrive(-10,53,10);
    driveDistance(calcDistance(-20,56),10);
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
    face(-4,95);
    driveDistance(calcDistance(-4, 95)+8,10);
    driveDistance(-8,10);
    //adaptiveDrive(-4, 95, 10);
    //driveDistance(6,10);
    //driveDistance(-6,10);
    //driveDistance(calcDistance(3,100),10);
    //intake(stop);
    face(-23,115);
    driveDistance(calcDistance(-15,115),10);
    //adaptiveDrive(-18,115,10);
    delayDrive(700,5500);
    //intake(inward);
    //pros::delay(500);
    shoot(1);
    pros::delay(750);
    intake(outward);
    driveDistance(-30,10);
    face(38,79);
    eject(2);
    intake(inward);
    driveDistance(calcDistance(38,79),10);
    face(36,120);
    driveDistance(calcDistance(36,120)-8,8);
    delayDrive(700, 5500);
    //pros::delay(500);
    shoot(1);
    pros::delay(500);
    intake(outward);
    driveDistance(-10,10);
    intake(stop);
    face(85,93);
    eject(1);
    intake(inward);
    driveDistance(calcDistance(85,93)+4,10);
    face(105,115);
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
  driverControl = true;
  pros::Task task_4 (thread_drive, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task task_5 (thread_control, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}
