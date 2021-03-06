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

void adjustMiddle(void*p)
{
  double v_l;
  double v_r;
  while(true) {
    v_l = fabs(leftIntake.get_actual_velocity());
    v_r = fabs(rightIntake.get_actual_velocity());

    if (v_r < 200 && v_l < 200){
      rightDrive.moveVoltage(3000);
      leftDrive.moveVoltage(3000);
    }
    else if(v_l < v_r-100 ) {
      rightDrive.moveVoltage(0);
      leftDrive.moveVoltage(4000);
    }
    else if(v_r < v_l-100){
      rightDrive.moveVoltage(4000);
      leftDrive.moveVoltage(0);
    }
    else {
      rightDrive.moveVoltage(0);
      leftDrive.moveVoltage(0);
    }
    pros::delay(50);
  }
}

void release(int n)
{
  conveyorState = 7;

  topConveyor.move_velocity(-600);
  botConveyor.move_velocity(-600);
  intake(outward);
  countReleaseBalls(n);
  botConveyor.move_velocity(600);
  //pros::delay(250);
  pros::delay(250);
  conveyorState = 0;
}

void release_slowed(int n)
{
  conveyorState = 7;

  topConveyor.move_velocity(-600);
  botConveyor.move_velocity(-300);
  intake(outward);
  countReleaseBalls(n);
  botConveyor.move_velocity(600);
  //pros::delay(250);
  pros::delay(250);
  conveyorState = 0;
}

int rd_balls;
int r_delay;
void release_all_delayed_thread(void*p)
{
  pros::delay(r_delay);
  release(countHeldBalls());
  intake(inward);
}

void release_all_asynch(int d)
{
  r_delay = d;
  pros::Task tas (release_all_delayed_thread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}

void release_thread(void* p) {
  pros::delay(250);
  release_slowed(1);
}

void release_thread2(void* p) {
  pros::delay(500);
  release_slowed(countHeldBalls()-1);
}
void shoot(int a){
    conveyorState = 3;
}

int r_ball = 0;
void release_thread_count(void*p) {
  release(r_ball);
}

void release_asynch(int r) {
  r_ball = r;
  pros::delay(250);
  pros::Task tas (release_thread_count, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}

// void release_all_asynch(int deelay) {
//   pros::delay(deelay);
//   r_ball = countHeldBalls();
//   pros::Task tas (release_thread_count, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
// }

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
  pros::delay(250);
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
  botConveyor.move_velocity(-250);
  pros::delay(500);
  conveyorState = 0;
}

void unfold_thread(void*p) {
  unfold();
}
//------------------------------------------------------
bool mid_ball_grabbed = false;
void check_mid_ball_grabbed(void*p) {
  int mtimer = 0;
  pros::delay(100);
  while(!botBall){
    pros::delay(10);
    mtimer += 10;
    if(mtimer > 2500) return;
  }
  mid_ball_grabbed = true;
}

void autonomous()
{
  pros::Task task_odometry (thread_Odometry, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  switch(auton) {
    case 0:
      start_theta = 0;
    break;
    case 1:
    case 2:
      start_theta = 3*M_PI/2;
    break;
    case 3:
      start_theta = 1.09694499;
    break;
    case 4:
    case 5:
      start_theta = 0;
    break;
  }
  pros::delay(200);
  std::uint32_t t = pros::millis();
  pros::Task f (asynchShoot, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task a (intake_delay, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task r (release_thread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task r2 (release_thread2, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task cm (check_mid_ball_grabbed, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task am (adjustMiddle, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  //pros::Task u (unfold_thread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  f.suspend();
  a.suspend();
  r.suspend();
  r2.suspend();
  am.suspend();
  cm.suspend();
  //pros::delay(100);
  isRobotDisabled = false;
  driverControl = false;
  double p = 18;
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
  // updateVarLabel(debugLabel3,"R1",debugValue3,goals[0],"",3);
  // updateVarLabel(debugLabel4,"R2",debugValue4,goals[1],"",3);
  // updateVarLabel(debugLabel5,"R3",debugValue5,goals[2],"",3);
  switch(auton) {
    case 0: //no auton
    intake(inward);
    driveUntilStopped(5000);
    intake(outward);
    delayDrive(150,-8000);
    intake(inward);
    am.resume();
    timer = 0;
    while(!thirdBall){
      pros::delay(10);
      timer+= 10;
      if(timer > 1000)
        break;
      }
    am.suspend();
    shooting_macro(1);
    timer = 0;
    while(!thirdBall){
      pros::delay(10);
      timer+= 10;
      if(timer > 1000)
        break;
      }
    a.resume();
    driveDistance(-20,12);
    release(countHeldBalls());

    break;

    case 1: //red auton

    case 2: //blue auton
      //unfold();
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
        super_macro_slowed(2,1); //score first goal
      else
        super_macro_slowed(2,2);
      intake(outward);
      r.resume();

      adaptiveDrive_reversed(32,10,0.5,12,0.5,8.0,10.0,100,2000);
      //release(countHeldBalls()-1);
      //facePID(180,p,i,d);
      intake(inward);
      //delayDriveSmooth(1000,8,0.5,fwd);
      adaptiveDrive(36,-15,0.5,12,0.6,7.0,10.0,100,1000);
      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      if(thirdBall || secondBall)
        shooting_macro(2);
      else
        shooting_macro(1);
      // if(thirdBall || secondBall)
      //   shooting_macro(2);
      // else
      //   shooting_macro(1);
      intake(outward);
      driveDistance(-20,12);

      intake(inward);
      //purePursuit(24,0,xPts,yPts,8,0.8,6.5,8.0,5000);

      adaptiveDrive(108,-13,0.4,12,0.5,6.0,10.0,100,2000);
      //conveyorState = 99;
      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      intake(stop);
      if(firstBall)
        super_macro(3,2); //score third goal
      else
        shooting_macro(2);
      intake(outward);
      delayDrive(400,-8000);


      // adaptiveDrive_reversed(34,16,9.5);
      // facePID(180,p,i,d);
      // intake(inward);
      // delayDrive(800,9000);
      // timer = 0;
      // while(!secondBall) {
      //   pros::delay(10);
      //   timer += 10;
      //   if(timer > 2000)
      //     break;
      // }
      // shooting_macro(1); //score second goal
      // intake(outward);
      // delayDriveSmooth(500,8,0.5,true);
      // intake(inward);
      // while(!firstBall) {
      //   pros::delay(10);
      //   timer += 10;
      //   if(timer > 1000)
      //     break;
      // }
      //
      // xPts.push_back(robotX);
      // yPts.push_back(robotY);
      //
      // xPts.push_back(70);
      // yPts.push_back(20);
      //
      // xPts.push_back(95);
      // yPts.push_back(0);
      //
      // //eject(countHeldBalls());
      // pros::delay(100);
      // purePursuit(24,0,xPts,yPts,8,0.8,5.0,8.0,5000);
      //
      // delayDrive(500,8000);
      // conveyorState = 99;
      // timer = 0;
      // while(!thirdBall) {
      //   pros::delay(10);
      //   timer += 10;
      //   if(timer > 1000)
      //     break;
      // }
      // intake(stop);
      // shooting_macro(2); //score third goal
      // intake(outward);
      // delayDrive(400,-8000);
    break;

    case 3: //SKILLS ================================================================================================================================================================
      intake(inward);
      // startXPts.push_back(robotX);
      // startYPts.push_back(robotY);
      //
      // startXPts.push_back(24); //set points for pure pursuit
      // startYPts.push_back(10);
      //
      // startXPts.push_back(36);
      // startYPts.push_back(24);

      //purePursuit(24,1.0,startXPts,startYPts,12,0.3,12.0,5000); //pure pursuit, pick 2 balls

      // driveDistance(24,12,1000);
      // facePID(45,18,p,i,d);
      // driveDistance(calcDistance(45,18),12,1000);
      //

      adaptiveDrive(43,18,0.5,12,0.5,12.0,1.0,100,2000);
      driveDistance(-11,12);
      intake(stop);
      adaptiveDrive(57,-7,0.5,12,0.5,12.0,10.0,100,1100); //approach  1st
      //facePID(57,-7,p-2,i,d); //face goal
      //delayDriveSmooth(1050,7.2,0.3,fwd);
      //driveDistance(calcDistance(57,-7)-8,12,1000);
      super_macro_slowed(countHeldBalls(), 2); // SCORE FIRST GOAL
      intake(outward);
      //driveDistance(-5,10);
      release_all_asynch(400);
      driveDistance(-16,12);
      facePID(19,51,p-2,i,d);
      driveDistance(calcDistance(14,51)+2,12);
      adaptiveDrive(14,51,0.7,12, 0.5, 5, 1.0,100,10000); //approach middle ball
      //facePID(63,55,p,i,d);
      //delayDriveSmooth(1250, 9, 0.5, fwd);
      //driveDistance(calcDistance(63,56)-15,12,1250);
      adaptiveDrive(65,54,0.5,12,0.2,5.0,10.0,100,1500); //approach 2nd goal

      reset(0);
      timer = 0;
      while(!thirdBall){
        pros::delay(10);
        timer+= 10;
        if(timer > 1000)
          break;
        }
      shooting_macro(countHeldBalls()-1);  // score SECOND GOAL
      intake(outward);
      driveDistance(-14,10); //back out from goal
      release_all_asynch(0);
      delay_turn(8000,200,1);
      pros::delay(250);
      intake(inward);
      //facePID(-29,48,p,i,d);
      //driveDistance(calcDistance(-29,48)-5,10);
      adaptiveDrive(-24,38,0.4,12,0.7,5.0,10.0,100,10000); //intake floating ball 3rd goal
      facePID(0,36,p-4,i,d); // face WALL BALL 1
      driveDistance(calcDistance(0,36)-5,10);
      //adaptiveDrive(0,34,0.3,12,0.5,8,10.0,100,2000);//intake wall  ball

      driveDistance(-11,10);
      intake(stop);
      adaptiveDrive(10,66,0.75,12,0.7,6.0,10.0,100,1000);// score 3rd goal
      super_macro_slowed(countHeldBalls(),2);
      intake(outward);
      release_all_asynch(400);
      driveDistance(-23,10);
      //intake(inward);
      adaptiveDrive(-55,29,0.4,12,0.7,4.0,10.0,250,10000); //get ball for 4th goal


      //adaptiveDrive(-58.5,23.5,0.6,9.3,0.7,7.5,20.0,100,2300);//intake floating ball
      //facePID(-60,57,p,i,d);
      //driveDistance(calcDistance(-61.5,57),10);
      adaptiveDrive(-64,59,0.4,12,0.4,5.0,10.0,100,2000);// goto 4th goal
      reset(1);
      timer = 0;
      while(!secondBall){
        pros::delay(10);
        timer+= 10;
        if(timer > 1000)
          break;
        }
      shooting_macro(1);
      intake(outward);
      driveDistance(-10,10);
      release_all_asynch(0);
      delay_turn(8000,200,1);
      pros::delay(350);
      ///facePID(-36,-15,p,i,d);
      intake(inward);
      //facePID(-34,-15,p,i,d);
      //driveDistance(calcDistance(-34,-15),10);
      adaptiveDrive(-34,-14,0.5,12,0.5,5.0,10.0,100,3700); //intake floating ball 5th goal
      facePID(-68,-31,p,i,d);
      //adaptiveDrive(-68,-40,0.1,8,0.7,9.0,5.0,100,1400);//intake floating ball 5th goal
      //facePID(-61,-39,p,i,d);
      driveDistance(calcDistance(-68,-31)-10,10); //intake wall ball 5th goal
      driveDistance(-13,12);
      intake(stop);
      adaptiveDrive(-68,0,0.5,12,0.5,7,10.0,100,1200);//approach 5th goal
      super_macro_slowed(countHeldBalls(),2);
      release_all_asynch(400);
      driveDistance(-20,12);

      // intake(outward);
      // release_all_asynch(0);
      // driveDistance(-14,10);
      // driveDistance(-10,10);
      adaptiveDrive(-28,-68,0.5,12,0.5,5.0,10.0,100,2000);//intake floating ball
      //driveDistance(-10,12);
      adaptiveDrive(-72,-64.5,0.5,12,0.3,5.0,10.0,100,1450); //approach 6th goal
      intake(inward);
      timer = 0;
      while(!thirdBall){
        pros::delay(10);
        timer+= 10;
        if(timer > 1000)
          break;
        }
      reset(2);
      intake(stop);
      shooting_macro(countHeldBalls()-1); //score 6th goal
      intake(outward);
      driveDistance(-20,12);
      release_all_asynch(0);

      delay_turn(8000,200,1);
      adaptiveDrive(24,-48,0.5,12,0.5,5.0,10.0,100,2000);//intake floating ball for 7th (near left corner)
      facePID(0,-36,p-6,i,d);
      driveDistance(calcDistance(0,-36)-8,12); //intake wall ball
      driveDistance(-10,12);
      intake(stop);
      adaptiveDrive(-9,-72.5,0.5,12,0.5,5.5,10.0,100,1400); //approach 7th goal
      super_macro_slowed(countHeldBalls(),2); //score 7th goal
      intake(outward);
      release_all_asynch(300);
      driveDistance(-22,12);
      intake(inward);
      adaptiveDrive(64,-30,0.5,12,0.5,5.0,10.0,100,2000); //get floating ball for 8th


      intake(outward);
      facePID(67,0,p,i,d);
      intake(inward);

      driveUntilStopped(5000);
      intake(outward);
      delayDrive(150,-8000);
      intake(inward);
      am.resume();
      timer = 0;
      while(!thirdBall){
        pros::delay(10);
        timer+= 10;
        if(timer > 1000)
          break;
        }
      am.suspend();
      leftDrive.moveVoltage(0);
      rightDrive.moveVoltage(0);
      shooting_macro(1);
      timer = 0;
      while(!thirdBall){
        pros::delay(10);
        timer+= 10;
        if(timer > 1000)
          break;
        }
      a.resume();
      driveDistance(-24,12);
      release(countHeldBalls());

      facePID(61.5,-75,p,i,d);
      intake(inward);
      driveDistance(calcDistance(64,-77)-1,12, 1200);

      shooting_macro(1);
      //release_asynch(400);
      //driveDistance(-20,12);
      //r.resume();
      intake(outward);
      driveDistance(-20,12);
      //eject(countHeldBalls());



      // intake(inward);
      // //eject(countHeldBalls());
      //
      // adaptiveDrive(-30,46,0.6,8.9,0.7,7.3,1.0,250,10000);//intake floating ball
      // facePID(0,35,p,i,d); //face wall ball
      // //delayDriveSmooth(1300,6,.4,fwd);
      // driveDistance(calcDistance(0,35)-13,8.5);
      // driveDistance(-14,10); //get wall ball and drive back
      // intake(stop);
      // facePID(5,74,p,i,d); //face FOURTHh goal (right corner)
      // driveDistance(calcDistance(10,68)-25,10);
      // delayDriveSmooth(350, 8.5, 0.55, fwd);
      // super_macro(countHeldBalls(),2); //score FOURTH GOAL
      // intake(outward);
      // driveDistance(-11,10);
      // //facePID(-62,28,p,i,d); //face ball for FIFTH goal (far middle)
      // intake(inward);
      // //eject(countHeldBalls());
      // adaptiveDrive(-66.25,25.5,0.4,8.6,0.6,5.0,1.0,250,10000); //get ball for FIFTH goal (far middle)
      // facePID(-69,68,p,i,d);
      // delayDriveSmooth(1300, 8, 0.4, fwd);
      // reset(1);
      // super_macro(1,1); //score FIFTH GOAL
      // intake(outward);
      // driveDistance(-18,10);
      // ///facePID(-36,-15,p,i,d);
      // skills2ndX.push_back(robotX);
      // skills2ndY.push_back(robotY);
      //
      // skills2ndX.push_back(-36); //set points for pure pursuit
      // skills2ndY.push_back(-18);
      //
      // skills2ndX.push_back(-44); //set points for pure pursuit
      // skills2ndY.push_back(-16);
      //
      // skills2ndX.push_back(-55);
      // skills2ndY.push_back(-10);
      // intake(inward);
      // //eject(countHeldBalls());
      // purePursuit(24,0,skills2ndX,skills2ndY,8,0.7,10.0,3000); //anglekp was 12 //get ball for SIXTH Goal (far left corner)
      // delayDriveSmooth(500,7.5,0.5,fwd);
      // timer = 0;
      // while(!firstBall)
      //   pros::delay(10);
      //   timer+= 10;
      //   if(timer > 1000)
      //     break;
      // intake(stop);
      // if(thirdBall)
      //   shooting_macro(1);
      // if(secondBall)
      //   super_macro(1,1);
      // if(firstBall)
      //   super_macro(1,2); //sore SIXTH Goal (far left corner)
      // intake(outward);
      // driveDistance(-14,10);
      // facePID(-62,-30,p,i,d);
      // intake(inward);
      // //eject(countHeldBalls());
      //
      // driveDistance2(calcDistance(-62,-24)-5,0.5,3,10,0.8,1,250,1500); //grab wall ball
      // delayDriveSmooth(600,9.2,0.5,rev);
      // //driveDistance(-19,10);
      // //facePID(-25,-70,p,i,d);
      // intake(inward);
      // adaptiveDrive(-20.5,-68,0.38,8.8,0.5,10,1.0,250,10000); //ball for SEVENTH (left side)
      // driveDistance(-5,10);
      // facePID(-100,-66,p,i,d);
      // driveDistance(26,10);
      // intake(stop);
      // delayDriveSmooth(650,8.6,0.4,fwd);
      // reset(2);
      // super_macro(countHeldBalls(),1);  // SCORE 8TH GOAL (left side)
      // intake(outward);
      // driveDistance(-20,10);
      // //facePID(25,-48,p,i,d);
      // intake(inward);
      // //eject(countHeldBalls());
      // adaptiveDrive(34.5,-48,0.5,8.6,0.8,7.0,1.0,250,10000); //ball for 9th goal (left near corner)
      // //driveDistance(calcDistance(25,-48)+3,10);
      // driveDistance(-12,10);
      // facePID(-5,-72.5,p,i,d);
      // delayDriveSmooth(1200,8,0.5,fwd);
      // super_macro(countHeldBalls(),2); //SCORE LEFT NEAR CORNER
      // intake(outward);
      // driveDistance(-20,10);
      // intake(inward);
      // //eject(countHeldBalls());
      // adaptiveDrive(66,-24,0.5,8,0.5,5.0,1.0,250,10000);
      // //driveDistance(-6,9);
      // facePID(68,5,p,i,d);
      // driveUntilStopped(7000);
      // timer = 0;
      // while(!thirdBall) {
      //   pros::delay(10);
      //   timer += 10;
      //   if(timer > 1000)
      //     break;
      // }
      // shooting_macro(1);    // SCORE MIDDLE
      // timer = 0;
      // while(!thirdBall) {
      //   pros::delay(10);
      //   timer += 10;
      //   if(timer > 1500)
      //     break;
      // }
      // pros::delay(200);
      // a.resume();
      // driveDistance2(-18,0.9,3,10,5,1,250,5000);

      break;
    case 4: //red mid auton

    case 5: //blue mid auton
      cm.resume();
      intake(inward);
      //driveDistance2(44, 0.7, 0, 8, 1.0, 3, 250, 2000);
      driveDistance(46,12);
      delayDriveSmooth(500,12,0.5,rev);
      //driveDistance(-24,10);
      adaptiveDrive(-36,-10,0.5,12,0.4,6.0,10.0,100,2000);
      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      intake(stop);
      //delayDriveSmooth(1000,5,0.5,fwd);
      if(mid_ball_grabbed)
        super_macro_slowed(3,2);
      else
        super_macro(2,1);
      intake(outward);
      r2.resume();
      //adaptiveDrive_reversed(45,16,0.5,8,0.5,6.0,1.0,250,2000);
      //pros::delay(150);
      //release_asynch(countHeldBalls()-1);
      //adaptiveDrive_reversed(30,15,8.0);
      adaptiveDrive_reversed(28,10,0.5,12,0.5,8.0,10.0,100,2000);
      //release(countHeldBalls()-1);
      //facePID(180,p,i,d);
      intake(inward);
      //delayDriveSmooth(1000,8,0.5,fwd);
      adaptiveDrive(33,-15,0.5,12,0.6,7.0,10.0,100,1000);

      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      intake(stop);
      if(thirdBall || secondBall)
        shooting_macro(2);
      else
        shooting_macro(1);
      intake(outward);

      release_all_asynch(300);
      adaptiveDrive_reversed(35,24,0.5,12,0.5,8.0,10.0,100,2000);

      facePID(270,p,i,d);
      // facePID(33,60,p-4,i,d);
      //
      // intake(inward);
      //
      // driveUntilStopped(5000);

      // shooting_macro(1);
      //
      // a.resume();
      //
      // driveDistance(-20,12);

      //driveDistance(-20,12);
      /* for home row
      intake(inward);
      //purePursuit(24,0,xPts,yPts,8,0.8,6.5,8.0,5000);

      adaptiveDrive(108,-13,0.4,12,0.5,6.0,10.0,100,2000);
      //conveyorState = 99;
      timer = 0;
      while(!thirdBall) {
        pros::delay(10);
        timer += 10;
        if(timer > 1000)
          break;
      }
      intake(stop);
      super_macro(3,2); //score third goal
      intake(outward);
      delayDrive(400,-8000);
      */
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
