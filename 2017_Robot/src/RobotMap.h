/*
 * RobotMap.h
 *
 *  Created on: Jan 24, 2017
 *      Author: Joey
 *
 *      2017 Robot Map
 */


#ifndef SRC_ROBOTMAP_H_
#define SRC_ROBOTMAP_H_

enum ManStickMap {
	BUTTON_M_SHOOT = 5,
	BUTTON_M_ADJ_SHOOT = 6,
	BUTTON_M_LIMIT_SHOOT = 1,
	BUTTON_M_REVERSEINDEX = 4, //TODO find real number for this.
	BUTTON_M_CLIMB = 2, //TODO find real number for this
	BUTTON_M_SPINUP = 3,
	BUTTON_M_REVERSECLIMB = 8, //TODO find real number for this
	BUTTOM_M_HOLD = 9,
};

enum LStickMap {
	BUTTON_L_PICKUP = 1,
	BUTTON_L_REVERSEPICKUP = 5, // TODO Figure out actual button value
	BUTTON_L_GEAR_RELEASE = 4,
	BUTTON_L_AIM_LIGHT = 6,
	BUTTON_L_ACCEPT_GEAR = 3,
	BUTTON_L_AUTO_DRIVE_TO_LIFT = 7,
};


enum RStickMap {
	BUTTON_R_REVERSE_DRIVE = 1,
	BUTTON_L_SHIFT_LOW = 3,
	BUTTON_L_SHIFT_HIGH = 2,
	BUTTON_R_SET_SPEED_MODE = 10,
	BUTTON_R_SET_VBUS_MODE = 11,
};

enum DS_USBMap {
	STICK_LEFT = 0,
	STICK_RIGHT = 1,
	STICK_MAN = 2,
};

enum DIOMap {
	LIGHT_AIM = 9,
	LIGHT_RUN_GUN = 8,
	LIGHT_1 = 100, //TODO:MJJ need real number
	LIGHT_2 = 101, //TODO:MJJ need real number
	LIGHT_3 = 102 //TODO:MJJ need real number
};

enum SolenoidMap {
	SOLENOID_GEAR_SHIFT=0,
	SOLENOID_GEAR_KICKER=1
};

enum ClimberMap {
	CLIMBER_AUTOSTOP_CURRENT_THRESHOLD = 55
};

enum CANTalonMap {
	MOTOR_RIGHT_DRIVE1 = 7,
	MOTOR_RIGHT_DRIVE2 = 8,
	MOTOR_LEFT_DRIVE1 = 5,
	MOTOR_LEFT_DRIVE2 = 6,
	MOTOR_SHOOT1 = 3,
	MOTOR_SHOOT2 = 4,
	MOTOR_SHOOT3 = 2,
	MOTOR_SHOOT4 = 1,
	MOTOR_INDEXER = 9,
	MOTOR_CLIMB = 10,
};

enum PWMMap {
	MOTOR_SHOOTER_FEEDER = 0,
	MOTOR_PICKUP_INTAKE = 1,
	MOTOR_PICKUP_HOPPER = 2,
	MOTOR_LEFT_GEAR_SERVO = 3,
	MOTOR_RIGHT_GEAR_SERVO = 4,
};

enum Camera {
	CAMERA_1 = 10001, //TODO:MJJ need real number
	CAMERA_2 = 20002, //TODO:MJJ need real number
	CAMERA_3 = 30003 //TODO:MJJ need real number
};
struct RobotDimensions {
	static constexpr float bumperWidth = 3.35;
	static constexpr float robotWidth = 33;
	static constexpr float robotLength = 29;
	static constexpr float betweenWheels = 29.5;
	static constexpr float centerToWheel = betweenWheels/2;
	static constexpr float width = robotWidth + (2 * bumperWidth);
	static constexpr float length = robotLength + (2 * bumperWidth);
};
#endif /* SRC_ROBOTMAP_H_ */

