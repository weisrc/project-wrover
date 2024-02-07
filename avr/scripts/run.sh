./scripts/build.sh
avrdude -c avrispmkII -p m8515 -U flash:w:Debug/avr.hex
