# add the below text in arduino.json file
# "output": "Build", or "output": "C:\\Users\\Tran_Luyen\\Desktop\\Build"
PORT = 25
# FILENAME = $(wildcard *.ino)
FILENAME = $(lastword $(subst /, ,$(CURDIR))).ino
BAUD = 921600
FlashSize = 4MB
CC = esptool

FILEPATH = $(CURDIR)/Build
# FILEPATH = C:\Users\Tran_Luyen\Desktop\Build

ARDUINOCC = C:\Users\$(USERNAME)\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/bin/avrdude -CC:\Users\$(USERNAME)\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/etc/avrdude.conf

default: esp32s3

esp32:
	$(CC) --chip esp32 --port COM$(PORT) --baud $(BAUD) --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size $(FlashSize) 0x1000 "$(FILEPATH)/$(FILENAME).bootloader.bin" 0x8000 "$(FILEPATH)/$(FILENAME).partitions.bin" 0xe000 C:\Users\$(USERNAME)\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.6/tools/partitions/boot_app0.bin 0x10000 "$(FILEPATH)/$(FILENAME).bin"
esp32_app:
	$(CC) --chip esp32 --port COM$(PORT) --baud $(BAUD) --before default_reset --after hard_reset write_flash -z 0x10000 "$(FILEPATH)/$(FILENAME).bin"

esp32c3:
	$(CC) --chip esp32c3 --port COM$(PORT) --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size $(FlashSize) 0x0 "$(FILEPATH)/$(FILENAME).bootloader.bin" 0x8000 "$(FILEPATH)/$(FILENAME).partitions.bin" 0xe000 C:\Users\$(USERNAME)\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.6/tools/partitions/boot_app0.bin 0x10000 "$(FILEPATH)/$(FILENAME).bin" 

esp32s3:
	$(CC) --chip esp32s3 --port COM$(PORT) --baud $(BAUD) --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size $(FlashSize) 0x0 "$(FILEPATH)/$(FILENAME).bootloader.bin" 0x8000 "$(FILEPATH)/$(FILENAME).partitions.bin" 0xe000 "C:\Users\$(USERNAME)\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.6/tools/partitions/boot_app0.bin" 0x10000 "$(FILEPATH)/$(FILENAME).bin"


esp8266_wifi: esp8266_erase_wifi esp8266
esp8266_flash: esp8266_erase_flash esp8266

# app
esp8266:
	$(CC) --chip esp8266 --port COM$(PORT) --baud $(BAUD) --before default_reset --after hard_reset write_flash 0x0 "$(FILEPATH)/$(FILENAME).bin"
#  erase wifi
esp8266_erase_wifi:
	$(CC) --chip esp8266 --port COM$(PORT) --baud $(BAUD) erase_region 0x3FC000 0x4000
# erase flash
esp8266_erase_flash:
	$(CC) --chip esp8266 --port COM$(PORT) --baud $(BAUD) erase_flash

# uno fuse : FF DE FD FF
pro_mini:
nano_old:
	$(ARDUINOCC) -v -patmega328p -carduino -PCOM$(PORT) -b57600 -D -Uflash:w:"$(FILEPATH)/$(FILENAME).hex":i 
nano:
uno:
	$(ARDUINOCC) -v -patmega328p -carduino -PCOM$(PORT) -b115200 -D -Uflash:w:"$(FILEPATH)/$(FILENAME).hex":i 
2560:
	$(ARDUINOCC) -v -patmega2560 -cwiring -PCOM$(PORT) -b$(BAUD) -D -Uflash:w:"$(FILEPATH)/$(FILENAME).hex":i

clean:
	rm -r -f Build