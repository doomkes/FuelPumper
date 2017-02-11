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
};

enum LStickMap {
	PICKUP = 1,
};

enum RStickMap {
	REVERSE_DRIVE = 1,
	SET_SPEED_MODE = 10,
	SET_VBUS_MODE = 11,
};

enum DIOMap {
	AIM_LIGHT = 9,
	RUN_GUN_LIGHT = 8
};

enum SolenoidMap {
	GEAR_SHIFTR,
	GEAR_SHIFTL
};

enum CANTalonMap {
	RIGHT_DRIVE1 = 0,
	RIGHT_DRIVE2 = 1,
	LEFT_DRIVE1 = 2,
	LEFT_DRIVE2 = 3,
	SHOOT1 = 4,
	SHOOT2 = 5,
};
#endif /* SRC_ROBOTMAP_H_ */



