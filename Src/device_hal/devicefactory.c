#include "devicefactory.h"
#include "devices/gpio_input.h"
#include "devices/gpio_output.h"
#include "devices/bme280_io.h"
#include "devices/current_sens.h"
#include "configuration.h"
#include <stdlib.h>
#include <string.h>

extern ADC_HandleTypeDef hadc1; // Assuming hadc1 is defined in adc.c

// create a array to hold all the configured devices
Device device_array[NUM_DEVICES];

static void CreateDevice(Device *dev, const DeviceConfig *cfg) {
    switch (cfg->type) {
        case DEVICE_TYPE_GPIO_INPUT:
            GPIOInput_Create(dev, cfg->gpio.port, cfg->gpio.pin, cfg->gpio.pull);
            break;
        case DEVICE_TYPE_GPIO_OUTPUT:
            GPIOOutput_Create(dev, cfg->gpio.port, cfg->gpio.pin);
            break;
        case DEVICE_TYPE_BME280:
            BME280_Device_Create(dev, cfg->bme280.hspi, cfg->bme280.cs_port, cfg->bme280.cs_pin);
            break;
        case DEVICE_TYPE_CURRENT_SENSOR:
            CurrentSensor_Create(dev, &hadc1, ADC_CHANNEL_9, 3.3f, 0.1f, 2.5f); // Example values
            break;
        // case DEVICE_TYPE_SERVO:
        //     Servo_Create(dev, cfg->pwm.htim, cfg->pwm.channel);
        //     break;
        // case DEVICE_TYPE_WS2812:
        //     WS2812_Create(dev, cfg->ws2812.port, cfg->ws2812.pin, cfg->ws2812.count);
        //     break;
        // Add more cases
    }
    dev->name = cfg->name;
}

void SetupDevices() {
    for (size_t i = 0; i < NUM_DEVICES; i++) {
        // Work directly with the device array elements - no need for malloc/free
        CreateDevice(&device_array[i], &device_configs[i]);
        
        // Initialize the device if an init function is defined
        if (device_array[i].init) {
            device_array[i].init(&device_array[i]);
        }
    }
}

// Utility function to find a device by name
Device* FindDeviceByName(const char* name) {
    for (size_t i = 0; i < NUM_DEVICES; i++) {
        if (device_array[i].name && strcmp(device_array[i].name, name) == 0) {
            return &device_array[i];
        }
    }
    return NULL;
}

// Utility function to get devices by type
size_t GetDevicesByType(DeviceType type, Device** devices, size_t max_devices) {
    size_t count = 0;
    for (size_t i = 0; i < NUM_DEVICES && count < max_devices; i++) {
        // You'd need to add a type field or use configuration lookup
        // This is a simplified version
        devices[count++] = &device_array[i];
    }
    return count;
}

