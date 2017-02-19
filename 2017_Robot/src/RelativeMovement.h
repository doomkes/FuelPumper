/*
 * GCode.h
 *
 *  Created on: Feb 17, 2017
 *      Author: Joey
 */

#ifndef SRC_RELATIVEMOVEMENT_H_
#define SRC_RELATIVEMOVEMENT_H_
#include "TankDrive.h"
#include "TrapezoidalMove.h"
#include "ChainCommands.h"

class RelativeMovement {
	TankDrive &m_tank;
	ChainCommands &m_chain;
	TrapezoidalMove m_move;
	Timer m_timer;
	int m_G1State = 0;
public:
	RelativeMovement(TankDrive&, ChainCommands&);
	virtual ~RelativeMovement();

	void Linear(float xDist, float yDist, float speed);
};

#endif /* SRC_RELATIVEMOVEMENT_H_ */
