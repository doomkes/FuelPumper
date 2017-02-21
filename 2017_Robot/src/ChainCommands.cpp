/*
 * ChainCommands.cpp
 *
 *  Created on: Feb 18, 2017
 *      Author: Joey
 */

#include "ChainCommands.h"

ChainCommands::ChainCommands(TankDrive& Tank, TrapezoidalMove Move):m_tank(Tank),m_move(Move) {
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

void ChainCommands::AddCommand(int commandNum, float variable1, float variable2,float variable3) {
	command Command;
	Command.number = commandNum;
	Command.param1 = variable1;
	Command.param2 = variable2;
	Command.param3 = variable3;
	commandArray.push(Command);
}

void ChainCommands::DoCommand(command Command){
	//Variable Declaration
	float speed;
	float distance;
	float turn;
	float arcLength;

	switch(Command.number) {
	case DRIVE_TURN:
		turn=Command.param1;
		m_move.SetAll(turn,turn,turn,turn);
		m_move.CalcParams();
		m_timer.Reset();
		m_timer.Start();
		break;

	case DRIVE_STRAIGHT:
		speed = Command.param2;
		distance = Command.param1;
		m_move.SetAll(speed,speed,speed,distance);
		m_move.CalcParams();
		m_timer.Reset();
		m_timer.Start();
		break;

	case DRIVE_ARC_CLOCKWISE:
		arcLength = Command.param1;
		speed = Command.param2;
		m_move.SetAll(speed,speed,speed,arcLength);
		m_move.CalcParams();
		m_timer.Reset();
		m_timer.Start();
		break;

	case DRIVE_ARC_COUNTERCLOCKWISE:
		arcLength = Command.param1;
		speed = Command.param2;
		m_move.SetAll(speed,speed,speed,arcLength);
		m_move.CalcParams();
		m_timer.Reset();
		m_timer.Start();
		break;

	}
}

void ChainCommands::ContinueCommand(command Command){
	//Variable declaration
	const float distBtwnWhl = 30.5/2; /*Distance from center of bot to Wheels*/
	float turn;
	float t;
	float pos;
	float radius;
	float rightPos;
	float leftPos;

	switch(Command.number) {

	case DRIVE_TURN:
		turn = Command.param1;
		t = m_timer.Get();
		pos=m_move.Position(t);
		m_tank.PositionDrive(-pos,pos);
		break;

	case DRIVE_STRAIGHT:
		t = m_timer.Get();
		pos=m_move.Position(t);
		m_tank.PositionDrive(pos,pos);
		break;

	case DRIVE_ARC_CLOCKWISE:
		t = m_timer.Get();
		pos=m_move.Position(t);
		radius = Command.param3;
		rightPos = ((radius-distBtwnWhl)/radius)*pos;
		leftPos = ((radius+distBtwnWhl)/radius)*pos;
		m_tank.PositionDrive(leftPos,rightPos);
		break;

	case DRIVE_ARC_COUNTERCLOCKWISE:
		t = m_timer.Get();
		pos=m_move.Position(t);
		radius = Command.param3;
		rightPos = ((radius+distBtwnWhl)/radius)*pos;
		leftPos = ((radius-distBtwnWhl)/radius)*pos;
		m_tank.PositionDrive(leftPos,rightPos);
		break;

	}
}
bool ChainCommands::CheckStatus(command Command){
	//Variable declaration
	float distance;
	float speed;
	float arcLength;

	switch(Command.number) {
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
		speed = Command.param2;
		distance = Command.param1;
		if (m_timer.Get()>=(arcLength/speed+1)) {
			m_timer.Stop();
			return(true);
		}
		else {
			return(false);
		}
		break;
	case DRIVE_ARC_CLOCKWISE:
		speed = Command.param2;
		arcLength = Command.param1;
		if (m_timer.Get()>=(distance/speed+1)) {
			m_timer.Stop();
			return(true);
		}
		else {
			return(false);
		}
		break;
	case DRIVE_ARC_COUNTERCLOCKWISE:
		speed = Command.param2;
		arcLength = Command.param1;
		if (m_timer.Get()>=(distance/speed+1)) {
			m_timer.Stop();
			return(true);
		}
		else {
			return(false);
		}
		break;

	}
}
ChainCommands::~ChainCommands() {
	// TODO Auto-generated destructor stub
}

