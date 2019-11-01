#include "LPC17xx.h"

#define IAP_LOCATION        0x1fff1ff1

#define IAP_CMD_PREPARE     50 
#define IAP_CMD_COPY        51 
#define IAP_CMD_ERASE       52 

#define WRITE_SIZE          4096
#define CPU_CLK_KHZ         100000

#define SECTOR_COUNT        30

typedef void (*IAP)(uint32_t *, uint32_t *);

extern uint32_t _bootstrap_payload_start;
extern uint32_t _bootstrap_payload_end;

__attribute__((section(".ramfunc")))
void iap_bootstrap(void) {
  uint32_t iap_cmd_prepare[5] = {IAP_CMD_PREPARE, 0, 0, 0, 0};
  uint32_t iap_cmd_copy[5] = {IAP_CMD_COPY, 0, 0, WRITE_SIZE, CPU_CLK_KHZ};
  uint32_t iap_cmd_erase[5] = {IAP_CMD_ERASE, 0, 0, CPU_CLK_KHZ, 0};

  uint32_t iap_resp[5];

  uint32_t data_buffer[WRITE_SIZE/4];
  uint32_t dest = 0, sector = 0, i;
  uint32_t *p_payload_next = &_bootstrap_payload_start;
  uint32_t *p_payload_end = &_bootstrap_payload_end;

  IAP iap_entry = (IAP)IAP_LOCATION;

  iap_cmd_copy[2] = (uint32_t)data_buffer;
 
  __disable_irq();
  while (p_payload_next < p_payload_end) {
    for (i = 0; i < sizeof(data_buffer)/sizeof(uint32_t); i++)
      data_buffer[i] = (p_payload_next < p_payload_end) ? *(p_payload_next++) : 0xFFFFFFFF;

    iap_cmd_prepare[1] = iap_cmd_prepare[2] = sector;
    iap_cmd_erase[1] = iap_cmd_erase[2] = sector;
    iap_cmd_copy[1] = dest;

    iap_entry(iap_cmd_prepare, iap_resp);
    iap_entry(iap_cmd_erase, iap_resp);

    iap_entry(iap_cmd_prepare, iap_resp);
    iap_entry(iap_cmd_copy, iap_resp);

    dest += WRITE_SIZE;
    sector++;
  }

  iap_cmd_prepare[1] = sector;
  iap_cmd_erase[1] = sector;
  iap_cmd_prepare[2] = SECTOR_COUNT-1;
  iap_cmd_erase[2] = SECTOR_COUNT-1;

  iap_entry(iap_cmd_prepare, iap_resp);
  iap_entry(iap_cmd_erase, iap_resp);

  NVIC_SystemReset();
}
