export CC      = gcc
export CFLAGS  = -Wall -pedantic
export LDFLAGS =
export EXEC    = kompressor

export BUILD_DIR  = build
export SOURCE_DIR = src
export TEST_DIR   = test

INSTALL_PATH = /usr/local/bin

.PHONY: test build install uninstall clean

all: build

test:
	make -C $(SOURCE_DIR)
	make -C $(TEST_DIR)

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
	make -C $(TEST_DIR) clean
	rm -f $(BUILD_DIR)$(EXEC)
