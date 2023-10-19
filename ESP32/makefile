# add the below text in arduino.json file
# "output": "Build", or "output": "C:\\Users\\Tran_Luyen\\Desktop\\Build"
PORT = 4
# FILENAME = $(wildcard *.ino)
FILENAME = $(lastword $(subst /, ,$(CURDIR))).ino
BAUD = 921600
FlashSize = 4MB
CC = esptool

packages_dir := C:/Users/$(USERNAME)/AppData/Local/Arduino15/packages

# ESP
ESP_TOOL_CC := $(wildcard $(packages_dir)/esp32/tools/esptool_py/**/esptool.exe)
BOOT_APP0 := $(wildcard $(packages_dir)/esp32/hardware/esp32/**/tools/partitions/boot_app0.bin)
# ota
ESP_OTA_CC := $(wildcard $(packages_dir)/esp32/**/**/**/**/espota.exe)
OTA_IP := 192.168.2.49
OTA_PORT := 3232
# default OTA_PORT = 3232

test_cc = $(filter %/, $(wildcard $(CURDIR)/*/))

FILEPATH = $(CURDIR)/Build
# FILEPATH = C:\Users\Tran_Luyen\Desktop\Build
# arduino
avrdude_dir:= $(filter %/, $(wildcard $(packages_dir)/arduino/tools/avrdude/*/))
avrdude:= $(avrdude_dir)/etc/avrdude.conf
ARDUINOCC = $(avrdude_dir) -C $(avrdude)

default: esp32s3

test:
	echo $(avrdude)

esp_ota:
	$(ESP_OTA_CC) -r -i $(OTA_IP) -p $(OTA_PORT) --auth= -f "$(FILEPATH)/$(FILENAME).bin"

esp32:
	$(ESP_TOOL_CC) --chip esp32 --port COM$(PORT) --baud $(BAUD) --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size $(FlashSize) 0x1000 "$(FILEPATH)/$(FILENAME).bootloader.bin" 0x8000 "$(FILEPATH)/$(FILENAME).partitions.bin" 0xe000 "$(BOOT_APP0)" 0x10000 "$(FILEPATH)/$(FILENAME).bin"
esp32_app:
	$(ESP_TOOL_CC) --chip esp32 --port COM$(PORT) --baud $(BAUD) --before default_reset --after hard_reset write_flash -z 0x10000 "$(FILEPATH)/$(FILENAME).bin"

esp32c3:
	$(ESP_TOOL_CC) --chip esp32c3 --port COM$(PORT) --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size $(FlashSize) 0x0 "$(FILEPATH)/$(FILENAME).bootloader.bin" 0x8000 "$(FILEPATH)/$(FILENAME).partitions.bin" 0xe000 "$(BOOT_APP0)" 0x10000 "$(FILEPATH)/$(FILENAME).bin" 

esp32s3:
	$(ESP_TOOL_CC) --chip esp32s3 --port COM$(PORT) --baud $(BAUD) --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size $(FlashSize) 0x0 "$(FILEPATH)/$(FILENAME).bootloader.bin" 0x8000 "$(FILEPATH)/$(FILENAME).partitions.bin" 0xe000 "$(BOOT_APP0)" 0x10000 "$(FILEPATH)/$(FILENAME).bin"


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
	echo $(ARDUINOCC) -v -patmega328p -carduino -PCOM$(PORT) -b115200 -D -Uflash:w:"$(FILEPATH)/$(FILENAME).hex":i 
2560:
	$(ARDUINOCC) -v -patmega2560 -cwiring -PCOM$(PORT) -b$(BAUD) -D -Uflash:w:"$(FILEPATH)/$(FILENAME).hex":i

clean:
	rm -r -f Build