#include "HMS_MCP_CAN.h"

#if HMS_MCP_CAN_DEBUG_ENABLED
    ChronoLogger *mcpCanLogger = nullptr;
#endif

HMS_MCP_CAN::HMS_MCP_CAN() {
    #if HMS_MCP_CAN_DEBUG_ENABLED
        if (!mcpCanLogger) {
            mcpCanLogger = new ChronoLogger("HMS_MCP_CAN");
            mcpCanLogger->setLevel(HMS_MCP_CAN_LOG_LEVEL);
        }
    #endif
    // Constructor implementation
}

HMS_MCP_CAN::~HMS_MCP_CAN() {
    // Destructor implementation
}

HMS_MCP_CAN_StatusTypeDef HMS_MCP_CAN::begin() {
    return init();
}
