/*
 * Climber.cpp
 *
 *  Created on: Feb 13, 2017
 *      Author: colin
 */

#include "Climber.h"

Climber::Climber(
		frc::JoystickButton* ClimbButton
		, frc::JoystickButton* ReverseButton
		, frc::JoystickButton* HoldButton
		, CANTalon* m_climbMotor
		)
		:
		m_climbBtn(ClimbButton)
		, m_reverseBtn(ReverseButton)
		, m_holdBtn(HoldButton)
		, m_climbMotor(m_climbMotor)
{
	m_climbMotor->SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m_climbMotor->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_climbing = false;
	m_direction = 1;
}


Climber::~Climber() {
	// TODO Auto-generated destructor stub
}

void Climber::TeleopInit() {
	m_climbMotor->SetControlMode(CANTalon::ControlMode::kPercentVbus);
}

void Climber::TeleopPeriodic() {
	if(!m_climbing) {
		if(m_climbBtn->Get()) {
			m_direction = m_reverseBtn->Get() ? -1 : 1;
			this->Climb(true, this -> m_direction);
		}
		else if (m_holdBtn->Get()) {
			Hold();
		}
	}
	else {
		if( !m_climbBtn->Get() && !m_reverseBtn->Get() && !m_holdBtn->Get()) {
			this -> m_direction = 0;
			this -> Climb(false,this -> m_direction);
		}
		else if ( m_climbBtn->Get() && m_reverseBtn->Get() && !m_holdBtn->Get()) {
			this -> m_direction = -1;
			this -> Climb(true, this -> m_direction);
		}
		else if(m_holdBtn->Get()) {
			Hold();
		}
	}
}

void Climber::Climb(bool shouldClimb, int direction) {
	float speed = Preferences::GetInstance()->GetFloat("ClimberSpeed",0);
	m_climbMotor->SetControlMode(CANTalon::ControlMode::kPercentVbus);
	if(shouldClimb){
		this -> m_climbing = true;
		m_climbMotor->SetSetpoint(0.250 * direction);
	}
	else {
		this -> m_climbing = false;
		m_climbMotor->SetSetpoint(0);
	}
}

void Climber::Hold() {
	const float holdCurrent = 3;

	m_climbMotor->SetControlMode(CANTalon::ControlMode::kCurrent);
	m_climbMotor->SetSetpoint(holdCurrent);
}
