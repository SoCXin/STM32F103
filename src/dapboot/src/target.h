#ifndef TARGET_H_INCLUDED
#define TARGET_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>
#include <libopencm3/usb/usbd.h>

extern void target_clock_setup(void);
extern void target_gpio_setup(void);
extern const usbd_driver* target_usb_init(void);
extern bool target_get_force_bootloader(void);
extern void target_get_serial_number(char* dest, size_t max_chars);
extern size_t target_get_max_firmware_size(void);
extern void target_log(const char* str);
extern void target_relocate_vector_table(void);
extern void target_manifest_app(void);
extern void target_flash_unlock(void);
extern void target_flash_lock(void);
extern bool target_flash_program_array(uint16_t* dest, const uint16_t* data, size_t half_word_count);

extern void target_pre_main(void);
extern size_t target_get_timeout(void);

#endif
