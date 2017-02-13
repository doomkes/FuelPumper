
/*
 * TankDrive.cpp
 *
 *  Created on: Jan 14, 2017
 *      Author: Joey
 */

#include "TankDrive.h"

TankDrive::TankDrive(
		frc::Joystick &m_leftStick
		,frc::Joystick &m_rightStick
		,frc::Solenoid &m_gearShift
		,CANTalon &m_leftMotor1
		,CANTalon &m_leftMotor2
		,CANTalon &m_rightMotor1
		,CANTalon &m_rightMotor2
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
	m_leftMotor2.Set(LEFT_DRIVE1);
	m_rightMotor2.Set(RIGHT_DRIVE1);
	direction = 1;
}

TankDrive::~TankDrive() {
 	 }

void TankDrive::TeleopPeriodic() {
	if (m_rightStick.GetRawButton(REVERSE_DRIVE) && direction == 1) {
		direction = -1;
	} else if (!m_rightStick.GetRawButton(REVERSE_DRIVE) && direction == -1) {
		direction = 1;
	}

	this->Drive(-this->m_leftStick.GetY() * direction, this->m_rightStick.GetY() * direction);
	float WheelSpeed = ((m_rightMotor1.GetSpeed()+m_leftMotor1.GetSpeed())/60)/2;
	//^^above^^Gives us the average rotations per second of the two encoders

	float DriveSpeed = (WheelSpeed*(4*3.1415))/12;
	//^^above^^Gives us speed in feet per second
	if (DriveSpeed >= 6.4){
		TankDrive::HighGear();
	}

	if (m_leftStick.GetRawButton(SHIFT_LOW)){
		TankDrive::LowGear();
	}
	else if (m_leftStick.GetRawButton(SHIFT_HIGH)){
		TankDrive::HighGear();
	}
}

void TankDrive::Drive(const float leftVal, const float rightVal) {
	m_leftMotor1.SetSetpoint(leftVal);
	m_rightMotor1.SetSetpoint(rightVal);
}

void TankDrive::LowGear() {
	m_gearShift.Set(false);

}

void TankDrive::HighGear() {
	m_gearShift.Set(true);
}
