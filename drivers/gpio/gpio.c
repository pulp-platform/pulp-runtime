#include "gpio.h"
#include "common/bitfield.h"
#include "gpio_hal.h"
#include "pulp.h"
#include "hal/utils.h"

void gpio_init(gpios_t* handle) {
  memset(handle, 0, sizeof(gpios_t));
  handle->base_addr = ARCHI_GPIO_ADDR;
}


void gpio_configure(gpios_t* handle, uint32_t number, uint32_t direction){
  // Now configure the GPIO peripheral
  uint32_t addr = handle->base_addr + GPIO_MODE_REG_OFFSET_BASE + number/16*4;
  uint32_t reg = archi_read32(addr);
  bitfield_field32_t field;
  field.mask = 0x3;
  field.index = number%16*2;
  reg = bitfield_field32_write(reg, field, (direction == GPIO_DIRECTION_INPUT)? GPIO_MODE_REG_VALUE_INPUT_ONLY: GPIO_MODE_REG_VALUE_OUTPUT_ACTIVE);
  archi_write32(addr, reg);
  // Enable/Disable input sampling
  addr = handle->base_addr + GPIO_EN_INPUT_REG_OFFSET_BASE + number/32*4;
  reg = archi_read32(addr);
  reg = bitfield_bit32_write(reg, number%32, direction == GPIO_DIRECTION_INPUT);
  archi_write32(addr, reg);
  hal_compiler_barrier();
}
