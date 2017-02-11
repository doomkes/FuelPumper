/*
 * Pickup.h
 *
 *  Created on: Jan 24, 2017
 *      Author: Joey
 */

#ifndef SRC_PICKUP_H_
#define SRC_PICKUP_H_
#include <WPILib.h>
class Pickup {
private:
	// Intake Motor picks up balls
	frc::Talon m_intakeMotor;
	// Hopper Motor pushs into Hopper
	frc::Talon m_hopperMotor;
	//PDP Variable
 	PowerDistributionPanel m_pdp;

public:
	Pickup();
	virtual ~Pickup();

	void Intake();
	void stop();
	void Reverse();
};

#endif /* SRC_PICKUP_H_ */
