/*
 * Climber.h
 *
 *  Created on: Feb 13, 2017
 *      Author: colin
 */
#include <WPILib.h>
#include <CANTalon.h>
#include "RobotMap.h"
#ifndef SRC_CLIMBER_H_
#define SRC_CLIMBER_H_

class Climber {
	frc::Joystick* m_manStick;
	frc::JoystickButton* m_climbBtn;
	CANTalon* m_climbMotor;
public:
	Climber(
			frc::Joystick* ManStick
			, frc::JoystickButton* ClimbButton
			, CANTalon* ClimbMotor
	);
	virtual ~Climber();
	void TeleopInit();
	void TeleopPeriodic();
	void Climb(float);
	void Hold();
private:
	bool m_climbing= false;
	bool reachedTop;
	double usedCurrent;
};

#endif /* SRC_CLIMBER_H_ */
