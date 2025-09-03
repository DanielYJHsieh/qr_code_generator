#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "qrcodegen.h"

// Function to print QR code to console as ASCII art
void printQR(const uint8_t qrcode[]) {
    int size = qrcodegen_getSize(qrcode);
    int border = 4;
    
    printf("\nQR Code (Version: %d, Size: %dx%d):\n", 
           ((size - 17) / 4), size, size);
    
    // Print top border
    for (int i = 0; i < size + border * 2; i++) {
        printf("██");
    }
    printf("\n");
    
    // Print QR code with borders
    for (int y = -border; y < size + border; y++) {
        // Left border
        for (int i = 0; i < border; i++) {
            printf("██");
        }
        
        // QR code content
        for (int x = 0; x < size; x++) {
            if (y < 0 || y >= size) {
                printf("██");  // Top/bottom border
            } else {
                printf(qrcodegen_getModule(qrcode, x, y) ? "  " : "██");
            }
        }
        
        // Right border
        for (int i = 0; i < border; i++) {
            printf("██");
        }
        printf("\n");
    }
    
    // Print bottom border
    for (int i = 0; i < size + border * 2; i++) {
        printf("██");
    }
    printf("\n");
}

// Function to generate QR code with specific version
int generateQRCode(const char* text, int version, enum qrcodegen_Ecc ecl) {
    // Calculate buffer size for the specified version
    size_t bufferSize = qrcodegen_BUFFER_LEN_FOR_VERSION(version);
    
    // Allocate buffers
    uint8_t* qrcode = malloc(bufferSize);
    uint8_t* tempBuffer = malloc(bufferSize);
    
    if (qrcode == NULL || tempBuffer == NULL) {
        printf("Error: Memory allocation failed\n");
        free(qrcode);
        free(tempBuffer);
        return 0;
    }
    
    // Generate QR code
    bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode, ecl, 
                                   version, version, qrcodegen_Mask_AUTO, true);
    
    if (!ok) {
        printf("Error: Failed to generate QR code\n");
        free(qrcode);
        free(tempBuffer);
        return 0;
    }
    
    int actualVersion = (qrcodegen_getSize(qrcode) - 17) / 4;
    printf("Generated QR Code - Requested Version: %d, Actual Version: %d\n", 
           version, actualVersion);
    
    // Print QR code to console
    printQR(qrcode);
    
    // Save as text representation
    char filename[256];
    snprintf(filename, sizeof(filename), "qrcode_v%d.txt", actualVersion);
    FILE* txtFile = fopen(filename, "w");
    if (txtFile != NULL) {
        int size = qrcodegen_getSize(qrcode);
        
        // Output QR code matrix without title
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                fprintf(txtFile, qrcodegen_getModule(qrcode, x, y) ? "██" : "  ");
            }
            fprintf(txtFile, "\n");
        }
        
        // Output expected scan result at the end
        fprintf(txtFile, "\n%s\n", text);
        fclose(txtFile);
        printf("Text representation saved as: %s\n", filename);
    }
    
    free(qrcode);
    free(tempBuffer);
    return 1;
}

int main() {
    printf("=== QR Code Generator ===\n");
    printf("Using Nayuki's QR-Code-generator library\n\n");
    
    // Device information variables
    const char* imei = "300078990002985";
    const char* serialNumber = "BCFVNDA4B0100074";
    const char* dateCode = "2025.9.2";
    
    // Generate QR code for device information
    printf("Device Information QR Code Generation:\n");
    printf("======================================\n");
    
    // Format device information string
    char deviceInfo[256];
    snprintf(deviceInfo, sizeof(deviceInfo), "IMEI:%s\nS/N:%s\nDate code:%s", 
             imei, serialNumber, dateCode);
    printf("Generating QR code for device information...\n");
    printf("Text: \"%s\"\n", deviceInfo);
    printf("Text length: %zu characters\n\n", strlen(deviceInfo));
    
    generateQRCode(deviceInfo, 5, qrcodegen_Ecc_MEDIUM);
    
    printf("\nQR Code generation completed!\n");
    printf("Check the 'output/' directory for generated files.\n");
    return 0;
}
