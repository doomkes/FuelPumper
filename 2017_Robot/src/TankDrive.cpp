/*
 * TankDrive.cpp
 *
 *  Created on: Jan 14, 2017
 *      Author: Joey
 */

#include "TankDrive.h"

TankDrive::TankDrive(
		frc::Joystick* m_leftStick
		,frc::Joystick* m_rightStick
		, frc::JoystickButton* joystickButton_reverseDrive
		, frc::JoystickButton* joystickButton_shiftLow
		, frc::JoystickButton* joystickButton_shiftHigh
		, frc::JoystickButton* joystickButton_vbus
		,frc::Solenoid* m_gearShift
		,CANTalon* m_leftMotor1
		,CANTalon* m_leftMotor2
		,CANTalon* m_rightMotor1
		,CANTalon* m_rightMotor2
	)
	:
	m_leftStick(m_leftStick)
	, m_rightStick(m_rightStick)
	, joystickButton_reverseDrive(joystickButton_reverseDrive)
	, joystickButton_shiftLow(joystickButton_shiftLow)
	, joystickButton_shiftHigh(joystickButton_shiftHigh)
	, joystickButton_vbus(joystickButton_vbus)
	, m_gearShift(m_gearShift)
	, m_leftMotor1(m_leftMotor1)
	, m_leftMotor2(m_leftMotor2)
	, m_rightMotor1(m_rightMotor1)
	, m_rightMotor2(m_rightMotor2)
{
	m_leftMotor1->SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m_leftMotor1->ConfigEncoderCodesPerRev(120); // enc resolution is 360 counts/rev, encoder spins at 1/3 output shaft.
	m_leftMotor2->SetControlMode(CANTalon::ControlMode::kFollower);
	m_leftMotor2->ConfigEncoderCodesPerRev(120);
	m_rightMotor1->SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m_rightMotor1->ConfigEncoderCodesPerRev(120);
	m_rightMotor2->SetControlMode(CANTalon::ControlMode::kFollower);
	m_rightMotor2->ConfigEncoderCodesPerRev(120);


	m_leftMotor1->SetCurrentLimit(30);
	m_rightMotor1->SetCurrentLimit(30);

	m_leftMotor1->EnableCurrentLimit(true);
	m_rightMotor1->EnableCurrentLimit(true);

	m_leftMotor1->SetInverted(false);
	m_rightMotor1->SetInverted(false);

	m_leftMotor1->SetSensorDirection(false);
	m_rightMotor1->SetSensorDirection(false);

	m_leftMotor2->Set(MOTOR_LEFT_DRIVE1);
	m_rightMotor2->Set(MOTOR_RIGHT_DRIVE1);



}

TankDrive::~TankDrive() {
 	 }

void TankDrive::Init() {
	direction = 1;
	highGear = true;
	leftPosOld = 0;
	rightPosOld = 0;
	m_xPosition = 0;
	m_yPosition = 0;
	distance = 0;
	angle = 0;
	heading = 0;


}
void TankDrive::TeleopPeriodic() {
	if (direction == 1 && joystickButton_reverseDrive->Get()) {
		direction = -1;
	} else if (direction == -1 && !joystickButton_reverseDrive->Get()) {
		direction = 1;
	}

	static float leftPos = 0;
	static float rightPos = 0;
//    float joystickMultiplier = Preferences::GetInstance()->GetDouble("JoystickMultpilier",1);
	float leftVal = this->m_leftStick->GetY() * direction/**joystickMultiplier*/;
	float rightVal = this->m_rightStick->GetY() * direction/**joystickMultiplier*/;
//

	static int prevMode = DriveMode::TELEPOSITION;


	// execute shift move.
	static bool shiftMoveDirLeft = false;
	if(m_leftStick->GetX() <= -0.8 && m_rightStick->GetX() <= -0.8){
		shiftMoveDirLeft = true;
		m_mode = DriveMode::SHIFT_MOVE;
	} else if(m_leftStick->GetX() >= 0.8 && m_rightStick->GetX() >= 0.8){
		shiftMoveDirLeft = false;
		m_mode = DriveMode::SHIFT_MOVE;
	}

	switch(m_mode) {
	case DriveMode::TELEPOSITION: // normal drive.
		leftPos = leftVal*10;
		rightPos= rightVal*10;

		m_leftMotor1->SetSetpoint(((m_leftMotor1->GetPosition()/m_revsPerInch) + -leftPos) * m_revsPerInch);
		m_rightMotor1->SetSetpoint(((m_rightMotor1->GetPosition()/m_revsPerInch) + rightPos) * m_revsPerInch);
		break;
	case DriveMode::VBUS: {

		if(leftVal > 1) leftVal = 1;
		else if(leftVal < -1) leftVal = -1;
		if(rightVal > 1) rightVal = 1;
		else if(rightVal < -1) rightVal = -1;

		float average = (leftVal+rightVal)/2;
		float split = (leftVal-rightVal)/2;
		float multiplier = 1-average;
		float leftScale = split*multiplier;
		float rightScale = -1 * (split*multiplier);
		leftVal+=leftScale;
		rightVal+=rightScale;

		float left = leftVal * leftVal; // square input.
		float right = rightVal * rightVal;

		left *= (leftVal < 0) ? -1 : 1;
		right *= (rightVal < 0) ? -1 : 1;
		this->Drive(left, right);
		break;
	}
	case DriveMode::SHIFT_MOVE:
		if( prevMode != DriveMode::SHIFT_MOVE) {
			ShiftMove(true, shiftMoveDirLeft);
			m_shiftMoveDone = false;
		} else {
			ShiftMove(false, shiftMoveDirLeft);
		}

		if(m_shiftMoveDone) {
			SetMode(DriveMode::TELEPOSITION);
			printf("shift move done\n");
		}
		break;
	}
	prevMode = m_mode;

	if ( joystickButton_shiftLow->Get()){
		TankDrive::LowGear();
		SmartDashboard::PutString("Gear","Low");
	}
	else {
		HighGear();
	}
	if ( joystickButton_vbus->Get()){
		this->SetMode(DriveMode::VBUS);
	}

	// Display stuff to dashboard.
	SmartDashboard::PutNumber("RobotLeftPos", m_leftMotor1->GetPosition());
	SmartDashboard::PutNumber("RobotRightPos", m_rightMotor1->GetPosition());

	SmartDashboard::PutNumber("JoysLeftPos", leftPos);
	SmartDashboard::PutNumber("JoysRightPos",rightPos);
	Position();
}

void TankDrive::Position() {
	const float Pi = 3.141592;
	//calculates position in inches from revolutions.
	float rightPosition = m_rightMotor1->GetPosition()*(4*Pi);
	float leftPosition = -m_leftMotor1->GetPosition()*(4*Pi);
	distance = ((rightPosition - rightPosOld)+(leftPosition - leftPosOld))/2;
	angle = m_gyro.GetAngle();
	float angleInRad = angle * (Pi/180);
	m_yPosition += distance * cos(angleInRad);
	m_xPosition += distance * -sin(angleInRad);
	SmartDashboard::PutNumber("robot x", -m_xPosition);
	SmartDashboard::PutNumber("robot y", m_yPosition);
	SmartDashboard::PutNumber("robot angle", angle);
	rightPosOld = rightPosition;
	leftPosOld = leftPosition;
}

void TankDrive::Drive(const float leftVal, const float rightVal) {
	//left motor speed is inverted because the motors are physically opposite the right motors
	float left = leftVal, right = rightVal;

	if(direction == -1) {
		std::swap(left, right);
	}
	m_leftMotor1->SetSetpoint(-left);
	m_rightMotor1->SetSetpoint(right);
}
void TankDrive::ShiftMove(bool start, bool dirLeft) {
	static frc::Timer timer;
	float leftPos = 0, rightPos = 0;

	const float PI = 3.14159;
	// about 5in left/right dist.
	const float amplitude = 12;
	if(start) {
		timer.Reset();
		timer.Start();
		printf("\n\n      Start Shift Move\n\n");
		printf("dir: %s\n", dirLeft ? "Left" : "Right");
		Zero();
	}
	float t = timer.Get();
	float alpha = (t/2) * (5.0/2.0)*PI;

	if(alpha < 2*PI) {
		leftPos = (-cos(alpha) + 1.0)/2.0 * amplitude;
	} else {
		leftPos = 0;
	}
	if((alpha > PI/2.0) && (alpha < 5.0/2.0 * PI)) {
		rightPos = (-cos(alpha + (3.0/2.0*PI)) + 1.0)/2.0 * amplitude;
	} else {
		rightPos = 0;
	}

	if(alpha > 5.0/2.0 * PI) {
		m_shiftMoveDone = true;
	}
	if(dirLeft) {
		swap(leftPos, rightPos);
	}

	PositionDrive(leftPos, rightPos, false);
	printf("left %f, right %f\n", leftPos, rightPos);
	printf("alpha %f, time %f\n\n", alpha, t);
}
void TankDrive::LowGear() {
	m_gearShift->Set(true);
	this->highGear = false;

}

void TankDrive::HighGear() {
	m_gearShift->Set(false);
	this->highGear = true;
}

void TankDrive::PositionDrive(float leftPos, float rightPos, bool relative) {

	if(!relative) {
		m_leftMotor1->SetSetpoint(leftPos * m_revsPerInch);
		m_rightMotor1->SetSetpoint(-rightPos * m_revsPerInch);
	} else {
		m_leftMotor1->SetSetpoint((m_leftMotor1->GetPosition() + -rightPos) * m_revsPerInch);
		m_rightMotor1->SetSetpoint((m_rightMotor1->GetPosition() + leftPos) * m_revsPerInch);
	}

	SmartDashboard::PutNumber("leftClosedLoopError", m_leftMotor1->GetClosedLoopError());
	SmartDashboard::PutNumber("rightClosedLoopError", m_rightMotor1->GetClosedLoopError());

//	SmartDashboard::PutNumber("RobotLeftPos", m_leftMotor1->GetPosition());
//	SmartDashboard::PutNumber("RobotRightPos", m_rightMotor1->GetPosition());
}

void TankDrive::StraightPositionDrive(float leftPos, float rightPos, double angleError) {
	// inches/angle error
	const float pGain = 0.01;

	const float driveCorrection = angleError * pGain;

	PositionDrive(leftPos + driveCorrection, rightPos - driveCorrection, false);
}

void TankDrive::SetMode(DriveMode mode){
	Zero();
	m_mode = mode;
	switch(mode){

	case DriveMode::SHIFT_MOVE:
	case DriveMode::TELEPOSITION:{
		m_leftMotor1->SetControlMode(frc::CANSpeedController::kPosition);
		m_rightMotor1->SetControlMode(frc::CANSpeedController::kPosition);
		//const float P = SmartDashboard::GetNumber("drive_P", 0);
		const float P = 4;
		const float I = SmartDashboard::GetNumber("drive_I", 0);
		const float D = SmartDashboard::GetNumber("drive_D", 0);
		if(P != m_leftMotor1->GetP()) {
			m_leftMotor1->SetPID(P, I, D, 0);
		}
		if(P != m_rightMotor1->GetP()) {
			m_rightMotor1->SetPID(P, I, D, 0);
		}
		break;
	}
	case DriveMode::POSITION: {

		m_leftMotor1->SetControlMode(frc::CANSpeedController::kPosition);
		m_rightMotor1->SetControlMode(frc::CANSpeedController::kPosition);
		//const float P = SmartDashboard::GetNumber("drive_P", 0);
		const float P = 15;
		const float I = SmartDashboard::GetNumber("drive_I", 0);
		const float D = SmartDashboard::GetNumber("drive_D", 0);

		if(P != m_leftMotor1->GetP()) {
			m_leftMotor1->SetPID(P, I, D, 0);
		}
		if(P != m_rightMotor1->GetP()) {
			m_rightMotor1->SetPID(P, I, D, 0);
		}
		break;
	}
	case DriveMode::VBUS:

		m_leftMotor1->SetControlMode(frc::CANSpeedController::kPercentVbus);
		m_rightMotor1->SetControlMode(frc::CANSpeedController::kPercentVbus);
		break;
	}
	m_leftMotor1->Set(0);
	m_rightMotor1->Set(0);
}

void TankDrive::Zero()
{
	m_leftMotor1->SetPosition(0);
	m_rightMotor1->SetPosition(0);
	m_leftMotor1->Set(0);
	m_rightMotor1->Set(0);
}

double TankDrive::GetAngle() {
	return m_gyro.GetAngle();
}
