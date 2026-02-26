#include "HMS_MCP_CAN.h"

// For generic Arduino platform or ESP32-Arduino
#if defined(HMS_MCP_CAN_PLATFORM_ARDUINO) || defined(HMS_MCP_CAN_PLATFORM_ESP32_ARDUINO)

HMS_MCP_CAN_StatusTypeDef HMS_MCP_CAN::readCANMessage(HMS_MCP_CAN_MessageTypeDef &message) {
// ... existing code ...
    if (mcpCan->checkReceive() == CAN_MSGAVAIL) {
        long unsigned int canId;
        byte len;
        byte buf[8];
        mcpCan->readMsgBuf(&canId, &len, buf);

        // Format data as hex string for logging
        char dataStr[3 * 8 + 1] = {0};
        char *ptr = dataStr;
        for (byte i = 0; i < len; i++) {
            int n = snprintf(ptr, sizeof(dataStr) - (ptr - dataStr), "0x%02X%s", buf[i], (i < len - 1) ? ", " : "");
            if (n < 0 || n >= (int)(sizeof(dataStr) - (ptr - dataStr))) break;
            ptr += n;
        }
        HMS_MCP_CAN_LOGGER(info, "Received CAN message - ID: 0x%lX, Length: %d, Data: [%s]", canId, len, dataStr);

        message.id = canId;
        message.length = len;
        memcpy(message.data, buf, len);
        return HMS_MCP_CAN_OK;
    }
    return HMS_MCP_CAN_ERROR;
}

HMS_MCP_CAN_StatusTypeDef HMS_MCP_CAN::init() {
    HMS_MCP_CAN_LOGGER(info, "Creating SPI instance...");
    spi = new SPIClass(HSPI);
    
    // Explicitly configure CS pin as OUTPUT to avoid ESP32-S3 GPIO errors
    pinMode(HMS_MCP_CAN_CS_PIN, OUTPUT);
    digitalWrite(HMS_MCP_CAN_CS_PIN, HIGH);

    HMS_MCP_CAN_LOGGER(info, "Creating MCP CAN instance...");
    mcpCan = new MCP_CAN(spi, (uint8_t)HMS_MCP_CAN_CS_PIN);

    HMS_MCP_CAN_LOGGER(info, "Initializing SPI interface for MCP CAN...");
    spi->begin(
        HMS_MCP_CAN_SCK_PIN, 
        HMS_MCP_CAN_MISO_PIN, 
        HMS_MCP_CAN_MOSI_PIN, 
        HMS_MCP_CAN_CS_PIN
    );
    
    HMS_MCP_CAN_LOGGER(info, "Initializing MCP CAN...");
    #if defined(HMS_MCP_CAN_CLK_8MHZ)
    if (mcpCan->begin(MCP_STDEXT, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        HMS_MCP_CAN_LOGGER(info, "MCP CAN initialized successfully at 500 Kbps (8MHz).");
    } else {
        HMS_MCP_CAN_LOGGER(error, "Failed to initialize MCP CAN at 500 Kbps (8MHz).");
    #else
    if (mcpCan->begin(MCP_STDEXT, CAN_500KBPS, MCP_16MHZ) == CAN_OK) {
        HMS_MCP_CAN_LOGGER(info, "MCP CAN initialized successfully at 500 Kbps (16MHz).");
    } else {
        HMS_MCP_CAN_LOGGER(error, "Failed to initialize MCP CAN at 500 Kbps (16MHz).");
    #endif
        return HMS_MCP_CAN_ERROR;
    }
    
    // Explicitly set filters to bypass everything (Sniff ALL traffic)
    mcpCan->init_Mask(0, 0, 0x00000000); // Init first mask...
    mcpCan->init_Filt(0, 0, 0x00000000); // Init first filter...
    mcpCan->init_Filt(1, 0, 0x00000000); // Init second filter...
    
    mcpCan->init_Mask(1, 0, 0x00000000); // Init second mask... 
    mcpCan->init_Filt(2, 0, 0x00000000); // Init third filter...
    mcpCan->init_Filt(3, 0, 0x00000000); // Init fourth filter...
    mcpCan->init_Filt(4, 0, 0x00000000); // Init fifth filter...
    mcpCan->init_Filt(5, 0, 0x00000000); // Init sixth filter...

    // Force MCP2515 into Listen Only Mode (Safer for sniffing)
    // In Listen Only mode, we receive messages and errors but do not transmit ACKs.
    // This prevents us from disturbing the bus if our baud rate is slightly off.
    mcpCan->setMode(MCP_LISTENONLY);
    
    HMS_MCP_CAN_LOGGER(info, "MCP2515 set to Listen Only Mode (Sniffer Mode).");

    return HMS_MCP_CAN_OK;
}
#elif defined (HMS_MCP_CAN_PLATFORM_ESP_IDF) 


#endif // HMS_MCP_CAN_PLATFORM_ESP_IDF