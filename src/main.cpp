#include "main.h"
bool isRobotDisabled = true;
bool driverControl = false;
int red = 1;
int blue = 2;

void initialize()
{
  imu.reset();
  pros::c::ext_adi_pin_mode(5, 'A', INPUT_ANALOG);
  pros::c::ext_adi_pin_mode(5, 'B', INPUT_ANALOG);
  pros::c::ext_adi_pin_mode(5, 'C', INPUT_ANALOG);
  pros::Task task_GUI (initGUI, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  //pros::Task task_1 (thread_sensors, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task task_1 (thread_sensors_filter, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::delay(250);
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

void shoot(int a){
    conveyorState = 3;
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
  conveyorState = 2;
  ejectBalls = numBalls;
  pros::Task sub (eject_thread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}

void waitForThirdBall()
{
  int timer = 0;
  int timeOut = 0;
  while(timer < 200) {
    if(thirdBall)
      timer += 10;
    else
      timer = 0;
    timeOut += 10;
    pros::delay(10);
    if(timeOut > 1000)
      return;
  }
}

void waitForFirstBall()
{

}

void asynchShoot(void*p)
{
  shooting_macro(1);
}
//----------------------------------------------

//------------------------------------------------------
void autonomous()
{
  pros::Task task_odometry (thread_Odometry, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task f (asynchShoot, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  f.suspend();
  pros::delay(200);
  isRobotDisabled = false;
  driverControl = false;
  double p = 25;
  double i = 0;
  double d = 50;

  switch(auton) {
    case 0: //no auton
      shooting_macro(2);
    break;

    case 1: //red auton
      robotTheta = M_PI/3;
      intake(inward);
      pointTurn(0,190,130,false,45,i,d);
      delayDrive(400,8000);
      waitForThirdBall();
      super_macro(countHeldBalls(), 1); //score first goal
      //pros::delay(100);
      intake(outward);
      adaptiveDrive_reversed(-32,16,9.5);
      //shooting_macro(1); //shoot oppposite ball
      f.resume();
      facePID(180,p,i,d);
      intake(inward);
      delayDrive(800,9000);
      while(!firstBall)
        pros::delay(10);
      shooting_macro(1); //score second goal
      pros::delay(200);
      intake(outward);
      driveDistance(-17,10);
      eject(1);
      facePID(270,p,i,d);
      //facePID(12,-108,p,i,d);
      intake(inward);
      adaptiveDrive(-84,-3, 8);
      delayDrive(400,8000);
      intake(outward);
      conveyorState = 99;
      while(!firstBall)
        pros::delay(10);
      shooting_macro(1); //score third goal
      //pros::delay(200);
      delayDrive(400,-8000);
    break;

    case 2: //blue auton
      robotTheta = M_PI/3;
      intake(inward);
      pointTurn(0,190,130,false,p*1.5,i,d);
      delayDrive(400,8000);
      waitForThirdBall();
      super_macro(countHeldBalls(), 1); //score first goal
      pros::delay(100);
      intake(outward);
      adaptiveDrive_reversed(-35,16,9.5);
      shooting_macro(1); //shoot oppposite ball
      facePID(180,p,i,d);
      intake(inward);
      delayDrive(800,9000);
      while(!firstBall)
        pros::delay(10);
      shooting_macro(1); //score second goal
      pros::delay(200);
      intake(outward);
      driveDistance(-17,10);
      eject(1);
      facePID(270,p,i,d);
      //facePID(12,-108,p,i,d);
      intake(inward);
      adaptiveDrive(-84,-3, 8);
      delayDrive(400,8000);
      intake(outward);
      while(!firstBall)
        pros::delay(10);
      shooting_macro(1); //score third goal
      pros::delay(200);
      delayDrive(400,-8000);
    break;

    case 3: // skills auton
      robotTheta = M_PI/3;
      intake(inward);
      driveDistance(calcDistance(25,20),8);
      facePID(46,0,p,i,d);
      intake(stop);
      //driveDistance(calcDistance(46,0)-7,6);
      delayDrive(900,8000);
      shooting_macro(1);
      pros::delay(100);
      driveDistance(-20,8);
      intake(inward);
      facePID(32,58,p,i,d);
      adaptiveDrive(32,58,8);
      //driveDistance(calcDistance(34,64),8);
      facePID(46,63,p,i,d);
      //driveDistance(calcDistance(46,58)-7,6);
      intake(stop);
      delayDrive(400,8000);
      shooting_macro(2);
      driveDistance(-10,8);
      intake(inward);
      facePID(24,110,p,i,d);
      driveDistance(calcDistance(24,110)+6,8);
      driveDistance(-12,8);
      intake(stop);
      facePID(52,130,p,i,d);
      delayDrive(900,9000);
      shooting_macro(1);
      pros::delay(100);
      driveDistance(-10,8);
      intake(inward);
      facePID(-12,93,p,i,d);
      adaptiveDrive(-12,93, 8);
      //driveDistance(calcDistance(-12,84),8);
      facePID(-17.5,125,p,i,d);
      intake(stop);
      delayDrive(1150,8000);
      shooting_macro(1);
      pros::delay(100);
      driveDistance(-14,8);
      intake(inward);
      facePID(-62,118,p,i,d);
      driveDistance(calcDistance(-62,118),8);
      facePID(-72,125,p,i,d);
      intake(stop);
      delayDrive(800,8000);
      shooting_macro(1);
      pros::delay(100);
      driveDistance(-10,8);
      facePID(-40,58,p,i,d);
      intake(inward);
      driveDistance(calcDistance(-40,58),8);
      driveDistance(-7,10);
      facePID(-120,62,p,i,d);
      delayDrive(900,8000);
      intake(stop);
      delayDrive(300,8000);
      shooting_macro(1);
      pros::delay(100);
      driveDistance(-20,8);
      facePID(-58,17,p,i,d);
      intake(inward);
      driveDistance(calcDistance(-58,17),9);
      driveDistance(-6,9);
      facePID(-94,-14,p,i,d);
      intake(stop);
      delayDrive(1000,8000);
      shooting_macro(1);
      pros::delay(100);
      //driveDistance(-20,9);
      //facePID(90,p,i,d);
      adaptiveDrive_reversed(-34,22,8);
      facePID(-12,-1,p,i,d);
      delayDrive(1200,5000);
      shooting_macro(1);
      pros::delay(100);
      driveDistance(-10,9);
      intake(inward);
      adaptiveDrive(-24.2,23,5);
      facePID(-24.2,33,p,i,d);
      intake(outward);
      delayDrive(900,9000);
      shooting_macro(1);
      pros::delay(800);
      driveDistance(-20,8);
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
