
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
		,frc::Solenoid& m_gearShift
		,CANTalon& m_leftMotor1
		,CANTalon& m_leftMotor2
		,CANTalon& m_rightMotor1
		,CANTalon& m_rightMotor2
	)
	:
	m_leftStick(m_leftStick)
	, m_rightStick(m_rightStick)
	, m_gearShift(m_gearShift)
	, m_leftMotor1(m_leftMotor1)
	, m_leftMotor2(m_leftMotor2)
	, m_rightMotor1(m_rightMotor1)
	, m_rightMotor2(m_rightMotor2)
{
	m_leftMotor1.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m_leftMotor1.ConfigEncoderCodesPerRev(120);
	m_leftMotor2.SetControlMode(CANTalon::ControlMode::kFollower);
	m_leftMotor2.ConfigEncoderCodesPerRev(120);
	m_rightMotor1.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m_rightMotor1.ConfigEncoderCodesPerRev(120);
	m_rightMotor2.SetControlMode(CANTalon::ControlMode::kFollower);
	m_rightMotor2.ConfigEncoderCodesPerRev(120);
	m_leftMotor2.Set(MOTOR_LEFT_DRIVE1);
	m_rightMotor2.Set(MOTOR_RIGHT_DRIVE1);
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

TankDrive::~TankDrive() {
 	 }

void TankDrive::TeleopInit() {

}

void TankDrive::TeleopPeriodic() {
	if (direction == 1 && m_rightStick->GetRawButton(BUTTON_R_REVERSE_DRIVE)) {
		direction = -1;
	} else if (direction == -1 && !m_rightStick->GetRawButton(BUTTON_R_REVERSE_DRIVE)) {
		direction = 1;
	}

	this->Drive(this->m_leftStick->GetY() * direction, this->m_rightStick->GetY() * direction);
	float WheelSpeed = ((m_rightMotor1.GetSpeed()+m_leftMotor1.GetSpeed())/60)/2;
	//^^above^^Gives us the average rotations per second of the two encoders

	float DriveSpeed = (WheelSpeed*(4*3.1415))/12;
	//^^above^^Gives us speed in feet per second
	if (DriveSpeed >= 6.4){
		TankDrive::HighGear();
	}

	if (this->highGear && m_leftStick->GetRawButton(BUTTON_L_SHIFT_LOW)){
		TankDrive::LowGear();
	}
	else if (!this->highGear && m_leftStick->GetRawButton(BUTTON_L_SHIFT_HIGH)){
		TankDrive::HighGear();
	}
}

void TankDrive::Position() {
	const float Circumference = 95.819;
	const float Pi = 3.141592;
	float rightPosition = m_rightMotor1.GetPosition()*(4*Pi);
	float leftPosition = m_leftMotor1.GetPosition()*(4*Pi);
	distance = ((rightPosition - rightPosOld)+(leftPosition - leftPosOld))/2;
	this->angle += ((((rightPosition - rightPosOld)-(leftPosition - leftPosOld))/Circumference)*180);
	if (0<=angle && angle<=360){
		heading = angle;
	}
	else if (angle<=0){
		heading = angle + 360;
	}
	else if (360<=angle){
		heading = angle -360;
	}
	m_xPosition = distance * cos((heading*Pi)/180) + m_xPosition;
	m_yPosition = distance * sin((heading*Pi)/180) + m_yPosition;
	rightPosOld = rightPosition;
	leftPosOld = leftPosition;
}

void TankDrive::Drive(const float leftVal, const float rightVal) {
	//left motor speed is inverted because the motors are physically opposite the right motors
	m_leftMotor1.SetSetpoint(-leftVal);
	m_rightMotor1.SetSetpoint(rightVal);
}

void TankDrive::LowGear() {
	m_gearShift.Set(false);
	this->highGear = false;

}

void TankDrive::HighGear() {
	m_gearShift.Set(true);
	this->highGear = true;
}
