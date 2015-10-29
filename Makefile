PREF := avr-
CC := $(PREF)gcc
OBJCOPY := $(PREF)objcopy
SIZE := $(PREF)size

MMCU:=attiny4313
PROG_MK:=t4313

INC_DIRS := include
INC_DIRS_OPT := $(addprefix -I, $(INC_DIRS))

DEFINED := F_CPU=4000000ULL
DEFINE_OPT := $(addprefix -D, $(DEFINED))

BIN_DIR := bin
PROGNAME := $(notdir $(CURDIR))

TESTS_DIR := tests
SRC_DIRS := src $(TESTS_DIR)

OBJ_DIR := obj

CC_OPTS :=  -MD $(INC_DIRS_OPT) $(DEFINE_OPT) -O2 -mmcu=$(MMCU) -Wall
LINK_OPTS := -mmcu=$(MMCU) -Os -Wl,-Map=$(OBJ_DIR)/mapfile.map

OBJFILES := kpd-al-lib.o main.o service.o regeneration.o
OBJFILES := $(addprefix $(OBJ_DIR)/, $(OBJFILES))

TEST_FILES := $(addprefix $(BIN_DIR)/test_, $(addsuffix .hex, $(basename $(notdir $(wildcard $(TESTS_DIR)/*.c)))))

target:$(BIN_DIR)/$(PROGNAME).hex $(BIN_DIR)/$(PROGNAME).eep
	@echo
	@echo "Success!"
	@$(SIZE) $<

all:tst target


$(BIN_DIR)/%.hex: $(OBJ_DIR)/%.elf
	@mkdir -p $(BIN_DIR)
	$(OBJCOPY) -O ihex -j .text -j .data $< $@

$(BIN_DIR)/%.eep: $(OBJ_DIR)/%.elf
	@mkdir -p $(BIN_DIR)
	#$(OBJCOPY) -O ihex -j .eeprom $< $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 --no-change-warnings -O ihex $< $@ || exit 0


prog:$(BIN_DIR)/$(PROGNAME).hex
	avrdude -p $(PROG_MK) -c stk500 -U flash:w:$<:i

prog_ee:$(BIN_DIR)/$(PROGNAME).eep
	avrdude -p $(PROG_MK) -c stk500 -U eeprom:w:$<:i

tst:$(TEST_FILES)


$(OBJ_DIR)/test_%.elf:$(OBJ_DIR)/%.o $(OBJ_DIR)/kpd-al-lib.o
	$(CC) $(LINK_OPTS) $^ -o $@

$(OBJ_DIR)/$(PROGNAME).elf: $(OBJFILES)
	$(CC) $(LINK_OPTS) $(OBJFILES) -o $@

VPATH := $(SRC_DIRS) 
$(OBJ_DIR)/%.o: $(OBJ_DIR)/%.s
	$(CC) -c $(CC_OPTS) $< -o $@
$(OBJ_DIR)/%.s: %.c Makefile
	@mkdir -p $(OBJ_DIR)
	$(CC) -S $(CC_OPTS) $< -o $@

include $(wildcard $(OBJ_DIR)/*.d)


clean:
	rm -fr $(OBJ_DIR)/* $(BIN_DIR)/* $(addsuffix /*~,$(INC_DIRS)) $(addsuffix /*~,$(SRC_DIRS)) *~


.PHONY: all target tst
.PRECIOUS: $(OBJ_DIR)/test_%.elf $(OBJ_DIR)/$(PROGNAME).elf
