
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


upload:	reset raw_upload

reset:	
	./leo_reset.py
	stty -F /dev/ttyACM0 57600


raw_upload:
	avrdude -F -v -pm168 -cstk500v1 -P/dev/ttyACM0 -b19200 -D -Uflash:w:"beacon.cpp.hex":i

