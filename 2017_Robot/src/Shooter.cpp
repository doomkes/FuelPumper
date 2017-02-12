/*
 * Shooter.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: Joey
 */


#include "Shooter.h"
#include "RobotMap.h"


Shooter::Shooter(
	frc::Joystick &m_joystick
	, CANTalon &m_shootWheel1
	, CANTalon &m_shootWheel2
	, CANTalon &m_indexMotor
	, frc::Joystick &m_joystick2
	, DigitalOutput &m_aimLight
	)
	:
	m_joystick(m_joystick)
	, m_shootWheel1(m_shootWheel1)
	, m_shootWheel2(m_shootWheel2)
	, m_indexMotor(m_indexMotor)
	, m_joystick2(m_joystick2)
	, m_aimLight(m_aimLight)
{
	// TODO Auto-generated constructor stub

}

Shooter::~Shooter() {
	// TODO Auto-generated destructor stub
}

void Shooter::TeleopPeriodic() {
	if (m_joystick.GetRawButton(SHOOT)) {
		if (m_joystick.GetRawButton(REVERSEINDEX)) {
			ReverseIndex();
		} else Shoot();
	} else Stop();

    if (m_joystick2.GetRawButton(AIM_LIGHT)) {
    	AimLight(true);
    }  else {
    	AimLight(false);
    }

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

void Shooter::AimLight(bool state) {

	if(state)
		m_aimLight.UpdateDutyCycle(0.7);
	else
		m_aimLight.UpdateDutyCycle(0);

}
