#include "internal.h"

#define SECTOR_SIZE         2048
#define SECTOR_COUNT        128

extern uint16_t _bootstrap_payload_start;
extern uint16_t _bootstrap_payload_end;

__attribute__((section(".ramfunc")))
void iap_bootstrap(void) {
  uint16_t *p_dest = (uint16_t *)FLASH_BASE;
  uint16_t *p_payload_next = &_bootstrap_payload_start;
  uint16_t *p_payload_end = &_bootstrap_payload_end;
  uint16_t sector_next = 0;

  RCC->AHBENR |= RCC_AHBENR_FLITFEN;
  RCC->AHBENR;

  __disable_irq();

  if (FLASH->CR & FLASH_CR_LOCK) {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
  }

  while (FLASH->SR & FLASH_SR_BSY)
    ;

  FLASH->CR = FLASH_CR_PG;
  while (p_payload_next < p_payload_end) {
    if ((((uint32_t)p_dest) & (SECTOR_SIZE-1)) == 0) { // sector start
      FLASH->CR = FLASH_CR_PER;
      FLASH->AR = (uint32_t)p_dest;
      FLASH->CR = FLASH_CR_PER | FLASH_CR_STRT;
      while (FLASH->SR & FLASH_SR_BSY)
        ;
      FLASH->CR = FLASH_CR_PG;
      sector_next++;
    }

    *p_dest++ = *p_payload_next++;
    while (FLASH->SR & FLASH_SR_BSY)
      ;
  }

  while (sector_next < SECTOR_COUNT) {
    FLASH->CR = FLASH_CR_PER;
    FLASH->AR = FLASH_BASE + SECTOR_SIZE*sector_next;
    FLASH->CR = FLASH_CR_PER | FLASH_CR_STRT;
    while (FLASH->SR & FLASH_SR_BSY)
      ;
    sector_next++;
  }

  NVIC_SystemReset();
}
