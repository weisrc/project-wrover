mkdir -p Debug
avra -o Debug/avr.hex \
     -l Debug/avr.lss \
     -e Debug/avr.eep \
     -m Debug/avr.map \
     -d Debug/avr.obj \
     main.asm