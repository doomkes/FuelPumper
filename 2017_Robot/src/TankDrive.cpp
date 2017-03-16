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

	this->Drive(this->m_leftStick->GetY() * direction, this->m_rightStick->GetY() * direction);
	float WheelSpeed = ((fabs(m_rightMotor1->GetSpeed())+fabs(m_leftMotor1->GetSpeed()))/60)/2;
	//^^above^^Gives us the average rotations per second of the two encoders

	float DriveSpeed = (WheelSpeed*(4*3.1415))/12;
	//^^above^^Gives us speed in feet per second
	if (DriveSpeed >= 6.4){
		TankDrive::HighGear();
	}

	if ( joystickButton_shiftLow->Get()){
		TankDrive::LowGear();
		SmartDashboard::PutString("Gear","Low");

	}
	else {
		HighGear();
	}
//	else if ( joystickButton_shiftHigh->Get()){
//		TankDrive::HighGear();
//		SmartDashboard::PutString("Gear","High");
//	}
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
	left *= left; // square input.
	right *= right;
	left *= (leftVal < 0) ? -1 : 1;
	right *= (rightVal < 0) ? -1 : 1;
	if(direction == -1) {
		std::swap(left, right);
	}
	m_leftMotor1->SetSetpoint(-left);
	m_rightMotor1->SetSetpoint(right);
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
	SmartDashboard::PutNumber("RobotLeftPos", m_leftMotor1->GetPosition());
	SmartDashboard::PutNumber("RobotRightPos", m_rightMotor1->GetPosition());
}

void TankDrive::StraightPositionDrive(float leftPos, float rightPos, double angleError) {
	// inches/angle error
	const float pGain = 0.01;

	const float driveCorrection = angleError * pGain;

	PositionDrive(leftPos + driveCorrection, rightPos - driveCorrection);
}

void TankDrive::SetMode(DriveMode mode){
	Zero();
	switch(mode){

	case DriveMode::SPEED:

		m_leftMotor1->SetControlMode(frc::CANSpeedController::kSpeed);
		m_rightMotor1->SetControlMode(frc::CANSpeedController::kSpeed);
		break;
	case DriveMode::POSITION: {

		m_leftMotor1->SetControlMode(frc::CANSpeedController::kPosition);
		m_rightMotor1->SetControlMode(frc::CANSpeedController::kPosition);
		//const float P = SmartDashboard::GetNumber("drive_P", 0);
		const float P = 15;
		const float I = SmartDashboard::GetNumber("drive_I", 0);
		const float D = SmartDashboard::GetNumber("drive_D", 0);

		m_leftMotor1->SetPID(P, I, D, 0);
		m_rightMotor1->SetPID(P, I, D, 0);
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
