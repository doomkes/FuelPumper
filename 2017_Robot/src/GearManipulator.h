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
	frc::JoystickButton* joystickButton_gearRelease;
	frc::Servo* m_leftServo;
	frc::Servo* m_rightServo;
public:
	void Release(bool release);

	GearManipulator(
		frc::JoystickButton* joystickButton_gearRelease
		,frc::Servo* m_leftServo
		,frc::Servo* m_rightServo
	);
	virtual ~GearManipulator();

	void TeleopInit();
	void TeleopPeriodic();

};

#endif /* GEARMANIPULATOR_H_ */
