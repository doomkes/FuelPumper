
/*
 * Pickup.cpp
 *
 *  Created on: Jan 24, 2017
 *      Author: Joey
 */

#include "Pickup.h"

Pickup::Pickup(
	frc::Joystick &m_leftStick
	,int pickupButton
	,frc::Talon &m_intakeMotor
	,frc::Talon &m_hopperMotor
	)
	:
	m_leftStick(m_leftStick)
	, pickupButton(pickupButton)
	, m_intakeMotor(m_intakeMotor)
	, m_hopperMotor(m_hopperMotor)
{
	// TODO Auto-generated constructor stub
}

void Pickup::TeleopPeriodic() {
	if(m_leftStick.GetRawButton(PICKUP)) {

			if(m_leftStick.GetRawButton(REVERSEPICKUP)){
				Reverse();
			}
			else {
				Intake();
			}
		}
		else {
			stop();
		}

}

Pickup::~Pickup() {
	// TODO Auto-generated destructor stub
}

// TODO Implement Pickup class methods


void Pickup::Intake() {

	double IntakeVoltageFactor = 1;
	double HopperVoltageFactor = 1;
	double bottomIntakeMotorAmps = m_pdp.GetCurrent(14);
	double topIntakeMotorAmps = m_pdp.GetCurrent(15);
	SmartDashboard::PutNumber("Intake Current", bottomIntakeMotorAmps);
	SmartDashboard::PutNumber("Hopper Current", topIntakeMotorAmps);

	//VoltageFactor = Bottom Intake Voltage
	if(bottomIntakeMotorAmps > 7.5){
		IntakeVoltageFactor -= .005;
	}
	else{
		IntakeVoltageFactor += .005;
	};
	//VoltageFactor2 = Top Intake Voltage
	if(topIntakeMotorAmps > 5.5){
		HopperVoltageFactor -= .005;
	}
	else{
		HopperVoltageFactor += .005;
	};


	m_intakeMotor.Set(-1*IntakeVoltageFactor);
	m_hopperMotor.Set(1*HopperVoltageFactor);
}

void Pickup::stop() {
	m_intakeMotor.Set(0);
	m_hopperMotor.Set(0);
}

void Pickup::Reverse(){
	m_intakeMotor.Set(1);
	m_hopperMotor.Set(-1);
}
