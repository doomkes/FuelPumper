/*
 * CI.h
 *
 *  Created on: Feb 18, 2017
 *      Author: administrator
 */

// Component Interface

#ifndef SRC_CI_H_
#define SRC_CI_H_

#include <WPILib.h>
#include "RobotMap.h"
#include <CANTalon.h>

class CI {
public:
	// Motors
	CANTalon* canTalon_leftMotor1 = new CANTalon(MOTOR_LEFT_DRIVE1);
	CANTalon* canTalon_leftMotor2 = new CANTalon(MOTOR_LEFT_DRIVE2);
	CANTalon* canTalon_rightMotor1 = new CANTalon(MOTOR_RIGHT_DRIVE1);
	CANTalon* canTalon_rightMotor2 = new CANTalon(MOTOR_RIGHT_DRIVE2);
	CANTalon* canTalon_shootWheel1 = new CANTalon(MOTOR_SHOOT1);
	CANTalon* canTalon_shootWheel2 = new CANTalon(MOTOR_SHOOT2);
	CANTalon* canTalon_indexMotor = new CANTalon(MOTOR_INDEXER);
	frc::Talon* canTalon_shooterFeeder = new frc::Talon(MOTOR_SHOOTER_FEEDER);
	CANTalon* canTalonclimbMotor = new CANTalon(MOTOR_CLIMB);
	frc::Talon* canTalon_intake = new frc::Talon(MOTOR_PICKUP_INTAKE);
	frc::Talon* canTalon_hopper = new frc::Talon(MOTOR_PICKUP_HOPPER);

	// Solenoids
	frc::Solenoid* solenoid_gearShift = new frc::Solenoid(SOLENOID_GEAR_SHIFT);

	// Servos
	frc::Servo* servo_leftGear = new frc::Servo(MOTOR_LEFT_GEAR_SERVO);;
	frc::Servo* servo_rightGear = new frc::Servo(MOTOR_RIGHT_GEAR_SERVO);;

	// DigitalOutput
	DigitalOutput* digitalOutput_aimLight = new DigitalOutput(LIGHT_AIM);

	CI();
	virtual ~CI();
};

#endif /* SRC_CI_H_ */
