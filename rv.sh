cmake -B build -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake
cmake --build build

avr-objcopy -O ihex -R .eeprom build/rvr out/rvr.hex

avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyUSB0 -b 115200 -U flash:w:out/rvr.hex
