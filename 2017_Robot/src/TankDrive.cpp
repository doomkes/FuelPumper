/*
 * TankDrive.cpp
 *
 *  Created on: Jan 14, 2017
 *      Author: Joey
 */

#include <TankDrive.h>
#include "Robotmap.h"
TankDrive::TankDrive()
	: m_leftMotor1(LEFT_DRIVE1),		m_leftMotor2(LEFT_DRIVE2),
	  m_rightMotor1(RIGHT_DRIVE1),	m_rightMotor2(RIGHT_DRIVE2),
	  m_gearShift(GEAR_SHIFT)
{
	m_leftMotor1.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m_leftMotor2.SetControlMode(CANTalon::ControlMode::kFollower);
	m_rightMotor1.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m_rightMotor2.SetControlMode(CANTalon::ControlMode::kFollower);

}

TankDrive::~TankDrive() {
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
