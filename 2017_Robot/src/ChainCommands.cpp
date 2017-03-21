/*
 * ChainCommands.cpp
 *
 *  Created on: Feb 18, 2017
 *      Author: Joey
 */

#include "ChainCommands.h"

ChainCommands::ChainCommands(TankDrive& Tank, Shooter& shooter):m_tank(Tank), m_shooter(shooter) {
	// TODO Auto-generated constructor stub


}
void ChainCommands::Init() {
	m_doingCommand=false;
	m_tank.SetMode(DriveMode::POSITION);

}
void ChainCommands::AutoPeriodic() {
	if (m_doingCommand) {
		printf("doing command %i\n", m_commandQueue.front().number);
		if (CheckStatus(m_commandQueue.front())) {
			m_commandQueue.pop();
			m_doingCommand = false;
		}
		else {
			ContinueCommand(m_commandQueue.front());
		}
	}
	else {
		if (!m_commandQueue.empty()) {
			m_doingCommand = true;
			DoCommand(m_commandQueue.front());
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
	m_commandQueue.push(Command);
}

void ChainCommands::DoCommand(command Command){
	//Variable Declaration
	float speed;
	float distance;
	float turn;
	float arcLength;

	m_tank.Zero();
	m_startAngle = m_tank.m_gyro.GetAngle();

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
		m_move.SetAll(speed,2*speed,speed,distance);
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

	case DRIVE_WAIT:
		m_timer.Reset();
		m_timer.Start();
		break;

	case SHOOT_START:
		m_shooter.Shoot();
		m_timer.Reset();
		m_timer.Start();
		break;

	case DRIVE_SHOOT:
		speed = Command.param2;
		distance = Command.param1;
		m_move.SetAll(speed,2*speed,speed,distance);
		m_move.CalcParams();
		m_shooter.Shoot();
		m_timer.Reset();
		m_timer.Start();
		break;

	}

	//m_runningCommands[Command.group] = m_commandQueue.front();
//	m_commandQueue.pop();
//
//	if(m_commandQueue.front().group != Command.group) {
//		DoCommand(m_commandQueue.front());
//	}
}

void ChainCommands::ContinueCommand(command Command){
	//Variable declaration
	const float distBtwnWhl = 30.5/2; /*Distance from center of bot to Wheels*/
	float turn;
	const float t = m_timer.Get();
	static float pos;
	float radius;
	float rightPos, leftPos;

	float angle;
	float angleError;
	float driveCorrection;
	float targetAngle;
	const float angleP = 0.01;

	switch(Command.number) {
	case DRIVE_TURN:
		turn = Command.param1;

		targetAngle = ((turn*6.28318)/wheelCircumInches) * (180/3.14159);
		angle = m_tank.m_gyro.GetAngle();
		angleError = (angle - m_startAngle)-targetAngle ;
		driveCorrection = angleError * angleP;

		// Get robot mostly rotated to targetAngle with trapezoidal move, then correct using error.
		if(t <= m_move.GetTotalTime()) {
			pos = m_move.Position(t);
		} else {
			pos -= driveCorrection;
			SmartDashboard::PutNumber("DriveCorrection", driveCorrection);
		}
		m_tank.PositionDrive(-pos, pos);

		break;

	case DRIVE_STRAIGHT:
		angle = m_tank.m_gyro.GetAngle();
		angleError = m_startAngle - angle;
		driveCorrection = angleError * angleP;

		pos=m_move.Position(t);
		m_tank.PositionDrive(pos - driveCorrection,
							 pos + driveCorrection);
		break;

	case DRIVE_ARC_CLOCKWISE:
		pos=m_move.Position(t);
		radius = Command.param3;
		rightPos = ((radius-distBtwnWhl)/radius)*pos;
		leftPos = ((radius+distBtwnWhl)/radius)*pos;
		m_tank.PositionDrive(leftPos,rightPos);
		break;

	case DRIVE_ARC_COUNTERCLOCKWISE:
		pos=m_move.Position(t);
		radius = Command.param3;
		rightPos = ((radius+distBtwnWhl)/radius)*pos;
		leftPos = ((radius-distBtwnWhl)/radius)*pos;
		m_tank.PositionDrive(leftPos,rightPos);
		break;
	case SHOOT_START:
		m_shooter.Shoot();
		break;
	case DRIVE_SHOOT:
		m_shooter.Shoot();
		angle = m_tank.m_gyro.GetAngle();
		angleError = m_startAngle - angle;
		driveCorrection = angleError * angleP;

		pos=m_move.Position(t);
		m_tank.PositionDrive(pos - driveCorrection,
							 pos + driveCorrection);
		break;
	}
}
bool ChainCommands::CheckStatus(command Command){
	//Variable declaration
	float distance;
	float speed;
	float arcLength;
	float time;
	float turn;
	float targetAngle;
	switch(Command.number) {
	case DRIVE_TURN:
		turn = Command.param1;
		targetAngle = ((turn*6.28318)/wheelCircumInches) * (180/3.14159);
		if(fabs((m_tank.m_gyro.GetAngle() - m_startAngle) - targetAngle) <= 0.1) {
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
		if (m_timer.Get()>= m_move.GetTotalTime()) {
			m_timer.Stop();
			m_tank.PositionDrive(28, 0);
			return(true);
		}
		else {
			return(false);
		}
		break;
	case DRIVE_ARC_CLOCKWISE:
		speed = Command.param2;
		arcLength = Command.param1;
		if (m_timer.Get()>=m_move.GetTotalTime()) {
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
		if (m_timer.Get()>=m_move.GetTotalTime()) {
			m_timer.Stop();
			return(true);
		}
		else {
			return(false);
		}
		break;

	case DRIVE_WAIT:
		time = Command.param1;
		if (m_timer.Get()>=time) {
			m_timer.Stop();
			return(true);
		}
		else {
			return(false);
		}
		break;

	case SHOOT_START:
		time = Command.param1;
		if (m_timer.Get()>=time) {
			m_timer.Stop();
			m_shooter.Stop();
			return(true);
		}
		else {
			return(false);
		}
		break;
	case DRIVE_SHOOT:
		if (m_timer.Get()>= m_move.GetTotalTime()) {
			m_timer.Stop();
			return(true);
		}
		else {
			return(false);
		}
		break;

	}
	return false;
}
ChainCommands::~ChainCommands() {
	// TODO Auto-generated destructor stub
}

