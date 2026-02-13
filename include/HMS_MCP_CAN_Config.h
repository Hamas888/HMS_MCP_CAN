/*
 ============================================================================================================================================
 * File:        HMS_MCP_CAN_Config.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        Jan 28 2026
 * Brief:       This file package provides MCP CAN driver configuration options.
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

#ifndef HMS_MCP_CAN_CONFIG_H
#define HMS_MCP_CAN_CONFIG_H

/*
  ┌─────────────────────────────────────────────────────────────────────┐
  │ Note:     Platform detection                                        │
  │ Requires: Development environment and platform-specific SDKs        │
  └─────────────────────────────────────────────────────────────────────┘
*/
#if defined(ARDUINO)
  #include <SPI.h>
  #include <Arduino.h>
  #include "../MCP_CAN_lib/mcp_can.h"
  #if defined(ESP32)
    #define HMS_MCP_CAN_PLATFORM_ESP32_ARDUINO
  #elif defined(ESP8266)
    // Include ESP8266 MQTT libraries here
    #define HMS_MCP_CAN_PLATFORM_ESP8266_ARDUINO
  #endif
  #define HMS_MCP_CAN_PLATFORM_ARDUINO
#elif defined(ESP_PLATFORM)
  #define HMS_MCP_CAN_PLATFORM_ESP_IDF
#elif defined(__ZEPHYR__)
  #define HMS_MCP_CAN_PLATFORM_ZEPHYR
#elif defined(__STM32__)
  #define HMS_MCP_CAN_PLATFORM_STM32_HAL
#elif defined(__linux__) || defined(_WIN32) || defined(__APPLE__)
  // Desktop specific includes
  #define HMS_MCP_CAN_PLATFORM_DESKTOP
#endif // Platform detection

/*
  ┌─────────────────────────────────────────────────────────────────────┐
  │ Note:     Enable only if ChronoLog is included                      │
  │ Requires: ChronoLog library → https://github.com/Hamas888/ChronoLog │
  └─────────────────────────────────────────────────────────────────────┘
*/
#if defined(CONFIG_HMS_MCP_CAN_DEBUG)
    #define HMS_MCP_CAN_DEBUG_ENABLED                   1
#elif defined(HMS_MCP_CAN_DEBUG)
    #define HMS_MCP_CAN_DEBUG_ENABLED                   1
#else
    #define HMS_MCP_CAN_DEBUG_ENABLED                   0                            // Enable debug messages (1=enabled, 0=disabled)
#endif


#if HMS_MCP_CAN_DEBUG_ENABLED
  #if __has_include("ChronoLog.h")
    #include "ChronoLog.h"
    #ifndef HMS_MCP_CAN_LOG_LEVEL
      #ifdef CONFIG_HMS_MCP_CAN_LOG_LEVEL
        #if CONFIG_HMS_MCP_CAN_LOG_LEVEL == 0
          #define HMS_MCP_CAN_LOG_LEVEL                 CHRONOLOG_LEVEL_ERROR
        #elif CONFIG_HMS_MCP_CAN_LOG_LEVEL == 1
          #define HMS_MCP_CAN_LOG_LEVEL                 CHRONOLOG_LEVEL_WARN
        #elif CONFIG_HMS_MCP_CAN_LOG_LEVEL == 2
          #define HMS_MCP_CAN_LOG_LEVEL                 CHRONOLOG_LEVEL_INFO
        #elif CONFIG_HMS_MCP_CAN_LOG_LEVEL == 3
          #define HMS_MCP_CAN_LOG_LEVEL                 CHRONOLOG_LEVEL_DEBUG
        #elif CONFIG_HMS_MCP_CAN_LOG_LEVEL == 4
          #define HMS_MCP_CAN_LOG_LEVEL                 CHRONOLOG_LEVEL_DEBUG
        #else
          #define HMS_MCP_CAN_LOG_LEVEL                 CHRONOLOG_LEVEL_DEBUG
        #endif
      #else
          #define HMS_MCP_CAN_LOG_LEVEL                 CHRONOLOG_LEVEL_DEBUG
      #endif
      extern ChronoLogger *mcpCanLogger;
    #endif
    #define HMS_MCP_CAN_LOGGER(level, msg, ...)         \
      do {                                              \
        if (mcpCanLogger)                               \
          mcpCanLogger->level(                          \
            msg, ##__VA_ARGS__                          \
          );                                            \
      } while (0)
  #else
    #error "ChronoLog library not found. Please include ChronoLog to enable HMS_MCP_CAN debugging."
  #endif
#else
  #define HMS_MCP_CAN_LOGGER(level, msg, ...)           do {} while (0)
#endif

/*
  ┌─────────────────────────────────────────────────────────────────────┐
  │ Note: HMS MCP CAN Custom Types & Definitions                        │
  └─────────────────────────────────────────────────────────────────────┘
*/
// Default pins are defined in platform blocks below

#define HMS_MCP_CAN_ERROR_CHECK(expr)                   \
  do {                                                  \
    if (!(expr)) {                                      \
      HMS_MCP_CAN_LOGGER(error, "Error: %s", #expr);    \
      return HMS_MCP_CAN_ERROR;                         \
    }                                                   \
  } while (0)

#ifdef HMS_MCP_CAN_PLATFORM_ESP_IDF
  #define HMS_MCP_CAN_CS_PIN                            CONFIG_HMS_MCP_CAN_CS_GPIO
  #define HMS_MCP_CAN_SCK_PIN                           CONFIG_HMS_MCP_CAN_SCK_GPIO
  #define HMS_MCP_CAN_MOSI_PIN                          CONFIG_HMS_MCP_CAN_MOSI_GPIO
  #define HMS_MCP_CAN_MISO_PIN                          CONFIG_HMS_MCP_CAN_MISO_GPIO
  #define HMS_MCP_CAN_Delay(ms)                         vTaskDelay(pdMS_TO_TICKS(ms))
#elif defined(HMS_MCP_CAN_PLATFORM_ARDUINO)
  #if defined(HMS_MCP_CAN_CS_GPIO)
    #define HMS_MCP_CAN_CS_PIN                          HMS_MCP_CAN_CS_GPIO
  #else
    #define HMS_MCP_CAN_CS_PIN                          GPIO_NUM_10
  #endif
  #if defined(HMS_MCP_CAN_SCK_GPIO)
    #define HMS_MCP_CAN_SCK_PIN                         HMS_MCP_CAN_SCK_GPIO
  #else
    #define HMS_MCP_CAN_SCK_PIN                         GPIO_NUM_12
  #endif
  #if defined(HMS_MCP_CAN_MOSI_GPIO)
    #define HMS_MCP_CAN_MOSI_PIN                        HMS_MCP_CAN_MOSI_GPIO
  #else
    #define HMS_MCP_CAN_MOSI_PIN                        GPIO_NUM_11
  #endif
  #if defined(HMS_MCP_CAN_MISO_GPIO)
    #define HMS_MCP_CAN_MISO_PIN                        HMS_MCP_CAN_MISO_GPIO
  #else
    #define HMS_MCP_CAN_MISO_PIN                        GPIO_NUM_13
  #endif
  #define HMS_MCP_CAN_Delay(ms)                         delay(ms)
#else
  #define HMS_MCP_CAN_Delay(ms)
#endif

typedef enum {
  HMS_MCP_CAN_OK                                        = 0x00,
  HMS_MCP_CAN_BUSY                                      = 0x01,
  HMS_MCP_CAN_ERROR                                     = 0x02,
  HMS_MCP_CAN_NO_MEM                                    = 0x03,
  HMS_MCP_CAN_TIMEOUT                                   = 0x04,
  HMS_MCP_CAN_NOT_FOUND                                 = 0x05,
} HMS_MCP_CAN_StatusTypeDef;

typedef struct {
  uint32_t  id;
  uint8_t   length;
  uint8_t   data[8];
} HMS_MCP_CAN_MessageTypeDef;

#endif // HMS_MCP_CAN_CONFIG_H