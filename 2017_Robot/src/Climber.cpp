/*
 * Climber.cpp
 *
 *  Created on: Feb 13, 2017
 *      Author: colin
 */

#include "Climber.h"

Climber::Climber(
		frc::JoystickButton* joystickButton_climb
		, frc::JoystickButton* joystickButton_reverseClimb
		, CANTalon& m_climbMotor
		)
		:
		joystickButton_climb(joystickButton_climb)
		, joystickButton_reverseClimb(joystickButton_reverseClimb)
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
	if ( !this -> climbing && joystickButton_climb->Get() && !joystickButton_reverseClimb->Get()) {
		this -> direction = 1;
		this -> Climb(true, this -> direction);
	}
	else if(this -> climbing && !joystickButton_climb->Get() && !joystickButton_reverseClimb->Get()) {
		this -> direction = 0;
		this -> Climb(false,this -> direction);
	}
	else if ( !this -> climbing && joystickButton_climb->Get() && joystickButton_reverseClimb->Get()) {
		this -> direction = -1;
		this -> Climb(true, this -> direction);
	}
	else if ( this -> climbing && joystickButton_climb->Get() && joystickButton_reverseClimb->Get()) {
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
