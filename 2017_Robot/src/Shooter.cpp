/*
 * Shooter.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: Joey
 */


#include "Shooter.h"
#include "RobotMap.h"
#include <SmartDashboard/SmartDashboard.h>

Shooter::Shooter(
		CANTalon &m_shootWheel1
		, CANTalon &m_shootWheel2
		, CANTalon &m_indexMotor
		, frc::DigitalOutput &m_aimLight
		, frc::JoystickButton &shootJoystickButton
		, frc::JoystickButton &reverseIndexJoystickButton
		, frc::JoystickButton &aimingLightJoystickButton
		, float m_shooterSpeed
)
:
m_shootWheel1(m_shootWheel1)
, m_shootWheel2(m_shootWheel2)
, m_indexMotor(m_indexMotor)
, m_aimLight(m_aimLight)
, shootJoystickButton(shootJoystickButton)
, reverseIndexJoystickButton(reverseIndexJoystickButton)
, aimingLightJoystickButton(aimingLightJoystickButton)
, m_shooterSpeed(m_shooterSpeed)
{
	// TODO Auto-generated constructor stub

}

Shooter::~Shooter() {
	// TODO Auto-generated destructor stub
}

void Shooter::TeleopInit() {

}

void Shooter::TeleopPeriodic() {
	if (shootJoystickButton.Get()) {
		if (reverseIndexJoystickButton.Get()) {
			ReverseIndex();
		} else Shoot(m_shooterSpeed);
	} else Stop();

	if (aimingLightJoystickButton.Get()) {
		AimLight(true);
	}  else {
		AimLight(false);
	}

}


void Shooter::Shoot(float shooterSpeed) {
	double IndexVoltageFactor = 1;
	double IndexMotorAmps = m_pdp.GetCurrent(4);
	SmartDashboard::PutNumber("Index Current", IndexMotorAmps);

	if (abs(IndexMotorAmps) > 7.5) {
		IndexVoltageFactor -= .005;
		SmartDashboard::PutBoolean ("Indexer Is Jammed.", true);
	}
	else {
		SmartDashboard::PutBoolean ("Indexer Is Jammed.", false);
		IndexVoltageFactor += .005;
	}

	m_shootWheel1.SetSetpoint(shooterSpeed);
	m_shootWheel2.SetSetpoint(shooterSpeed);
	if (abs(m_shootWheel1.GetSpeed()-shooterSpeed)<=50) {
		m_indexMotor.SetSetpoint (1000);
		//TODO Find actual RPM values
	}
}

void Shooter::ReverseIndex() {
	double IndexVoltageFactor = 1;
	double IndexMotorAmps = m_pdp.GetCurrent(16666);
	SmartDashboard::PutNumber("Index Current", IndexMotorAmps);

	if (abs(IndexMotorAmps) > 7.5) {
		IndexVoltageFactor -= .005;
		SmartDashboard::PutBoolean ("Indexer Is Jammed.", true);
	}
	else {
		SmartDashboard::PutBoolean ("Indexer Is Jammed.", false);
		IndexVoltageFactor += .005;
	}
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

void Shooter::AimLight(bool state) {

	if(state)
		m_aimLight.UpdateDutyCycle(0.7);
	else
		m_aimLight.UpdateDutyCycle(0);

}
