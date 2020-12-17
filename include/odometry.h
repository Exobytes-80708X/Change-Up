extern double robotTheta;
extern double robotX;
extern double robotY;
extern void thread_Odometry(void*param);
extern double calcDistance(double x1, double y1, double x2, double y2);
extern double calcDistance(double x2, double y2);
extern double calcAngleError(double theta);
extern double calcAngleError(double targetX, double targetY);
extern double calcAngleErrorReversed(double targetX, double targetY);
extern double calcAngleErrorReversed(double theta);
extern double* calcPointOfIntersection(double x1, double y1, double theta1);
extern void resetOdometry(bool,bool);
extern void adaptiveDrive(double x, double y, double maxV);
extern void adaptiveDrive(double x, double y, double accel, double maxV, double distkP, double anglekP, double scalePower, int settleTime, int timeout);
extern void adaptiveDrive_reversed(double x, double y, double maxV);
extern void face(double x, double y);
extern void face(double theta);
extern void driveDistance(double distance, double maxV);
extern void delayDrive(int ms, double vel);
extern void delayTurn(int ms, double velL, double velR);
extern void delayFacePID(double, bool, double, double, double, double, int, int, int);
extern void followQuad(int);
extern void followQuadDrive(int,double);
extern void facePID(double, double, bool, double, double, double, double, int, int);
extern void facePID(double, bool, double, double, double, double, int, int);
extern void facePID(double, double,double,double,double);
extern void facePID(double,double,double,double);
extern void accel(double, int);
extern void pointTurn(int side, int oppRPM, double theta, bool reversed, double maxV, double kP, double kI, double kD, int settleTime, int timeout);
extern void pointTurn(int side, int oppRPM, double theta, bool reversed, double kP, double kI, double kD);
extern void driveVector(double currentSpeed, double angleSpeed, double maxV);
extern void driveUntilStopped(double);
