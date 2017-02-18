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
enum DriveMode{
	POSITION,
	SPEED,
	VBUS
};
class TankDrive {
	frc::Joystick* m_leftStick;
	frc::Joystick* m_rightStick;
	frc::JoystickButton* joystickButton_reverseDrive;
	frc::JoystickButton* joystickButton_shiftLow;
	frc::JoystickButton* joystickButton_shiftHigh;
	frc::Solenoid* m_gearShift;
	CANTalon* m_leftMotor1;
	CANTalon* m_leftMotor2;
	CANTalon* m_rightMotor1;
	CANTalon* m_rightMotor2;
private:
	int direction;
	bool highGear;
	float rightPosOld;
	float leftPosOld;
	float distance;
	float angle;
	float heading;
	float m_leftDistance = 0, m_rightDistance = 0;
	const float m_countsPerInch = 1/(4*3.141592);
public:
	TankDrive(
			frc::Joystick* m_leftStick
			,frc::Joystick* m_rightStick
			, frc::JoystickButton* joystickButton_reverseDrive
			, frc::JoystickButton* joystickButton_shiftLow
			, frc::JoystickButton* joystickButton_shiftHigh
			,frc::Solenoid* m_gearShift
			,CANTalon* m_leftMotor1
			,CANTalon* m_leftMotor2
			,CANTalon* m_rightMotor1
			,CANTalon* m_rightMotor2
	);
	frc::ADXRS450_Gyro m_gyro;
	virtual ~TankDrive();
	void TeleopInit();
	void TeleopPeriodic();

	void Drive(const float leftVal, const float rightVal);
	void PositionDrive(const float leftPos, const float rightPos);
	void SpeedDrive(const float leftSpeed, const float rightSpeed);

	void LowGear();
	void HighGear();
	void Position();
	float m_xPosition;
	float m_yPosition;

	void SetMode(DriveMode);
	void Zero();
};

#endif /* SRC_TANKDRIVE_H_ */



