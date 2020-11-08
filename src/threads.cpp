#include "main.h"

int intakeState = 2;
int conveyorState = 0;

int idle = 0;
int shooting = 1;
int ejecting = 2;
int macro1_trigger = 3;
int macro2_trigger = 4;
int macro3_trigger = 6;

bool topBall_low;
bool topBall_high;
bool botBall;
bool botBall_low;
bool firstBall;
bool secondBall;
bool ballInEjector;
bool thirdBall;

int iBalls = 0;

const int NO_BALL = 0;
const int RED_BALL = 1;
const int BLUE_BALL = 2;

int optical_state = NO_BALL;


void thread_sensors(void *p)
{
  while(true) {
    if(topDetector.get_value() < 2800)
      topBall_low = true;
    else topBall_low = false;

    if(pros::c::ext_adi_analog_read(5,'B') < 2800)
      topBall_high = true;
    else topBall_high = false;

    pros::c::optical_set_led_pwm(4, 50);
    if(botDetector.get_value() < 2800) {
      botBall = true;
      if(pros::c::optical_get_rgb(4).red/pros::c::optical_get_rgb(4).blue >= 2)
        optical_state = RED_BALL;
      else
        optical_state = BLUE_BALL;
    }
    else {
      botBall = false;
      optical_state = NO_BALL;
    }

    if(pros::c::ext_adi_analog_read(5,'C') < 2700)
      ballInEjector = true;
    else ballInEjector = false;

    if(pros::c::ext_adi_analog_read(5,'A') < 2750)
      botBall_low = true;
    else botBall_low = false;

    if(topBall_low || topBall_high)
      firstBall = true;
    else firstBall = false;

    if(firstBall && botBall)
      secondBall = true;
    else secondBall = false;

    if(secondBall && botBall_low)
      thirdBall = true;
    else thirdBall = false;


    pros::delay(10);
  }
}

void waitForBallToEject()
{
  while(ballInEjector) pros::delay(10);
  while(!ballInEjector) pros::delay(10);
  while(ballInEjector) pros::delay(10);
}

void waitForTopBalltoLower()
{
  while(firstBall) {
    topConveyor.move_velocity(-200);
    botConveyor.move_velocity(-200);
    pros::delay(10);
  }
}

void countBalls(int numOfBalls)
{
  if(numOfBalls == 0) return;
  int timeOut = 500;
  int timer = 0;
  botConveyor.move_velocity(600);
  while(topBall_high)
    pros::delay(10);
  while(!topBall_high) {
    pros::delay(10);
    timer += 10;
    if(timer >= timeOut) {
      return;
    }
  }
  for(int n = 0; n < numOfBalls; n++) {
    timer = 0;
    if(n == numOfBalls-1)
      botConveyor.move_velocity(0);

    while(topBall_high) pros::delay(10);

    if(n <= numOfBalls-2) {
      while(!topBall_high) {
        pros::delay(10);
        timer += 10;
        if(timer >= timeOut) {
          return;
        }
      }
    }
    else {
      pros::delay(200);
    }
  }
}

void countIntakeBalls(int numOfBalls)
{
  int timer=0;
  while(botBall_low){
       pros::delay(10);
  }
  for(int n = 0; n < numOfBalls; n++) {
    timer = 0;
    while(!botBall_low) {
      timer+=10;
      pros::delay(10);
      if(timer > 5000){
        return;
      }
    }
    timer = 0;
    while(botBall_low) {
      timer+=10;
      pros::delay(10);
      if(timer > 5000) {
        return;
      }
    }
    //break;
  }
  //pros::delay(200);
}

bool intakeFinished = false;
void intake_subthread(void*p)
{
  intakeFinished = false;
  intakeState = inward;
  countIntakeBalls(iBalls);
  intakeState = outward;
  pros::delay(300);
  intakeState = stop;
  intakeFinished = true;
}

void shooting_macro(int numOfBalls)
{
  if(!driverControl)
    conveyorState = 7;
  if(!firstBall) {
    if(!driverControl)
      conveyorState = 0;
    return;
  }

  topConveyor.move_velocity(600);
  botConveyor.move_velocity(0);
  pros::delay(200);
  countBalls(numOfBalls-1);

  if(!driverControl)
    conveyorState = 0;
}

void centerTopBall()
{
  if(topBall_high && !topBall_low)
    topConveyor.move_velocity(-25);
  else if(!topBall_high && topBall_low)
    topConveyor.move_velocity(25);
  else
    topConveyor.move_velocity(0);
}

void adjustTopBall()
{
  while(!(!topBall_low && topBall_high)) {
    topConveyor.move_velocity(25);
    pros::delay(10);
  }
}

void thread_centerTopBall(void*p)
{
  while(true) {
    if( (topBall_high && topBall_low) || (!topBall_high && topBall_low))
      topConveyor.move_velocity(25);
    else
      topConveyor.move_velocity(0);
    pros::delay(10);
  }
}

int countHeldBalls()
{
  if(thirdBall) return 3;
  else if(secondBall) return 2;
  else if(firstBall) return 1;
  else return 0;
}

void idleConveyor()
{
  if(firstBall)
    centerTopBall();
  else
    topConveyor.move_velocity(200);

  if(secondBall && firstBall)
    botConveyor.move_velocity(0);
  else
    botConveyor.move_velocity(300);
}

void super_macro(int shootBalls, int intakeBalls)
{
  intakeFinished = false;
  if(!driverControl)
    conveyorState = 7;
  iBalls = intakeBalls;
  pros::Task subthread (intake_subthread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  shooting_macro(shootBalls);
  while(!intakeFinished) {
    idleConveyor();
  }
  if(!driverControl)
    conveyorState = 0;
}

void thread_intakecontrol(void*p)
{
  while(true) {
    if(driverControl) {
      if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        intakeState = inward;
      }
      else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        intakeState = outward;
      }
      else {
        intakeState = stop;
      }
    }
    pros::delay(10);
  }
}

int inward = 0;
int outward = 1;
int stop = 2;
void thread_intake(void* p)
{
  while(true) {
    switch(intakeState)
    {
      case 0:
        leftIntake.move_voltage(12000);
        rightIntake.move_voltage(12000);
        break;
      case 1:
        leftIntake.move_voltage(-12000);
        rightIntake.move_voltage(-12000);
        break;
      case 2:
        leftIntake.move_voltage(0);
        rightIntake.move_voltage(0);
        break;
    }
    pros::delay(10);
  }
}

void thread_subsystems(void* p)
{
  botConveyor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  topConveyor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  pros::Task topBall_task (thread_centerTopBall, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "For opcontrol ONLY");
  topBall_task.suspend();
  pros::Task intake_thread (thread_intake, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task intake_control (thread_intakecontrol, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");

  while(true) {
    switch(conveyorState) {
      case 0: //idle state
        if(auton == red && driverControl) { //RED
          if(optical_state == BLUE_BALL) {
            if(firstBall){
              botConveyor.move_velocity(600);
              topBall_task.resume();
              waitForBallToEject();
              topBall_task.suspend();
            }
            else {
              topConveyor.move_velocity(-600);
              botConveyor.move_velocity(200);
              waitForBallToEject();
            }
          }
        }
        else if (auton == blue && driverControl) { //BLUE
          if(optical_state == RED_BALL) {
            if(firstBall){
              botConveyor.move_velocity(600);
              topBall_task.resume();
              waitForBallToEject();
              topBall_task.suspend();
            }
            else {
              topConveyor.move_velocity(-600);
              botConveyor.move_velocity(200);
              waitForBallToEject();
            }
          }
        }
        idleConveyor();

        break;

      case 1: //shooting manually
        if(firstBall)
          botConveyor.move_velocity(0);
        topConveyor.move_voltage(12000);
        pros::delay(200);
          botConveyor.move_velocity(600);
        while(conveyorState == shooting)
          pros::delay(10);

        break;

      case 2: //ejecting manually
        if(topBall_high && !secondBall) {
          waitForTopBalltoLower();
          topConveyor.move_velocity(-600);
          botConveyor.move_velocity(600);
          waitForBallToEject();
        }
        else if(topBall_high && secondBall) {
          botConveyor.move_velocity(600);
          topBall_task.resume();
          waitForBallToEject();
          topBall_task.suspend();
        }
        else if(!topBall_high) {
          topConveyor.move_velocity(-600);
          botConveyor.move_velocity(600);
        }
        break;

      case 3: //macro1
        shooting_macro(countHeldBalls());
        break;

      case 4: //macro2
        intake_control.suspend();
        super_macro(3,1);
        intake_control.resume();
        break;

      case 5: //ejecting macro top 2 balls
        if(secondBall) {
          botConveyor.move_velocity(600);
          topBall_task.resume();
          waitForBallToEject();
          topBall_task.suspend();

          waitForTopBalltoLower();
          topConveyor.move_velocity(-600);
          botConveyor.move_velocity(600);
          waitForBallToEject();

          botConveyor.move_velocity(0);
          topConveyor.move_velocity(200);
          pros::delay(100);
        }
        break;

      case 6: //maro3
        //intake_control.suspend();
        //super_macro(countHeldBalls(),2);
        //intake_control.resume();
        shooting_macro(2);
        break;

      case 7: //pause
        while(conveyorState == 7)
          pros::delay(10);
        break;
    }
    pros::delay(10);
  }
}

void thread_control(void* p)
{
  //pros::Task sub_task (thread_intake_control, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "For opcontrol ONLY");
  while(true)
  {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      conveyorState = shooting;
    }
    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      conveyorState = ejecting;
    }
    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
      conveyorState = macro1_trigger;
    }
    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
       conveyorState = macro2_trigger;
    }
    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
       conveyorState = macro3_trigger;
    }
    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
      conveyorState = 5;
    }
    else {
       conveyorState = idle;
    }

    pros::delay(10);
  }
}

void thread_drive(void* p)
{
  rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
  leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
  while(true) {
    if(abs(controller.get_analog(ANALOG_LEFT_Y)) > 5) {
        leftDrive.moveVoltage(controller.get_analog(ANALOG_LEFT_Y)/127.0*12000);
    }
    else
      leftDrive.moveVoltage(0);

    if(abs(controller.get_analog(ANALOG_RIGHT_Y)) > 5) {
        rightDrive.moveVoltage(controller.get_analog(ANALOG_RIGHT_Y)/127.0*12000);
    }
    else
      rightDrive.moveVoltage(0);
    pros::delay(10);
  }
}
