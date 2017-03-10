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
	DRIVE_WAIT = 4,
	SHOOT_START = 5,
	DRIVE_SHOOT= 6,
};
struct command {
	int number;
	float param1, param2, param3;
};
class ChainCommands {
	TankDrive &m_tank;
	TrapezoidalMove m_move;
	Timer m_timer;
	Shooter &m_shooter;
	bool doingCommand = false;
	bool CheckStatus(command);
	void DoCommand(command);
	queue<command> commandArray;
	void ContinueCommand(command);
	float m_startAngle = 0;
	const float wheelCircumInches = 91.891;
public:
	ChainCommands(TankDrive&, Shooter&);
	virtual ~ChainCommands();
	void AutoPeriodic();
	void AddCommand(int,float,float,float);
	void Init();
};

#endif /* SRC_CHAINCOMMANDS_H_ */
