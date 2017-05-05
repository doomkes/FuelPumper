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
	ARC_SIDE_GEAR,
	STATIONARY_SHOOT,
	BASE_LINE,
	SHOOT_AND_GEAR,
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
	bool m_startBoiler = true;
	string m_autoName = ""; 
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
	void DisabledPeriodic();
	void AutonomousPeriodic();
	void AutonomousInit();

	void ShowAutoName();

	void StraightGear();
	void ArcSideGear();
	void ShootFromHopper();
	void ArcShootFromHopper();

	void PositionTest();
	void VBusTest();
	void StationaryShoot();
	void BaseLine();
	void ShootAndGear();
};



#endif /* SRC_AUTONOMOUS_H_ */
