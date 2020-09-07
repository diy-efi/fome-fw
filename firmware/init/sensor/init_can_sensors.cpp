/**
 * @file	init_can_sensors.cpp
 *
 * @date March 31, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "engine_configuration.h"

#if EFI_CAN_SUPPORT
#include "can_sensor.h"
#include "can.h"

EXTERN_CONFIG
;


#define TIMEOUT MS2NT(100)

CanSensor<int16_t, PACK_MULT_PERCENT> canPedalSensor(
	CAN_DEFAULT_BASE + CAN_PEDAL_TPS_OFFSET, /*offset =*/ 0,
	SensorType::AcceleratorPedal, TIMEOUT
);

ObdCanSensor<int16_t, ODB_RPM_MULT> obdRpmSensor(
		OBD_TEST_RESPONSE, PID_RPM,
	SensorType::Rpm, TIMEOUT
);

ObdCanSensor<int16_t, 1> obdCltSensor(
		OBD_TEST_RESPONSE, PID_COOLANT_TEMP,
	SensorType::Clt, TIMEOUT
);


void initCanSensors() {
	if (CONFIG(consumeObdSensors)) {
//		registerCanSensor(canPedalSensor);
		registerCanSensor(obdRpmSensor);
//		registerCanSensor(obdCltSensor);
	}
}
#endif // EFI_CAN_SUPPORT
