/*
 * Shooter.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: Joey
 */

#include "RobotMap.h"
#include "Shooter.h"
Shooter::Shooter()
	: m_shootWheel1(0), m_shootWheel2(1), m_indexMotor(0)
{
	// TODO Auto-generated constructor stub

}

Shooter::~Shooter() {
	// TODO Auto-generated destructor stub
}

void Shooter::Shoot() {

	m_shootWheel1.SetSetpoint(1000);
	//TODO Find actual RPM value
	m_shootWheel2.SetSetpoint(1000);
	//TODO Find actual RPM value
	if (m_shootWheel1.GetSpeed()>=1000) {
		m_indexMotor.SetSetpoint (1000);
		//TODO Find actual RPM values
	}


}

void Shooter::ReverseIndex() {

	m_indexMotor.SetSetpoint (-1000);
}


void Shooter::Stop() {
m_shootWheel1.SetSetpoint(0);
m_shootWheel2.SetSetpoint(0);

}

void Shooter::Init() {
	m_shootWheel1.SetControlMode(frc::CANSpeedController::kSpeed);
	m_shootWheel2.SetControlMode(frc::CANSpeedController::kSpeed);
}



