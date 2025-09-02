CC=gcc
CFLAGS=-Wall -O2 -std=c99
SRC_DIR=src
OUTPUT_DIR=output
TARGET=$(OUTPUT_DIR)/qrcode.exe
SIMPLE_TARGET=$(SRC_DIR)/simple_test.exe

# Source files
SOURCES=$(SRC_DIR)/main.c $(SRC_DIR)/qrcodegen.c
SIMPLE_SOURCES=$(SRC_DIR)/simple_test.c $(SRC_DIR)/qrcodegen.c
HEADERS=$(SRC_DIR)/qrcodegen.h

.PHONY: all clean run test help

all: $(TARGET)

# Main QR code generator
$(TARGET): $(SOURCES) $(HEADERS) | $(OUTPUT_DIR)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

# Simple test program
$(SIMPLE_TARGET): $(SIMPLE_SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SIMPLE_SOURCES) -o $(SIMPLE_TARGET)

# Create output directory
$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

# Run the main program
run: $(TARGET)
	cd $(OUTPUT_DIR) && ./qrcode.exe

# Run simple test
test: $(SIMPLE_TARGET)
	$(SIMPLE_TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET)
	rm -f $(SIMPLE_TARGET)
	rm -f $(OUTPUT_DIR)/*.pbm
	rm -f $(OUTPUT_DIR)/*.txt

# Show help
help:
	@echo Available targets:
	@echo   all     - Build the main QR code generator
	@echo   test    - Build and run simple test
	@echo   run     - Run the main QR code generator
	@echo   clean   - Clean build artifacts
	@echo   help    - Show this help message
