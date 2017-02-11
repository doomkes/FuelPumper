
/*
 * TankDrive.h
 *
 *  Created on: Jan 14, 2017
 *      Author: Joey Day
 */

#ifndef SRC_TANKDRIVE_H_
#define SRC_TANKDRIVE_H_

#include <WPIlib.h>
#include <CANTalon.h>
#include "Robotmap.h"

class TankDrive {
	frc::Joystick& m_leftStick;
	frc::Joystick& m_rightStick;
	frc::Solenoid m_gearShiftR;
	frc::Solenoid m_gearShiftL;
	CANTalon& m_leftMotor1;
	CANTalon& m_leftMotor2;
	CANTalon& m_rightMotor1;
	CANTalon& m_rightMotor2;
public:
	TankDrive(
		frc::Joystick &
		,frc::Joystick &
		,frc::Solenoid &
		,CANTalon &
		,CANTalon &
		,CANTalon &
		,CANTalon &
	);
	virtual ~TankDrive();
	void TeleopPeriodic();
	void Drive(const float leftVal, const float rightVal);
	void LowGear();
	void HighGear();
};

#endif /* SRC_TANKDRIVE_H_ */


