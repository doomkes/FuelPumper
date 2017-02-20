/*
 * ChainCommands.h
 *
 *  Created on: Feb 18, 2017
 *      Author: Joey
 */

#ifndef SRC_CHAINCOMMANDS_H_
#define SRC_CHAINCOMMANDS_H_
#include <map>
#include <queue>
#include "GearManipulator.h"
#include "Pickup.h"
#include "Shooter.h"
#include "TankDrive.h"
#include "TrapezoidalMove.h"

using namespace std;
enum Commands {
	DRIVE_TURN = 0,
	DRIVE_STRAIGHT=1,
	DRIVE_ARC_CLOCKWISE=2,
	DRIVE_ARC_COUNTERCLOCKWISE=3,
};

class ChainCommands {
	TankDrive &m_tank;
	ChainCommands &m_chain;
	TrapezoidalMove m_move;
	Timer m_timer;
	bool doingCommand;
	bool CheckStatus(map<int, float, float>&);
	void DoCommand(map<int, float, float>&);
	queue<map<int, float, float>> commandArray;
	void ContinueCommand(map<int,float,float>&);
public:
	ChainCommands(TankDrive&, ChainCommands&, TrapezoidalMove, Timer);
	virtual ~ChainCommands();
	void AutoPeriodic();
	void AddCommand(int,float,float);
};

#endif /* SRC_CHAINCOMMANDS_H_ */
