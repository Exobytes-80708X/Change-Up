#include "main.h"

okapi::Motor ld_1(3,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor ld_2(7,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor rd_1(2,false,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor rd_2(6,false,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);

okapi::MotorGroup leftDrive({ld_1,ld_2});
okapi::MotorGroup rightDrive({rd_1,rd_2});
pros::Motor leftIntake(21,pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightIntake(1,pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor topConveyor(15,pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor botConveyor(14,pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

//pros::ADIAnalogIn topDetector2 ('E'); //moded to B on ext ports
// pros::ADIAnalogIn topDetector ('H');
// pros::ADIAnalogIn botDetector ('G');
//pros::ADIAnalogIn ejectDetector ('F'); //moved to C on ext ports
pros::ADIDigitalIn ejectDetector('B'); 
pros::Optical topDetector_low(12);
pros::Optical topDetector_high(11);
pros::Optical botDetector_high(13);
pros::Distance botDetector_low(4);

okapi::ADIEncoder left('C','D',false);
okapi::ADIEncoder right('G','H',false);
okapi::ADIEncoder middle('E','F',true);

pros::Imu imu (5);
pros::Controller controller(pros::E_CONTROLLER_MASTER);
