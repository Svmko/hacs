# HACS-ASV: Hybrid Autonomy Control Stack

**A Vehicle-Agnostic Hardware-in-the-Loop (HIL) Testbed for Maritime Systems**

## Project Overview

HACS-ASV is a modular, multi-node control architecture designed for **Autonomous Surface Vessels (ASVs)**. The project utilizes a **Hardware-in-the-Loop (HIL)** configuration to validate high-level mission logic and low-level control loops against a simulated maritime environment.

The primary objective is to demonstrate a **Modular Open System Architecture (MOSA)** where mission-level autonomy is decoupled from safety-critical real-time flight management.

## System Architecture

The system is distributed across three physical hardware nodes, communicating via a prioritized **CAN bus** and high-speed **UART**.

* **Autonomy Engine (AE):** An **Orange Pi 5** running high-level mission planning and pathfinding logic in C++. It acts as the "Mission Computer," processing filtered telemetry and issuing navigation commands.
* **Pilot (VCU):** An **STM32 Nucleo** running **FreeRTOS**. This node handles deterministic, real-time tasks including sensor fusion (IMU/GPS), PID control loops, and vehicle safety states.
* **Gateway:** A **Seeed Studio XIAO SAMD21** that serves as the physical bridge between the PC-based simulator (Gazebo) and the embedded hardware. It handles packet synchronization and frame validation.

## Hardware-in-the-Loop (HIL) Data Flow

The project implements a closed-loop simulation to trick the embedded hardware into "feeling" the water:

1. **Simulation:** The PC (running Gazebo) calculates hydrodynamics and streams virtual sensor data.
2. **Ingress:** The **Gateway** ingests USB data and translates it to raw UART for the **Pilot**.
3. **Estimation:** The **Pilot** performs real-time sensor fusion and broadcasts the state over **CAN**.
4. **Logic:** The **AE** receives the state, calculates the mission goal, and sends steering commands back via **CAN**.
5. **Actuation:** The **Pilot** updates the virtual actuators, which the PC reflects in the physics engine.

## Directory Structure

```text
/HACS-ASV
├── /nodes
│   ├── /gateway          # XIAO SAMD21: Simulation interface and packet bridge
│   ├── /pilot            # STM32: FreeRTOS flight management and sensor fusion
│   └── /ae               # Orange Pi 5: High-level autonomy and mission logic
├── /shared               # Shared C++ headers and protocol definitions (hacs_protocol.h)
├── /simulator            # Python-based plant models and Gazebo configurations
├── /docs                 # CAN message matrix, schematics, and HIL block diagrams
├── .gitignore            # Standard filters for C++ and Python build artifacts
└── README.md             # Project documentation and setup guide

```

## Communication Protocols

* **Internal Bus:** CAN 2.0B (Standard/Extended IDs) for inter-node communication.
* **Telemetry Link:** Framed Binary Serial (UART) with DMA-backed ingestion for the Pilot node.
* **Messaging Schema:** Prioritized IDs for safety-critical heartbeats and command signals.

---

### Technical Highlights

* **Deterministic Real-Time Control:** Utilization of FreeRTOS on the Pilot node to ensure sub-10ms control loop latency.
* **Hardware Agnosticism:** The Pilot node abstracts hardware-specific motor signals, allowing the AE to remain vessel-independent.
* **Robust Serial Framing:** Implementation of a custom synchronization protocol with CRC validation to ensure data integrity across the HIL bridge.