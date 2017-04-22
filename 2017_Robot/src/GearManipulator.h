/*
 * GearManipulator.h
 *
 *  Created on: Feb 6, 2017
 *      Author: Joey
 */

#ifndef GEARMANIPULATOR_H_
#define GEARMANIPULATOR_H_
#include <WPILib.h>
#include "OI.h"
class GearManipulator {
	frc::JoystickButton* joystickButton_gearRelease;
	OI *m_oi;
	frc::Servo* m_leftServo;
	frc::Servo* m_rightServo;
	frc::Solenoid m_kicker;
public:
	void Release(bool release, bool Accept = false);
	GearManipulator(
		frc::JoystickButton* joystickButton_gearRelease
		,OI *oi
		,frc::Servo* m_leftServo
		,frc::Servo* m_rightServo
	);
	virtual ~GearManipulator();

	void TeleopInit();
	void TeleopPeriodic();

};

#endif /* GEARMANIPULATOR_H_ */
