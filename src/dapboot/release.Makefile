
ifneq ($(V),1)
Q              := @
NULL           := 2>/dev/null
MAKE           := $(MAKE) --no-print-directory
endif
export V

BUILD_DIR      ?= ./build

all: dapboot-bluepill.bin \
     dapboot-maplemini.bin \
     dapboot-stlink.bin \
     dapboot-bluepill-high.bin \
     dapboot-maplemini-high.bin \
     dapboot-stlink-high.bin \
     dapboot-bluepill-high-128.bin \
     dapboot-maplemini-high-128.bin \
     dapboot-stlink-high-128.bin \

clean:
	$(Q)$(RM) $(BUILD_DIR)/*.bin
	$(Q)$(MAKE) -C src/ clean

.PHONY = all clean

$(BUILD_DIR):
	$(Q)mkdir -p $(BUILD_DIR)

dapboot-bluepill.bin: | $(BUILD_DIR)
	@printf "  BUILD $(@)\n"
	$(Q)$(MAKE) TARGET=BLUEPILL -C src/ clean
	$(Q)$(MAKE) TARGET=BLUEPILL -C src/
	$(Q)cp src/dapboot.bin $(BUILD_DIR)/$(@)

dapboot-stlink.bin: | $(BUILD_DIR)
	@printf "  BUILD $(@)\n"
	$(Q)$(MAKE) TARGET=STLINK -C src/ clean
	$(Q)$(MAKE) TARGET=STLINK -C src/
	$(Q)cp src/dapboot.bin $(BUILD_DIR)/$(@)

dapboot-maplemini.bin: | $(BUILD_DIR)
	@printf "  BUILD $(@)\n"
	$(Q)$(MAKE) TARGET=MAPLEMINI -C src/ clean
	$(Q)$(MAKE) TARGET=MAPLEMINI -C src/
	$(Q)cp src/dapboot.bin $(BUILD_DIR)/$(@)

dapboot-bluepill-high.bin: | $(BUILD_DIR)
	@printf "  BUILD $(@)\n"
	$(Q)$(MAKE) TARGET=BLUEPILL_HIGH -C src/ clean
	$(Q)$(MAKE) TARGET=BLUEPILL_HIGH -C src/
	$(Q)cp src/dapboot.bin $(BUILD_DIR)/$(@)

dapboot-stlink-high.bin: | $(BUILD_DIR)
	@printf "  BUILD $(@)\n"
	$(Q)$(MAKE) TARGET=STLINK_HIGH -C src/ clean
	$(Q)$(MAKE) TARGET=STLINK_HIGH -C src/
	$(Q)cp src/dapboot.bin $(BUILD_DIR)/$(@)

dapboot-maplemini-high.bin: | $(BUILD_DIR)
	@printf "  BUILD $(@)\n"
	$(Q)$(MAKE) TARGET=MAPLEMINI_HIGH -C src/ clean
	$(Q)$(MAKE) TARGET=MAPLEMINI_HIGH -C src/
	$(Q)cp src/dapboot.bin $(BUILD_DIR)/$(@)

dapboot-bluepill-high-128.bin: | $(BUILD_DIR)
	@printf "  BUILD $(@)\n"
	$(Q)$(MAKE) TARGET=BLUEPILL_HIGH_128 -C src/ clean
	$(Q)$(MAKE) TARGET=BLUEPILL_HIGH_128 -C src/
	$(Q)cp src/dapboot.bin $(BUILD_DIR)/$(@)

dapboot-stlink-high-128.bin: | $(BUILD_DIR)
	@printf "  BUILD $(@)\n"
	$(Q)$(MAKE) TARGET=STLINK_HIGH_128 -C src/ clean
	$(Q)$(MAKE) TARGET=STLINK_HIGH_128 -C src/
	$(Q)cp src/dapboot.bin $(BUILD_DIR)/$(@)

dapboot-maplemini-high-128.bin: | $(BUILD_DIR)
	@printf "  BUILD $(@)\n"
	$(Q)$(MAKE) TARGET=MAPLEMINI_HIGH_128 -C src/ clean
	$(Q)$(MAKE) TARGET=MAPLEMINI_HIGH_128 -C src/
	$(Q)cp src/dapboot.bin $(BUILD_DIR)/$(@)
