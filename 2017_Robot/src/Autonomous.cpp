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

	m_state = 0;
	m_chain.Init();
	m_tank.Init();
	m_tank.SetMode(DriveMode::POSITION);
	m_tank.Zero();

	int autoSelect = Preferences::GetInstance()->GetInt("AutoMode", 0);
	m_mode = static_cast<AutoMode>(autoSelect);

	string autoName = "";

	switch(autoSelect) {
	default:
		autoName = "INVALID";
		break;
	case AutoMode::DO_NOTHING:
		autoName = "DO_NOTHING";
		break;
	case AutoMode::DIAGONAL_HOPPER_SHOOT:
		autoName = "DIAGONAL_HOPPER_SHOOT";
		break;
	case AutoMode::ARC_HOPPER_SHOOT:

		break;
	case AutoMode::CENTER_GEAR_BASELINE:
		autoName = "CENTER_GEAR";
		break;
	case AutoMode::STATIONARY_SHOOT:
		autoName = "STATIONARY_SHOOT";
		break;

	}
	SmartDashboard::PutString("AutoName", autoName);
}

void Autonomous::AutonomousPeriodic() {
	m_tank.Position();
	switch(m_mode) {
	case AutoMode::DO_NOTHING:
		break;
	case AutoMode::DIAGONAL_HOPPER_SHOOT:
		ShootFromHopper();
		break;
	case AutoMode::CENTER_GEAR_BASELINE:
		StraightGear();
		break;
	case AutoMode::ARC_HOPPER_SHOOT:
		ArcShootFromHopper();
		break;
	case AutoMode::POS_TEST:
		PositionTest();
		break;
	case AutoMode::VBUS_TEST:
		m_tank.SetMode(DriveMode::VBUS);
		VBusTest();
		break;
	case AutoMode::STATIONARY_SHOOT:
		StationaryShoot();
		break;
	}
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

/*
 * Drive forward and place a gear on the middle peg
 * then backup halfway and do a 45 deg turn, & drive past the baseline.
 */
void Autonomous::StraightGear() {
	float leftPos, rightPos;
	float t = m_timer.Get();

	// Distance to the face of the airship, in inches.
	const float airShipDist = 93.25;
	// inches.
	const float pegLength = 13;
	switch(m_state) {
		case 0: {// initiallize.
			//m_move.SetAll(12, 6, 12, 52.5);
			m_move.SetAll(12, 6, 12, airShipDist - pegLength);
			m_shooter.Stop();
			m_timer.Reset();
			m_timer.Start();
			m_gear.Release(false);
			m_state++;
			break;
		}
		case 1: { // drive to peg and release.
			leftPos = m_move.Position(t);
			rightPos = leftPos;

			if(t >= m_move.GetTotalTime()) {
				m_gear.Release(true);
				leftPos += 2;
			}
			float angleError = 0;//m_startAngle = m_tank.GetAngle();
			//m_tank.StraightPositionDrive(leftPos, rightPos, angleError);
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
		case 2: { // backup.

			leftPos = m_move.Position(t);
			rightPos = leftPos;
			m_tank.PositionDrive(leftPos, rightPos, false);
			if(t > m_move.GetTotalTime()) {
//				m_move.SetAll(12,6, 12, 12);
//				m_timer.Reset();
//				m_timer.Start();
				//m_state++;
			}
			break;
		}
		case 3: { // 45 deg turn.
			leftPos = m_move.Position(t);
			rightPos = -leftPos;

			m_tank.PositionDrive(leftPos, rightPos, false);
			if(t > m_move.GetTotalTime()) {
				m_move.SetAll(24,24, 24, 60);
				m_tank.Zero();
				m_state++;
			}
			break;
		}
		case 4: { // final move to cross baseline.
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
			m_startAngle = m_tank.GetAngle();
			m_shooter.Stop();
			m_tank.Zero();
			m_timer.Reset();
			m_timer.Start();
			m_state++;
			break;
		}
		case 1: {
			float leftPos, rightPos;
			float angleError = 0;
			float t = m_timer.Get();
			leftPos = m_move.Position(t);
			rightPos = leftPos;


			if(leftPos < 85) {
				angleError = m_startAngle - m_tank.GetAngle();
			} else { // freeze right side after 85 in.
				m_shooter.Spinup();
				rightPos = 85;
				angleError = 0;
			}

			// swap left/drive if field is mirrored.
			if(DriverStation::GetInstance().GetAlliance() == DriverStation::kBlue) {
				std::swap(rightPos,leftPos);
			}

			m_tank.PositionDrive(leftPos, rightPos, false);
			//m_tank.StraightPositionDrive(leftPos, rightPos, angleError);

			if(t > m_move.GetTotalTime()) {
				m_shooter.Shoot();
				m_tank.SetMode(DriveMode::VBUS);
				m_state++;
			}

			printf("t: %f\nleftPos: %f \nrightPos %f\n", t, leftPos, rightPos);
			break;
		}
		case 2: {
			//if(DriverStation::GetInstance().GetAlliance() == DriverStation::kBlue) {
			//	std::swap(rightPos,leftPos);
			//}

			m_shooter.Shoot();
			m_tank.Drive(-.2, -.45);
			break;
		}
	}
}

void Autonomous::ArcShootFromHopper() {
	const float straight1Dist = 56.88;
	const float innerArcDist = 23.56;
	const float outerArcDist = 69.712;
	const float straight2Dist = 20.15;
	constexpr float totalDist = straight1Dist + innerArcDist + straight2Dist;
	constexpr float ratio = outerArcDist / innerArcDist; //2.9586;

	switch(m_state) {
		case 0: {// initiallize.
			m_move.SetAll(45,50, 60, totalDist);
			m_startAngle = m_tank.GetAngle();
			m_shooter.Stop();
			m_tank.Zero();
			m_timer.Reset();
			m_timer.Start();
			m_state++;
			break;
		}
		case 1: {
			float leftPos, rightPos;
			float angleError = 0;
			float t = m_timer.Get();
			rightPos = m_move.Position(t);
			leftPos = rightPos;

			if(rightPos > straight1Dist && rightPos < innerArcDist) {
				leftPos *= ratio;
				angleError = 0;
			}

			// swap left/drive if field is mirrored.
			if(DriverStation::GetInstance().GetAlliance() == DriverStation::kBlue) {
				std::swap(rightPos,leftPos);
			}

			//m_tank.PositionDrive(leftPos, rightPos, false);
			m_tank.StraightPositionDrive(leftPos, rightPos, angleError);

			if(t > m_move.GetTotalTime()) {
				m_shooter.Shoot();
				m_tank.SetMode(DriveMode::VBUS);
				m_state++;
			}

			printf("t: %f\nleftPos: %f \nrightPos %f\n", t, leftPos, rightPos);
			break;
		}
		case 2: { // Shoot.
			m_shooter.Shoot();
			// keep a small driving force against hopper.
			m_tank.Drive(.4, .2);
			break;
		}
	}
}

void Autonomous::PositionTest() {
	m_tank.PositionDrive(5,5);
}

void Autonomous::VBusTest() {
	m_tank.Drive(-0.2, -0.45);
}

void Autonomous::StationaryShoot() {
	m_shooter.Shoot();

}
