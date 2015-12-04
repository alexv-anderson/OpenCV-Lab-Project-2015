
#ifndef DRIVE_GPIO_H
#define DRIVE_GPIO_H

#include "navigation.h"
#include "gpio.h"

// Globals
GPIOClass* goPin;
GPIOClass* rightPin;
GPIOClass* leftPin;
GPIOClass* gpio13;

// Only call this once
void driveGPIO_init() {
	// Initialize the four GPIO outputs
	// GPIO 6 -- Go bit
	goPin = new GPIOClass("6"); //create new GPIO object to be attached to  GPIO4
	// GPIO 13 -- unused
	gpio13 = new GPIOClass("13");
	// GPIO 19 -- right
	rightPin = new GPIOClass("19");
	// GPIO 26 -- left
	leftPin = new GPIOClass("26");

	// Set all of the directions to out
	goPin->setdir_gpio("out");
	gpio13->setdir_gpio("out");
	rightPin->setdir_gpio("out");
	leftPin->setdir_gpio("out");
}

void sendNavBits(NavBits navBits)
{
	//TODO: Add code to send bits through GPIO on Pi
	if (navBits.go == true) {
		goPin->setval_gpio("1");
	}
	else {
		goPin->setval_gpio("0");
	}
	if (navBits.left == true) {
		leftPin->setval_gpio("1");
	}
	else {
		leftPin->setval_gpio("0");
	}
	if (navBits.right == true) {
		rightPin->setval_gpio("1");
	}
	else {
		rightPin->setval_gpio("0");
	}
}

void driveGPIO_close() {
	delete goPin;
	delete leftPin;
	delete rightPin;
	delete gpio13;
	goPin = NULL;
	leftPin = NULL;
	rightPin = NULL;
	gpio13 = NULL;
}

#endif	//DRIVE_GPIO_H

