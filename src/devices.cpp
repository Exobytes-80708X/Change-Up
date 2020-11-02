#include "main.h"

okapi::Motor ld_1(6,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor ld_2(20,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor rd_1(7,false,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor rd_2(18,false,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);

okapi::MotorGroup leftDrive({ld_1,ld_2});
okapi::MotorGroup rightDrive({rd_1,rd_2});
pros::Motor leftIntake(12,pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightIntake(11,pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor topConveyor(15,pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor botConveyor(14,pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

//pros::ADIAnalogIn topDetector2 ('E'); //moded to B on ext ports
pros::ADIAnalogIn topDetector ('H');
pros::ADIAnalogIn botDetector ('G');
//pros::ADIAnalogIn ejectDetector ('F'); //moved to C on ext ports
pros::Optical optical(4);

okapi::ADIEncoder left('C','D',false);
okapi::ADIEncoder right('A','B',false);
okapi::ADIEncoder middle('E','F',true);

pros::Imu imu (1);
pros::Controller controller(pros::E_CONTROLLER_MASTER);
