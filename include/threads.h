#include "main.h"
using namespace okapi;
extern int intakeState;
extern int inward;
extern int outward;
extern int stop;
extern int conveyorState;
extern const int RED_BALL;
extern const int BLUE_BALL;
extern const int NO_BALL;
extern int optical_state;
extern bool firstBall;
extern void thread_subsystems(void*p);
extern void thread_intake(void*p);
extern void thread_control(void*p);
extern void thread_drive(void*p);
extern void thread_sensors_v2(void*p);
//extern void thread_sensors_filter(void*p);
extern void shooting_macro(int);
//extern void waitForBallToEject();
//extern void waitForBallsToEject(int);
extern void countIntakeBalls(int);
extern void countReleaseBalls(int);
extern void super_macro(int,int);
extern void super_macro_slowed(int,int);
extern void super_macro2(int,int);
extern void countIntakeBalls(int);
extern int countHeldBalls();
extern void shooting_macro2(int);
extern bool thirdBall;
extern bool secondBall;
extern bool firstBall;
extern bool botBall;
extern bool midBall;
extern bool topBall;

extern int top_high_avg;
extern int top_low_avg;
extern int bot_high_avg;
extern int bot_low_avg;
extern int ejector_avg;
