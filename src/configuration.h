// clang-format off
#pragma once
#include <Arduino.h>

/***************************
 * I2C SLAVE CONFIGURATION *
 ***************************/
#define I2C_SLAVE_ADDRESS 0x45
#define I2C_TX_BUFFER_SIZE 32 // don't change this value if you don't know what you're doing

/************************
 * BME280 CONFIGURATION *
 ************************/
#define BME_CS PA4
#define BME_SCK PA5
#define BME_MISO PA6
#define BME_MOSI PA7

/*********************
 * LED CONFIGURATION *
 *********************/
#define LED_PIN PC6
#define LED_COUNT 1

/**********************
 * GPIO CONFIGURATION *
 **********************/
#define NUM_DIGITAL_OUTPUT_PINS 8
#define DIGITAL_OUTPUT_PINS {PB3, PB4, PB5, PB6, PB7, PB8, PA11, PA12}
#define NUM_DIGITAL_INPUT_PINS 0
#define INPUT_DIGITAL_PINS {}
// Analog Input Configuration
#define NUM_ANALOG_INPUT_PINS 2
#define INPUT_ANALOG_PINS {PB1, PA0}

/***********************
 * SERVO CONFIGURATION *
 ***********************/
#define NUM_SERVOS 0 
#define SERVO_PINS {}

// clang-format on