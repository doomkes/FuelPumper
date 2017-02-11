/*
 * GearManipulator.h
 *
 *  Created on: Feb 6, 2017
 *      Author: Joey
 */

#ifndef GEARMANIPULATOR_H_
#define GEARMANIPULATOR_H_
#include <WPILib.h>

class GearManipulator {
private:
	frc::Servo m_leftServo, m_rightServo;
public:
	void Release(bool release);

	GearManipulator();
	virtual ~GearManipulator();
};

#endif /* GEARMANIPULATOR_H_ */
