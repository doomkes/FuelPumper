/*
 * Autonomous.h
 *
 *  Created on: Feb 13, 2017
 *      Author: Joey
 */
#include <RelativeMovement.h>
#include <WPILib.h>

#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_
#include "TankDrive.h"
#include "BoilerVision.h"
#include "TrapezoidalMove.h"
#include "GearManipulator.h"
using namespace cs;
using namespace grip;

enum AutoMode {
	DO_NOTHING,
	DIAGONAL_HOPPER_SHOOT,
	ARC_HOPPER_SHOOT,
	CENTER_GEAR_BASELINE,
	STATIONARY_SHOOT,
	POS_TEST = 99,
	VBUS_TEST = 98,
};

class Autonomous {
	CameraServer *m_cameraServer = nullptr;
	cs::CvSource m_outputStream;
	cs::UsbCamera camera;
	grip::BoilerVision m_boilerVision;
	TankDrive &m_tank;
	Shooter &m_shooter;
	GearManipulator& m_gear;
	ChainCommands m_chain;
	RelativeMovement m_relMove;
	AutoMode m_mode = DO_NOTHING;
	int m_state = 0;
	float m_startAngle = 0;
	TrapezoidalMove m_move;
	Timer m_timer;
public:
	Autonomous(
		CameraServer *,
		CvSource &m_outputStream,
		UsbCamera &camera,
		TankDrive &m_tank,
		Shooter &shooter,
		GearManipulator&
	);
	virtual ~Autonomous();

	void AutonomousPeriodic();
	void AutonomousInit();

	void StraightGear();
	void ShootFromHopper();
	void ArcShootFromHopper();
	void PositionTest();
	void VBusTest();
	void StationaryShoot();
};

#endif /* SRC_AUTONOMOUS_H_ */
