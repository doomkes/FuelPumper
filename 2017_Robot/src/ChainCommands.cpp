/*
 * ChainCommands.cpp
 *
 *  Created on: Feb 18, 2017
 *      Author: Joey
 */

#include "ChainCommands.h"

ChainCommands::ChainCommands(TankDrive& Tank, ChainCommands& Chain, TrapezoidalMove Move, Timer Time):m_tank(Tank),m_chain(Chain),m_move(Move),m_timer(Time) {
	// TODO Auto-generated constructor stub
	doingCommand=false;

}
void ChainCommands::AutoPeriodic() {
	if (doingCommand) {
		if (CheckStatus(commandArray.front())) {
			commandArray.pop();
			doingCommand = false;
		}
	}
	else {
		if (!commandArray.empty()) {
			DoCommand(commandArray.front());
		}
	}
	//Currently doing commands? yes=Check status no=start new

	//Commands to do? yes= doingcommand=yes, start command No=skip

	//check various commands if done yes= doingcommand=false, remove command no=continue
}
void ChainCommands::DoCommand(map<int, float, float>& Command){
	switch(Command[0]) {
	case DRIVE_TURN:
		float turn=Command[1];
		m_move.SetAll(turn,turn,turn,turn);

	}
}
ChainCommands::~ChainCommands() {
	// TODO Auto-generated destructor stub
}

