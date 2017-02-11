/*
 * Pickup.cpp
 *
 *  Created on: Jan 24, 2017
 *      Author: Joey
 */

#include <Pickup.h>


Pickup::Pickup()
	: m_intakeMotor(0), m_hopperMotor(1)
{
	// TODO Auto-generated constructor stub
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


