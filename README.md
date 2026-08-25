# SECUREPASS DUO – GSM & RFID OTP-Based Authentication System

A **two-factor authentication embedded security system** developed using the **LPC2148 ARM7 microcontroller**. The system combines **RFID authentication** with a **GSM-based OTP** to provide secure access control.

## Features

* RFID-based user identification
* Dynamic 4-digit OTP generation
* GSM-based OTP delivery through SMS
* RTC-based OTP timeout
* 16x2 LCD user interface
* 4x4 matrix keypad for OTP entry
* Interrupt-driven UART communication
* Administrator configuration mode
* Relay-controlled door access
* Limited retry attempts for invalid OTPs

## Hardware

* **Microcontroller:** LPC2148 ARM7
* **RFID Reader:** AT89C2051-based RFID module
* **GSM Modem:** M660A
* **Display:** 16x2 LCD
* **Input:** 4x4 Matrix Keypad
* **Timekeeping:** On-chip RTC
* **Output:** Relay-based door lock

## Software

* **Language:** Embedded C
* **IDE:** Keil uVision
* **Programming Tool:** Flash Magic
* **Communication:** UART0 & UART1
* **Architecture:** Bare-metal modular firmware

## Working

1. RFID reader scans the user's card.
2. LPC2148 receives and validates the RFID ID through **UART1**.
3. After successful authentication, a dynamic OTP is generated using RTC values.
4. OTP is sent to the registered mobile number through GSM using **UART0**.
5. User enters the OTP through the keypad.
6. The system verifies the OTP within the allowed time.
7. If valid, the relay unlocks the door; otherwise, access is denied.

## Key Modules

* **RFID:** User identification and card authentication
* **GSM:** SMS and AT-command communication
* **RTC:** Time management and OTP validity
* **UART0:** GSM communication
* **UART1:** RFID communication
* **Keypad:** OTP input
* **LCD:** User feedback
* **External Interrupt:** Administrator mode
* **Relay:** Door access control

## My Contribution

* Developed Embedded C firmware for LPC2148.
* Implemented RFID communication using UART1.
* Implemented GSM communication and SMS transmission using UART0.
* Developed RTC-based OTP generation and verification.
* Implemented interrupt-driven serial communication.
* Integrated LCD, keypad, and relay control.
* Developed administrator configuration functionality.

## Technical Concepts

**ARM7 • Embedded C • UART • Interrupts • RFID • GSM • RTC • LCD • Keypad • GPIO • Relay Control • Real-Time Embedded Systems**
