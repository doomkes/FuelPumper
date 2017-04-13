/*
 * Autonomous.cpp
 *
 *  Created on: Feb 13, 2017
 *      Author: Joey
 */

#include <Autonomous.h>
#include "RobotMap.h"
#include <vector>
using namespace cs;
using namespace grip;


RobotDimensions robotDimensions;

struct FieldDimensions {
	//TODO put field dimensions in here
};

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
	m_gear.Release(false);

	int autoSelect = Preferences::GetInstance()->GetInt("AutoMode", 0);
	m_mode = static_cast<AutoMode>(autoSelect);

	ShowAutoName();
}

void Autonomous::ShowAutoName() {
	int autoSelect = Preferences::GetInstance()->GetInt("AutoMode", 0);
	m_mode = static_cast<AutoMode>(autoSelect);

	switch(autoSelect) {
	default:
		m_autoName = "INVALID";
		break;
	case AutoMode::DO_NOTHING:
		m_autoName = "DO_NOTHING";
		break;
	case AutoMode::DIAGONAL_HOPPER_SHOOT:
		m_autoName = "DIAGONAL_HOPPER_SHOOT";
		break;
	case AutoMode::ARC_HOPPER_SHOOT:
		m_autoName = "ARC_HOPPER_SHOOT";
		break;
	case AutoMode::CENTER_GEAR_BASELINE:
		m_autoName = "CENTER_GEAR";
		break;

	case AutoMode::ARC_SIDE_GEAR:
		m_autoName = "ARC_SIDE_GEAR";
		break;
	case AutoMode::STATIONARY_SHOOT:
		m_autoName = "STATIONARY_SHOOT";
		break;
	case AutoMode::BASE_LINE:
	    m_autoName = "BASE_LINE";
	    break;
	case AutoMode::SHOOT_AND_GEAR:
		m_autoName = "SHOOT_AND_GEAR";
		break;
	}
	SmartDashboard::PutString("AutoName", to_string(autoSelect) + ": " + m_autoName);
}
void Autonomous::DisabledPeriodic() {
	ShowAutoName();
}

void Autonomous::AutonomousPeriodic() {
//	m_tank.Position();
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
	case AutoMode::ARC_SIDE_GEAR:
		ArcSideGear();
		break;
	case AutoMode::VBUS_TEST:
		m_tank.SetMode(DriveMode::VBUS);
		VBusTest();
		break;
	case AutoMode::STATIONARY_SHOOT:
		StationaryShoot();
		break;
	case AutoMode::BASE_LINE:
	     BaseLine();
	     break;
	case AutoMode::SHOOT_AND_GEAR:
	     ShootAndGear();
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
	const float airShipDist = 114.3;
	const float pegLength = 10.5;
	const float moveDist = airShipDist - RobotDimensions::length - pegLength+4;
	// inches.

	switch(m_state) {
		case 0: {// initiallize.
			//m_move.SetAll(12, 6, 12, 52.5);
			m_move.SetAll(30, 30, 50, moveDist);
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

			//if(t >= m_move.GetTotalTime()) {
				//m_gear.Release(true);
				//leftPos += 2;
			//}
			float angleError = 0;//m_startAngle = m_tank.GetAngle();
			//m_tank.StraightPositionDrive(leftPos, rightPos, angleError);
			m_tank.PositionDrive(-leftPos, -rightPos, false);

			printf("t: %f\nleftPos: %f \nrightPos %f\n", t, leftPos, rightPos);

			if(t >= m_move.GetTotalTime()) {
				m_gear.Release(true);
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
				m_shooter.Spinup(3150);
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
	float straight1Dist = 27;//32
	float arcRadius = 40;
	float arcAngle = 1.9198621771937625346160598453375; // 110 Deg
	float straight2Dist = 0;

	if(Preferences::GetInstance()->GetBoolean("LongHopper", false)) {
		straight1Dist = 41.96;//32
		arcRadius = 86.46;
		straight2Dist = 0;
	}

	float arcDistance = arcRadius*arcAngle;
	float innerArcRadius = arcRadius - robotDimensions.centerToWheel;
	float outerArcRadius = arcRadius + robotDimensions.centerToWheel;
	float innerArcDist = innerArcRadius*arcAngle;
	float outerArcDist = outerArcRadius*arcAngle;

	float innerRatio = innerArcDist/arcDistance;
	float outerRatio =  outerArcDist/arcDistance;

	float totalDist = straight1Dist + arcDistance + straight2Dist;

	switch(m_state) {
		case 0: {// initiallize.
			m_move.SetAll(50, 80, 100, totalDist);
			m_startAngle = m_tank.GetAngle();
			m_shooter.Spinup(3150);
			m_tank.Zero();
			m_timer.Reset();
			m_timer.Start();
			m_state++;
			break;
		}
		case 1: {
			float t = m_timer.Get();
			float centerPos = m_move.Position(t);
			float leftPos = centerPos;
			float rightPos = centerPos;
			if(centerPos > straight1Dist && centerPos < (straight1Dist+arcDistance)) {
				m_shooter.Shoot();
				centerPos -= straight1Dist;
				leftPos = centerPos;
				rightPos = centerPos;
				leftPos *= outerRatio;
				rightPos *= innerRatio;
				leftPos += straight1Dist;
				rightPos += straight1Dist;
			}
//			if(centerPos > (straight1Dist+arcDistance)) {
//				centerPos -= (straight1Dist+arcDistance);
//				leftPos = centerPos;
//				rightPos = centerPos;
//				leftPos +=arcLeftPos;
//				rightPos +=arcRightPos;
//			}

			// swap left/drive if field is mirrored.
			if(DriverStation::GetInstance().GetAlliance() == DriverStation::kBlue) {
				std::swap(rightPos,leftPos);
			}

			m_tank.PositionDrive(leftPos, rightPos, false);

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
			m_tank.Drive(-.2, -.2);
			break;
		}
	}
}


void Autonomous::ArcSideGear() {
	static float arcRightPos = 0;
	static float arcLeftPos= 0;
	bool startBoiler = Preferences::GetInstance()->GetBoolean("StartBoiler", true);

//	constexpr float straight1Dist = 51.14 - (RobotDimensions::length);
//	constexpr float arcRadius = 76.3;
//	constexpr float arcAngle = 1.0471975511965977461542144610932;//0.52359;
//	constexpr float straight2Dist = 56.69 - (RobotDimensions::length/2);
	constexpr float straight1Dist = (RobotDimensions::length/2) - 2;
	constexpr float arcRadius = 87.3;
	constexpr float arcAngle = 1.0471975511965977461542144610932;//0.52359;
	float straight2Dist = 26.66 - (RobotDimensions::length/2) + 2;
	if (startBoiler){
		straight2Dist += 2;
	}
	constexpr float arcDistance = (arcRadius*arcAngle);
	constexpr float innerArcRadius = arcRadius - robotDimensions.centerToWheel;
	constexpr float outerArcRadius = arcRadius + robotDimensions.centerToWheel;
	constexpr float innerArcDist = innerArcRadius*arcAngle;
	constexpr float outerArcDist = outerArcRadius*arcAngle;

	constexpr float innerRatio = innerArcDist/arcDistance;
	constexpr float outerRatio =  outerArcDist/arcDistance;

	const float totalDist = straight1Dist + arcDistance + straight2Dist;

	switch(m_state) {
		case 0: {// initiallize.
			m_move.SetAll(30, 30, 50, totalDist);
			m_startAngle = m_tank.GetAngle();
			m_shooter.Stop();
			m_tank.Zero();
			m_timer.Reset();
			m_timer.Start();
			m_gear.Release(false);
			m_state++;
			break;
		}
		case 1: {
			float t = m_timer.Get();
			float centerPos = m_move.Position(t);
			float leftPos = centerPos;
			float rightPos = centerPos;

			if(centerPos > straight1Dist && centerPos < (straight1Dist+arcDistance)) {
				centerPos -= straight1Dist;
				leftPos = centerPos;
				rightPos = centerPos;
				leftPos *= outerRatio;
				rightPos *= innerRatio;
				leftPos += straight1Dist;
				rightPos += straight1Dist;
				arcRightPos = rightPos;
				arcLeftPos = leftPos;
			}
			if(centerPos > (straight1Dist+arcDistance)) {
				centerPos -= (straight1Dist+arcDistance);
				leftPos = centerPos;
				rightPos = centerPos;
				leftPos +=arcLeftPos;
				rightPos +=arcRightPos;
			}

			// swap left/drive if field is mirrored.
			if(DriverStation::GetInstance().GetAlliance() == DriverStation::kBlue) {
				std::swap(rightPos,leftPos);
			}

			if(startBoiler == false) {
				std::swap(rightPos,leftPos);
			}

			m_tank.PositionDrive(-leftPos, -rightPos, false);

			if(t > m_move.GetTotalTime()) {
				m_gear.Release(true);
				m_move.SetAll(10,10,10, 22);
				m_tank.Zero();
				m_timer.Reset();
				m_timer.Start();
				m_state++;
			}

			printf("t: %f\nleftPos: %f \nrightPos %f\n", t, leftPos, rightPos);
			break;
		}
		case 2: { // Shoot.
			m_gear.Release(true);
			float t = m_timer.Get();
			float leftPos = m_move.Position(t);
			float rightPos = leftPos;
			m_tank.PositionDrive(leftPos, rightPos, false);
			printf("t: %f\nleftPos: %f \nrightPos %f\n", t, leftPos, rightPos);
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

/*
 * Drive forward and place a gear on the middle peg
 * then backup halfway and do a 45 deg turn, & drive past the baseline.
 */

void Autonomous::BaseLine() {
	float leftPos, rightPos;
	float t = m_timer.Get();

	const float baseLineDist = 93.25 + 35.7/2;

	switch(m_state) {
	case 0 : {// initialize.
		m_move.SetAll(50, 50, 50, baseLineDist);
		m_shooter.Stop();
		m_timer.Reset();
		m_timer.Start();
		m_gear.Release(false);
		m_state++;
		break;
	}
	case 1: {// drive to base line
		float leftPos, rightPos;
		float t = m_timer.Get();
		leftPos = m_move.Position(t);
		rightPos = leftPos;
		m_tank.PositionDrive(leftPos, rightPos, false);
					break;
	}
	}
}

void Autonomous::ShootAndGear() {

}
