#include "gpio.h"
#include "common/bitfield.h"
#include "gpio_hal.h"
#include "pulp.h"


void gpio_configure(uint32_t number, uint32_t direction){
  // Now configure the GPIO peripheral
  uint32_t addr = ARCHI_GPIO_ADDR + GPIO_MODE_REG_OFFSET_BASE + number/16*4;
  uint32_t reg = archi_read32(addr);
  bitfield_field32_t field;
  field.mask = 0x3;
  field.index = number%16*2;
  reg = bitfield_field32_write(reg, field, (direction == GPIO_DIRECTION_INPUT)? GPIO_MODE_REG_VALUE_INPUT_ONLY: GPIO_MODE_REG_VALUE_OUTPUT_ACTIVE);
  archi_write32(addr, reg);
  // Enable/Disable input sampling
  addr = ARCHI_GPIO_ADDR + GPIO_EN_INPUT_REG_OFFSET_BASE + number/32*4;
  reg = archi_read32(addr);
  reg = bitfield_bit32_write(reg, number%32, direction == SIRACUSA_GPIO_DIRECTION_INPUT);
  archi_write32(addr, reg);
  hal_compiler_barrier();
}


void gpio_set(uint32_t number, uint32_t value){
  uint32_t addr;
  if (value == 1) {
    addr = ARCHI_GPIO_ADDR + GPIO_SET_REG_OFFSET_BASE + number/32*4;
  } else {
    addr = SIRACUSA_GPIO_BASE_ADDR + GPIO_CLEAR_REG_OFFSET_BASE + number/32*4;
  }
  archi_write32(addr, 1<<(number%32));
  hal_compiler_barrier();
}

uint32_t gpio_get(uint32_t number){
  uint32_t addr = SIRACUSA_GPIO_BASE_ADDR + GPIO_GPIO_IN_0_REG_OFFSET + number/32*4;
  uint32_t reg = archi_read32(addr);
  return bitfield_bit32_read(reg, number%32);
  hal_compiler_barrier();
}
