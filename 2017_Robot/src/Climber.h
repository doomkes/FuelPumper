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
	frc::JoystickButton* m_climbBtn;
	frc::JoystickButton* m_reverseBtn;
	frc::JoystickButton* m_holdBtn;
	CANTalon* m_climbMotor;
public:
	Climber(
			frc::JoystickButton* ClimbButton
			, frc::JoystickButton* ReverseButton
			, frc::JoystickButton* HoldButton
			, CANTalon* ClimbMotor
	);
	virtual ~Climber();
	void TeleopInit();
	void TeleopPeriodic();
	void Climb(bool, int);
	void Hold();
	void Stop();
private:
	bool m_climbing;
	int m_direction;
};

#endif /* SRC_CLIMBER_H_ */
