/*
 * OI.h
 *
 *  Created on: Feb 18, 2017
 *      Author: administrator
 */

// Operator Interface

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
	frc::JoystickButton* joystickButton_shoot = new frc::JoystickButton(joystick_manipulator, BUTTON_M_SHOOT);
	frc::JoystickButton* joystickButton_reverseIndex = new frc::JoystickButton(joystick_manipulator, BUTTON_M_REVERSEINDEX);
	frc::JoystickButton* joystickButton_aimLight = new frc::JoystickButton(joystick_driverLeft, BUTTON_L_AIM_LIGHT);

	frc::JoystickButton* joystickButton_climb = new frc::JoystickButton(joystick_manipulator, BUTTON_M_CLIMB);
	frc::JoystickButton* joystickButton_reverseClimb = new frc::JoystickButton(joystick_manipulator, BUTTON_M_REVERSECLIMB);

	frc::JoystickButton* joystickButton_gearRelease = new frc::JoystickButton(joystick_driverLeft, BUTTON_L_GEAR_RELEASE);

	frc::JoystickButton* joystickButton_pickup = new frc::JoystickButton(joystick_driverLeft, BUTTON_L_PICKUP);
	frc::JoystickButton* joystickButton_reversePickup = new frc::JoystickButton(joystick_driverLeft, BUTTON_L_REVERSEPICKUP);

	frc::JoystickButton* joystickButton_reverseDrive = new frc::JoystickButton(joystick_driverRight, BUTTON_R_REVERSE_DRIVE);
	frc::JoystickButton* joystickButton_shiftLow = new frc::JoystickButton(joystick_driverLeft, BUTTON_L_SHIFT_LOW);
	frc::JoystickButton* joystickButton_shiftHigh = new frc::JoystickButton(joystick_driverLeft, BUTTON_L_SHIFT_HIGH);


	OI();
	virtual ~OI();
};

#endif /* SRC_OI_H_ */
