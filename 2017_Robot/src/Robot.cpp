#include <iostream>
#include <memory>
#include <string>
//mjj004
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <WPILib.h>
#include "RobotMap.h"
#include "TankDrive.h"
#include "Pickup.h"
#include "BoilerVision.h"
using frc::SmartDashboard;
using namespace std;
using namespace frc;
class Robot: public frc::IterativeRobot {
	frc::Solenoid m_gearShift;
	CANTalon m_leftMotor1;
	CANTalon m_leftMotor2;
	CANTalon m_rightMotor1;
	CANTalon m_rightMotor2;

	TankDrive m_tank;

	grip::BoilerVision Vision;
	CameraServer *cameraServer = nullptr;
	cs::CvSource m_outputStream;
	cs::UsbCamera camera;
	Pickup m_pickup;
	Joystick m_leftStick, m_rightStick;

	frc::Talon m_intakeMotor;
	// Hopper Motor pushs into Hopper
	frc::Talon m_hopperMotor;

public:
	Robot()
		:
		m_leftStick(1) //todo:mjj use config constant instead of literal number
		,m_rightStick(2) //todo:mjj use config constant instead of literal number
		,m_gearShift(GEAR_SHIFT)
		,m_leftMotor1(LEFT_DRIVE1)
		,m_leftMotor2(LEFT_DRIVE2)
		,m_rightMotor1(RIGHT_DRIVE1)
		,m_rightMotor2(RIGHT_DRIVE2)
		, m_intakeMotor(0)
		, m_hopperMotor(1)
		,m_tank(
			m_leftStick
			, m_rightStick
			, m_gearShift
			, m_leftMotor1
			, m_leftMotor2
			, m_rightMotor1
			, m_rightMotor2
			)
		,m_pickup(
			m_leftStick
			, m_intakeMotor
			, m_hopperMotor
			)

	{

	}

	void RobotInit() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		m_tank.TeleopPeriodic();

		if(m_leftStick.GetRawButton(PICKUP)) {
			m_pickup.Intake(true);
		} else m_pickup.stop();
	}

	void AutonomousInit() override {
		cameraServer = CameraServer::GetInstance();
		camera = cameraServer->StartAutomaticCapture();
		m_outputStream = CameraServer::GetInstance()->PutVideo("thresh", 640, 480);
		camera.SetResolution(640, 480);
		camera.SetExposureManual(1);
	}

	void AutonomousPeriodic() {
		cv::Mat frame;

		cameraServer->GetVideo().GrabFrame(frame);
		Vision.process(frame);
		std::vector<std::vector<cv::Point>> foundContours;
		foundContours = *Vision.getfindContoursOutput();

		vector<cv::Moments> mu(foundContours.size());

		vector<cv::Point2f> mc(foundContours.size());

		for( unsigned i = 0; i < foundContours.size(); i++ ) {
			mu[i] = moments( foundContours[i], false );
		}

		for(unsigned i = 0; i < foundContours.size(); i++) {
			mc[i] = cv::Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}

		for(unsigned i = 0; i < foundContours.size(); i++) {
			printf("Found Contours!:  x: %f, y: %f\n", mc[i].x, mc[i].y);
		}

//		cv::Mat contourImg;
//		cv::Mat mymat;
//		for(auto contour : foundContours) {
//			cv::drawContours(contourImg, contour, {255, 0, 0});
//			cv::Mat mymat = *Vision.gethsvThresholdOutput();
//		}
//		m_outputStream.PutFrame(mymat);
	}

	void TestPeriodic() {

	}
};

START_ROBOT_CLASS(Robot)
