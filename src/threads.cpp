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

// int avg(int* array, int size) {
//   int sum = 0;
//   for (int i = 0; i < size; i++)
//     sum += array[i];
//   return int(sum/size);
// }
//
// int top_high_avg;
// int top_low_avg;
// int bot_high_avg;
// int bot_low_avg;
// int ejector_avg;
// int color_red_avg;
// int color_blue_avg;
//
// void thread_sensors_filter(void*p) //rolling average
// {
//   int sb_delay = 0;
//   const int SIZE = 5;
//   int top_high_raw = pros::c::ext_adi_analog_read(5,'B');
//   int top_low_raw = topDetector.get_value();
//   int bot_high_raw = botDetector.get_value();
//   int bot_low_raw = pros::c::ext_adi_analog_read(5,'A');
//   int ejector_raw = pros::c::ext_adi_analog_read(5,'C');
//   int color_red_raw = pros::c::optical_get_rgb(4).red;
//   int color_blue_raw = pros::c::optical_get_rgb(4).blue;
//
//   int top_high_values[SIZE]{top_high_raw,top_high_raw,top_high_raw,top_high_raw,top_high_raw};
//   int top_low_values[SIZE]{top_low_raw,top_low_raw,top_low_raw,top_low_raw,top_low_raw,};
//   int bot_high_values[SIZE]{bot_high_raw,bot_high_raw,bot_high_raw,bot_high_raw,bot_high_raw};
//   int bot_low_values[SIZE]{bot_low_raw,bot_low_raw,bot_low_raw,bot_low_raw,bot_low_raw};
//   int ejector_values[SIZE]{ejector_raw,ejector_raw,ejector_raw,ejector_raw,ejector_raw};
//   int color_red_values[SIZE]{color_red_raw,color_red_raw,color_red_raw,color_red_raw,color_red_raw};
//   int color_blue_values[SIZE]{ color_blue_raw, color_blue_raw, color_blue_raw, color_blue_raw, color_blue_raw};
//   int a;
//
//   while(true) {
//     top_high_raw = pros::c::ext_adi_analog_read(5,'B'); //update raw values
//     top_low_raw = topDetector.get_value();
//     bot_high_raw = botDetector.get_value();
//     bot_low_raw = pros::c::ext_adi_analog_read(5,'A');
//     ejector_raw = pros::c::ext_adi_analog_read(5,'C');
//     color_red_raw = pros::c::optical_get_rgb(4).red;
//     color_blue_raw = pros::c::optical_get_rgb(4).blue;
//
//     for(int i = 1; i < SIZE; i++) { //shift arrays
//       top_high_values[i] = top_high_values[i-1];
//       top_low_values[i] = top_low_values[i-1];
//       bot_high_values[i] = bot_high_values[i-1];
//       bot_low_values[i] = bot_low_values[i-1];
//       ejector_values[i] = ejector_values[i-1];
//       color_red_values[i] = color_red_values[i-1];
//       color_blue_values[i] = color_blue_values[i-1];
//     }
//     top_high_values[0] = top_high_raw; //add new raw value
//     top_low_values[0] = top_low_raw;
//     bot_high_values[0] = bot_high_raw;
//     bot_low_values[0] = bot_low_raw;
//     ejector_values[0] = ejector_raw;
//     color_red_values[0] = color_red_raw;
//     color_blue_values[0] = color_blue_raw;
//
//     top_high_avg = avg(top_high_values,SIZE); //calc avg for all arrays
//     top_low_avg = avg(top_low_values,SIZE);
//     bot_high_avg = avg(bot_high_values,SIZE);
//     bot_low_avg = avg(bot_low_values,SIZE);
//     ejector_avg = avg(ejector_values,SIZE);
//     color_red_avg = avg(color_red_values,SIZE);
//     color_blue_avg = avg(color_blue_values,SIZE);
//
//     //======================================================================================================
//
//     if(pros::c::optical_get_proximity(2) >= 253) {
//       if(top_low_avg < 2700)
//         topBall_low = true;
//       else topBall_low = false;
//
//       if(top_high_avg < 2700)
//         topBall_high = true;
//       else topBall_high = false;
//     }
//     else {
//       topBall_high = false;
//       topBall_low = false;
//     }
//
//     pros::c::optical_set_led_pwm(4, 50);
//     if( bot_high_avg < 2700 && pros::c::optical_get_proximity(4) >= 253) {
//       botBall = true;
//       sb_delay += 1;
//       a = color_red_raw/color_blue_raw;
//       if(pros::c::optical_get_proximity(4) >= 253) {
//         if( a >= 3)
//           optical_state = RED_BALL;
//         else if( a <= 1)
//           optical_state = BLUE_BALL;
//       }
//       else optical_state = NO_BALL;
//     }
//     else {
//       botBall = false;
//       sb_delay = 0;
//       optical_state = NO_BALL;
//     }
//
//     if(ejector_avg < 2000)
//       ballInEjector = true;
//     else ballInEjector = false;
//
//     if(bot_low_avg < 2800) {
//       botBall_low = true;
//     }
//     else {
//       botBall_low = false;
//     }
//
//     if(topBall_low || topBall_high || pros::c::optical_get_proximity(2) >= 253)
//       firstBall = true;
//     else firstBall = false;
//
//     if(firstBall && botBall)
//       secondBall = true;
//     else secondBall = false;
//
//     if(secondBall && botBall_low)
//       thirdBall = true;
//     else thirdBall = false;
//
//     pros::delay(10);
//   }
// }

void thread_sensors_v2(void*p)
{
  ballInEjector = false;
  botDetector_high.set_led_pwm(50);
  botDetector_high2.set_led_pwm(50);
  std::uint32_t t;
  int blue_counter = 0;
  int red_counter = 0;
  while(true) {
    t = pros::millis();
    if(topDetector_high.get_value() == 1 )
      topBall_high = true;
    else topBall_high = false;

    if(topDetector_low.get_proximity() > 100)
      topBall_low = true;
    else topBall_low = false;

    if(botDetector_high2.get_proximity() > 100) {
      botBall = true;
      if(botDetector_high.get_hue() > 100 || botDetector_high2.get_hue() > 100) {
        blue_counter += 1;
        red_counter = 0;
      }
      else if(botDetector_high.get_hue() < 60 && botDetector_high2.get_hue() < 60) { /*(botDetector_high.get_rgb().red/botDetector_high.get_rgb().blue >= 2*/
        red_counter += 1;
        blue_counter = 0;
      }
      else {
        red_counter = 0;
        blue_counter = 0;
      }

      if(blue_counter >= 5) {
        optical_state = BLUE_BALL;
      }
      else if(red_counter >= 5) {
        optical_state = RED_BALL;
      }
      else {
        optical_state = NO_BALL;
      }
    }
    else {
      botBall = false;
      optical_state = NO_BALL;
      red_counter = 0;
      blue_counter = 0;
    }

    if(botDetector_low.get() < 10)
      botBall_low = true;
    else botBall_low = false;

    if(ejectDetector.get_value() == 1)
      ballInEjector = true;
    else ballInEjector = false;

    if(topBall_low || topBall_high)
      firstBall = true;
    else firstBall = false;

    if(firstBall && botBall)
      secondBall = true;
    else secondBall = false;

    if(secondBall && botBall_low)
      thirdBall = true;
    else thirdBall = false;
    pros::Task::delay_until(&t,10);
  }
}

void waitForBallToEject()
{
  int timer = 0;
  int* r_r = std::find(red,redEnd,auton%NUM_OF_AUTONS);
  int* r_b = std::find(blue,blueEnd,auton%NUM_OF_AUTONS);
  while(!ballInEjector) {
     pros::delay(2);
     timer += 2;
     if( driverControl  && (r_r != redEnd || r_b != blueEnd) ){
       if( (r_r != redEnd && optical_state != BLUE_BALL) || (r_b != blueEnd && optical_state != RED_BALL) ) {
         return;
       }
     }
     if(timer > 1000) return;
     updateVarLabel(debugLabel6,"ONE",debugValue6,1,"",7);
   }
  while(ballInEjector) {
     pros::delay(2);
     timer += 2;
     if( driverControl  && (r_r != redEnd || r_b != blueEnd) ){
       if( (r_r != redEnd && optical_state != BLUE_BALL) || (r_b != blueEnd && optical_state != RED_BALL) ) {
         return;
       }
     }
     if(timer > 1000) return;
     updateVarLabel(debugLabel6,"TWO",debugValue6,2,"",7);
   }
}

void waitForTopBalltoLower()
{
  while(firstBall) {
    topConveyor.move_velocity(-200);
    botConveyor.move_velocity(-200);
    pros::delay(10);
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
  while(topBall_high)
    pros::delay(10);
  botConveyor.move_velocity(600);

  while(!topBall_high) {
    updateVarLabel(debugLabel2,"BALL COUNT",debugValue2,1.5,"",1);
    pros::delay(10);
    timer += 10;
    if(timer >= timeOut) {
      fi = true;
      return;
    }
  }
  for(int n = 0; n < numOfBalls; n++) {
    timer = 0;
    while(topBall_high) pros::delay(10);
    if(n == numOfBalls-1)
      pros::Task subthread (untilsecondball_thread, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
    if(n < numOfBalls-1/*n == 0 && numOfBalls == 2*/) {
      while(!topBall_high) {
        pros::delay(10);
        timer += 10;
        if(timer >= timeOut) {
          fi = true;
          return;
        }
      }
    }
    updateVarLabel(debugLabel2,"BALL COUNT",debugValue2,2,"",0);
  }
  fi = true;
}

void countReleaseBalls(int numOfBalls)
{
  int timer=0;
  for(int n = 0; n < numOfBalls; n++) {
    timer = 0;
    while(!botBall_low) {
      timer+=10;
      pros::delay(10);
      if(timer > 2500){
        return;
      }
    }
    timer = 0;
    if(numOfBalls == 3 && n == 2) {
      pros::delay(500);
      continue;
    }
    while(botBall_low) {
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
    while(botBall_low) {
      timer+=10;
      pros::delay(10);
      if(timer > 2500){
        return;
      }
    }
    timer = 0;
    while(!botBall_low) {
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
  if(!firstBall) {
    // if(!driverControl)
    //   conveyorState = 0;
    topConveyor.move_velocity(600);
    botConveyor.move_velocity(600);
    int timer = 0;
    while(!topBall_high) {
      timer+=10;
      pros::delay(10);
      if(timer > 2500) {
        return;
      }
    }
  }
  else {
    topConveyor.move_velocity(600);
    botConveyor.move_velocity(0);
    updateVarLabel(debugLabel2,"BALL COUNT",debugValue2,99,"",0);
    int timer = 0;
    pros::delay(200);
  }
  while(topBall_high) pros::delay(10);
  countBalls(numOfBalls-1);
  pros::delay(200);
  topConveyor.move_velocity(0);
  pros::delay(100);
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

void centerTopBall()
{
  if(topBall_high && !topBall_low)
    topConveyor.move_velocity(25);
  else if(!topBall_high && topBall_low)
    topConveyor.move_velocity(-25);
  else
    topConveyor.move_velocity(0);
}

void thread_centerTopBall(void*p)
{
  while(true) {
    if(!topBall_high && topBall_low)
      topConveyor.move_velocity(50);
    else if(topBall_high && !topBall_low)
      topConveyor.move_velocity(-25);
    else if(topBall_high && topBall_low)
      topConveyor.move_velocity(0);
    else
      topConveyor.move_velocity(200);
    pros::delay(10);
  }
}

int counter = 0;
int total_count;
void eject_task(void*p)
{
  pros::Task topBall_task (thread_centerTopBall, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "For opcontrol ONLY");
  topBall_task.suspend();
  if(thirdBall) {
    topBall_task.resume();
    botConveyor.move_velocity(300);
    waitForBallToEject();
    counter += 1;
    topBall_task.suspend();
    if(counter != total_count) {
      topBall_task.resume();
      botConveyor.move_velocity(600);
      waitForBallToEject();
      topBall_task.suspend();
      counter += 1;
      if(counter != total_count) {
        waitForTopBalltoLower();
        topConveyor.move_velocity(-600);
        botConveyor.move_velocity(600);
        waitForBallToEject();
        counter += 1;
      }
    }
  }
  else if(secondBall) {
    topBall_task.resume();
    botConveyor.move_velocity(600);
    waitForBallToEject();
    topBall_task.suspend();
    counter += 1;
    if(counter != total_count) {
      waitForTopBalltoLower();
      topConveyor.move_velocity(-600);
      botConveyor.move_velocity(600);
      waitForBallToEject();
      counter += 1;
    }
  }
  else if(firstBall) {
    waitForTopBalltoLower();
    topConveyor.move_velocity(-600);
    botConveyor.move_velocity(600);
    waitForBallToEject();
    counter += 1;
  }
  else if(!topBall_high) {
    topConveyor.move_velocity(-600);
    botConveyor.move_velocity(600);
    waitForBallToEject();
    counter += 1;
  }
}

void waitForBallsToEject(int b)
{
  total_count = b;
  counter = 0;
  updateVarLabel(debugLabel6,"TOTAL",debugValue6,total_count,"",0);
  pros::Task sub (eject_task, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "For opcontrol ONLY");
  int timerjj = 0;
  while(counter != total_count) {
    updateVarLabel(debugLabel5,"COUNTER",debugValue5,counter,"",0);
    timerjj += 10;
    if (timerjj > 3000) return;
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
  if(!topBall_high && topBall_low)
    topConveyor.move_velocity(50);
  else if(topBall_high && !topBall_low)
    topConveyor.move_velocity(-25);
  else if(topBall_high && topBall_low)
    topConveyor.move_velocity(0);
  else
    topConveyor.move_velocity(200);

  if(secondBall)
      botConveyor.move_velocity(0);
  else
    botConveyor.move_velocity(300);
}

void idleConveyor(int rpm)
{
  if(firstBall)
    topConveyor.move_velocity(0);
  else
    topConveyor.move_velocity(200);

    if(secondBall)
        botConveyor.move_velocity(0);
    else
      botConveyor.move_velocity(rpm);

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
  pros::Task topBall_task (thread_centerTopBall, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "For opcontrol ONLY");
  topBall_task.suspend();
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
        if( driverControl  && (r_r != redEnd || r_b != blueEnd) ){
          if( (r_r != redEnd && optical_state == BLUE_BALL) || (r_b != blueEnd && optical_state == RED_BALL) ) {
            if (firstBall) {
              topBall_task.resume();
              botConveyor.move_velocity(300);
              waitForBallToEject();
              topBall_task.suspend();
            }
            else {
              topConveyor.move_voltage(-12000);
              botConveyor.move_velocity(300);
              waitForBallToEject();
            }
          }
        }
        idleConveyor();
        break;
      case 99:
        idleConveyor(600);
        break;
      case 1: //shooting manually
        if(firstBall) {
          topConveyor.move_voltage(12000);
          botConveyor.move_velocity(0);
          pros::delay(400);
        }
        while(conveyorState == shooting) {
          if( r_r != redEnd || r_b != blueEnd ) {
            if( (r_r != redEnd && optical_state == BLUE_BALL) || (r_b != blueEnd && optical_state == RED_BALL) ) {
              if(firstBall) {
                topBall_task.resume();
                botConveyor.move_velocity(300);
                waitForBallToEject();
                topBall_task.suspend();
              }
              topConveyor.move_voltage(-12000);
              botConveyor.move_velocity(0);
              pros::delay(100);
              botConveyor.move_velocity(300);
              waitForBallToEject();
            }
          }
          topConveyor.move_voltage(12000);
          botConveyor.move_velocity(600);
          pros::delay(10);
        }
        break;
      case 2: //ejecting manually
        ejectState = countHeldBalls();
        while(conveyorState == 2) {
          switch(ejectState) {
            case int(-1E9)...0:
              topConveyor.move_velocity(-600);
              botConveyor.move_velocity(600);
              break;
            case 1:
              waitForTopBalltoLower();
              topConveyor.move_velocity(-600);
              botConveyor.move_velocity(600);
              waitForBallToEject();
              break;
            case 2:
              topBall_task.resume();
              botConveyor.move_velocity(600);
              waitForBallToEject();
              topBall_task.suspend();
              break;
            case 3:
              topBall_task.resume();
              botConveyor.move_velocity(300);
              waitForBallToEject();
              topBall_task.suspend();
              break;
          }
          ejectState -= 1;
        }
        break;
        // if(thirdBall) {
        //   topBall_task.resume();
        //   botConveyor.move_velocity(300);
        //   waitForBallToEject();
        //   topBall_task.suspend();
        //   if(conveyorState == 2) {
        //     topBall_task.resume();
        //     botConveyor.move_velocity(600);
        //     waitForBallToEject();
        //     topBall_task.suspend();
        //     if(conveyorState == 2) {
        //       waitForTopBalltoLower();
        //       topConveyor.move_velocity(-600);
        //       botConveyor.move_velocity(600);
        //       waitForBallToEject();
        //     }
        //   }
        // }
        // else if(secondBall) {
        //   topBall_task.resume();
        //   botConveyor.move_velocity(600);
        //   waitForBallToEject();
        //   topBall_task.suspend();
        //   if(conveyorState == 2) {
        //     waitForTopBalltoLower();
        //     topConveyor.move_velocity(-600);
        //     botConveyor.move_velocity(600);
        //     waitForBallToEject();
        //   }
        // }
        // else if(firstBall) {
        //   waitForTopBalltoLower();
        //   topConveyor.move_velocity(-600);
        //   botConveyor.move_velocity(600);
        //   waitForBallToEject();
        // }
        // else if(!topBall_high) {
        //   topConveyor.move_velocity(-600);
        //   botConveyor.move_velocity(600);
        // }
        // break;

      case 3: //macro1
        shooting_macro(countHeldBalls());
        break;

      case 4: //macro2
        intake_control.suspend();
        super_macro(2,1);
        intake_control.resume();
        break;

      case 5: //ejecting macro top 2 balls
        if(thirdBall) {
          botConveyor.move_velocity(600);
          topBall_task.resume();
          waitForBallToEject();
          topBall_task.suspend();
          botConveyor.move_velocity(-300);
          pros::delay(250);
          waitForTopBalltoLower();
          topConveyor.move_velocity(-600);
          botConveyor.move_velocity(600);
          waitForBallToEject();
        }
        else if(secondBall) {
          botConveyor.move_velocity(600);
          topBall_task.resume();
          waitForBallToEject();
          topBall_task.suspend();

          waitForTopBalltoLower();
          topConveyor.move_velocity(-600);
          botConveyor.move_velocity(600);
          waitForBallToEject();
        }
        else if(firstBall) {
          waitForTopBalltoLower();
          topConveyor.move_velocity(-600);
          botConveyor.move_velocity(600);
          waitForBallToEject();
        }
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
