#include "main.h"

typedef double db;
typedef std::vector<db> vdb;
typedef std::vector<std::pair<db,db>> vpdb;

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

vpdb bezToLines(vdb xPts, vdb yPts, int numLines){
  std::vector<vpdb> bez;
  bez = multiBez(xPts,yPts);
  vpdb p0,p1,p2,p3;
  p0 = bez[0];
  p1 = bez[1];
  p2 = bez[2];
  p3 = bez[3];
  db dt = 1.0/(double)numLines;
  vpdb ret;
  for(int i = 0; i < p0.size(); i++){
    for(int j = 0; j < numLines; j++){
      db t = dt * j;
      db curX = bezX(p0[i].first,p1[i].first,p2[i].first,p3[i].first,t);
      db curY = bezY(p0[i].second,p1[i].second,p2[i].second,p3[i].second,t);
      ret.push_back(std::pair(curX,curY));
    }
    updateVarLabel(debugLabel1,"P0",debugValue1,p0[i],"",2);
    updateVarLabel(debugLabel2,"P1",debugValue2,p1[i],"",2);
    updateVarLabel(debugLabel3,"P2",debugValue3,p2[i],"",2);
    updateVarLabel(debugLabel4,"P3",debugValue4,p3[i],"",2);
    updateVarLabel(debugLabel5,"ITERATION",debugValue5,i,"",2);
    //pros::delay(5000);
  }
  return ret;
}
vpdb bezToLines(vdb xPts, vdb yPts, db theta, int numLines){
    std::vector<vpdb> bez;
    bez = multiBez(xPts,yPts,theta);
    vpdb p0,p1,p2,p3;
    p0 = bez[0];
    p1 = bez[1];
    p2 = bez[2];
    p3 = bez[3];
    db dt = 1.0/(double)numLines;
    vpdb ret;
    for(int i = 0; i < p0.size(); i++){
      for(int j = 0; j < numLines; j++){
        db t = dt * j;
        db curX = bezX(p0[i].first,p1[i].first,p2[i].first,p3[i].first,t);
        db curY = bezY(p0[i].second,p1[i].second,p2[i].second,p3[i].second,t);
        ret.push_back(std::pair(curX,curY));
      }
      updateVarLabel(debugLabel1,"P0",debugValue1,p0[i],"",2);
      updateVarLabel(debugLabel2,"P1",debugValue2,p1[i],"",2);
      updateVarLabel(debugLabel3,"P2",debugValue3,p2[i],"",2);
      updateVarLabel(debugLabel4,"P3",debugValue4,p3[i],"",2);
      updateVarLabel(debugLabel5,"ITERATION",debugValue5,i,"",2);
      //pros::delay(5000);
    }
    return ret;
}

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
  std::vector<double> xPts;
  std::vector<double> yPts;

  // xPts.push_back(robotX);
  // yPts.push_back(robotY);

  xPts.push_back(0.0);
  yPts.push_back(0.0);

  xPts.push_back(0.0);
  yPts.push_back(24.0);

  xPts.push_back(24.0);
  yPts.push_back(48.0);

  xPts.push_back(24.0);
  yPts.push_back(72.0);


  vdb ptsX;
  vdb ptsY;
  vpdb temp = bezToLines(xPts,yPts,10);
  for(int i = 0; i < temp.size(); i++){
    ptsX.push_back(temp[i].first);
    ptsY.push_back(temp[i].second);
  }
  switch(auton) {
    case 0: //no auton
      purePursuit(12,0,ptsX,ptsX,6,0.5,10,5000);
      break;

    case 1: //red auton
      robotTheta = M_PI/3;
      intake(inward);
      pointTurn(0,190,135,false,40,i,d);
      while(!secondBall)
        pros::delay(10);
      intake(stop);
      delayDrive(400,8000);
      //waitForThirdBall();
      // while(!thirdBall)
      //   pros::delay(10);
      // pros::delay(200);
      // super_macro(countHeldBalls(), 1); //score first goal
      shooting_macro(1);
      pros::delay(200);
      intake(outward);
      adaptiveDrive_reversed(-32,16,9.5);
      //shooting_macro(1); //shoot oppposite ball
      //f.resume();
      facePID(180,p,i,d);
      intake(inward);
      delayDrive(800,9000);
      // while(!firstBall)
      //   pros::delay(10);
      while(!thirdBall)
        pros::delay(10);
      pros::delay(200);
      shooting_macro(2); //score second goal
      intake(stop);
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
    pointTurn(0,190,135,false,40,i,d);
    delayDrive(400,8000);
    //waitForThirdBall();
    while(!thirdBall)
      pros::delay(10);
    pros::delay(200);
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
    //eject(1);
    // facePID(270,p,i,d);
    // //facePID(12,-108,p,i,d);
    // intake(inward);
    // adaptiveDrive(-84,-3, 8);
    // delayDrive(400,8000);
    // intake(outward);
    // conveyorState = 99;
    // while(!firstBall)
    //   pros::delay(10);
    // shooting_macro(1); //score third goal
    // //pros::delay(200);
    // delayDrive(400,-8000);
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
      shooting_macro(2);
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
      facePID(-100,-12,p,i,d);
      intake(stop);

      delayDrive(1000,8000);
      shooting_macro(1);
      pros::delay(100);
      ///???
      driveDistance(-30,9);
      facePID(-18,33,p,i,d);
      intake(inward);
      driveDistance(calcDistance(-18, 3),9);
      facePID(-18,-10,p,i,d);
      intake(stop);
      delayDrive(1500, 8000);
      shooting_macro(2);
      delayDrive(400,-8000);
      //driveDistance(-20,9);
      // //facePID(90,p,i,d);
      // adaptiveDrive_reversed(-22,16,8);
      // facePID(-22,-1,p,i,d);
      // delayDrive(1200,5000);
      // shooting_macro(1);
      // pros::delay(100);
      // driveDistance(-10,9);
      // intake(inward);
      // facePID(-16,33,p,i,d);
      // driveDistance(calcDistance(-16,33),8);
      // intake(inward);
      // delayDrive(900,9000);
      // delayTurn(600,8000,-2000);
      // delayDrive(450,9000);
      // shooting_macro(1);
      // pros::delay(800);
      // intake(outward);
      // delayDrive(750,-9000);
      updateVarLabel(debugLabel1, "TEMP X",debugValue1,robotX,"IN",3);
      updateVarLabel(debugLabel2, "TEMP Y",debugValue2,robotY,"IN",3);
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
