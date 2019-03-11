
#ifndef _GATT_DB_H_
#define _GATT_DB_H_

#include "bluenrg1_api.h"

#include "HardwareUtil/HW_Init.h"

#define X_OFFSET 200
#define Y_OFFSET 50
#define Z_OFFSET 1000

/**
 * @brief Number of application services
 */
#define NUMBER_OF_APPLICATION_SERVICES (3)


extern uint8_t HW_ID;
extern uint8_t HW_VERSION;
extern uint8_t HW_BUS_DEV_NUM;

enum {
  INFO_SERVICE_INDEX = 0,
  CMD_SERVICE_INDEX = 1,
  DIRECT_SERVICE_INDEX = 2
};

tBleStatus bl_gatt_addHWInfoService(void);
tBleStatus bl_gatt_addCMDService(void);
tBleStatus bl_gatt_addDirectService(void);

tBleStatus bl_gatt_initHWInfo(void);

tBleStatus bl_gatt_updateDirectParam(uint8_t *param);
tBleStatus bl_gatt_updateErrorCode(void);

tBleStatus Add_Environmental_Sensor_Service(void);

extern uint8_t Services_Max_Attribute_Records[];
#endif /* _GATT_DB_H_ */
