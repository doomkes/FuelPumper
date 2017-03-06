/*
 * GCode.h
 *
 *  Created on: Feb 17, 2017
 *      Author: Joey
 */

#ifndef SRC_RELATIVEMOVEMENT_H_
#define SRC_RELATIVEMOVEMENT_H_
#include "ChainCommands.h"

class RelativeMovement {
	ChainCommands &m_chain;
	int m_G1State = 0;
public:
	RelativeMovement(ChainCommands&);
	virtual ~RelativeMovement();
	void Arc(bool,float,float,float,float,float);
	void Linear(float, float, float);
	void Wait(float);
};

#endif /* SRC_RELATIVEMOVEMENT_H_ */
