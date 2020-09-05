#include "main.h"

int intakeState = 2;
int conveyorState = 0;

int idle = 0;
int shooting = 1;
int ejecting = 2;
int macro1_trigger = 3;
int macro2_trigger = 4;

bool topBall_low;
bool topBall_high;
bool botBall;
bool firstBall;
bool secondBall;
bool ballInEjector;

void thread_sensors(void *p)
{
  while(true) {
    if(topDetector.get_value() < 2800)
      topBall_low = true;
    else topBall_low = false;

    if(topDetector2.get_value() < 2800)
      topBall_high = true;
    else topBall_high = false;

    if(botDetector.get_value() < 2800)
      botBall = true;
    else botBall = false;

    if(ejectDetector.get_value() < 2700)
      ballInEjector = true;
    else ballInEjector = false;

    if(topBall_low || topBall_high)
      firstBall = true;
    else firstBall = false;

    if(firstBall && botBall)
      secondBall = true;
    else secondBall = false;

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
  bool noBalls = false;
  botConveyor.move_velocity(600);
  while(topBall_high)
    pros::delay(10);
  while(!topBall_high) {
    pros::delay(10);
    timer += 10;
    if(timer >= timeOut) {
      noBalls = true;
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
          noBalls = true;
          break;
        }
      }
    }
    else {
      pros::delay(200);
    }
    if(noBalls) break;
  }
}

void shooting_macro(int numOfBalls)
{
  if(!firstBall)
    return;

  topConveyor.move_velocity(600);
  botConveyor.move_velocity(0);
  pros::delay(200);
  countBalls(numOfBalls-1);
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

void thread_conveyor(void* p)
{
  botConveyor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  topConveyor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  pros::Task topBall_task (thread_centerTopBall, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "For opcontrol ONLY");
  topBall_task.suspend();
  while(true) {
    switch(conveyorState) {
      case 0: //idle state
        if(firstBall)
          centerTopBall();
        else
          topConveyor.move_velocity(200);

        if(secondBall && firstBall)
          botConveyor.move_velocity(0);
        else
          botConveyor.move_velocity(400);

        break;

      case 1: //shooting manually
        if(firstBall)
          botConveyor.move_velocity(0);
        topConveyor.move_velocity(600);
        pros::delay(400);
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

      case 3: //shooting macro 3 balls
        shooting_macro(3);
        break;

      case 4: //shooting macro 2 balls
        shooting_macro(2);
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

      case 6:
        shooting_macro(1);
        break;
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

void thread_intake_control(void* p)
{
  while(true) {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      intakeState = inward;
    }
    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      intakeState = outward;
    }
    else {
      intakeState = stop;
    }
    pros::delay(10);
  }
}

void thread_control(void* p)
{
  pros::Task sub_task (thread_intake_control, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "For opcontrol ONLY");
  while(true)
  {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      conveyorState = shooting;
    }
    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      conveyorState = ejecting;
    }
    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
      conveyorState = macro1_trigger;
    }
    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
      conveyorState = macro2_trigger;
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
