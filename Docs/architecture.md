sequenceDiagram
    autonumber
    participant GCS as HACS-Control (PC)
    participant AE as Mission Computer (AE)
    participant Pilot as Pilot VCU (STM32)
    participant Gateway as Link Gateway (XIAO)
    participant Sim as Gazebo/Physics (PC)

    Note over GCS, Sim: --- DOWNLINK: Command Execution ---

    GCS->>AE: Set Waypoint (UDP: Lat/Lon)
    AE->>AE: Calculate Path & Desired Heading
    AE->>Pilot: Send Steering Command (CAN: 0x200)
    Pilot->>Pilot: PID Loop: Calculate PWM
    Pilot->>Gateway: Send Actuator Packet (UART)
    Gateway->>Sim: Inject Virtual Thrust (USB/Raw)
    Sim->>Sim: Update Physics (Boat Moves)

    Note over GCS, Sim: --- UPLINK: Perception Loop ---

    Sim->>Gateway: Stream Raw Telemetry (USB: IMU/GPS)
    Gateway->>Gateway: Frame Sync & Checksum
    Gateway->>Pilot: Stream Validated Packet (UART)
    
    Note right of Pilot: Pilot runs FreeRTOS <br/>Sensor Fusion Task
    
    Pilot->>Pilot: Kalman/Complementary Filter (Fusion)
    Pilot->>AE: Broadcast State (CAN: 0x110, 0x120)
    AE->>GCS: Telemetry Update (UDP: Status)
    
    Note over GCS: Loop Closed: User sees boat move on Map