/*
 * Shooter.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: Joey
 */


#include "Shooter.h"
#include "RobotMap.h"

Shooter::Shooter(
		CANTalon* m_shootWheel1
		, CANTalon* m_shootWheel2
		, CANTalon* m_indexMotor
		, CANTalon* m_shooterFeeder
		, frc::DigitalOutput* m_aimLight
		, frc::JoystickButton* joystickButton_shoot
		, frc::JoystickButton* joystickButton_reverseIndex
		, frc::JoystickButton* joystickButton_aimingLight
		, float m_shooterSpeed
)
:
m_particleAccelerator(m_shootWheel1)
, m_afterBurner(m_shootWheel2)
, m_indexMotor(m_indexMotor)
, m_aimLight(m_aimLight)
, joystickButton_shoot(joystickButton_shoot)
, joystickButton_reverseIndex(joystickButton_reverseIndex)
, joystickButton_aimingLight(joystickButton_aimingLight)
, m_shooterSpeed(m_shooterSpeed)
, m_shooterFeeder(m_shooterFeeder)
{

}

Shooter::~Shooter() {
	// TODO Auto-generated destructor stub
}

void Shooter::TeleopInit() {

}

void Shooter::TeleopPeriodic() {
	if (joystickButton_shoot->Get()) {
//		if (reverseIndexJoystickButton.Get()) {
//			ReverseIndex();
//		} else Shoot(m_shooterSpeed);
		Shoot(m_shooterSpeed);
	} else Stop();
	SmartDashboard::PutBoolean("Shooter_ShootBtn", joystickButton_shoot->Get());
	if (joystickButton_shoot->Get()) {
		AimLight(true);
	}  else {
		AimLight(false);
	}

}


void Shooter::Shoot(float shooterSpeed) {
	double IndexVoltageFactor = 1;
	double IndexMotorAmps = m_pdp.GetCurrent(4);
	SmartDashboard::PutNumber("Index Current", IndexMotorAmps);

	// TODO Figure out actual threshold
//	if (abs(IndexMotorAmps) > 7.5) {
//		IndexVoltageFactor -= .005;
//		SmartDashboard::PutBoolean ("Indexer Is Jammed.", true);
//	}
//	else {
//		SmartDashboard::PutBoolean ("Indexer Is Jammed.", false);
//		IndexVoltageFactor += .005;
//	}

	const float acceleratorSpeed = 3500;
	const float afterBurnerSpeed = 3250;

	SmartDashboard::PutNumber("PASpeed", m_particleAccelerator->GetSpeed());
	SmartDashboard::PutNumber("ABSpeed", m_afterBurner->GetSpeed());
	m_particleAccelerator->SetSetpoint(acceleratorSpeed);
	m_afterBurner->SetSetpoint(afterBurnerSpeed);
	if (fabs(m_particleAccelerator->GetSpeed()-acceleratorSpeed)<=50
			&& fabs(m_afterBurner->GetSpeed()-afterBurnerSpeed) <= 50 )
	{
		m_indexMotor->SetSetpoint (60);
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
	m_indexMotor->SetSetpoint (-1000);
}


void Shooter::Stop() {
	m_particleAccelerator->SetSetpoint(0);
	m_afterBurner->SetSetpoint(0);
	m_indexMotor->SetSetpoint(0);
}

void Shooter::Init() {
	m_particleAccelerator->SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);
	m_afterBurner->SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);

	m_particleAccelerator->ConfigEncoderCodesPerRev(20);
	m_afterBurner->ConfigEncoderCodesPerRev(20);

	m_particleAccelerator->SetControlMode(frc::CANSpeedController::kSpeed);
	m_afterBurner->SetControlMode(frc::CANSpeedController::kSpeed);
	m_indexMotor->SetControlMode(frc::CANSpeedController::kSpeed);

	m_particleAccelerator->SetPID(4,.005,0,1.5);
	m_afterBurner->SetPID(4,.005,0,1.5);
	m_particleAccelerator->SetIzone(200);
	m_afterBurner->SetIzone(200);

	m_indexMotor->SetPID(1,0,0, 1);

	m_indexMotor->SetSetpoint(0);
	m_particleAccelerator->SetSetpoint(0);
	m_afterBurner->SetSetpoint(0);
}

void Shooter::AimLight(bool state) {

	if(state)
		m_aimLight->UpdateDutyCycle(0.7);
	else
		m_aimLight->UpdateDutyCycle(0);

}
