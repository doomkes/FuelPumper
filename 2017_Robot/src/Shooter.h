/*
 * Shooter.h
 *
 *  Created on: Jan 16, 2017
 *      Author: Joey
 */

#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_

#include <WPILib.h>
#include <CANTalon.h>
#include "RobotMap.h"


class Shooter {
	CANTalon& m_shootWheel1;
	CANTalon& m_shootWheel2;
	CANTalon& m_indexMotor;
	frc::DigitalOutput& m_aimLight;

	int m_shootButton;
	frc::Joystick& m_joystickForShootButton;
	int m_reverseIndexButton;
	frc::Joystick& m_joystickForReverseIndexButton;
	int m_aimingLightButton;
	frc::Joystick& m_joystickForAimingLightButton;
	PowerDistributionPanel m_pdp;
public:
	Shooter(
		CANTalon &
		, CANTalon &
		, CANTalon &
		, DigitalOutput &
		, int
		, Joystick&
		, int
		, Joystick&
		, int
		, Joystick&
	);
	virtual ~Shooter();
	void TeleopInit();
	void TeleopPeriodic();

	//Want to allow it to activate shooter motors let
	//then get up to speed then activate index motor
	void Shoot();
    void Stop();
    void ReverseIndex();
    void Init();
    void AimLight(bool state);

};

#endif /* SRC_SHOOTER_H_ */
