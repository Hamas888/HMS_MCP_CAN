/*
 ============================================================================================================================================
 * File:        HMS_MCP_CAN.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        Jan 28 2026
 * Brief:       This file package provides MCP CAN driver for embedded systems.
 ============================================================================================================================================
 * License: 
 * MIT License
 * 
 * Copyright (c) 2025 Hamas Saeed
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, 
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do 
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For any inquiries, contact Hamas Saeed at hamasaeed@gmail.com
 ============================================================================================================================================
 */

#ifndef HMS_MCP_CAN_H
#define HMS_MCP_CAN_H

#include "HMS_MCP_CAN_Config.h"

class HMS_MCP_CAN {
public:
    HMS_MCP_CAN();
    ~HMS_MCP_CAN();
    
    HMS_MCP_CAN_StatusTypeDef begin();
    HMS_MCP_CAN_StatusTypeDef readCANMessage(HMS_MCP_CAN_MessageTypeDef &message);
    
    // Platform-specific member declarations
    #if defined(HMS_MCP_CAN_PLATFORM_ESP32_ARDUINO) || defined(HMS_MCP_CAN_PLATFORM_ARDUINO)

    #elif defined(HMS_MCP_CAN_PLATFORM_ESP_IDF)

    #endif

private:
    #if defined(HMS_MCP_CAN_PLATFORM_ESP32_ARDUINO) || defined(HMS_MCP_CAN_PLATFORM_ARDUINO)
        SPIClass    *spi            = nullptr;
        MCP_CAN     *mcpCan         = nullptr; 

    #elif defined(HMS_MCP_CAN_PLATFORM_ESP_IDF)

    #endif

    HMS_MCP_CAN_StatusTypeDef init();
};

#endif // HMS_MCP_CAN_H