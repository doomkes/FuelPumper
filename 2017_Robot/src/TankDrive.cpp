
/*
 * TankDrive.cpp
 *
 *  Created on: Jan 14, 2017
 *      Author: Joey
 */

#include <TankDrive.h>

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
	m_leftMotor2.SetControlMode(CANTalon::ControlMode::kFollower);
	m_rightMotor1.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m_rightMotor2.SetControlMode(CANTalon::ControlMode::kFollower);

}

TankDrive::~TankDrive() {
 	 }

void TankDrive::TeleopPeriodic() {
	this->Drive(-this->m_leftStick.GetY(), this->m_rightStick.GetY());
	float WheelSpeed = ((m_rightMotor1.GetSpeed()*3)+(m_leftMotor1.GetSpeed()*3))/2;
	float DriveSpeed = (WheelSpeed*(4*3.1415))/12;
	if (DriveSpeed >= 6.4){
		TankDrive::HighGear();
	}
	else {
		TankDrive::LowGear();
	}
	if (m_leftStick.GetRawButton(7)){
		TankDrive::HighGear();
	}
	else {
		TankDrive::LowGear();
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
