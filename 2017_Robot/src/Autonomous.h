/*
 * Autonomous.h
 *
 *  Created on: Feb 13, 2017
 *      Author: Joey
 */
#include <WPILib.h>
#include "GCode.h"

#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_
#include "TankDrive.h"
#include "BoilerVision.h"
using namespace cs;
using namespace grip;
class Autonomous {
	CameraServer *m_cameraServer = nullptr;
	cs::CvSource m_outputStream;
	cs::UsbCamera camera;
	grip::BoilerVision m_boilerVision;
	TankDrive &m_tank;
	GCode m_gcode;
public:
	Autonomous(
		CameraServer *,
		CvSource &m_outputStream,
		UsbCamera &camera,
		TankDrive &m_tank
//		,
//		BoilerVision &m_boilerVision
	);
	virtual ~Autonomous();

	void AutonomousPeriodic();
	void AutonomousInit();
};

#endif /* SRC_AUTONOMOUS_H_ */
