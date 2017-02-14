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
public:
	Vision();
	virtual ~Vision();
	void TeleopInit();
	void TeleopPeriodic();
};

#endif /* SRC_VISION_H_ */
