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
	SHOOT = 5,
	LIMIT_SHOOT = 1,
	SPINUP = 2,
	REVERSEINDEX = 6, //TODO find real number for this.
	BUTTON_CLIMB = 7 //TODO find real number for this
};

enum LStickMap {
	PICKUP = 1,
	REVERSEPICKUP = 4, // TODO Figure out actual button value
	GEAR_RELEASE,
	SHIFT_LOW = 3,
	SHIFT_HIGH = 2,
};



enum RStickMap {
	REVERSE_DRIVE = 1,
	SET_SPEED_MODE = 10,
	SET_VBUS_MODE = 11,
	RELEASE_GEAR = 3
};

enum DS_USBMap {
	LEFTSTICK = 0,
	RIGHTSTICK = 1,
	MANSTICK = 2,
};

enum DIOMap {
	AIM_LIGHT = 9,
	RUN_GUN_LIGHT = 8
};

enum SolenoidMap {
	GEAR_SHIFT=0
};

enum CANTalonMap {
	RIGHT_DRIVE1 = 5,
	RIGHT_DRIVE2 = 6,
	LEFT_DRIVE1 = 7,
	LEFT_DRIVE2 = 8,
	SHOOT1 = 4,
	SHOOT2 = 3,
	SHOOT3 = 2,
	SHOOT4 = 1,
	INDEXER = 9,
	CLIMB = 10,
};

enum PWMMap {
	PICKUP_INTAKE = 0,
	PICKUP_HOPPER = 1,
	LEFT_GEAR_SERVO = 2,
	RIGHT_GEAR_SERVO = 3,
};

#endif /* SRC_ROBOTMAP_H_ */

