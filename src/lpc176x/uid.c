#include "LPC17xx.h"
#include "generic/misc.h"
#include "uid.h"

#define IAP_LOCATION        0x1fff1ff1

#define IAP_CMD_READ_UID    58 

#define SECTOR_COUNT        30

typedef void (*IAP)(uint32_t *, uint32_t *);

uint16_t
platform_get_uid(void) {
    uint32_t iap_cmd_uid[5] = {IAP_CMD_READ_UID, 0, 0, 0, 0};
    uint32_t iap_resp[5];

    IAP iap_entry = (IAP)IAP_LOCATION;
    __disable_irq();
    iap_entry(iap_cmd_uid, iap_resp);
    __enable_irq();

    return crc16_ccitt((uint8_t *)&iap_resp[1], sizeof(iap_resp[1])*4);
}
