/*
 * Shooter.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: Joey
 */


#include "Shooter.h"
#include "RobotMap.h"


Shooter::Shooter(
	CANTalon &m_shootWheel1
	, CANTalon &m_shootWheel2
	, CANTalon &m_indexMotor
	, frc::DigitalOutput &m_aimLight
	, int m_shootButton
	, frc::Joystick &m_joystickForShootButton
	, int m_reverseIndexButton
	, frc::Joystick &m_joystickForReverseIndexButton
	, int m_aimingLightButton
	, frc::Joystick &m_joystickForAimingLightButton
	)
	:
	m_shootWheel1(m_shootWheel1)
	, m_shootWheel2(m_shootWheel2)
	, m_indexMotor(m_indexMotor)
	, m_aimLight(m_aimLight)
	, m_shootButton(m_shootButton)
	, m_joystickForShootButton(m_joystickForShootButton)
	, m_reverseIndexButton(m_reverseIndexButton)
	, m_joystickForReverseIndexButton(m_joystickForReverseIndexButton)
	, m_aimingLightButton(m_aimingLightButton)
	, m_joystickForAimingLightButton(m_joystickForAimingLightButton)
{
	// TODO Auto-generated constructor stub

}

Shooter::~Shooter() {
	// TODO Auto-generated destructor stub
}

void Shooter::TeleopInit() {

}

void Shooter::TeleopPeriodic() {
	if (m_joystickForShootButton.GetRawButton(m_shootButton)) {
		if (m_joystickForReverseIndexButton.GetRawButton(m_reverseIndexButton)) {
			ReverseIndex();
		} else Shoot();
	} else Stop();

    if (m_joystickForAimingLightButton.GetRawButton(m_aimingLightButton)) {
    	AimLight(true);
    }  else {
    	AimLight(false);
    }

}

void Shooter::Shoot() {
	//todo:mjj should an instance of Preferences be instantiated at Robot level and passed in for use?
	float speed = frc::Preferences::GetInstance()->GetFloat("ShooterSpeed",0);
	m_shootWheel1.SetSetpoint(speed);
	m_shootWheel2.SetSetpoint(speed);
	if (abs(m_shootWheel1.GetSpeed()-speed)<=50) {
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

void Shooter::AimLight(bool state) {

	if(state)
		m_aimLight.UpdateDutyCycle(0.7);
	else
		m_aimLight.UpdateDutyCycle(0);

}

