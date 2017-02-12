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
	frc::Joystick& m_joystick;
	int m_button;
	frc::Servo& m_leftServo;
	frc::Servo& m_rightServo;
public:
	void Release(bool release);

	GearManipulator(
		frc::Joystick &
		, int
		, frc::Servo &
		, frc::Servo &
	);
	virtual ~GearManipulator();

	void TeleopPeriodic();

};

#endif /* GEARMANIPULATOR_H_ */
