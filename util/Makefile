#  command line Make for the Arduino beacon code
#     Program to cause a reset to a Leonardo board
#     invoke avrdude to load the hex file.
#
#     lot of very specific paths in here, due to the way the various programs are written
#   
#    on hte target laod linux box - make leo_reset, make upload
#   Need to copy the hex file over to the machine to load with
#
ARDUINO=/Applications/Arduino_164.app/Contents/MacOS
IBPV2='/Users/kevin/working/Ham Radio/IBPV2/util'
# BUILD_FLAGS=--verify -v
BUILD_FLAGS=--verify 
PREF_BUILD_PATH=--pref build.path=$(IBPV2)/build
PORT = /dev/ttyACM0

test:
	@echo "Test"
	@echo $(PREF_BUILD_PATH)

# Blink:		./Blink/Blink.ino
BlinkX:		
	$(ARDUINO)/Arduino $(PREF_BUILD_PATH) $(BUILD_FLAGS) $(IBPV2)/Blink/Blink.ino


upload:	reset raw_upload

reset:	
	./leo_reset $(PORT)
	sleep 5
	stty -F $(PORT) 57600


raw_upload:
	avrdude -F -v -p atmega32u4 -c avr109 -P/dev/ttyACM0 -b57600 -D -U flash:w:"build/Blink.cpp.hex":i

leo_reset:	leo_reset.c
	gcc -o leo_reset leo_reset.c
