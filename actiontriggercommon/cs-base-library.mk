QMAKE = $(QNX_HOST)/usr/bin/qmake
I18N_DIR ?= ./translations

ifdef NUMBER_OF_PROCESSORS
  MAKEFLAGS += -j${NUMBER_OF_PROCESSORS}
endif

default: Device-Release

.PHONY: FORCE

Makefile-all: arm/Makefile arm-p/Makefile x86/Makefile translations/Makefile

arm/Makefile: $(QMAKE_TARGET).pro
	@mkdir -p arm
	cd arm && $(QMAKE) -spec blackberry-armv7le-qcc ../$(QMAKE_TARGET).pro CONFIG+=debug_and_release CONFIG+=device

arm-p/Makefile: $(QMAKE_TARGET).pro
	@mkdir -p arm-p
	cd arm-p && $(QMAKE) -spec blackberry-armv7le-qcc ../$(QMAKE_TARGET).pro CONFIG+=debug_and_release CONFIG+=device CONFIG+=profile

x86/Makefile: $(QMAKE_TARGET).pro
	@mkdir -p x86
	cd x86 && $(QMAKE) -spec blackberry-x86-qcc ../$(QMAKE_TARGET).pro CONFIG+=debug_and_release CONFIG+=simulator

Device-Release: arm/Makefile translate FORCE
	$(MAKE) -C ./arm -f Makefile release

Device-Debug: arm/Makefile translate FORCE
	$(MAKE) -C ./arm -f Makefile debug

Device-Profile: arm-p/Makefile translate FORCE
	$(MAKE) -C ./arm-p -f Makefile debug

Simulator-Debug: x86/Makefile translate FORCE
	$(MAKE) -C ./x86 -f Makefile debug

translate: FORCE
	$(MAKE) -C $(I18N_DIR) -f Makefile update
	$(MAKE) -C $(I18N_DIR) -f Makefile release

clean: FORCE
	rm -rf arm arm-p x86 $(I18N_DIR)/qm
