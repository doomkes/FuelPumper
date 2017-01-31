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

void Pickup::Intake(bool Highspeed) {

	m_intakeMotor.Set(-1);
	m_hopperMotor.Set(1);
}

void Pickup::stop() {
	m_intakeMotor.Set(0);
	m_hopperMotor.Set(0);
}
