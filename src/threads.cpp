#include "main.h"

int intakeState = 2;
int conveyorState = 0;

int idle = 0;
int shooting = 1;
int ejecting = 2;
int macro1_trigger = 3;
int macro2_trigger = 4;
int macro3_trigger = 6;
int sort_trigger = 8;

bool topBall;
bool midBall;
bool botBall;
bool firstBall;
bool secondBall;
bool thirdBall;

int iBalls = 0;

const int NO_BALL = 0;
const int RED_BALL = 1;
const int BLUE_BALL = 2;

int optical_state = NO_BALL;

void thread_sensors_v2(void*p)
{
  std::uint32_t t;
  int blue_counter = 0;
  int red_counter = 0;
  while(true) {
    t = pros::millis();
    if(topDetector.get_value() == 1 )
      topBall = true;
    else topBall = false;

    if(midDetector.get() < 75)
      midBall = true;
    else midBall = false;

    if(botDetector.get() < 20)
      botBall = true;
    else botBall = false;

    if(topBall)
      firstBall = true;
    else firstBall = false;

    if(firstBall && midBall)
      secondBall = true;
    else secondBall = false;

    if(secondBall && botBall)
      thirdBall = true;
    else thirdBall = false;
    pros::Task::delay_until(&t,10);
  }
}


bool fi = false;
void untilsecondball_thread(void*p)
{
  while(!botBall && !fi) {
    botConveyor.move_velocity(200);
    pros::delay(10); }
  botConveyor.move_velocity(0);
  while(!fi) {
    pros::delay(10); }
}

void countBalls(int numOfBalls)
{
  fi = false;

  if(numOfBalls == 0){
    fi = true;
    return;
  }
  int timeOut = 10000;
  int timer = 0;
  for(int n = 0; n < numOfBalls; n++) {
    timer = 0;
    while(!topBall) {
      pros::delay(10);
      timer += 10;
      if(timer >= timeOut) {
        fi = true;
        return;
      }
    }
    while(topBall) pros::delay(10);
    updateVarLabel(debugLabel2,"BALL COUNT",debugValue2,2,"",0);
  }
  fi = true;
}

void countReleaseBalls(int numOfBalls)
{
  int timer=0;
  for(int n = 0; n < numOfBalls; n++) {
    timer = 0;
    while(!botBall) {
      timer+=10;
      pros::delay(10);
      if(timer > 2500){
        return;
      }
    }
    timer = 0;
    while(botBall) {
      timer+=10;
      pros::delay(10);
      if(timer > 2500) {
        return;
      }
    }
  }
}

void countIntakeBalls(int numOfBalls)
{
  int timer=0;
  for(int n = 0; n < numOfBalls; n++) {
    timer = 0;
    while(botBall) {
      timer+=10;
      pros::delay(10);
      if(timer > 2500){
        return;
      }
    }
    timer = 0;
    while(!botBall) {
      timer+=10;
      pros::delay(10);
      if(timer > 2500) {
        return;
      }
    }
  }
}

bool intakeFinished = false;
void intake_subthread(void*p)
{
  intakeFinished = false;
  intakeState = inward;
  bool wait = false;
  countIntakeBalls(iBalls);
  if(botBall)
    pros::delay(100);
  intakeState = outward;
  pros::delay(100);
  intakeState = stop;
  intakeFinished = true;
}

void shooting_macro(int numOfBalls)
{
  if(!driverControl)
    conveyorState = 7;
    // if(!driverControl)
    //   conveyorState = 0;
  topConveyor.move_voltage(12000);
  botConveyor.move_voltage(12000);
  int timer = 0;
  while(!topBall) {
    timer+=10;
    pros::delay(10);
    if(timer > 2500) {
      return;
    }
  }
  while(topBall) pros::delay(10);
  updateVarLabel(debugLabel2,"BALL COUNT",debugValue2,1,"",0);
  countBalls(numOfBalls-1);
  botConveyor.move_velocity(0);
  pros::delay(400);
  if(!driverControl)
    conveyorState = 0;
}

int numOfBalls2;
bool finished;
void shooting_macro_2_thread(void*p)
{
  finished = false;
  if(!driverControl)
    conveyorState = 7;
  if(!firstBall) {
    if(!driverControl)
      conveyorState = 0;

    finished = true;
    return;
  }

  topConveyor.move_velocity(600);
  botConveyor.move_velocity(0);
  pros::delay(200);

  if(numOfBalls2 == 1) {
    finished = true;
    pros::delay(200);
    topConveyor.move_velocity(0);
    if(!driverControl)
      conveyorState = 0;
    return;
  }
  pros::delay(200);
  countBalls(numOfBalls2-1);
  pros::delay(200);
  topConveyor.move_velocity(0);
  //pros::delay(100);
  if(!driverControl)
    conveyorState = 0;
  finished = true;
}

void shooting_macro2(int nb)
{
  numOfBalls2 = nb;
  pros::Task subthread (shooting_macro_2_thread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::delay(10);
  while(!finished)
    pros::delay(10);
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
  if(secondBall) {
    botConveyor.move_velocity(0);
    topConveyor.move_velocity(0);
  }
  else {
    topConveyor.move_velocity(-300);
    botConveyor.move_voltage(12000);
  }
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

void super_macro2(int shootBalls, int intakeBalls)
{
  intakeFinished = false;
  if(!driverControl)
    conveyorState = 7;
  iBalls = intakeBalls;
  pros::Task subthread (intake_subthread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  shooting_macro2(shootBalls);
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
  //pros::Task topBall_task (thread_centerTopBall, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "For opcontrol ONLY");
  //topBall_task.suspend();
  pros::Task intake_thread (thread_intake, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  pros::Task intake_control (thread_intakecontrol, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  std::uint32_t t;

  int ejectState;
  while(true) {
    int* r_r = std::find(red,redEnd,auton%NUM_OF_AUTONS);
    int* r_b = std::find(blue,blueEnd,auton%NUM_OF_AUTONS);
    t = pros::millis();
    switch(conveyorState) {
      case 0: //idle state
        idleConveyor();
        break;
      case 1: //shooting manually
          topConveyor.move_voltage(12000);
          botConveyor.move_velocity(12000);
          pros::delay(400);
          while(conveyorState == 1) {
            pros::delay(10);
          }
          topConveyor.move_voltage(-12000);
          pros::delay(100);
          topConveyor.move_voltage(0);
          break;
        case 2:
          intake_thread.suspend();
          while(conveyorState == 2) {
            topConveyor.move_voltage(-12000);
            botConveyor.move_velocity(-12000);
            leftIntake.move_voltage(-12000);
            rightIntake.move_voltage(-12000);
            pros::delay(10);
          }
          intake_thread.resume();
          break;
      case 3: //macro1
        shooting_macro(countHeldBalls());
        break;

      case 4: //macro2
        intake_control.suspend();
        super_macro(2,1);
        intake_control.resume();
        break;
      case 6: //macro3
        shooting_macro(2);
        break;

      case 7: //pause
        while(conveyorState == 7)
          pros::delay(10);
        break;

    }
    pros::Task::delay_until(&t,10);
  }
}

void thread_control(void* p)
{
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

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
      auton = 1;
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))
      auton = 2;

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
