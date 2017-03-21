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
	this->reachedTop = false;
}

void Climber::TeleopPeriodic() {
	if(!this->reachedTop && m_climbBtn->Get()) {
		float power = m_manStick->GetY();
		this->Climb(power);
	}
	else if (this->reachedTop || m_manStick->GetRawButton(BUTTOM_M_HOLD)) {
		this->Hold();
	} else {
		Climb(0);
	}

	this->usedCurrent = m_climbMotor->GetOutputCurrent();

	if (this->usedCurrent > CLIMBER_AUTOSTOP_CURRENT_THRESHOLD) {
		this->reachedTop = true;
	}

	SmartDashboard::PutNumber("ClimbCurrent", this->usedCurrent);
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
