#include "main.h"
//================ Odometry Variables ================

const double WHEEL_DIAMETER = 2.875;
const double ENCODER_WIDTH = 7.0;
const double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER*M_PI;
const double IMU_WEIGHT = 0.5;
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

    double dTheta = 0.0;
    double dX = 0.0;
    double dY = 0.0;

    double currentLeft = 0.0;
    double currentRight = 0.0;

    double prevLeft = 0.0;
    double prevRight = 0.0;

    double dLeftVal = 0.0;
    double dRightVal = 0.0;

    int leftReset = left.reset();
    int rightReset = right.reset();

    pros::delay(200);

    while(true)
    {
        currentLeft = left.get()/360.0*WHEEL_CIRCUMFERENCE; //read quadature encoders
        currentRight = right.get()/360.0*WHEEL_CIRCUMFERENCE;

        //currentLeft = leftDrive.getPosition()/900*WHEEL_CIRCUMFERENCE; //read integrated encoders
        //currentRight = rightDrive.getPosition()/900*WHEEL_CIRCUMFERENCE;

        dLeftVal = (currentLeft - prevLeft);
        dRightVal = (currentRight - prevRight);

        prevLeft = currentLeft; //update prev values
        prevRight = currentRight;

        dTheta = (dLeftVal - dRightVal) / ENCODER_WIDTH; //calculate change in angle in radians
        robotTheta_encoders += dTheta;
        robotTheta_encoders = fmod(robotTheta, 2*M_PI);

        robotTheta_imu = imu.get_heading()*M_PI/180.0; //imu heading in radians

        robotTheta = robotTheta_imu*IMU_WEIGHT + robotTheta_encoders*(1.0-IMU_WEIGHT); //weighted average between imu heading and encoder heading for (hopefully) more consistent heading calculation
        robotTheta = fmod(robotTheta, 2*M_PI);
        if(robotTheta < 0) robotTheta += 2*M_PI;

        dX = (dLeftVal + dRightVal)/2 * sin( (robotTheta) ); //calculate change in x
        dY = (dLeftVal + dRightVal)/2 * cos( (robotTheta) ); //calculate change in y

        robotX += dX; //add to current x and ys
        robotY += dY;

        if(DEBUGGING_ENABLED) {
          updateValueLabel(xValue,robotX, "IN",3);
          updateValueLabel(yValue,robotY, "IN",3);
          updateValueLabel(thetaValue,robotTheta*180/M_PI,"DEG",3);
        }
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
  theta = theta*M_PI/180.0;
  double radius = 100;
  double predictedX = radius*sin(robotTheta) + robotX;
  double predictedY = radius*cos(robotTheta) + robotY;
  double targetX = radius*sin(theta) + robotX;
  double targetY = radius*cos(theta) + robotY;
  double chord = calcDistance(predictedX, predictedY, targetX, targetY);

  double angleError = 2*asin( (chord / 2) / (100) );

  predictedX = radius*sin( fmod(angleError + robotTheta, 2*M_PI) ) + robotX;
  predictedY = radius*cos( fmod(angleError + robotTheta, 2*M_PI) ) + robotY;

  if( (predictedX < targetX + 0.1) && (predictedX > targetX - 0.1) && (predictedY < targetY + 0.1) && (predictedY > targetY - 0.1) )
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

	if(maxCurrentSpeed > maxV) { //scales down left and right voltage so that it is not greater than maxV
		speedScale = fabs(maxCurrentSpeed/maxV);
		leftSpeed /= speedScale;
		rightSpeed /= speedScale;
	}

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

		if(currentSpeed > 0 && currentSpeed < minV)
			currentSpeed = minV;
		else if(currentSpeed < 0 && currentSpeed > -minV)
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
	rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	rightDrive.moveVelocity(0);
  leftDrive.moveVelocity(0);
}

void driveDistance(double distance, double maxV)
{
  driveDistance(distance,0.05,2.5,maxV,0.7,4,250,5000);
}

void face(double x, double y, bool reversed, double accel, double minV, double maxV, double kP, int settleTime, int timeout)
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
    double pseudoI = 0.0;

		int settleTimer = 0;
		int timeoutTimer = 0;
    //initialize timers
		accel *= 12000;
		minV *= 1000;
		maxV *= 1000;
		kP *= 1000;
    //scales all arguments to be the correct units

		while(settleTimer < settleTime && timeoutTimer < timeout)
		{
				if(reversed == true)
					error = calcAngleErrorReversed(x,y); //calculates angle error based off back of robot
				else
					error = calcAngleError(x,y); //calculate angle error based off front of robot

				currentSpeed = error*kP; //scales error

				if(fabs(error) < 0.02)
					settleTimer+=10;
        else
          settleTimer = 0;
        //if robot is within 0.02 radians (1.2 degrees) of facing (x,y), increase settleTimer
        //else reset settleTimer
				timeoutTimer+=10;

				if(currentSpeed < -maxV)
					currentSpeed = -maxV;
				else if(currentSpeed > maxV)
					currentSpeed = maxV;
        //limits currentSpeed to maxV

				if(currentSpeed > 0 && fabs(currentSpeed) < minV)
					currentSpeed = minV;
				else if(currentSpeed < 0 && fabs(currentSpeed) < minV)
					currentSpeed = -minV;
        //makes sure currentSpeed is greater than minV

        if(fabs(error) < 2000/kP) {
          pseudoI += accel;
          if(pseudoI > 1000)
            pseudoI = 1000;
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
		rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		rightDrive.moveVelocity(0);
	  leftDrive.moveVelocity(0);
}

void face(double theta, bool reversed, double accel, double minV, double maxV, double kP, int settleTime, int timeout)
//does exact same thing as the other face, but instead of inputting (x,y) point, it takes a specific absolute theta as an argument
{
		double error = calcAngleError(theta);
		double currentSpeed;
    double pseudoI = 0.0;
		int settleTimer = 0;
		int timeoutTimer = 0;
		accel *= 12000;
		minV *= 1000;
		maxV *= 1000;
		kP *= 1000;

		while(settleTimer < settleTime && timeoutTimer < timeout)
		{
				if(reversed == true)
					error = calcAngleErrorReversed(theta);
				else
					error = calcAngleError(theta);

					currentSpeed = error*kP;

				if(fabs(error) < 0.02) //~2 deg each side
					settleTimer+=10;
        else
          settleTimer = 0;
				timeoutTimer+=10;

				if(currentSpeed < -maxV)
					currentSpeed = -maxV;
				else if(currentSpeed > maxV)
					currentSpeed = maxV;

				if(currentSpeed > 0 && fabs(currentSpeed) < minV)
					currentSpeed = minV;
				else if(currentSpeed < 0 && fabs(currentSpeed) < minV)
					currentSpeed = -minV;

        if(fabs(error) < 2000/kP) {
          pseudoI += accel;
          if(pseudoI > 1000)
            pseudoI = 1000;
        }
        else pseudoI = 0;
        if(error > 0)
          currentSpeed += pseudoI;
        else currentSpeed -= pseudoI;

				leftDrive.moveVoltage(currentSpeed);
				rightDrive.moveVoltage(-currentSpeed);
				pros::delay(10);

        if(DEBUGGING_ENABLED) {
          updateVarLabel(debugLabel1,"ERROR",debugValue1,error*180/M_PI,"DEG",3);
          updateVarLabel(debugLabel2,"CURRENT SPEED",debugValue2,currentSpeed,"mV",0);
          updateVarLabel(debugLabel3,"PSEUDO I SPEED",debugValue3,pseudoI,"mV",0);
        }
		}
		rightDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		leftDrive.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		rightDrive.moveVelocity(0);
	  leftDrive.moveVelocity(0);
}

void face(double x, double y)
{
  face(x,y,false,0.03,1.0,10,6.5,250,5000);
}

void face(double theta)
{
  face(theta,false,0.03,1.0,10,6.5,250,20000);
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

	accel *= 12000;
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

void adaptiveDrive(double x, double y, double maxV)
{
	adaptiveDrive(x,y,0.05,maxV,0.65,5.0,1.0,250,10000);
}
