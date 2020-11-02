#include "main.h"
//================ Odometry Variables ================

const double WHEEL_DIAMETER = 2.875;
const double ENCODER_WIDTH = 7.0;
const double MIDDLE_ENCODER_LENGTH = 10.0;
const double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER*M_PI;
const double IMU_WEIGHT = 1.0;
const bool DEBUGGING_ENABLED = true;
int test = 0;

double robotTheta_imu = 0.0;
double robotTheta_encoders = 0.0;
double robotTheta = 0.0;
double robotX = 0.0;
double robotY = 0.0;

void resetOdometry()
{
  robotX = 0;
  robotY = 0;
}

void thread_Odometry(void*param)
{
    robotTheta = 0.0;
    robotX = 0.0;
    robotY = 0.0;

    double dTheta_encoders = 0.0;
    double dTheta_imu = 0.0;
    double dTheta = 0.0;
    double dX = 0.0;
    double dY = 0.0;

    double currentLeft = 0.0;
    double currentRight = 0.0;
    double currentMiddle = 0.0;
    double currentImu = 0.0;

    double prevLeft = 0.0;
    double prevRight = 0.0;
    double prevMiddle = 0.0;
    double prevImu = 0.0;

    double dLeftVal = 0.0;
    double dRightVal = 0.0;
    double dMiddleVal = 0.0;
    double dImu = 0.0;

    double avgTheta;

    int leftReset = left.reset();
    int rightReset = right.reset();
    int midReset = middle.reset();

    pros::delay(200);

    while(true)
    {
        currentLeft = left.get()/360.0*WHEEL_CIRCUMFERENCE; //read quadature encoders
        currentRight = right.get()/360.0*WHEEL_CIRCUMFERENCE;
        currentMiddle = middle.get()/360.0*WHEEL_CIRCUMFERENCE;
        currentImu = imu.get_heading()*M_PI/180.0;  //imu heading in radians

        //currentLeft = leftDrive.getPosition()/900*WHEEL_CIRCUMFERENCE; //read integrated encoders
        //currentRight = rightDrive.getPosition()/900*WHEEL_CIRCUMFERENCE;

        dLeftVal = (currentLeft - prevLeft);
        dRightVal = (currentRight - prevRight);
        dMiddleVal = (currentMiddle - prevMiddle);
        dImu = currentImu - prevImu;

        prevLeft = currentLeft; //update prev values
        prevRight = currentRight;
        prevMiddle = currentMiddle;
        prevImu = currentImu;

        dTheta_encoders = (dLeftVal - dRightVal) / ENCODER_WIDTH; //calculate change in angle in radians
        dTheta_imu = dImu;
        if(fabs(dTheta_imu) > 180) {
          dTheta_imu = 2*M_PI*(dTheta_imu/fabs(dTheta_imu)) - dTheta_imu;
        }
        dTheta = dTheta_encoders*(1.0-IMU_WEIGHT) + dTheta_imu*IMU_WEIGHT;

        avgTheta = robotTheta + dTheta/2.0;
        avgTheta = fmod(avgTheta, 2*M_PI);
        if(avgTheta < 0) avgTheta += 2*M_PI;
        /*robotTheta_encoders += dTheta_encoders;
        robotTheta_encoders = fmod(robotTheta, 2*M_PI);

        robotTheta_imu = currentImu;*/

        //robotTheta = robotTheta_imu*IMU_WEIGHT + robotTheta_encoders*(1.0-IMU_WEIGHT); //weighted average between imu heading and encoder heading for (hopefully) more consistent heading calculation
        robotTheta += dTheta;
        robotTheta = fmod(robotTheta, 2*M_PI);
        if(robotTheta < 0) robotTheta += 2*M_PI;

        //dX = (dLeftVal + dRightVal)/2 * sin( (robotTheta) ); //calculate change in x
        //dY = (dLeftVal + dRightVal)/2 * cos( (robotTheta) ); //calculate change in y

        dX = (dLeftVal + dRightVal)/2 * sin(avgTheta) + dMiddleVal * cos(avgTheta);//calculate change in x
        dY = (dLeftVal + dRightVal)/2 * cos(avgTheta) - dMiddleVal * sin(avgTheta);; //calculate change in y

        robotX += dX; //add to current x and ys
        robotY += dY;

        //if(DEBUGGING_ENABLED) {
          updateValueLabel(xValue,robotX, "IN",3);
          updateValueLabel(yValue,robotY, "IN",3);
          updateValueLabel(thetaValue,robotTheta*180/M_PI,"DEG",3);
        //}
        pros::delay(10); //reupdate every dT msec
    }
}
void thread_Odom2(void*param)
{
    robotTheta = 0.0;
    robotX = 0.0;
    robotY = 0.0;

    double dTheta_encoders = 0.0;
    double dTheta_imu = 0.0;
    double dTheta = 0.0;
    double dX = 0.0;
    double dY = 0.0;

    double currentLeft = 0.0;
    double currentRight = 0.0;
    double currentMiddle = 0.0;
    double currentImu = 0.0;

    double prevLeft = 0.0;
    double prevRight = 0.0;
    double prevMiddle = 0.0;
    double prevImu = 0.0;

    double dLeftVal = 0.0;
    double dRightVal = 0.0;
    double dMiddleVal = 0.0;
    double dImu = 0.0;

    db totDL = 0.0;
    db totDR = 0.0;
    db offsetX;
    db offsetY;
    db TRACKING_TO_BACK = 3.75;
    double avgTheta;

    db sumX;
    db sumY;
    int leftReset = left.reset();
    int rightReset = right.reset();
    int midReset = middle.reset();

    pros::delay(200);

    while(true)
    {
        currentLeft = left.get()/360.0*WHEEL_CIRCUMFERENCE; //read quadature encoders
        currentRight = right.get()/360.0*WHEEL_CIRCUMFERENCE;
        currentMiddle = middle.get()/360.0*WHEEL_CIRCUMFERENCE;
        currentImu = imu.get_heading()*M_PI/180.0;  //imu heading in radians

        dLeftVal = (currentLeft - prevLeft);
        dRightVal = (currentRight - prevRight);
        dMiddleVal = (currentMiddle - prevMiddle);
        dImu = currentImu - prevImu;

        prevLeft = currentLeft; //update prev values
        prevRight = currentRight;
        prevMiddle = currentMiddle;
        prevImu = currentImu;


        dTheta = (dLeftVal - dRightVal)/ ENCODER_WIDTH;

        avgTheta = robotTheta + dTheta/2.0;
        avgTheta = fmod(avgTheta, 2*M_PI);
        if(avgTheta < 0) avgTheta += 2*M_PI;

        robotTheta += dTheta;
        robotTheta = fmod(robotTheta, 2*M_PI);
        if(robotTheta < 0) robotTheta += 2*M_PI;

        if(dTheta == 0.0){
          offsetX = dMiddleVal;
          offsetY = dRightVal;
        }
        else{
          offsetX = 2.0 * sin(dTheta/2.0) * (dMiddleVal/dTheta + TRACKING_TO_BACK);
          offsetY = 2.0 * sin(dTheta/2.0) * (dRightVal/dTheta + ENCODER_WIDTH/2.0);
        }

        sumX += offsetX;
        sumY += offsetY;
        dX = offsetY * cos(avgTheta) + offsetX * sin(avgTheta);
        dY = offsetY * sin(avgTheta) - offsetX * cos(avgTheta);

        robotX += dX;
        robotY += dY;

        //if(DEBUGGING_ENABLED) {
          updateValueLabel(xValue,robotX, "IN",3);
          updateValueLabel(yValue,robotY, "IN",3);
          updateValueLabel(thetaValue,robotTheta*180/M_PI,"DEG",3);
        //}
        pros::delay(10); //reupdate every dT msec
    }
}
// =============================================== math ===============================================================

double calcDistance(double x1, double y1, double x2, double y2) //distance formula in inches
{
    return sqrt(pow((y1 - y2), 2) + pow((x1 - x2), 2));
}

double calcDistance(double x2, double y2) //distance formula in inches, uses robot position as x1 and y1
{
  double distance = calcDistance(robotX,robotY,x2,y2);
  return distance;
}

double calcDistance_signed(double x2, double y2) //distance formula in inches, uses robot position as x1 and y1, can return negative values so robot can move backwards
{
  double distance = calcDistance(robotX,robotY,x2,y2);
  if(fabs(calcAngleError(x2,y2)) > M_PI/2) distance *= -1;
  return distance;
}

double calcAngleError(double theta) //calculate shortest angle error in radians from current robot theta
{
  theta = theta*M_PI/180.0; //comvert deg to rad
  double radius = 100;
  double predictedX = radius*sin(robotTheta) + robotX; //simulate point 100 units away from robot at current theta
  double predictedY = radius*cos(robotTheta) + robotY;
  double targetX = radius*sin(theta) + robotX; //simulate point 100 units away from robot at desired theta
  double targetY = radius*cos(theta) + robotY;
  double chord = calcDistance(predictedX, predictedY, targetX, targetY); //distance between two simulated points

  double angleError = 2*asin( (chord / 2) / (100) ); //calculate angle between simulated points

  predictedX = radius*sin( fmod(angleError + robotTheta, 2*M_PI) ) + robotX; //add that angle to robot's theta and calculate another simulated point
  predictedY = radius*cos( fmod(angleError + robotTheta, 2*M_PI) ) + robotY; //adding angleError is the same as the robot turning right

  if( (predictedX < targetX + 0.1) && (predictedX > targetX - 0.1) && (predictedY < targetY + 0.1) && (predictedY > targetY - 0.1) ) //if the predicted point match up woth the target point, then robot turns right (+angleError), if not turn left (-angleError)
      return angleError;
  else
      return angleError*-1;
}

double calcAngleError(double targetX, double targetY) //calculate shortest angle error to face (targetX,targetY) in radians from current robot theta
{
    double radius = calcDistance(robotX, robotY, targetX, targetY);
    double predictedX = radius*sin(robotTheta) + robotX;
    double predictedY = radius*cos(robotTheta) + robotY;
    double chord = calcDistance(predictedX, predictedY, targetX, targetY);

    double angleError = 2*asin( (chord / 2) / (radius) );

    predictedX = radius*sin( fmod(angleError + robotTheta, 2*M_PI) ) + robotX;
    predictedY = radius*cos( fmod(angleError + robotTheta, 2*M_PI) ) + robotY;

    if( (predictedX < targetX + 0.1) && (predictedX > targetX - 0.1) && (predictedY < targetY + 0.1) && (predictedY > targetY - 0.1) )
        return angleError;
    else
        return angleError*-1;
}

double calcAngleErrorReversed(double targetX, double targetY) //same as calcAngleError, but uses the back of the robot by adding 180 deg to robots current heading
{
    double radius = calcDistance(robotX, robotY, targetX, targetY);
    double predictedX = radius*sin(robotTheta+M_PI) + robotX;
    double predictedY = radius*cos(robotTheta+M_PI) + robotY;
    double chord = calcDistance(predictedX, predictedY, targetX, targetY);

    double angleError = 2*asin( (chord / 2) / (radius) );

    predictedX = radius*sin( fmod(angleError + robotTheta+M_PI, 2*M_PI) ) + robotX;
    predictedY = radius*cos( fmod(angleError + robotTheta+M_PI, 2*M_PI) ) + robotY;

    if( (predictedX < targetX + 0.1) && (predictedX > targetX - 0.1) && (predictedY < targetY + 0.1) && (predictedY > targetY - 0.1) )
        return angleError;
    else
        return angleError*-1;
}

double calcAngleErrorReversed(double theta) //same as calcAngleError, but uses the back of the robot by adding 180 deg to robots current heading
{
  theta = theta*M_PI/180.0;
  double radius = 100;
  double predictedX = radius*sin(robotTheta+M_PI) + robotX;
  double predictedY = radius*cos(robotTheta+M_PI) + robotY;
  double targetX = radius*sin(theta) + robotX;
  double targetY = radius*cos(theta) + robotY;
  double chord = calcDistance(predictedX, predictedY, targetX, targetY);

  double angleError = 2*asin( (chord / 2) / (100) );

  predictedX = radius*sin( fmod(angleError + robotTheta+M_PI, 2*M_PI) ) + robotX;
  predictedY = radius*cos( fmod(angleError + robotTheta+M_PI, 2*M_PI) ) + robotY;

  if( (predictedX < targetX + 0.1) && (predictedX > targetX - 0.1) && (predictedY < targetY + 0.1) && (predictedY > targetY - 0.1) )
      return angleError;
  else
      return angleError*-1;
}

double* calcLineEqn (double x1, double y1, double theta) //calculates a line based on robots position
{
  static double lineEqn [3];
  theta = theta*M_PI/180;

  double x2 = 10*sin(theta) + x1;
  double y2 = 10*cos(theta) + y1;

  lineEqn[0] = (y2-y1)/(x2-x1); //{m,b,theta}
  lineEqn[1] = y1 - lineEqn[0]*x1;
  lineEqn[2] = theta;

  return lineEqn;
}

double calcErrorToLine( double* lineEqn ) //not sure what this was supposed to do lmao
{
  double error = robotY - lineEqn[0]*robotX - lineEqn[1];
  double tempTheta = fmod(robotTheta - lineEqn[2], 2*M_PI);
  if( (tempTheta > M_PI && error > 0) || (tempTheta < M_PI && error < 0))
    error *= -1;
  return error;
}

double* convertSlopeIntToStandard(double* lineEqn) //converts slope intercept form eqn to standard form
{
  static double standardEqn [3]; //{a,b,c}
  if(lineEqn[0] <= 0)  {
      standardEqn[0] = fabs(lineEqn[0]);
      standardEqn[1] = 1;
      standardEqn[2] = lineEqn[1];
  }
  else {
    standardEqn[0] = fabs(lineEqn[0]);
    standardEqn[1] = -1;
    standardEqn[2] = -lineEqn[1];
  }

  return standardEqn;
}

double* calcPointOfIntersection(double x1, double y1, double theta1) //calulates point of intersection between robot's line and a given line
{
  double* eqn1 = convertSlopeIntToStandard(calcLineEqn(robotX,robotY,robotTheta+.0000001));
  double eqn1_0 = eqn1[0];
  double eqn1_1 = eqn1[1];
  double eqn1_2 = eqn1[2];
  double* eqn2 = convertSlopeIntToStandard(calcLineEqn(x1,y1,theta1));
  double eqn2_0 = eqn2[0];
  double eqn2_1 = eqn2[1];
  double eqn2_2 = eqn2[2];
  static double intersection [2];
  intersection[0] = (eqn1_2*eqn2_1 - eqn2_2*eqn1_1) / (eqn1_0*eqn2_1 - eqn2_0*eqn1_1);
  intersection[1] = (eqn1_2 - eqn1_0*intersection[0])/eqn1_1;
  return intersection;
}

//===========================================Movement Methods====================================================================================================================

void driveVector(double currentSpeed, double angleSpeed, double maxV)
{
  /*
  Arguments:
  currentSpeed  - based off of distance p-controller, uses mV
  angleSpeed    - based off of angle p-controller, uses mV
  maxV          - maximum voltage applid to motors, uses mV
  debugOn       - enables/disables debugging on V5 display
  */
	if(fabs(currentSpeed) > maxV)
		currentSpeed = currentSpeed/fabs(currentSpeed)*maxV;
    //Limits currentSpeed to maxV

	double maxCurrentSpeed = fabs(currentSpeed) + fabs(angleSpeed);
  //calculates new maximum currentSpeed with angleSpeed added
	double leftSpeed = currentSpeed + angleSpeed;
	double rightSpeed = currentSpeed - angleSpeed;
	double speedScale;

	/*if(maxCurrentSpeed > maxV) { //scales down left and right voltage so that it is not greater than maxV
		speedScale = fabs(maxCurrentSpeed/maxV);
		leftSpeed /= speedScale;
		rightSpeed /= speedScale;
	}*/

	if(DEBUGGING_ENABLED) {
    updateVarLabel(debugLabel3,"LEFT SPEED",debugValue3,leftSpeed,"mV",7);
    updateVarLabel(debugLabel4,"RIGHT SPEED",debugValue4,rightSpeed,"mV",7);
    updateVarLabel(debugLabel5,"CURRENT SPEED",debugValue5,currentSpeed,"mV",7);
    updateVarLabel(debugLabel6,"ANGLE SPEED",debugValue6,angleSpeed,"mV",7);
	}

	leftDrive.moveVoltage(leftSpeed);
	rightDrive.moveVoltage(rightSpeed);
}

void driveDistance(double distance, double accel, double minV, double maxV, double distkP, double anglekP, int settleTime, int timeout)
{
  /*
  Arguments:
  distance    - number of inches robot needs to travel (can take negative values too)
  accel       - rate of acceleration being applied to motors per 10 msec, where 0 = no acceleration, 1 = accelerate to max speed immediately
  minV        - minimum voltage applied to motors
  maxV        - maximum voltage applid to motors
  distkP      - constant for tuning distance p-controller
  anglekP      - constant for tuning angle p-controller
  settleTime  - the amount of time robot must be within a certain range of the target distance before declaring the movement as finished
  timeout     - maximum amount of time the movement can take
  */
	double simX = distance*sin(robotTheta) + robotX;
  double simY = distance*cos(robotTheta) + robotY;
  //calculates a point that is the target distance away from the robots current position
	double initX = robotX;
  double initY = robotY;
	double initTheta = robotTheta;
  //keeps track of robots intial position before the movement
	double distError;
	double angleError;
	double distSpeed;
	double angleSpeed;
	double currentSpeed = 0.0;

	accel *= 12000;
	minV *= 1000;
	maxV *= 1000;
	distkP *= 1000;
	anglekP *= 1000;
  //scales all arguments to be the correct units

	int settleTimer = 0;
	int timeoutTimer = 0;
  //initialize timers

	while(settleTimer < settleTime && timeoutTimer < timeout)
	{
		distError = calcDistance_signed(simX,simY);
		if(distance < 0)
			angleError = calcAngleErrorReversed(simX,simY);
      //angle error based on the back of the robot
		else
			angleError = calcAngleError(simX,simY);
      //angle error based on the front of the robot

		angleSpeed = angleError*anglekP;
		distSpeed = distError*distkP;
    //scales angle error and distance error

		if(distSpeed > 0 && currentSpeed < distSpeed)	currentSpeed += accel;
		else if(distSpeed < 0 && currentSpeed > distSpeed) currentSpeed -= accel;
		else currentSpeed = distSpeed;
    //if going forward, accelerate forwards
    //if going reverse, accelerate backwards
    //if decelerating (whether forwards or backwards), let p-controllers determine speed

		if(distError > 0 && currentSpeed < minV)
			currentSpeed = minV;
		else if(distError < 0 && currentSpeed > -minV)
			currentSpeed = -minV;
    //makes sure currentSpeed is greater than minV

		if(fabs(distError) < 0.5 || fabs(angleError) > 85.0*M_PI/180.0)
			settleTimer+=10;
    else
      settleTimer = 0;
    //if robot is within 0.5 inches of simulated point or is roughly perpendicular to the simulated point, increase the settleTimer
    //else if robot is outside that range of error, reset settleTimer to 0
		timeoutTimer+=10;

		if(fabs(distError) < 6.0)
			angleSpeed = 0;
    //if robot is with 6 inches of target distance, robot will no longer adjust to face point
    //as that will result in the robot making sudden turns at the end of a straight movement, which is bad for straight movements

    driveVector(currentSpeed,angleSpeed,maxV); //send calculated speeds to motors
		pros::delay(10);

    if(DEBUGGING_ENABLED) {
      updateVarLabel(debugLabel1,"DISTANCE ERROR",debugValue1,distError,"IN",3);
      updateVarLabel(debugLabel2,"TIMEOUT TIMER",debugValue2,timeoutTimer,"SEC",0);
    }
	}
  if(DEBUGGING_ENABLED) resetAutonDebug();
	rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	rightDrive.moveVelocity(0);
  leftDrive.moveVelocity(0);
}

void driveDistance2(double distance, double accel, double minV, double maxV, double distkP, double anglekP, int settleTime, int timeout)
{
  double simX = distance*sin(robotTheta) + robotX;
  double simY = distance*cos(robotTheta) + robotY;
  //calculates a point that is the target distance away from the robots current position
	double initX = robotX;
  double initY = robotY;
	double initTheta = robotTheta;
  //keeps track of robots intial position before the movement
	double distError;
	double angleError;
	double distSpeed;
	double angleSpeed;
	double currentSpeed = minV*1000 * distance/abs(distance);

	accel *= 1000;
	minV *= 1000;
	maxV *= 1000;
	distkP *= 1000;
	anglekP *= 1000;
  //scales all arguments to be the correct units

	int settleTimer = 0;
	int timeoutTimer = 0;
  //initialize timers

	while(settleTimer < settleTime && timeoutTimer < timeout)
	{
		distError = calcDistance_signed(simX,simY);
		if(distance < 0)
			angleError = calcAngleErrorReversed(simX,simY);
      //angle error based on the back of the robot
		else
			angleError = calcAngleError(simX,simY);
      //angle error based on the front of the robot

		angleSpeed = angleError*anglekP;
		distSpeed = distError*distkP;
    //scales angle error and distance error

		if(distSpeed > 0 && currentSpeed < distSpeed)	currentSpeed += accel;
		else if(distSpeed < 0 && currentSpeed > distSpeed) currentSpeed -= accel;
		else currentSpeed = distSpeed;
    //if going forward, accelerate forwards
    //if going reverse, accelerate backwards
    //if decelerating (whether forwards or backwards), let p-controllers determine speed

		/*if(distError > 0 && currentSpeed < minV)
			currentSpeed = minV;
		else if(distError < 0 && currentSpeed > -minV)
			currentSpeed = -minV;*/
    //makes sure currentSpeed is greater than minV

		if(fabs(distError) < 0.5 || fabs(angleError) > 85.0*M_PI/180.0)
			settleTimer+=10;
    else
      settleTimer = 0;
    //if robot is within 0.5 inches of simulated point or is roughly perpendicular to the simulated point, increase the settleTimer
    //else if robot is outside that range of error, reset settleTimer to 0
		timeoutTimer+=10;

		if(fabs(distError) < 6.0)
			angleSpeed = 0;
    //if robot is with 6 inches of target distance, robot will no longer adjust to face point
    //as that will result in the robot making sudden turns at the end of a straight movement, which is bad for straight movements

    driveVector(currentSpeed,angleSpeed,maxV); //send calculated speeds to motors
    //leftDrive.moveVoltage(currentSpeed);
  	//rightDrive.moveVoltage(currentSpeed);
		pros::delay(10);

    if(DEBUGGING_ENABLED) {
      updateVarLabel(debugLabel1,"DISTANCE ERROR",debugValue1,distError,"IN",3);
      updateVarLabel(debugLabel2,"TIMEOUT TIMER",debugValue2,timeoutTimer,"SEC",0);
    }
	}
  if(DEBUGGING_ENABLED) resetAutonDebug();
	rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	rightDrive.moveVelocity(0);
  leftDrive.moveVelocity(0);
}

void driveDistance(double distance, double maxV)
{
  driveDistance2(distance,0.5,3,maxV,0.9,1,250,5000);
}

void accel(double accel, int ms)
{
  int timer = 0;
  double currentSpeed = 0;
  while(timer < ms)
  {
    leftDrive.moveVoltage(currentSpeed);
    rightDrive.moveVoltage(currentSpeed);
    currentSpeed += accel;
    pros::delay(10);
    timer += 10;
  }
}

double pseudoI = 0.0;

void face_alg(double error, double accelTime, double minV, double medV, double maxV, double kP) //deprecated
{
  double currentSpeed = error*kP; //scales error

  if(currentSpeed < -maxV)
    currentSpeed = -maxV;
  else if(currentSpeed > maxV)
    currentSpeed = maxV;
  //limits currentSpeed to maxV

  if(error > 0 && fabs(currentSpeed) < minV)
    currentSpeed = minV;
  else if(error < 0 && fabs(currentSpeed) < minV)
    currentSpeed = -minV;
  //makes sure currentSpeed is greater than minV

  if(fabs(error) < medV/kP && fabs(error) > 0.04) {
    pseudoI += medV/accelTime*10;
    if(pseudoI > medV - minV)
      pseudoI = medV - minV;
  }
  else pseudoI = 0;

  if(error > 0)
    currentSpeed += pseudoI;
  else currentSpeed -= pseudoI;

  leftDrive.moveVoltage(currentSpeed);
  rightDrive.moveVoltage(-currentSpeed);
  //send voltages to motors
  pros::delay(10);

  if(DEBUGGING_ENABLED) {
    updateVarLabel(debugLabel1,"ERROR",debugValue1,error*180/M_PI,"DEG",3);
    updateVarLabel(debugLabel2,"CURRENT SPEED",debugValue2,currentSpeed,"mV",0);
    updateVarLabel(debugLabel3,"PSEUDO I SPEED",debugValue3,pseudoI,"mV",0);
  }
}
void facePID(double x, double y, bool reversed, double maxV, double kP, double kI, double kD, int settleTime, int timeout){
      /*
      Arguments:
      x         - x value of desired point
      y         - y value of desired point
      reversed  - true means front of robot will face, false means back of robot will face
      accel     - rate of acceleration, usually 0 for turning
      minV      - minimum voltage
      maxV      - maximum voltage
      kP        - constant for tuning angle p-controller
      settleTime- the amount of time robot must be within a certain range of the target distance before declaring the movement as finished
      timeout   - maximum amount of time the movement can take
      */
  		double error = calcAngleError(x,y);
      //calculates shortest number of radians needed to turn to face (x,y)
  		double p = 0;
      double i = 0;
      double d = 0;
      double prevError = 0;
  		int settleTimer = 0;
  		int timeoutTimer = 0;
      //initialize timers
  		//accel *= 1000;
  		//minV *= 1000;
      //medV *= 1000;
  		maxV *= 1000;
  		kP *= 1000;
      kI *= 1000;
      kD *= 1000;
      //scales all arguments to be the correct units

  		while(settleTimer < settleTime && timeoutTimer < timeout)
  		{
  				if(reversed == true)
  					error = calcAngleErrorReversed(x,y); //calculates angle error based off back of robot
  				else
  					error = calcAngleError(x,y); //calculate angle error based off front of robot

            if(fabs(error) < 0.02 || (fabs(error) < 0.04 && d < 0.01) )
    					settleTimer+=10;
            else
              settleTimer = 0;
            //if robot is within 0.04 radians (2.5 degrees) of facing (x,y), increase settleTimer
            //else reset settleTimer
    				timeoutTimer+=10;

            p = error;
            if(fabs(error) < 0.02 || fabs(error) > 0.1)
              i = 0;
            else
              i = i + error;
            d = error - prevError;
            prevError = error;

            double currentSpeed = p * kP + i * kI + d * kD;
            if (fabs(currentSpeed) > 8000) currentSpeed = 8000*currentSpeed/fabs(currentSpeed);

          leftDrive.moveVoltage(currentSpeed);
          rightDrive.moveVoltage(-currentSpeed);
          pros::delay(10);
          if(DEBUGGING_ENABLED) {
            updateVarLabel(debugLabel1,"ERROR",debugValue1,error*180/M_PI,"DEG",3);
            updateVarLabel(debugLabel2,"P SPEED",debugValue2,p*kP,"mV",0);
            updateVarLabel(debugLabel3,"I SPEED",debugValue3,i*kI,"mV",0);
            updateVarLabel(debugLabel4,"D SPEED",debugValue4,d*kD,"mV",0);
          }
  		}
      if(DEBUGGING_ENABLED) resetAutonDebug();
  		rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
  		leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
  		rightDrive.moveVelocity(0);
  	  leftDrive.moveVelocity(0);
}

void facePID(double x, double y, double kP,double kI, double kD)
{
  facePID(x, y, false, 12, kP, kI, kD, 200, 5000);
}

void facePID(double theta, bool reversed, double maxV, double kP, double kI, double kD, int settleTime, int timeout){
      /*
      Arguments:
      x         - x value of desired point
      y         - y value of desired point
      reversed  - true means front of robot will face, false means back of robot will face
      accel     - rate of acceleration, usually 0 for turning
      minV      - minimum voltage
      maxV      - maximum voltage
      kP        - constant for tuning angle p-controller
      settleTime- the amount of time robot must be within a certain range of the target distance before declaring the movement as finished
      timeout   - maximum amount of time the movement can take
      */
  		double error = calcAngleError(theta);
      //calculates shortest number of radians needed to turn to face (x,y)
  		double p = 0;
      double i = 0;
      double d = 0;
      double prevError = 0;
  		int settleTimer = 0;
  		int timeoutTimer = 0;
  		maxV *= 1000;
  		kP *= 1000;
      kI *= 1000;
      kD *= 1000;
      //scales all arguments to be the correct units

  		while(settleTimer < settleTime && timeoutTimer < timeout)
  		{
  				if(reversed == true)
  					error = calcAngleErrorReversed(theta); //calculates angle error based off back of robot
  				else
  					error = calcAngleError(theta); //calculate angle error based off front of robot

  				if(fabs(error) < 0.02 || (fabs(error) < 0.04 && d < 0.01) )
  					settleTimer+=10;
          else
            settleTimer = 0;
          //if robot is within 0.04 radians (2.5 degrees) of facing (x,y), increase settleTimer
          //else reset settleTimer
  				timeoutTimer+=10;

          p = error;
          if(fabs(error) < 0.02 || fabs(error) > 0.1)
            i = 0;
          else
            i = i + error;
          d = error - prevError;
          prevError = error;

          double currentSpeed = p * kP + i * kI + d * kD;
          if (fabs(currentSpeed) > 8000) currentSpeed = 8000*currentSpeed/fabs(currentSpeed);

          leftDrive.moveVoltage(currentSpeed);
          rightDrive.moveVoltage(-currentSpeed);
          pros::delay(10);
          if(DEBUGGING_ENABLED) {
            updateVarLabel(debugLabel1,"ERROR",debugValue1,error*180/M_PI,"DEG",3);
            updateVarLabel(debugLabel2,"P SPEED",debugValue2,p*kP,"mV",0);
            updateVarLabel(debugLabel3,"I SPEED",debugValue3,i*kI,"mV",0);
            updateVarLabel(debugLabel4,"D SPEED",debugValue4,d*kD,"mV",0);
          }
  		}
      if(DEBUGGING_ENABLED) resetAutonDebug();
  		rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
  		leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
  		rightDrive.moveVelocity(0);
  	  leftDrive.moveVelocity(0);
}

void facePID(double theta, double kP, double kI, double kD)
{
  facePID(theta, false, 12, kP,kI,kD, 200, 10000);
}

void delayFacePID(double theta, bool reversed, double maxV, double kP, double kI, double kD, int timeout, int delay, int speed){
      /*
      Arguments:
      x         - x value of desired point
      y         - y value of desired point
      reversed  - true means front of robot will face, false means back of robot will face
      accel     - rate of acceleration, usually 0 for turning
      minV      - minimum voltage
      maxV      - maximum voltage
      kP        - constant for tuning angle p-controller
      settleTime- the amount of time robot must be within a certain range of the target distance before declaring the movement as finished
      timeout   - maximum amount of time the movement can take
      */
  		double error = calcAngleError(theta);
      //calculates shortest number of radians needed to turn to face (x,y)
  		double p = 0;
      double i = 0;
      double d = 0;
      double prevError = 0;
  		int settleTimer = 0;
  		int timeoutTimer = 0;
  		maxV *= 1000;
  		kP *= 1000;
      kI *= 1000;
      kD *= 1000;
      //scales all arguments to be the correct units

  		while(fabs(error) > 0.04)
  		{
  				if(reversed == true)
  					error = calcAngleErrorReversed(theta); //calculates angle error based off back of robot
  				else
  					error = calcAngleError(theta); //calculate angle error based off front of robot

  				if((fabs(error) < 0.04))
  					delayDrive(delay,speed);
          //if robot is within 0.04 radians (2.5 degrees) of facing (x,y), increase settleTimer
          //else reset settleTimer
  				timeoutTimer+=10;

          p = error;
          if(fabs(error) < 0.02 || fabs(error) > 0.1)
            i = 0;
          else
            i = i + error;
          d = error - prevError;
          prevError = error;

          double currentSpeed = p * kP + i * kI + d * kD;
          if (fabs(currentSpeed) > 8000) currentSpeed = 8000*currentSpeed/fabs(currentSpeed);

          leftDrive.moveVoltage(currentSpeed);
          rightDrive.moveVoltage(-currentSpeed);
          pros::delay(10);
          if(DEBUGGING_ENABLED) {
            updateVarLabel(debugLabel1,"ERROR",debugValue1,error*180/M_PI,"DEG",3);
            updateVarLabel(debugLabel2,"P SPEED",debugValue2,p*kP,"mV",0);
            updateVarLabel(debugLabel3,"I SPEED",debugValue3,i*kI,"mV",0);
            updateVarLabel(debugLabel4,"D SPEED",debugValue4,d*kD,"mV",0);
          }
  		}
      if(DEBUGGING_ENABLED) resetAutonDebug();
  		rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
  		leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
  		rightDrive.moveVelocity(0);
  	  leftDrive.moveVelocity(0);
}


void face(double x, double y, bool reversed, double accelTime, double minV, double medV, double maxV, double kP, int settleTime, int timeout)
{
    /*
    Arguments:
    x         - x value of desired point
    y         - y value of desired point
    reversed  - true means front of robot will face, false means back of robot will face
    accel     - rate of acceleration, usually 0 for turning
    minV      - minimum voltage
    maxV      - maximum voltage
    kP        - constant for tuning angle p-controller
    settleTime- the amount of time robot must be within a certain range of the target distance before declaring the movement as finished
    timeout   - maximum amount of time the movement can take
    */
		double error = calcAngleError(x,y);
    //calculates shortest number of radians needed to turn to face (x,y)
		double currentSpeed;
    pseudoI = 0.0;

		int settleTimer = 0;
		int timeoutTimer = 0;
    //initialize timers
		//accel *= 1000;
		minV *= 1000;
    medV *= 1000;
		maxV *= 1000;
		kP *= 1000;
    //scales all arguments to be the correct units

		while(settleTimer < settleTime && timeoutTimer < timeout)
		{
				if(reversed == true)
					error = calcAngleErrorReversed(x,y); //calculates angle error based off back of robot
				else
					error = calcAngleError(x,y); //calculate angle error based off front of robot

				if(fabs(error) < 0.04)
					settleTimer+=10;
        else
          settleTimer = 0;
        //if robot is within 0.04 radians (2.5 degrees) of facing (x,y), increase settleTimer
        //else reset settleTimer
				timeoutTimer+=10;

      face_alg(error,accelTime,minV,medV,maxV,kP);
		}
    if(DEBUGGING_ENABLED) resetAutonDebug();
		rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		rightDrive.moveVelocity(0);
	  leftDrive.moveVelocity(0);
}

void face(double theta, bool reversed, double accelTime, double minV, double medV, double maxV, double kP, int settleTime, int timeout)
//does exact same thing as the other face, but instead of inputting (x,y) point, it takes a specific absolute theta as an argument
{
		double error = calcAngleError(theta);
		double currentSpeed;
    pseudoI = 0.0;
		int settleTimer = 0;
		int timeoutTimer = 0;
		//accel *= 1000;
		minV *= 1000;
    medV *= 1000;
		maxV *= 1000;
		kP *= 1000;

		while(settleTimer < settleTime && timeoutTimer < timeout)
		{
				if(reversed == true)
					error = calcAngleErrorReversed(theta);
				else
					error = calcAngleError(theta);

				if(fabs(error) < 0.04)
					settleTimer+=10;
        else
          settleTimer = 0;
				timeoutTimer+=10;

				face_alg(error,accelTime,minV,medV,maxV,kP);
		}
    if(DEBUGGING_ENABLED) resetAutonDebug();
		rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		rightDrive.moveVelocity(0);
	  leftDrive.moveVelocity(0);
}

void face(double x, double y)
{
  face(x,y,false,500,0,2.5,10,5.8,250,5000);
}

void face(double theta)
{
  face(theta,false,750,0,2.5,10,5.8,250,5000);
}

void adaptiveDrive(double x, double y, double accel, double maxV, double distkP, double anglekP, double scalePower, int settleTime, int timeout)
{
  /*
  Arguments:
  x           - desired x coordinate
  y           - desired y coordinate
  accel       - rate of acceleration, affeceted by distance p-controller
  maxV        - maximum voltage
  distkP      - constant for tuning distance p-controller
  anglekP     - constant for tuning angle p-controller
  scalePower  - used to tune how soon distkP should start ramping up
  settleTime  - the amount of time robot must be within a certain range of the target point before declaring the movement as finished
  timeout     - maximum amount of time the movement can take
  debugOn     - disables/enables debugging
  */
	double initX = robotX;
	double initY = robotY;
	double initTheta = robotTheta;
  //saves intial position the robot

	double distError; //distance from robots current position to target point
	double angleError; //how much robot has to turn to face the target point

	double distSpeed; //speed based on distError*distkP
	double angleSpeed; //speed based on angleError*anglekP
	double currentSpeed; //speed sent to robot based on distkP, used for controlling acceleration

	double projection; //vector projection to calculate how far robot has to travel to get as close to target point WITHOUT changing its heading
	double distkPScale; //used to scale distkP
	double scaledDistkP; //scaled distkP value

	double leftSpeed; //speed of left side
	double rightSpeed; //speed of right side

	accel *= 1200;
	//minV *= 1000;
	maxV *= 1000;
	distkP *= 1000;
	anglekP *= 1000;
  //scales all arguments to be the correct units

	int settleTimer = 0;
	int timeoutTimer = 0;
  //initialize timers

	double settleMargin = 0.5; //if robot is this distance from target point, robot is settling
	double adjustMargin = 6.0; //if robot is this distance from the target point, stop adjusting angle
	double minSpeedMargin = 3.0; //if robot is this distance from the target point, don't decrease in speed anymore
  //measured in inches

	while(settleTimer < settleTime && timeoutTimer < timeout)
	{
		distError = calcDistance(x,y);
    //calculate distance to target point
		angleError = calcAngleError(x,y);
    //calculate shortest angle to face target point
		projection = fabs(distError)*cos(angleError);
    //if you represent the robot as a unit vector with a direction of robotTheta (call this robot vector), and represent the distance and angle between the target point and the robot as another vector (call this point vector)
    //you can prorject the robot's vector onto the point vector. The magnitude of the projected vector represents how far the robot can travel to get as close to the target point as possible without changing its heading
    //as the robot's heading is adjusted by the angle p-controller, the magnitude of the projected vector will become closer and closer to the actual distError
		if(projection < 0) projection = 0;
    //prevents robot from moving backwards
    //with adaptiveDrive, robot should only be able allowed to move ONE direction, allowing it to have the option to move forwards and backwards whenever it chooses creates strange and erratic behavior
		distkPScale = pow(fabs(projection/distError),scalePower);
    scaledDistkP = distkP * distkPScale;
    //calculates a new scaled distkP based on projection/distError
    //scale power is used to tune sensitivity of scaled kP

		if(fabs(distError) < settleMargin || (fabs(distError) < adjustMargin && fabs(angleError) > 85.0*M_PI/180.0) )
			settleTimer+=10;
    else
      settleTimer = 0;
		timeoutTimer+=10;
    //if robot is within settleMargin of point or robot is with adjustMargin and is roughly perpendicular to the point settleTimer will increase

		if(fabs(distError) < minSpeedMargin) {
			angleSpeed = 0;
			distSpeed = (distError/fabs(distError))*minSpeedMargin*distkP;
      //if robot is within minSpeedMargin of the target point, the robot's speed is locked and angleSpeed is set to 0
		}
		else if(fabs(distError) < adjustMargin) {
			angleSpeed = 0;
			distSpeed = distError*distkP;
      //if robot is within adjustMargin of the target point, robot's speed is still p-controlled, but angleSpeed is set to 0
		}
		else {
      angleSpeed = angleError*anglekP;
			distSpeed = distError*scaledDistkP;
      //if robot is outside either of those margins, angle and dist speed are controlled by p-controller
    }

    if(currentSpeed < distSpeed)	currentSpeed += accel;
		else currentSpeed = distSpeed;
    //distSpeed is aways positive and robot doesn't go backwards, so if currentSpeed < distSpeed then accelerate
    //if decelerating let p-controllers determine speed

		driveVector(currentSpeed,angleSpeed,maxV); //send speeds to motors
		pros::delay(10);

    if(DEBUGGING_ENABLED) {
      updateVarLabel(debugLabel1,"DISTANCE ERROR",debugValue1,distError,"IN",3);
      updateVarLabel(debugLabel2,"ANGLE ERROR",debugValue2,angleError*180/M_PI,"DEG",3);
    }
	}
	rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	rightDrive.moveVelocity(0);
  leftDrive.moveVelocity(0);
}

void adaptiveDrive_reversed(double x, double y, double accel, double maxV, double distkP, double anglekP, double scalePower, int settleTime, int timeout)
{
  /*
  Arguments:
  x           - desired x coordinate
  y           - desired y coordinate
  accel       - rate of acceleration, affeceted by distance p-controller
  maxV        - maximum voltage
  distkP      - constant for tuning distance p-controller
  anglekP     - constant for tuning angle p-controller
  scalePower  - used to tune how soon distkP should start ramping up
  settleTime  - the amount of time robot must be within a certain range of the target point before declaring the movement as finished
  timeout     - maximum amount of time the movement can take
  debugOn     - disables/enables debugging
  */
	double initX = robotX;
	double initY = robotY;
	double initTheta = robotTheta;
  //saves intial position the robot

	double distError; //distance from robots current position to target point
	double angleError; //how much robot has to turn to face the target point

	double distSpeed; //speed based on distError*distkP
	double angleSpeed; //speed based on angleError*anglekP
	double currentSpeed; //speed sent to robot based on distkP, used for controlling acceleration

	double projection; //vector projection to calculate how far robot has to travel to get as close to target point WITHOUT changing its heading
	double distkPScale; //used to scale distkP
	double scaledDistkP; //scaled distkP value

	double leftSpeed; //speed of left side
	double rightSpeed; //speed of right side

	accel *= 1200;
	//minV *= 1000;
	maxV *= 1000;
	distkP *= 1000;
	anglekP *= 1000;
  //scales all arguments to be the correct units

	int settleTimer = 0;
	int timeoutTimer = 0;
  //initialize timers

	double settleMargin = 0.5; //if robot is this distance from target point, robot is settling
	double adjustMargin = 6.0; //if robot is this distance from the target point, stop adjusting angle
	double minSpeedMargin = 3.0; //if robot is this distance from the target point, don't decrease in speed anymore
  //measured in inches

	while(settleTimer < settleTime && timeoutTimer < timeout)
	{
		distError = calcDistance(x,y);
    //calculate distance to target point
		angleError = calcAngleErrorReversed(x,y);
    //calculate shortest angle to face target point
		projection = fabs(distError)*cos(angleError);
    //if you represent the robot as a unit vector with a direction of robotTheta (call this robot vector), and represent the distance and angle between the target point and the robot as another vector (call this point vector)
    //you can prorject the robot's vector onto the point vector. The magnitude of the projected vector represents how far the robot can travel to get as close to the target point as possible without changing its heading
    //as the robot's heading is adjusted by the angle p-controller, the magnitude of the projected vector will become closer and closer to the actual distError
		if(projection < 0) projection = 0;
    //prevents robot from moving backwards
    //with adaptiveDrive, robot should only be able allowed to move ONE direction, allowing it to have the option to move forwards and backwards whenever it chooses creates strange and erratic behavior
		distkPScale = pow(fabs(projection/distError),scalePower);
    scaledDistkP = distkP * distkPScale;
    //calculates a new scaled distkP based on projection/distError
    //scale power is used to tune sensitivity of scaled kP

		if(fabs(distError) < settleMargin || (fabs(distError) < adjustMargin && fabs(angleError) > 85.0*M_PI/180.0) )
			settleTimer+=10;
    else
      settleTimer = 0;
		timeoutTimer+=10;
    //if robot is within settleMargin of point or robot is with adjustMargin and is roughly perpendicular to the point settleTimer will increase

		if(fabs(distError) < minSpeedMargin) {
			angleSpeed = 0;
			distSpeed = (distError/fabs(distError))*minSpeedMargin*distkP;
      //if robot is within minSpeedMargin of the target point, the robot's speed is locked and angleSpeed is set to 0
		}
		else if(fabs(distError) < adjustMargin) {
			angleSpeed = 0;
			distSpeed = distError*distkP;
      //if robot is within adjustMargin of the target point, robot's speed is still p-controlled, but angleSpeed is set to 0
		}
		else {
      angleSpeed = angleError*anglekP;
			distSpeed = distError*scaledDistkP;
      //if robot is outside either of those margins, angle and dist speed are controlled by p-controller
    }

    if(currentSpeed < distSpeed)	currentSpeed += accel;
		else currentSpeed = distSpeed;
    //distSpeed is aways positive and robot doesn't go backwards, so if currentSpeed < distSpeed then accelerate
    //if decelerating let p-controllers determine speed

		driveVector(-currentSpeed,angleSpeed,maxV); //send speeds to motors
		pros::delay(10);

    if(DEBUGGING_ENABLED) {
      updateVarLabel(debugLabel1,"DISTANCE ERROR",debugValue1,distError,"IN",3);
      updateVarLabel(debugLabel2,"ANGLE ERROR",debugValue2,angleError*180/M_PI,"DEG",3);
    }
	}
	rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	rightDrive.moveVelocity(0);
  leftDrive.moveVelocity(0);
}

void adaptiveDrive_reversed(double x, double y, double maxV){
  adaptiveDrive_reversed(x,y,0.2,maxV,0.7,5.0,1.0,250,10000);
}
void adaptiveDrive(double x, double y, double maxV)
{
	adaptiveDrive(x,y,0.2,maxV,0.7,5.0,1.0,250,10000);
}

void delayDrive(int ms,double vel){
  rightDrive.moveVoltage(vel);
  leftDrive.moveVoltage(vel);
  pros::delay(ms);
  rightDrive.moveVoltage(0);
  leftDrive.moveVoltage(0);

}

double quadX(double x,double y,double t){
  return 45*t + x;
}
double quadY(double x,double y,double t){
  return -pow(10*(t-.5),2) + y + 25.0;
}
double quadDerivX(double x,double y,double t){
  return 15;
}
double quadDerivY(double x,double y,double t){
  return -2*(10*(t-.5))*10;
}
double dydx(double x, double y, double t){
  return quadDerivY(x,y,t)/quadDerivX(x,y,t);
}

double lineX(double x, double y, double slope, double t){
  return x + slope * t;
}
double lineY(double x, double y, double slope, double t){
  return y + slope * t;
}
void followQuad(int numPoints){
  double deltaT = 1.0/numPoints;
  double len = 63.829;
  double t = deltaT;
  double x = robotX;
  double y = robotY;
  for(int i = 1; i <= numPoints; i++){
    double x1 = quadX(x,y,t);
    double y1 = quadY(x,y,t);
    updateVarLabel(debugLabel1,"QUAD X",debugValue1,x1,"IN",3);
    updateVarLabel(debugLabel2,"QUAD Y",debugValue2,y1,"IN",3);
    updateVarLabel(debugLabel3,"t",debugValue3,t,"SECONDS",3);
    updateVarLabel(debugLabel4,"dt",debugValue4,deltaT,"SECONDS",3);
    //face(x1,y1);
    //driveDistance(calcDistance(quadX(x,y,t),quadY(x,y,t)), 10);
    adaptiveDrive(x1, y1, 8);
    t+=deltaT;
    //pros::delay(deltaT*20);
  }
}
void followQuadDeriv(int numPoints){
  double t = 0;
  double deltaT = 1.0/numPoints;
  double x = robotX;
  double y = robotY;
  double s = 0.5;
  for(int i = 0; i <= numPoints; i++){
    double a = lineX(x,y,quadDerivX(x,y,t),s);
    double b = lineY(x,y,quadDerivY(x,y,t),s);
    updateVarLabel(debugLabel1,"QUAD X",debugValue1,a,"IN",3);
    updateVarLabel(debugLabel2,"QUAD Y",debugValue1,b,"IN",3);
    face(a,b);
    driveDistance(calcDistance(a,b), 8);
    t+=deltaT;
    pros::delay(deltaT*20);
  }
}
double findLength(double startT, double endT, int numPoints){
    double t = (endT - startT)/numPoints;
    double total = 0;
    while(startT < endT){
      double p1x = quadX(robotX,robotY,startT);
      double p2x = quadX(robotX,robotY,startT + t);
      double p1y = quadY(robotX,robotY,startT);
      double p2y = quadY(robotX,robotY,startT + t);
      total += calcDistance(p1x,p1y,p2x,p2y);
      startT += t;
    }
    return total;
}

double findLength(std::pair<double,double> points[], int cPoint, int numPoints, double deltaT, double x1, double y1)
{
  double total = 0;
  //double t = deltaT;
  for(int n = cPoint; n < numPoints; n++)
  {
    double p1x = quadX(x1,y1,n*deltaT);
    double p2x = quadX(x1,y1,(n+1)*deltaT);
    double p1y = quadY(x1,y1,n*deltaT);
    double p2y = quadY(x1,y1,(n+1)*deltaT);
    total += calcDistance(p1x,p1y,p2x,p2y);
  }
  return total;
}

void followQuadDrive(int numPoints, double accel, double maxV, double distkP, double anglekP, double scalePower, int settleTime, int timeout, double dkP)
{
  double deltaT = 1.0/numPoints;
  double t = 0;
  double initX = robotX;
	double initY = robotY;
	double initTheta = robotTheta;
  //saves intial position the robot

  double real_distError;
  double filter_distError;
//	double distError; //distance from robots current position to target point
  double sub_distError;
	double angleError; //how much robot has to turn to face the target point

	double distSpeed; //speed based on distError*distkP
	double angleSpeed; //speed based on angleError*anglekP
	double currentSpeed; //speed sent to robot based on distkP, used for controlling acceleration

	double projection; //vector projection to calculate how far robot has to travel to get as close to target point WITHOUT changing its heading
	double distkPScale; //used to scale distkP
	double scaledDistkP; //scaled distkP value

	double leftSpeed; //speed of left side
	double rightSpeed; //speed of right side

	accel *= 12000;
	//minV *= 1000;
	maxV *= 1000;
	distkP *= 1000;
	anglekP *= 1000;
  dkP *= 1000;
  //scales all arguments to be the correct units

	int settleTimer = 0;
	int timeoutTimer = 0;
  //initialize timers

	double settleMargin = 0.5; //if robot is this distance from target point, robot is settling
	double adjustMargin = 6.0; //if robot is this distance from the target point, stop adjusting angle
	double minSpeedMargin = 3.0; //if robot is this distance from the target point, don't decrease in speed anymore

  double x;
  double y;
  double deriv;
  double derivAdjust;

  int pointCounter = 0;
  std::pair <double, double> waypoints[numPoints];
  for(int n = 0; n < numPoints; n++) { //fill waypoints
    t += deltaT;
    waypoints[n].first = quadX(initX,initY,t);
    waypoints[n].second = quadY(initX,initY,t);
  }
  while(settleTimer < settleTime && timeoutTimer < timeout)
  {
    x = waypoints[pointCounter].first;
    y = waypoints[pointCounter].second;
    deriv = dydx(initX,initY,(pointCounter+1)*deltaT);

    sub_distError = calcDistance(x,y);

    real_distError = calcDistance(x,y) + findLength(waypoints,pointCounter,numPoints-2,deltaT,initX,initY);

    if(real_distError > maxV/distkP) filter_distError = maxV/distkP;
    else filter_distError = real_distError;

		angleError = calcAngleError(x,y);
		projection = fabs(sub_distError)*cos(angleError);
		if(projection < 0) projection = 0;
		distkPScale = pow(fabs(projection/sub_distError),scalePower);
    scaledDistkP = distkP * distkPScale;

		if(fabs(real_distError) < settleMargin || (fabs(real_distError) < adjustMargin && fabs(angleError) > 85.0*M_PI/180.0) )
			settleTimer+=10;
    else
      settleTimer = 0;
		timeoutTimer+=10;

		if(fabs(sub_distError) < minSpeedMargin) {
      pointCounter++;
      if(pointCounter > numPoints-1)
        pointCounter = numPoints-1;
			angleSpeed = 0;
			distSpeed = (filter_distError/fabs(filter_distError))*minSpeedMargin*distkP;
		}
		else if(fabs(real_distError) < adjustMargin) {
			angleSpeed = 0;
			distSpeed = filter_distError*distkP;
		}
		else {
      angleSpeed = angleError*anglekP;
			distSpeed = filter_distError*scaledDistkP;
    }

    if(currentSpeed < distSpeed)	currentSpeed += accel;
		else currentSpeed = distSpeed;

    if(currentSpeed > maxV) currentSpeed = maxV;

    /*if(abs(deriv) < 2) {
      derivAdjust = (1/abs(deriv)+0.0000001)*dkP;
      if(derivAdjust >= maxV-5000) derivAdjust = maxV-5000;
    }
    else derivAdjust = 0;*/

		driveVector(currentSpeed-derivAdjust,angleSpeed,maxV); //send speeds to motors
		pros::delay(10);

    if(true) {
      updateVarLabel(debugLabel1,"DISTANCE ERROR",debugValue1,real_distError,"IN",3);
      updateVarLabel(debugLabel2,"ANGLE ERROR",debugValue2,angleError*180/M_PI,"DEG",3);
      updateVarLabel(debugLabel3,"CURRENT POINT",debugValue3,pointCounter,"",0);
      updateVarLabel(debugLabel4,"CURRENT X",debugValue4,x,"IN",0);
      updateVarLabel(debugLabel5,"CURRENT Y",debugValue5,y,"IN",0);
      updateVarLabel(debugLabel6,"DERIV",debugValue6,deriv,"",3);

      /*updateVarLabel(debugLabel1,"POINT 1 X",debugValue1,waypoints[6].first,"IN",3);
      updateVarLabel(debugLabel2,"POINT 1 Y",debugValue2,waypoints[6].second,"IN",3);
      updateVarLabel(debugLabel3,"POINT 2 X",debugValue3,waypoints[7].first,"IN",0);
      updateVarLabel(debugLabel4,"POINT 2 Y",debugValue4,waypoints[7].second,"IN",0);
      updateVarLabel(debugLabel5,"POINT 3 X",debugValue5,waypoints[8].first,"IN",0);
      updateVarLabel(debugLabel6,"POINT 3 X",debugValue6,waypoints[8].second,"IN",0);*/
    }
  }
  rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	rightDrive.moveVelocity(0);
  leftDrive.moveVelocity(0);
}

void followQuadDrive(int numPoints, double maxV)
{
  followQuadDrive(numPoints,0.05,maxV,0.65,6.0,0.5,250,20000,0.3);
}

void waypointDrive(std::pair<double,double> waypoints[], double accel, double maxV, double distkP, double anglekP, double scalePower, int settleTime, int timeout)
{

}
