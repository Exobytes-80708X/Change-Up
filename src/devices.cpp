#include "main.h"

okapi::Motor ld_1(2,true,okapi::AbstractMotor::gearset::green,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor ld_2(6,true,okapi::AbstractMotor::gearset::green,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor rd_1(5,false,okapi::AbstractMotor::gearset::green,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor rd_2(4,false,okapi::AbstractMotor::gearset::green,okapi::AbstractMotor::encoderUnits::counts);

okapi::MotorGroup leftDrive({ld_1,ld_2});
okapi::MotorGroup rightDrive({rd_1,rd_2});
pros::Motor leftIntake(1,pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightIntake(12,pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor topConveyor(7,pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor botConveyor(13,pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

//pros::ADIAnalogIn topDetector2 ('E'); //moded to B on ext ports
// pros::ADIAnalogIn topDetector ('H');
// pros::ADIAnalogIn botDetector ('G');
//pros::ADIAnalogIn ejectDetector ('F'); //moved to C on ext ports
//pros::ADIDigitalIn ejectDetector('B');
//pros::Optical topDetector_low(12);
//pros::Optical botDetector_high2(11);
pros::ADIDigitalIn topDetector('G');
pros::Distance midDetector(14);
pros::Distance botDetector(3);

okapi::ADIEncoder left('E','F',true);
okapi::ADIEncoder right('A','B',false);
okapi::ADIEncoder middle('C','D',false);

pros::Imu imu (5);
pros::Controller controller(pros::E_CONTROLLER_MASTER);
