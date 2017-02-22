/*
 * Vision.h
 *
 *  Created on: Feb 13, 2017
 *      Author: administrator
 */

#ifndef SRC_VISION_H_
#define SRC_VISION_H_

#include <WPILib.h>

class Vision {
	frc::CameraServer* cameraServer;
	cs::UsbCamera* usbCamera_1;
	cs::UsbCamera* usbCamera_2;
	cs::UsbCamera* usbCamera_3;
	frc::DigitalOutput* digitalOutput_cameraLight1;
	frc::DigitalOutput* digitalOutput_cameraLight2;
	frc::DigitalOutput* digitalOutput_cameraLight3;
	frc::JoystickButton* joystickButton_camera1;
	frc::JoystickButton* joystickButton_camera2;
	frc::JoystickButton* joystickButton_camera3;
	frc::JoystickButton* joystickButton_cameraLight1;
	frc::JoystickButton* joystickButton_cameraLight2;
	frc::JoystickButton* joystickButton_cameraLight3;

public:
	Vision(
		frc::CameraServer* cameraServer,
		cs::UsbCamera* usbCamera_1,
		cs::UsbCamera* usbCamera_2,
		cs::UsbCamera* usbCamera_3,
		frc::DigitalOutput* digitalOutput_cameraLight1,
		frc::DigitalOutput* digitalOutput_cameraLight2,
		frc::DigitalOutput* digitalOutput_cameraLight3,
		frc::JoystickButton* joystickButton_camera1,
		frc::JoystickButton* joystickButton_camera2,
		frc::JoystickButton* joystickButton_camera3,
		frc::JoystickButton* joystickButton_cameraLight1,
		frc::JoystickButton* joystickButton_cameraLight2,
		frc::JoystickButton* joystickButton_cameraLight3
	);
	virtual ~Vision();
	void TeleopInit();
	void TeleopPeriodic();
	void CameraLight(frc::DigitalOutput* cameraLight, bool state);
	void CameraLight1(bool state);
	void CameraLight2(bool state);
	void CameraLight3(bool state);
	void Camera(cs::UsbCamera* usbCamera);


};

#endif /* SRC_VISION_H_ */
