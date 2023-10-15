cmake_minimum_required(VERSION 3.15)

project(pr LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.avr.core
	VERSION 1.0
	MCU ATMEGA328P
	HWREGISTER_LIST_FILE DeviceDefinitions/atmega328p.xml
	DISABLE_GNU_EXTENSIONS)

add_bsp_based_executable(
	NAME pr
	SOURCES Main.cpp
	GENERATE_BIN
	GENERATE_MAP
	GENERATE_HEX)
