/*
 * Climber.cpp
 *
 *  Created on: Feb 13, 2017
 *      Author: colin
 */

#include "Climber.h"

Climber::Climber(
		frc::Joystick* ManStick,
		frc::JoystickButton* ClimbButton
		, CANTalon* ClimbMotor
		)
		:
		m_manStick(ManStick)
		,m_climbBtn(ClimbButton)
		,m_climbMotor(ClimbMotor)
{
	m_climbMotor->SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m_climbMotor->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);

	this->reachedTop = false;
	this->usedCurrent = 0;
}


Climber::~Climber() {
	// TODO Auto-generated destructor stub
}

void Climber::TeleopInit() {
	m_climbMotor->SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m_climbMotor->SetCurrentLimit(30);
	m_climbMotor->EnableCurrentLimit(true);
	this->reachedTop = false;
	m_climbing = false;
}

void Climber::TeleopPeriodic() {
	static float floor = 0;
	const float release = -.3;
	float joyVal = -m_manStick->GetY();
	float power;

	if(joyVal >= 0.3) {
		floor = .3;
	}
	if(joyVal <= release) {
		floor = -1.0;
	}

	if(joyVal < floor) {
		power = floor;
	} else {
		power = joyVal;
	}

	if(m_manStick->GetRawButton(ManStickMap::BUTTON_M_CLIMB)) {
		m_climbing = true;
	}

	if( m_climbing) {
		Climb(-power);
	} else {
		Climb(0);
	}

	SmartDashboard::PutNumber("ClimbCurrent", m_climbMotor->GetOutputCurrent());
}
void Climber::Climb(float power) {
	m_climbMotor->SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m_climbMotor->SetSetpoint(power);

}

void Climber::Hold() {
	const float holdCurrent = 4;
	m_climbMotor->SetControlMode(CANTalon::ControlMode::kCurrent);
	m_climbMotor->SetSetpoint(holdCurrent);
}
