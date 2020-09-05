#include "main.h"
using namespace okapi;
extern int intakeState;
extern int inward;
extern int outward;
extern int stop;
extern void thread_conveyor(void*p);
extern void thread_intake(void*p);
extern void thread_control(void*p);
extern void thread_drive(void*p);
extern void thread_sensors(void*p);
