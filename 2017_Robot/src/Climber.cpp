/*
 * Climber.cpp
 *
 *  Created on: Feb 13, 2017
 *      Author: colin
 */

#include "Climber.h"

Climber::Climber(
		frc::Joystick &m_manStick
		, CANTalon &m_climbMotor
		)
		:
		m_manStick(m_manStick)
		, m_climbMotor(m_climbMotor)
{
	m_climbMotor.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	climbing = false;
}


Climber::~Climber() {
	// TODO Auto-generated destructor stub
}

void Climber::TeleopPeriodic() {
	if ( !this -> climbing && m_manStick.GetRawButton(BUTTON_CLIMB)) {
		this -> Climb(true);
	}
	else if(this -> climbing && !m_manStick.GetRawButton(BUTTON_CLIMB)) {
		this -> Climb(false);
	}
}

void Climber::Climb(bool shouldClimb) {
	float speed = Preferences::GetInstance()->GetFloat("ClimberSpeed",0);
	if(shouldClimb){
		this -> climbing = true;
		m_climbMotor.SetSetpoint(speed);
	}
	else {
		this -> climbing = false;
		m_climbMotor.SetSetpoint(0);
	}

}
