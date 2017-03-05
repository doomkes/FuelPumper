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
		Shooter &shooter
		)
:

		m_cameraServer(m_cameraServer),
		m_outputStream(m_outputStream),
		camera(camera),
		m_tank(Tank),
		m_shooter(shooter),
		m_chain(m_tank, m_shooter),
		m_relMove(m_chain)
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
	static TrapezoidalMove move;
	m_tank.Position();
	static Timer timer;
	switch(m_state) {
	case 0:
//		m_relMove.Linear(0, 71, 30);
//		m_chain.AddCommand(DRIVE_TURN,-24,0,0);

		m_relMove.Linear(0, 88, 30);
				m_chain.AddCommand(SHOOT_START,38,50,0);
		//m_relMove.Arc(false,48,48,48,0,48);
		m_state++;

		break;
	case 1:
		m_chain.AutoPeriodic();
		m_tank.Position();
		break;
	}

//	float t = m_timer.Get();
//	m_tank.PositionDrive(m_move.Position(t), m_move.Position(t));
//		cv::Mat frame;
//
//		m_cameraServer->GetVideo().GrabFrame(frame);
//		m_boilerVision.process(frame);
//		std::vector<std::vector<cv::Point>> foundContours;
//		foundContours = *m_boilerVision.getfindContoursOutput();
//
//		vector<cv::Moments> mu(foundContours.size());
//
//		vector<cv::Point2f> mc(foundContours.size());
//
//		for( unsigned i = 0; i < foundContours.size(); i++ ) {
//			mu[i] = moments( foundContours[i], false );
//		}
//
//		for(unsigned i = 0; i < foundContours.size(); i++) {
//			mc[i] = cv::Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
//		}
//
//		for(unsigned i = 0; i < foundContours.size(); i++) {
//			printf("Found Contours!:  x: %f, y: %f\n", mc[i].x, mc[i].y);
//		}
//		for(unsigned i = 0; i < foundContours.size(); i++) {
//			cv::Rect boundingRect = cv::boundingRect(foundContours[i]);
//			printf("X: %i, Y: %i, W: %i, H: %i\n",boundingRect.x,boundingRect.y,boundingRect.width,boundingRect.height);
//		}
//
//
//		cv::Mat contourImg;
//		cv::Mat mymat;
//		for(auto contour : foundContours) {
//			cv::drawContours(contourImg, contour, {255, 0, 0});
//			cv::Mat mymat = *Vision.gethsvThresholdOutput();
//		}
//		m_outputStream.PutFrame(mymat);
	}
