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
	GEAR_SHIFT
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
};
#endif /* SRC_ROBOTMAP_H_ */



