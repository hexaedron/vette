#pragma once

// 12/16/93   formula change mode1 byte 13          B.Henson
//                        DATA STREAM SPECIFICATION
//
//       SPECIFICATIONS FOR  DATA STREAM INFORMATION
//       -------------------------------------------
//
//       CAR LINE:  1992 Y Cars
//                  1993 Y Cars
//                  1994 Y CARS
//       DATA PIN  :  M
//       BAUD RATE :  8192
//
//       ABS/ASR ECU OVERVIEW
//       --------------------
//          The ABS/ASR ECU is a slave module on the data link. Before
//          communications can be established a mode 8 must be issued to the
//          master.
//
//        MODE 0 (RETURN TO NORMAL MODE)
//                ALDL REQUEST:
//                - MESSAGE ID     = $F9
//                - MESSAGE LENGTH = $56
//                - MODE           = $00
//                - SUM CHECK
//
//                THE ABS CONTROLLER WILL NOT RESPOND.
//
//        MODE 1 MESSAGE 0: (TRANSMIT FIXED DATA STREAM)
//                ALDL REQUEST:
//                - MESSAGE ID     = $F9
//                - MESSAGE LENGTH = $57
//                - MODE           = $01
//                - MESSAGE NUMBER = $00
//                - SUM CHECK
//                THE ABS CONTROLLER WILL RESPOND WITH THE FOLLOWING MESSAGE:
//                - MESSAGE ID     = $F9
//                - MESSAGE LENGTH = $55 + $01 + $13 + (5*N)
//                - MODE           = $01
//                - DATA BYTE 1
//                  .
//                  .
//                - DATA BYTE 19 + (5*N)
//                - SUM CHECK
//N is the number of fault codes stored. Possible values are 0, 1, 2 or 3.
//
//        MODE 10 (CLEAR MALFUNCTION CODES)
//                ALDL REQUEST:
//                - MESSAGE ID     = $F9
//                - MESSAGE LENGTH = $56
//                - MODE           = $0A
//                - CHECKSUM
//
//                THE ABS CONTROLLER SENDS NO RESPONSE
//
// # indicates a change for this data stream.
//
//ALDL ABS DATA LIST
//NUMBER OF DATA WORDS - 19 + 5N (N can be 0, 1, 2, or 3)
//ALDL MODE 1 MESSAGE 0 DATA LIST
//                          DATA STREAM LENGTH
//                          ------------------
//          The data stream length will variable depending on the number of
//          malfunction codes:
//          19 bytes if no codes are set
//          24 bytes if one code is set
//          29 bytes if two codes are set
//          34 bytes if three bytes are set
//

#include <stdint.h>
#include <stdlib.h>    // itoa
#include <string.h>    // strlen

#include "include/A172ALDL.h"

typedef struct 
{
    uint8_t faultCodeNum;
// *20          Fault Code 1
//
    uint8_t faultStatusWord;
// *21          Fault Status Word 1
//        7      ABS State                   1 = active   0 = passive
//        6      Brake Light Switch State    1 = on       0 = off
//      # 5      ABS state                   1 = active   0 = passive
//      # 4      ASR switched off by driver  1 = yes      0 = no
//        3      not used
//        2      not used
//        1      not used
//        0      not used
    uint16_t speedWhenSet;
// *22          Vehicle Speed when Fault Code 1 is set (MSB)
// *23          Vehicle Speed when Fault Code 1 is set (LSB)
//               km/Hr = (256MSB + LSB)/16
//               Mi/Hr = (256MSB + LSB)/25.75
    uint8_t ignitionCyclesSinceFaultCodeSet;
// *24          Ignition Cycles since Fault Code 1 Set
//               count = N
} __attribute__ ((__packed__)) ABS_FAULT_CODE;

typedef struct 
{
// WORD#       DESCRIPTION

    uint8_t msgID;
    uint8_t msgLen;
    uint8_t msgMode;

//  1           System Identification
//                 (00 = B/D, K-Special, E/K/V, and Y-Car)
    uint8_t sysID1;

//  2           System Identification
//                 Display number in second two xx
//                 $01 - Y car
//                 $02 - B & D car
//                 $03 - E, K, & V Car
    uint8_t sysID2;

//  3-10        Controller ID (8 ASCII codes for controller identification)
//               Display codes as they are, no conversion necessary.
    char controllerID[8];

//  11          System status word
//      # 7      not used
//        6      Pump monitoring                   0 = pump off     1 = pump on
//      # 5      ASR enable switch state           0 = ASR on       1 = ASR off
//        4      Low brake fluid switch            0 = fluid ok     1 = fluid low
//        3      Brake Light Switch                0 = off          1 = on
//        2      not used
//        1      not used
//        0      not used
    uint8_t systemStatus;

//  12          Valve relay voltage
//               V = .085N
    uint8_t valveRelayVoltage;

//  13          Lateral acceleration
//               G's = .01N               (this is a signed variable where N 0 to
//               volts = 2.7v + .01875N    127 is positive and N 128 to 255 are
//                                         two's complement negative numbers.)
    uint8_t lateralAcceleration;

//  14          Engine speed
//               RPM = 30N
    uint8_t engineRPM;

//  15          Throttle Angle
//               % = N/2.55
    uint8_t throttleAngle;

//  16          Right Front Wheel Speed
//               km/Hr = N/2
//               Mi/Hr = .3107N
    uint8_t RFWheelSpeed;

//  17          Left Front Wheel Speed
//               km/Hr = N/2
//               Mi/Hr = .3107N
    uint8_t LFWheelSpeed;

//  18          Right Rear Wheel Speed
//               km/Hr = N/2
//               Mi/Hr = .3107N
    uint8_t RRWheelSpeed;

//  19          Left Rear Wheel Speed
//               km/Hr = N/2
//               Mi/Hr = .3107N
    uint8_t LRWheelSpeed;

// Up to 3 fault codes
    ABS_FAULT_CODE fc1;
    ABS_FAULT_CODE fc2;
    ABS_FAULT_CODE fc3;
    uint8_t checksum;
} __attribute__ ((__packed__)) ABSALDL;

typedef struct
{
    uint8_t code;
    const char* desc;
} ABSCODE;


class ABSParser: protected ALDLParser
{
    public:
        const char* getABSMessage(uint8_t);
        bool fixAndCheckABSData(ABSALDL*);
        char* getPaddedSpeed(uint8_t);  
};