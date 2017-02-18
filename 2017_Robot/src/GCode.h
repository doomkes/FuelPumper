/*
 * GCode.h
 *
 *  Created on: Feb 17, 2017
 *      Author: Joey
 */

#ifndef SRC_GCODE_H_
#define SRC_GCODE_H_
#include "TankDrive.h"
#include "trapezoidalMove.h"

class GCode {
	TankDrive *m_tank;
	int m_G1State;
	TrapezoidalMove m_move;
	Timer m_timer;
public:
	GCode();
	virtual ~GCode();

	void G1(float xDist, float yDist, float speed);
	void SetTank(TankDrive*tank) {m_tank =tank;}
};

#endif /* SRC_GCODE_H_ */
