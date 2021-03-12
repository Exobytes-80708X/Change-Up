#include "main.h"

typedef double db;
typedef std::vector<db> vdb;
typedef std::vector<std::pair<db,db>> vpdb;
typedef std::pair<double,double> pdd;

bool isRobotDisabled = true;
bool driverControl = false;
int red[] = { 1,4 };
size_t redSize = sizeof(red) / sizeof(int);
int *redEnd = red + redSize;
int blue[] = { 2,5 };
size_t blueSize = sizeof(blue) / sizeof(int);
int *blueEnd = blue + blueSize;

vpdb goals;

void initialize()
{
  imu.reset();
  pros::c::ext_adi_pin_mode(5, 'A', INPUT_ANALOG);
  pros::c::ext_adi_pin_mode(5, 'B', INPUT_ANALOG);
  pros::c::ext_adi_pin_mode(5, 'C', INPUT_ANALOG);
  //pros::Task task_1 (thread_sensors, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task task_1 (thread_sensors_filter, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::delay(250);
  pros::Task task_GUI (initGUI, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task task_2 (thread_subsystems, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}

void disabled(){}

void competition_initialize(){}

void delay_drive_thread(void*p)
{
  delayDrive(500,5000);
}

int position;
void reset_thread(void*p)
{
  pros::delay(250);
  db r = 5.645+4.125;
  pdd xy = repos_robot(goals,r,robotTheta,position);
  robotX = xy.first;
  robotY = xy.second;
}

void reset(int p)
{
  position = p;
  pros::Task sub (delay_drive_thread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task sub2 (reset_thread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}

void intake(int state){
  // state - What intake state to switch to. in = 0, out = 1, stop = 2
  intakeState = state;
}

void shoot(int a){
    conveyorState = 3;
}

void thread_intake_sim(void*p)
{
  int timer = 0;
  intake(inward);
  while(!botBall_low){
    pros::delay(10);
    timer += 10;
    if(timer > 2000)
      break;
    }
  intake(stop);
}

void intake_sim()
{
  pros::Task task_2 (thread_subsystems, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}

int ejectBalls = 0;

void eject_thread(void*p)
{
  // for(int n = 0; n < ejectBalls; n++) {
  //   waitForBallToEject();
  // }
  waitForBallsToEject(ejectBalls);
  ejectBalls = 0;
  conveyorState = 0;
}

void eject(int numBalls){
  conveyorState = 7;
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
  std::vector<double> xPts1;
  std::vector<double> yPts1;
  pdd start_pos = std::make_pair(-53.375,13.25);
  goals.push_back(std::make_pair(-5.645,70.25));                // GOAL 1
  goals.push_back(std::make_pair(-70.25,134.855));              // GOAL 2
  goals.push_back(std::make_pair(-134.855,70.25));              // GOAL 3
  goals.push_back(std::make_pair(-70.25,5.645));                // GOAL 4
  int timer = 0;
  goals = repos_goals(goals,-53.375,13.25);
  updateVarLabel(debugLabel3,"R1",debugValue3,goals[0],"",3);
  updateVarLabel(debugLabel4,"R2",debugValue4,goals[1],"",3);
  updateVarLabel(debugLabel5,"R3",debugValue5,goals[2],"",3);

  // xPts.push_back(robotX);
  // yPts.push_back(robotY);

  // xPts.push_back(0.0);
  // yPts.push_back(0.0);
  //
  // xPts.push_back(0.0);
  // yPts.push_back(24.0);
  //
  // xPts.push_back(24.0);
  // yPts.push_back(48.0);
  //
  // xPts.push_back(24.0);
  // yPts.push_back(72.0);
  //
  //
  // vdb ptsX;
  // vdb ptsY;
  // vpdb temp = bezToLines(xPts,yPts,10);
  // for(int i = 0; i < temp.size(); i++){
  //   ptsX.push_back(temp[i].first);
  //   ptsY.push_back(temp[i].second);
  // }
  switch(auton) {
    case 0: //no auton
      //super_macro(2,2);
      //purePursuit(24,0,ptsX,ptsY,8,0.5,10.0,5000);
      //driveUntilStopped(8000);
      //shoot(1);
      //delayDriveSmooth(2000,8,0.3,fwd);
      // intake(inward);
      // timer = 0;
      // while(!thirdBall) {
      //   pros::delay(10);
      //   timer += 10;
      //   if(timer > 2000)
      //     break;
      // }
      //super_macro2(2,2); //score second goal middle
      //delayDrive(1000,3000);
      eject(1);
      break;

    case 1: //red auton

    case 2: //blue auton
      robotTheta = M_PI/2;
      //robotX = 12.0;
      //robotY = 6.0;
      intake(inward);
      pointTurn(0,300,135,false,40,i,d);
      // xPts1.push_back(robotX);
      // yPts1.push_back(robotY);
      // xPts1.push_back(24*sin(robotTheta));
      // yPts1.push_back(24*cos(robotTheta));
      // xPts1.push_back(22);
      // yPts1.push_back(robotY);
      //
      // purePursuit(3,0,xPts1,yPts1,6,0.6,15.0,3000);

      timer = 0;
      while(!secondBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      //delayDrive(400,8000);
      //driveUntilStopped(8000);
      //waitForThirdBall();
      //intake(stop);
      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      // pros::delay(200);
      super_macro(countHeldBalls(), 1); //score first goal
      //shooting_macro(2);
      //pros::delay(200);
      intake(outward);
      adaptiveDrive_reversed(-36,16,9.5);
      //shooting_macro(1); //shoot oppposite ball
      f.resume();
      facePID(180,p,i,d);
      intake(inward);
      delayDrive(800,9000);
      //driveUntilStopped(8000);
      timer = 0;
      while(!firstBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      // while(!thirdBall)
      //   pros::delay(10);
      //pros::delay(200);
      // while(!secondBall) {
      //   timer += 10;
      //   pros::delay(10);
      //   if(timer > 500) break;
      // }
      shooting_macro(1); //score second goal
      intake(stop);
      pros::delay(200);
      intake(outward);
      driveDistance(-17,10);
      if(countHeldBalls() == 1)
        eject(1);
      facePID(270,p,i,d);
      //facePID(12,-108,p,i,d);
      intake(inward);
      //adaptiveDrive(-84,-3, 8);

      xPts.push_back(robotX);
      yPts.push_back(robotY);

      xPts.push_back(-80);
      yPts.push_back(robotY);

      xPts.push_back(-100);
      yPts.push_back(-6);

      purePursuit(24,0,xPts,yPts,8,0.5,12.0,5000);

      delayDrive(500,8000);
      //driveUntilStopped(8000);

      //intake(outward);
      conveyorState = 99;
      // while(!botBall_low && !firstBall && !botBall)
      //   pros::delay(10);
      //intake(outward);
      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      while(!firstBall){
        pros::delay(10);
      }
      shooting_macro(2); //score third goal
      intake(outward);
      //pros::delay(200);
      delayDrive(400,-8000);
    break;

    case 3: // skills auton
      robotTheta = 1.09694499; //M_PI/3;
      intake(inward);
      //adaptiveDrive(25, 20, 8);
      driveDistance(calcDistance(25,20)-9,6);
      facePID(48,-4,p,i,d);
      intake(outward);
      //driveDistance(calcDistance(46,0)-7,6);
      delayDrive(950,6000);
      //shooting_macro2(1); //score first goal corner
      super_macro2(2,2);
      //pros::delay(100);
      //intake(outward);
      intake(outward);
      driveDistance(-20,7);
      intake(inward);
      facePID(13,62,p,i,d);
      eject(2);
      adaptiveDrive(13,60.5,8);
      facePID(-10,robotY+3.5,p,i,d);
      //delayDrive(2500,5000);
      driveUntilStopped(5000);
      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 2000)
          break;
      }
      super_macro2(1,1); //score second goal middle
      pros::delay(300);
      intake(outward);
      delayDrive(500,-7000);
      intake(inward);
      facePID(robotX-25,120,p,i,d);
      eject(3); //eject middle balls
      //waitForBallToEject();
      waitForBallToEject();
      waitForBallToEject();
      waitForBallToEject();
      //waitForBallToEject();
      intake(inward);
      //adaptiveDrive(32,robotY,8);
      //adaptiveDrive(32,robotY,0.2,7,0.6,10.0,1.0,250,10000);
      //adaptiveDrive(32,robotY,8); //face third goal side
      //driveDistance(calcDistance(34,64),8);
      facePID(46,robotY-2,p,i,d);
      //driveDistance(calcDistance(46,58)-7,6);\
      intake_sim();
      delayDriveSmooth(1400,7.2,0.3,fwd);
      //shooting_macro2(1); //score third goal side
      timer = 0;
      while(!firstBall){
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
        }
      reset(0);
      super_macro2(1,1); //score 3rd goals
      intake(outward);
      driveDistance(-10,7.6);
      eject(2);
      intake(inward);
      adaptiveDrive(21.5,104,0.2,7,0.6,10.0,1.0,250,10000);
      //facePID(21,110,p,i,d)
      //driveDistance(calcDistance(17,110)+6,8);
      driveDistance(-11,8);
      intake(stop);
      facePID(56,125,p,i,d);
      //delayDrive(1000,8200);
      delayDriveSmooth(1200,8.3,0.3,fwd);
      //shooting_macro2(1); //score fourth goal corner

      timer = 0;
      while(!firstBall){
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
        }
      super_macro2(1,2); //score 4th goal
      intake(outward);
      driveDistance(-10,9);
      intake(inward);
      //facePID(-22,91,p,i,d);
      eject(2);
      adaptiveDrive(-21,87,0.2,8,0.7,7.0,1.0,250,10000);
      //adaptiveDrive(-22,90, 8);
      driveDistance(-5,9);
      //driveDistance(calcDistance(-12,84),8);
      facePID(-15.5,125,p,i,d);
      intake(stop);
      //delayDrive(1150,8000);
      delayDriveSmooth(1300,9,0.2,fwd);
      //shooting_macro2(2); //score fifth goal side
      timer = 0;
      while(!firstBall){
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
        }
      reset(1);
      super_macro2(1,1); //score 5th goal
      intake(outward);
      driveDistance(-20,8);
      intake(inward);
      //facePID(-62,118,p,i,d);
      //driveDistance(calcDistance(-62,118),8);
      eject(1);
      adaptiveDrive(-60,109,0.2,7,0.6,8.0,1.0,250,10000);
      facePID(-93,132,p,i,d);
      intake(stop);
      delayDriveSmooth(1100,8.5,0.3,fwd);
      //shooting_macro2(1); //score 6th goal corner
      timer = 0;
      while(!firstBall){
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      super_macro(1,2); //score 6th goal
      intake(outward);
      driveDistance(-10,9.5);
      facePID(-47,52,p,i,d);
      eject(2);
      intake(inward);
      driveDistance(calcDistance(-46,62)+9,8);
      driveDistance(-8,10);
      facePID(-120,61,p,i,d);
      delayDriveSmooth(1300,8,0.3,fwd);
      //intake(stop);
      //delayDrive(500,8000);
      //shooting_macro2(1); //7th goal side
      timer = 0;
      while(!firstBall){
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
        }
      reset(2);
      super_macro2(1,1); //score 7th goals
      intake(outward);
      driveDistance(-20,9);
      intake(inward);
      //facePID(-58,20,p,i,d);
      pros::delay(50);
      eject(1);
      adaptiveDrive(-57,8,0.2,7,0.6,8.0,1.0,250,10000);
      //driveDistance(calcDistance(-60,20),9);
      driveDistance(-5,9);
      facePID(-89,-15,p,i,d);
      intake(stop);

      delayDriveSmooth(1150,9,0.3,fwd);
      timer = 0;
      while(!firstBall){
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      //shooting_macro2(1); //8th goal
      super_macro2(1,2); //score 8th goals
      intake(outward);
      driveDistance(-15,9);
      intake(inward);
      eject(2);
      adaptiveDrive(-19,21,0.2,7,0.6,7.0,1.0,250,10000);
      facePID(-19,-10,p,i,d);
      intake(stop);
      delayDriveSmooth(1300,8.5,0.3,fwd);
      timer = 0;
      while(!firstBall){
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
        }
      shooting_macro2(2); //last goal
      //delayDrive(400,-8000);
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
    case 4: //red mid auton

    case 5: //blue mid auton
      robotTheta = M_PI/2;
      //robotX = 12.0;
      //robotY = 6.0;
      intake(inward);
      pointTurn(0,300,135,false,40,i,d);
      // xPts1.push_back(robotX);
      // yPts1.push_back(robotY);
      // xPts1.push_back(24*sin(robotTheta));
      // yPts1.push_back(24*cos(robotTheta));
      // xPts1.push_back(22);
      // yPts1.push_back(robotY);
      //
      // purePursuit(3,0,xPts1,yPts1,6,0.6,15.0,3000);

      timer = 0;
      while(!secondBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      //delayDrive(400,8000);
      //driveUntilStopped(8000);
      //waitForThirdBall();
      //intake(stop);
      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      // pros::delay(200);
      super_macro(2, 1); //score first goal
      //shooting_macro(2);
      intake(stop);
      driveDistance(-10,8);
      intake(inward);
      adaptiveDrive_reversed(-36,16,9.5);
      //shooting_macro(1); //shoot oppposite ball
      facePID(-40,-40,p,i,d);
      intake(stop);
      //intake(inward);
      delayDriveSmooth(1200,8,0.2,fwd);
      //driveUntilStopped(8000);
      timer = 0;
      while(!firstBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      // while(!thirdBall)
      //   pros::delay(10);
      //pros::delay(200);
      // while(!secondBall) {
      //   timer += 10;
      //   pros::delay(10);
      //   if(timer > 500) break;
      // }
      super_macro(1,2); //score second goal
      intake(stop);
      pros::delay(200);
      intake(outward);
      driveDistance(-17,10);
      facePID(140,p,i,d);
      shooting_macro(1);
      //facePID(12,-108,p,i,d);
      intake(inward);
      facePID(-38,50,p,i,d);
      //adaptiveDrive(-42,40,6);
      driveDistance(calcDistance(-42,40)-5,8);
      driveUntilStopped(5000);
      shooting_macro(1);
      pros::delay(200);
      intake(outward);
      driveDistance(-12,8);
      // //adaptiveDrive(-84,-3, 8);

      //
      // xPts.push_back(robotX);
      // yPts.push_back(robotY);
      //
      // xPts.push_back(-80);
      // yPts.push_back(robotY);
      //
      // xPts.push_back(-100);
      // yPts.push_back(-6);
      //
      // purePursuit(24,0,xPts,yPts,8,0.5,12.0,5000);
      //
      // delayDrive(500,8000);
      // //driveUntilStopped(8000);
      //
      // //intake(outward);
      // conveyorState = 99;
      // // while(!botBall_low && !firstBall && !botBall)
      // //   pros::delay(10);
      // //intake(outward);
      // timer = 0;
      // while(!thirdBall) {
      //   pros::delay(10);
      //   timer += 10;
      //   if(timer > 1000)
      //     break;
      // }
      // shooting_macro(2); //score third goal
      // intake(outward);
      // //pros::delay(200);
      // delayDrive(400,-8000);
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
