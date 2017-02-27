ARDUINO=/Applications/Arduino_164.app/Contents/MacOS
IBPV2='/Users/kevin/working/Ham Radio/IBPV2_VE8AT_version/util/load_eeprom'
VERSION=2_8b
VERSION_P="2.8b"
# BUILD_FLAGS=--verify -v
BUILD_FLAGS=--verify
PREF_BUILD_PATH=--pref build.path=$(IBPV2)/build


load_eeprom:		load_eeprom.ino
	$(ARDUINO)/Arduino $(PREF_BUILD_PATH) $(BUILD_FLAGS) $(IBPV2)/load_eeprom.ino
	cp $(IBPV2)/build/load_eeprom.cpp.hex ../../release/load_eeprom.hex

clean:
	rm build/*.o
	rm ../../release/load_eeprom_$(VERSION).hex

