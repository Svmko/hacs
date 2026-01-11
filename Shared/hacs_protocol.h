#ifndef HACS_PROTOCOL_H
#define HACS_PROTOCOL_H

#include <stdint.h>

/**
 * @brief HACS System Constants
 */
#define HACS_START_BYTE 0xAA
#define HACS_END_BYTE   0x55
#define HACS_UART_BAUD  115200

/**
 * @brief CAN Bus ID Matrix
 * Priority is determined by the lowest ID value.
 */
typedef enum {
    CAN_ID_HEARTBEAT = 0x100, // Status/Health (Highest Priority)
    CAN_ID_ATTITUDE  = 0x110, // Yaw, Pitch, Roll (100Hz)
    CAN_ID_POSITION  = 0x120, // Lat, Lon, Alt (10Hz)
    CAN_ID_BATTERY   = 0x130, // Voltage, Current, %
    CAN_ID_STEER_CMD = 0x200, // Commands from AE to Pilot
    CAN_ID_THRUST_CMD= 0x210  // Commands from AE to Pilot
} HacsCanId;

/**
 * @brief System Status Bitmask
 */
typedef enum {
    STATUS_OK       = 0x00,
    STATUS_SIM_MODE = 0x01,
    STATUS_FAILSAFE = 0x02,
    STATUS_ERROR    = 0x04
} HacsStatus;

/**
 * @brief Telemetry Packet Structure (HIL Bridge)
 * Uses #pragma pack to ensure no padding bytes are added, 
 * keeping the binary footprint identical across different processors.
 */
#pragma pack(push, 1)
typedef struct {
    uint8_t  start_byte;  // Fixed: 0xAA
    uint32_t timestamp;   // System time in ms
    
    // Orientation (Deg)
    float    yaw;         
    float    pitch;       
    float    roll;        
    
    // Motion
    float    velocity;    // Knots
    
    // GPS
    double   lat;         
    double   lon;         
    
    uint8_t  status;      // Bitmask using HacsStatus
    uint16_t checksum;    // Simple additive checksum
    uint8_t  end_byte;    // Fixed: 0x55
} HacsTelemetryPacket;
#pragma pack(pop)

#endif // HACS_PROTOCOL_H