export CC      = gcc
export CFLAGS  = -Wall -pedantic
export LDFLAGS =
export EXEC    = acdc

BUILD_DIR    = build
SOURCE_DIR   = src
INSTALL_PATH = /usr/local/bin

.PHONY: clean build install uninstall

all: build

build:
	mkdir -p $(BUILD_DIR)
	make -C $(SOURCE_DIR)
	mv $(SOURCE_DIR)/$(EXEC) build

install: build
	mv $(BUILD_DIR)$(EXEC) $(INSTALL_PATH)

uninstall:
	rm -f $(INSTALL_PATH)/$(EXEC)

clean:
	make -C $(SOURCE_DIR) clean
	rm -f $(BUILD_DIR)$(EXEC)
