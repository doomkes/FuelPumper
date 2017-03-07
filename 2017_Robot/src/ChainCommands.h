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
#include <array>
#include "GearManipulator.h"
#include "Pickup.h"
#include "Shooter.h"
#include "TankDrive.h"
#include "TrapezoidalMove.h"
using namespace std;
enum Commands {
	DRIVE_TURN,
	DRIVE_STRAIGHT,
	DRIVE_ARC_CLOCKWISE,
	DRIVE_ARC_COUNTERCLOCKWISE,

	DRIVE_WAIT,

	SHOOT_START,
	SHOOT_STOP,

	DRIVE_SHOOT,
};
enum commandGroup {
	MOTION,
	MANIPULATION,
};
struct command {
	int number;
	commandGroup group;
	float param1, param2, param3;
};
class ChainCommands {
protected:
	TankDrive &m_tank;
	TrapezoidalMove m_move;
	Timer m_timer;
	Shooter &m_shooter;
	bool m_doingCommand = false;

	array<command, 2> m_runningCommands;
	queue<command> m_commandQueue;

	float m_startAngle = 0;
	const float wheelCircumInches = 91.891;
private:
	bool CheckStatus(command);
	void DoCommand(command);
	void ContinueCommand(command);
public:
	ChainCommands(TankDrive&, Shooter&);
	virtual ~ChainCommands();
	void AutoPeriodic();
	void AddCommand(int,float,float,float);
	void Init();
};

#endif /* SRC_CHAINCOMMANDS_H_ */
