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

double start_theta = 0.0;

void initialize()
{
  imu.reset();
  pros::Task task_1 (thread_sensors_v2, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
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
  db r = 5.645+4;
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

void release(int n)
{
  conveyorState = 7;

  topConveyor.move_velocity(-300);
  botConveyor.move_velocity(-300);
  intake(outward);
  countReleaseBalls(n);
  conveyorState = 0;
  pros::delay(250);
}

void release_thread(void* p) {
  pros::delay(250);
  release(1);
}

void release_thread2(void* p) {
  pros::delay(250);
  release(countHeldBalls()-1);
}
void shoot(int a){
    conveyorState = 3;
}

void thread_intake_sim(void*p)
{
  int timer = 0;
  intake(inward);
  while(!botBall){
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

// int ejectBalls = 0;
//
// void eject_thread(void*p)
// {
//   // for(int n = 0; n < ejectBalls; n++) {
//   //   waitForBallToEject();
//   // }
//   waitForBallsToEject(ejectBalls);
//   ejectBalls = 0;
//   conveyorState = 0;
// }
//
// void eject(int numBalls){
//   conveyorState = 7;
//   ejectBalls = numBalls;
//   pros::Task sub (eject_thread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
// }

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

void intake_delay(void*p)
{
  intake(stop);
  pros::delay(100);
  intake(outward);
}

void waitForBall(bool ball) {
  int atimer = 0;
  while(!ball) {
    pros::delay(10);
    atimer += 10;
    if(atimer > 2000)
      break;
  }
}

// void delayed_eject(void*p){
//   pros::delay(500);
//   eject(countHeldBalls());
// }

void unfold(){
  conveyorState = 7;
  topConveyor.move_velocity(100);
  botConveyor.move_velocity(100);
  pros::delay(250);
  conveyorState = 0;
}
//------------------------------------------------------
void autonomous()
{
  std::uint32_t t = pros::millis();
  pros::Task task_odometry (thread_Odometry, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task f (asynchShoot, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task a (intake_delay, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task r (release_thread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task r2 (release_thread2, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  f.suspend();
  a.suspend();
  r.suspend();
  r2.suspend();
  pros::delay(200);
  isRobotDisabled = false;
  driverControl = false;
  double p = 20;
  double i = 0;
  double d = 70;
  double tempTheta;
  double moveBack;
  std::vector<double> xPts;
  std::vector<double> yPts;
  std::vector<double> xPts1;
  std::vector<double> yPts1;
  std::vector<double> startXPts;
  std::vector<double> startYPts;
  std::vector<double> skills2ndX;
  std::vector<double> skills2ndY;
  std::vector<double> skills3rdX;
  std::vector<double> skills3rdY;
  pdd start_pos = std::make_pair(0,0);
  goals.push_back(std::make_pair(0,0));
  goals.push_back(std::make_pair(0,0));
  goals.push_back(std::make_pair(0,0));
  goals.push_back(std::make_pair(0,0));
  int timer = 0;
  //goals = repos_goals(goals,-53.375,13.25);
  updateVarLabel(debugLabel3,"R1",debugValue3,goals[0],"",3);
  updateVarLabel(debugLabel4,"R2",debugValue4,goals[1],"",3);
  updateVarLabel(debugLabel5,"R3",debugValue5,goals[2],"",3);
  switch(auton) {
    case 0: //no auton
    delayDriveSmooth(500,8,0.5,true);
    break;

    case 1: //red auton

    case 2: //blue auton
      //robotTheta = M_PI/2;
      start_theta = 3*M_PI/2;
      unfold();
      intake(inward);
      pointTurn(1,100,225,false,40,i,d);
      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      // pros::delay(200);
      if(thirdBall)
        super_macro(3,1); //score first goal
      else
        super_macro(3,2);
      intake(outward);
      r.resume();
      adaptiveDrive_reversed(34,16,9.5);
      facePID(180,p,i,d);
      intake(inward);
      delayDrive(800,9000);
      timer = 0;
      while(!secondBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 2000)
          break;
      }
      shooting_macro(1); //score second goal
      intake(outward);
      delayDriveSmooth(500,8,0.5,true);
      intake(inward);
      while(!firstBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }

      xPts.push_back(robotX);
      yPts.push_back(robotY);

      xPts.push_back(70);
      yPts.push_back(20);

      xPts.push_back(95);
      yPts.push_back(0);

      //eject(countHeldBalls());
      pros::delay(100);
      purePursuit(24,0,xPts,yPts,8,0.8,5.0,8.0,5000);

      delayDrive(500,8000);
      conveyorState = 99;
      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      intake(stop);
      shooting_macro(2); //score third goal
      intake(outward);
      delayDrive(400,-8000);
    break;

    case 3: //SKILLS ================================================================================================================================================================
      start_theta = 1.09694499; //M_PI/3;
      conveyorState = 7;
      topConveyor.move_velocity(600);
      pros::delay(400);
      conveyorState = 0; //lift hood, shoot ball into FIRST goal
      intake(inward);
      startXPts.push_back(robotX);
      startYPts.push_back(robotY);

      startXPts.push_back(24); //set points for pure pursuit
      startYPts.push_back(10);

      startXPts.push_back(44);
      startYPts.push_back(19);

      purePursuit(24,0,startXPts,startYPts,8.8,0.9,12.0,5000); //pure pursuit, pick 2 balls

      driveDistance(-16,11);
      intake(stop);

      facePID(57,-7,p,i,d); //face goal
      delayDriveSmooth(1050,7.2,0.3,fwd);
      super_macro(2, 2); // SCORE SECOND GOAL
      intake(outward);
      driveDistance(-13,10);
      //facePID(14,60,p,i,d); //face ball for middle goal
      intake(inward);
      ////eject(countHeldBalls());
      //e.resume();

      adaptiveDrive(14,57,0.2,8,0.5,5.0,1.0,250,10000);
      facePID(63,58,p,i,d); //face THIRD goal (right side)
      intake(inward);
      delayDriveSmooth(1350, 8.2, 0.4, fwd);
      reset(0);
      shooting_macro(2);  // score THIRD GOAL
      intake(outward);
      driveDistance(-11,10); //back out from goal
      intake(inward);
      //eject(countHeldBalls());

      adaptiveDrive(-30,46,0.6,8.9,0.7,7.3,1.0,250,10000);//intake floating ball
      facePID(0,35,p,i,d); //face wall ball
      //delayDriveSmooth(1300,6,.4,fwd);
      driveDistance(calcDistance(0,35)-13,8.5);
      driveDistance(-14,10); //get wall ball and drive back
      intake(stop);
      facePID(5,74,p,i,d); //face FOURTHh goal (right corner)
      driveDistance(calcDistance(10,68)-25,10);
      delayDriveSmooth(350, 8.5, 0.55, fwd);
      super_macro(countHeldBalls(),2); //score FOURTH GOAL
      intake(outward);
      driveDistance(-11,10);
      //facePID(-62,28,p,i,d); //face ball for FIFTH goal (far middle)
      intake(inward);
      //eject(countHeldBalls());
      adaptiveDrive(-66.25,25.5,0.4,8.6,0.6,5.0,1.0,250,10000); //get ball for FIFTH goal (far middle)
      facePID(-69,68,p,i,d);
      delayDriveSmooth(1300, 8, 0.4, fwd);
      reset(1);
      super_macro(1,1); //score FIFTH GOAL
      intake(outward);
      driveDistance(-18,10);
      ///facePID(-36,-15,p,i,d);
      skills2ndX.push_back(robotX);
      skills2ndY.push_back(robotY);

      skills2ndX.push_back(-36); //set points for pure pursuit
      skills2ndY.push_back(-18);

      skills2ndX.push_back(-44); //set points for pure pursuit
      skills2ndY.push_back(-16);

      skills2ndX.push_back(-55);
      skills2ndY.push_back(-10);
      intake(inward);
      //eject(countHeldBalls());
      purePursuit(24,0,skills2ndX,skills2ndY,8,0.7,10.0,3000); //anglekp was 12 //get ball for SIXTH Goal (far left corner)
      delayDriveSmooth(500,7.5,0.5,fwd);
      timer = 0;
      while(!firstBall)
        pros::delay(10);
        timer+= 10;
        if(timer > 1000)
          break;
      intake(stop);
      if(thirdBall)
        shooting_macro(1);
      if(secondBall)
        super_macro(1,1);
      if(firstBall)
        super_macro(1,2); //sore SIXTH Goal (far left corner)
      intake(outward);
      driveDistance(-14,10);
      facePID(-62,-30,p,i,d);
      intake(inward);
      //eject(countHeldBalls());

      driveDistance2(calcDistance(-62,-24)-5,0.5,3,10,0.8,1,250,1500); //grab wall ball
      delayDriveSmooth(600,9.2,0.5,rev);
      //driveDistance(-19,10);
      //facePID(-25,-70,p,i,d);
      intake(inward);
      adaptiveDrive(-20.5,-68,0.38,8.8,0.5,10,1.0,250,10000); //ball for SEVENTH (left side)
      driveDistance(-5,10);
      facePID(-100,-66,p,i,d);
      driveDistance(26,10);
      intake(stop);
      delayDriveSmooth(650,8.6,0.4,fwd);
      reset(2);
      super_macro(countHeldBalls(),1);  // SCORE 8TH GOAL (left side)
      intake(outward);
      driveDistance(-20,10);
      //facePID(25,-48,p,i,d);
      intake(inward);
      //eject(countHeldBalls());
      adaptiveDrive(34.5,-48,0.5,8.6,0.8,7.0,1.0,250,10000); //ball for 9th goal (left near corner)
      //driveDistance(calcDistance(25,-48)+3,10);
      driveDistance(-12,10);
      facePID(-5,-72.5,p,i,d);
      delayDriveSmooth(1200,8,0.5,fwd);
      super_macro(countHeldBalls(),2); //SCORE LEFT NEAR CORNER
      intake(outward);
      driveDistance(-20,10);
      intake(inward);
      //eject(countHeldBalls());
      adaptiveDrive(66,-24,0.5,8,0.5,5.0,1.0,250,10000);
      //driveDistance(-6,9);
      facePID(68,5,p,i,d);
      driveUntilStopped(7000);
      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      shooting_macro(1);    // SCORE MIDDLE
      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1500)
          break;
      }
      pros::delay(200);
      a.resume();
      driveDistance2(-18,0.9,3,10,5,1,250,5000);

      break;
    case 4: //red mid auton

    case 5: //blue mid auton

      bool worked = false;
      start_theta = 0;
      intake(inward);
      driveDistance2(44, 0.3, 0, 8, 0.5, 3, 250, 2000);
      delayDriveSmooth(250,8,0.5,rev);
      //driveDistance(-24,10);
      adaptiveDrive(-20,4,0.5,8,0.5,6.0,1.0,250,2000);
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      if(countHeldBalls()==3)
        worked = true;
      intake(stop);
      delayDriveSmooth(1000,5,0.5,fwd);
      if(worked)
        super_macro(3,2);
      else
        super_macro(2,2);
      r2.resume();
      adaptiveDrive_reversed(45,16,0.5,8,0.5,7.0,1.0,250,2000);
      release(countHeldBalls()-1);
      facePID(180,p,i,d);
      intake(inward);
      delayDriveSmooth(1000,7,0.5,fwd);
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      intake(stop);
      if(thirdBall)
        shooting_macro(2);
      else if(secondBall)
        shooting_macro(1);
      intake(outward);
      delayDriveSmooth(1000,8,0.5,rev);
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
