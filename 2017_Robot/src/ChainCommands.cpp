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
		else {
			ContinueCommand(commandArray.front());
		}
	}
	else {
		if (!commandArray.empty()) {
			doingCommand = true;
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
		m_move.CalcParams();
		m_timer.Reset();
		m_timer.Start();
		break;
	case DRIVE_STRAIGHT:
		float speed = Command[2];
		float distance = Command[1];
		m_move.SetAll(speed,speed,speed,distance);
		m_move.CalcParams();
		m_timer.Reset();
		m_timer.Start();
		break;
	}
}
void ChainCommands::AddCommand(int command, float variable1, float variable2) {
	commandArray.push(map<int command,float variable1,float variable2>);
}
void ChainCommands::ContinueCommand(map<int, float, float>& Command){
	switch(Command[0]) {
	case DRIVE_TURN:
		float turn=Command[1];
		float t = m_timer.Get();
		float pos=m_move.Position(t);
		m_tank.PositionDrive(-pos,pos);
		break;
	case DRIVE_STRAIGHT:
			float distance = Command[1];
			float t = m_timer.Get();
			float pos=m_move.Position(t);
			m_tank.PositionDrive(pos,pos);
			break;
	}
}
bool ChainCommands::CheckStatus(map<int, float, float>& Command){
	switch(Command[0]) {
	case DRIVE_TURN:
		//float turn = Command[1];
		//float angle = m_tank.m_gyro.GetAngle()*(3.14159/180);
		//float targetAngle = turn/95.819;
		if (m_timer.Get()>=2)/*(fabs(fabs(targetAngle)-fabs(angle))<=1)*/ {
			m_timer.Stop();
			return(true);
		}
		else {
			return(false);
		}
		break;
	case DRIVE_STRAIGHT:
		float speed = Command[2];
		float distance = Command[1];
		if (m_timer.Get()>=(distance/speed+1)) {
			m_timer.Stop();
			return(true);
		}
		else {
			return(false);
		}

	}
}
ChainCommands::~ChainCommands() {
	// TODO Auto-generated destructor stub
}

