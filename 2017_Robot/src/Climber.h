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
	frc::JoystickButton* joystickButton_climb;
	frc::JoystickButton* joystickButton_reverseClimb;
	CANTalon& m_climbMotor;
public:
	Climber(
			frc::JoystickButton* joystickButton_climb
			, frc::JoystickButton* joystickButton_reverseClimb
			, CANTalon& m_climbMotor
	);
	virtual ~Climber();
	void TeleopInit();
	void TeleopPeriodic();
	void Climb(bool, int);
	void Stop();
private:
	bool climbing;
	int direction;
};

#endif /* SRC_CLIMBER_H_ */
