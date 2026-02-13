# HMS_MCP_CAN/CMakeLists.txt

set(HMS_MCP_CAN_VERSION 1.0.0)

# Check if we're building with Zephyr
if(DEFINED ZEPHYR_BASE)
    zephyr_library_sources(src/HMS_MCP_CAN.cpp)
    zephyr_include_directories(include)

# Check if we're building with ESP-IDF
elseif(DEFINED ESP_PLATFORM OR DEFINED IDF_VER OR DEFINED ENV{IDF_PATH})
    idf_component_register(
        INCLUDE_DIRS "include" "MCP_CAN_lib"
        SRCS 
            "src/HMS_MCP_CAN.cpp"
            "src/HMS_MCP_CAN_ESP32.cpp"
            "MCP_CAN_lib/mcp_can.cpp"
        REQUIRES
            "driver"
            "freertos"
            "spi_flash"
    )

    # Force C++17 standard
    target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_17)

    # (optional) if you want strict enforcement instead of compiler default fallback
    set_target_properties(${COMPONENT_LIB} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    
# STM32 / generic CMake project
else()
    add_library(HMS_MCP_CAN INTERFACE)
    target_include_directories(HMS_MCP_CAN INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_compile_features(HMS_MCP_CAN INTERFACE cxx_std_17)
endif()