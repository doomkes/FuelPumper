/*
 * TankDrive.h
 *
 *  Created on: Jan 14, 2017
 *      Author: Joey Day
 */

#ifndef SRC_TANKDRIVE_H_
#define SRC_TANKDRIVE_H_

#include <WPILib.h>
#include <CANTalon.h>
#include "RobotMap.h"

class TankDrive {
	frc::Joystick* m_leftStick;
	frc::Joystick* m_rightStick;
	frc::Solenoid& m_gearShift;
	CANTalon& m_leftMotor1;
	CANTalon& m_leftMotor2;
	CANTalon& m_rightMotor1;
	CANTalon& m_rightMotor2;
private:
	int direction;
	bool highGear;
	float rightPosOld;
	float leftPosOld;
	float distance;
	float angle;
	float heading;
public:
	TankDrive(
		frc::Joystick* m_leftStick
		,frc::Joystick* m_rightStick
		,frc::Solenoid& m_gearShift
		,CANTalon& m_leftMotor1
		,CANTalon& m_leftMotor2
		,CANTalon& m_rightMotor1
		,CANTalon& m_rightMotor2
	);
	virtual ~TankDrive();
	void TeleopInit();
	void TeleopPeriodic();
	void Drive(const float leftVal, const float rightVal);
	void LowGear();
	void HighGear();
	void Position();
	float m_xPosition;
	float m_yPosition;
};

#endif /* SRC_TANKDRIVE_H_ */



