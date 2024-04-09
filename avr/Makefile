.PHONY: build run monitor clean

Debug/avr.hex: main.asm *.inc
	mkdir -p Debug
	avra -o Debug/avr.hex \
     -l Debug/avr.lss \
     -e Debug/avr.eep \
     -m Debug/avr.map \
     -d Debug/avr.obj \
   	 main.asm

build: Debug/avr.hex

run: build
	avrdude -c avrispmkII -p m8515 -U flash:w:Debug/avr.hex

monitor:
	cu -l /dev/ttyUSB0 -s 2400

clean:
	rm -rf Debug
