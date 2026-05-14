FQBN = esp32:esp32:m5stack_stickc_plus2
PORT = /dev/ttyACM0

compile:
	arduino-cli compile --fqbn $(FQBN) .

upload:
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) .

build:
	arduino-cli compile --fqbn $(FQBN) . && arduino-cli upload -p $(PORT) --fqbn $(FQBN) .
