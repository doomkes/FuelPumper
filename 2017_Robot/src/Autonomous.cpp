/*
 * Autonomous.cpp
 *
 *  Created on: Feb 13, 2017
 *      Author: Joey
 */

#include <Autonomous.h>
#include <vector>
using namespace cs;
using namespace grip;

Autonomous::Autonomous(
	CameraServer *m_cameraServer,
		cs::CvSource &m_outputStream,
		cs::UsbCamera &camera,
		TankDrive &Tank,
		Shooter &shooter,
		GearManipulator &gear
		)
:

		m_cameraServer(m_cameraServer),
		m_outputStream(m_outputStream),
		camera(camera),
		m_tank(Tank),
		m_shooter(shooter),
		m_chain(m_tank, m_shooter),
		m_relMove(m_chain),
		m_gear(gear)
{

}

Autonomous::~Autonomous() {
	// TODO Auto-generated destructor stub
}

void Autonomous::AutonomousInit() {
//		cameraServer = CameraServer::GetInstance();
	camera = m_cameraServer->StartAutomaticCapture(0);
	m_outputStream = CameraServer::GetInstance()->PutVideo("thresh", 640, 480);
	camera.SetResolution(640, 480);
	camera.SetExposureManual(1);
	m_state = 0;
	m_chain.Init();
	m_tank.Init();
	m_tank.SetMode(DriveMode::POSITION);
	m_tank.Zero();
}

void Autonomous::AutonomousPeriodic() {
	m_tank.Position();

//	ShootFromHopper();
	StraightGear();

//	switch(m_state) {
//	case 0:
////		m_relMove.Linear(0, 71, 30);
////		m_chain.AddCommand(DRIVE_TURN,-24,0,0);
//
//		m_relMove.Linear(0, 88, 30);
//				m_chain.AddCommand(SHOOT_START,38,50,0);
//		//m_relMove.Arc(false,48,48,48,0,48);
//		m_state++;
//
//		break;
//	case 1:
//		m_chain.AutoPeriodic();
//		m_tank.Position();
//		break;
//	}

}

void Autonomous::StraightGear() {
	switch(m_state) {
		case 0: {// initiallize.
			m_move.SetAll(12, 6, 12, 52.5);
			m_shooter.Stop();
			m_timer.Reset();
			m_timer.Start();
			m_gear.Release(false);
			m_state++;
			break;
		}
		case 1: {
			float leftPos, rightPos;
			float t = m_timer.Get();
			leftPos = m_move.Position(t);
			rightPos = leftPos;

			if(leftPos >= 52.5) {
				m_gear.Release(true);
				leftPos += 2;
			}

			m_tank.PositionDrive(-leftPos, -rightPos, false);
			printf("t: %f\nleftPos: %f \nrightPos %f\n", t, leftPos, rightPos);

			if(t >= m_move.GetTotalTime() + 1) {
				m_move.SetAll(3,6, 12, 20);
				m_timer.Reset();
				m_timer.Start();
				m_tank.Zero();
				m_state++;
			}
			break;
		}
		case 2: {
			float leftPos, rightPos;
			float t = m_timer.Get();
			leftPos = m_move.Position(t);
			rightPos = leftPos;
			m_tank.PositionDrive(leftPos, rightPos, false);

			break;
		}
	}
}

void Autonomous::ShootFromHopper() {
	switch(m_state) {
		case 0: {// initiallize.
			m_move.SetAll(45,50, 60, 116);
			m_shooter.Stop();
			m_timer.Reset();
			m_timer.Start();
			m_state++;
			break;
		}
		case 1: {
			float leftPos, rightPos;
			float t = m_timer.Get();
			leftPos = m_move.Position(t);
			rightPos = leftPos;
			if(leftPos > 90) { // freeze right side after 68 in.
				rightPos = 90;
			}
			if(leftPos >= 115) {
				m_shooter.Shoot(0);
			}
			m_tank.PositionDrive(leftPos, rightPos, false);
			printf("t: %f\nleftPos: %f \nrightPos %f\n", t, leftPos, rightPos);
			break;
		}
	}
}
