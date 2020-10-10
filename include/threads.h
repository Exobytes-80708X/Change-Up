#include "main.h"
using namespace okapi;
extern int intakeState;
extern int inward;
extern int outward;
extern int stop;
extern int conveyorState;
extern bool firstBall;
extern void thread_subsystems(void*p);
extern void thread_intake(void*p);
extern void thread_control(void*p);
extern void thread_drive(void*p);
extern void thread_sensors(void*p);
extern void shooting_macro(int a);
extern void waitForBallToEject();
extern void countIntakeBalls(int);
