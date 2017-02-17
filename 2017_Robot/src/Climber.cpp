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
	direction = 1;
}


Climber::~Climber() {
	// TODO Auto-generated destructor stub
}

void Climber::TeleopInit() {

}

void Climber::TeleopPeriodic() {
	if ( !this -> climbing && m_manStick.GetRawButton(BUTTON_M_CLIMB) && !m_manStick.GetRawButton(BUTTON_M_REVERSECLIMB)) {
		this -> direction = 1;
		this -> Climb(true, this -> direction);
	}
	else if(this -> climbing && !m_manStick.GetRawButton(BUTTON_M_CLIMB) && !m_manStick.GetRawButton(BUTTON_M_REVERSECLIMB)) {
		this -> direction = 0;
		this -> Climb(false,this -> direction);
	}
	else if ( !this -> climbing && m_manStick.GetRawButton(BUTTON_M_CLIMB) && m_manStick.GetRawButton(BUTTON_M_REVERSECLIMB)) {
		this -> direction = -1;
		this -> Climb(true, this -> direction);
	}
	else if ( this -> climbing && m_manStick.GetRawButton(BUTTON_M_CLIMB) && m_manStick.GetRawButton(BUTTON_M_REVERSECLIMB)) {
			this -> direction = -1;
			this -> Climb(true, this -> direction);
	}
}

void Climber::Climb(bool shouldClimb, int direction) {
	float speed = Preferences::GetInstance()->GetFloat("ClimberSpeed",0);
	if(shouldClimb){
		this -> climbing = true;
		m_climbMotor.SetSetpoint(speed*direction);
	}
	else {
		this -> climbing = false;
		m_climbMotor.SetSetpoint(0);
	}

}
