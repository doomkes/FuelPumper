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

class TankDrive {
	CANTalon m_leftMotor1, m_leftMotor2;
	CANTalon m_rightMotor1, m_rightMotor2;
	frc::Solenoid m_gearShift;
public:
	TankDrive();
	virtual ~TankDrive();

	void Drive(const float leftVal, const float rightVal);

	void LowGear();
	void HighGear();
};

#endif /* SRC_TANKDRIVE_H_ */
