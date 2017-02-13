ARDUINO=/Applications/Arduino_164.app/Contents/MacOS
IBPV2='/Users/kevin/working/Ham Radio/IBPV2_VE8AT_version/beacon'
VERSION=2_7f
VERSION_P="2.7f"
# BUILD_FLAGS=--verify -v
BUILD_FLAGS=--verify
PREF_BUILD_PATH=--pref build.path=$(IBPV2)/build

beacon:		beacon.ino
	$(ARDUINO)/Arduino $(PREF_BUILD_PATH) $(BUILD_FLAGS) $(IBPV2)/beacon.ino
	cp $(IBPV2)/build/beacon.cpp.hex ../release/beacon_$(VERSION).hex
	md5 -q ../release/beacon_$(VERSION).hex >../release/md5sum_$(VERSION)
	cat ../release/md5sum_$(VERSION)

clean:
	rm build/*.o
	rm ../release/beacon_$(VERSION).hex
	rm ../release/md5sum_$(VERSION)

test:
	@echo "Test"
	@echo $(PREF_BUILD_PATH)

