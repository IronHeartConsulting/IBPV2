
ARDUINO=/Applications/Arduino_164.app/Contents/MacOS
IBPV2='/Users/kevin/working/Ham Radio/IBPV2/beacon'
# BUILD_FLAGS=--verify -v
BUILD_FLAGS=--verify 
PREF_BUILD_PATH=--pref build.path=$(IBPV2)/build

test:
	@echo "Test"
	@echo $(PREF_BUILD_PATH)

beacon:		beacon.ino
	$(ARDUINO)/Arduino $(PREF_BUILD_PATH) $(BUILD_FLAGS) $(IBPV2)/beacon.ino
	cp $(IBPV2)/build/beacon.cpp.hex ../release/beacon.hex

