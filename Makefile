
DIR=$(shell pwd)
AINCS = -I.\
       -I$(DIR) \
       -I$(DIR)/hardware \
       -I$(DIR)/hardware/arduino \
       -I$(DIR)/hardware/arduino/cores \
       -I$(DIR)/hardware/arduino/cores/arduino \
       -I$(DIR)/hardware/arduino/variants \
       -I$(DIR)/hardware/arduino/variants/sunxi \
       -I$(DIR)/libraries \
       -I$(DIR)/libraries/Serial \
       -I$(DIR)/libraries/SPI \
	   -I$(DIR)/libraries/Wire \
	   -I$(DIR)/libraries/LiquidCrystal 


PLATFORM = PC

ifeq ($(PLATFORM),ARM)
	CC = arm-linux-gnueabihf-g++
	EXENAME = RT_Video_Arm
else
	CC = g++
	EXENAME = RT_Video
endif

#PATH
SRC_PATH = codes/scr/
INC_PATH = codes/inc/
BIN_PATH = bin/
OBJ_PATH = obj/

#FLAGS
CFLAGS = -pipe -O2 
LDFLAGS = -pipe -O2 
INC_C = -lx264 -lm -lpthread -ldl -I./$(INC_PATH) -I./codes/inc/rtp 

ifeq ($(PLATFORM),ARM)
	LIBS = -lx264 -lm -lpthread -ljrtp -ljthread  -ldl -larduino
	DEP_LIBS = -L./libs/libarm
	EXENAME = rva
else
	LIBS = -lx264 -lm -lpthread -ljrtp -ljthread  -ldl
	DEP_LIBS = -L./libs/libpc64
	EXENAME = rv
endif

OBJECTS = $(OBJ_PATH)main.o $(OBJ_PATH)option.o $(OBJ_PATH)capture.o \
			$(OBJ_PATH)trans.o $(OBJ_PATH)encoder.o $(OBJ_PATH)rtp.o


all: $(BIN_PATH)$(EXENAME)

$(BIN_PATH)$(EXENAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS) $(DEP_LIBS)
ifeq ($(PLATFORM),ARM)
	scp bin/rva ubuntu@192.168.1.106:~/Real_Time_Video/
endif

$(OBJ_PATH)main.o: $(SRC_PATH)main.cpp
	$(CC) $(CFLAGS) -c $< $(INC_C) $(AINCS) -o $@

$(OBJ_PATH)option.o: $(SRC_PATH)option.cpp
	$(CC) $(CFLAGS) -c $< $(INC_C) -o $@

$(OBJ_PATH)capture.o: $(SRC_PATH)capture.cpp
	$(CC) $(CFLAGS) -c $< $(INC_C) -o $@

$(OBJ_PATH)trans.o: $(SRC_PATH)trans.cpp
	$(CC) $(CFLAGS) -c $< $(INC_C) -o $@

$(OBJ_PATH)encoder.o: $(SRC_PATH)encoder.cpp
	$(CC) $(CFLAGS) -c $< $(INC_C) -o $@

$(OBJ_PATH)rtp.o: $(SRC_PATH)rtp.cpp
	$(CC) $(CFLAGS) -c $< $(INC_C) -o $@


clean:
	rm -f $(OBJ_PATH)*.o $(BIN_PATH)$(EXENAME)

.PHONY: all clean
