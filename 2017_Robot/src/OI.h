/*
 * OI.h
 *
 *  Created on: Feb 18, 2017
 *      Author: administrator
 */

#ifndef SRC_OI_H_
#define SRC_OI_H_

#include <WPILib.h>
#include "RobotMap.h"

class OI {

public:
	// Joysticks
	frc::Joystick* joystick_driverLeft = new frc::Joystick(STICK_LEFT);
	frc::Joystick* joystick_driverRight = new frc::Joystick(STICK_RIGHT);
	frc::Joystick* joystick_manipulator = new frc::Joystick(STICK_MAN);

	// Buttons
	frc::JoystickButton* joystickButton_shoot = new frc::JoystickButton(joystick_manipulator, BUTTON_M_SHOOT);// = nullptr;
	frc::JoystickButton* joystickButton_reverseIndex = new frc::JoystickButton(joystick_manipulator, BUTTON_M_REVERSEINDEX);// = nullptr;
	frc::JoystickButton* joystickButton_aimLight = new frc::JoystickButton(joystick_driverLeft, BUTTON_L_AIM_LIGHT);// = nullptr;

	OI();
	virtual ~OI();
};

#endif /* SRC_OI_H_ */
