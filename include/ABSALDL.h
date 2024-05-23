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

const ABSCODE ABS_codes[30] = 
{
//Code#       Description
  {0x21,        "RF Sens Fault"},
  {0x23,        "RF Sens CNT Fault"},
  {0x25,        "LF Sens Fault"},
  {0x27,        "LF Sens CNT Fault"},
  {0x28,        "Spd Sens Freq Err"},
  {0x31,        "RR Sens Fault"},
  {0x33,        "RR Sens CNT Fault"},
  {0x35,        "LR Sens Fault"},
  {0x37,        "LR Sens CNT Fault"},
  {0x41,        "RF Valve Sol Fault"},
  {0x44,        "ASR Valve Sol Fault"},
  {0x45,        "LF Valve Sol Fault"},
  {0x51,        "RR Valve Sol Fault"},
  {0x55,        "LR Valve Sol Fault"},
  {0x57,        "Crs Cntr Mon Fault"},
  {0x58,        "ECU Internal Fault"},
  {0x61,        "Motor Pump Fault"},
  {0x62,        "Tach Pulse Fault"},
  {0x63,        "Valve Relay Fault"},
  {0x64,        "TPS Fault"},
  {0x65,        "Throt Relaxer Fault"},
  {0x66,        "Accuracy TPS Fault"},
  {0x68,        "Fault code deleted."},
  {0x71,        "ECU Internal Fault"},
  {0x72,        "Data Link Fault"},
  {0x73,        "Spark Ret Mon Fault"},
  {0x74,        "Low Voltage"},
  {0x75,        "Lat Accel Wire Fault"},
  {0x76,        "Lat Accel untrusted"},
  {0x83,        "Brake Fluid Low"}
};

const char* getABSMessage(uint8_t msg)
{
    for(uint8_t i = 0; i < 30; i++)
    {
        if(ABS_codes[i].code == msg)
        {
            return ABS_codes[i].desc;
        }

        if(msg == 255)
        {
            return "";
        }
    }

    return "UNDEFINED";
}

uint8_t generateChecksum(uint8_t *buf, size_t len) 
{
       uint8_t x = 0;
       uint8_t sum = 0;
       for(x = 0; x < len; x++) sum += buf[x];
       return ( UINT8_MAX - sum + 1);
};

bool fixAndCheckABSData(ABSALDL* data)
{
    uint8_t N = data->msgLen - 0x52 - 1;

    #define FC_LENGTH       5
    #define MSG_LENGTH_RAW (19 + 3 /*header*/ )
    #define MSG_LENGTH_1   (MSG_LENGTH_RAW + FC_LENGTH)
    #define MSG_LENGTH_2   (MSG_LENGTH_RAW + FC_LENGTH * 2)
    #define MSG_LENGTH_3   (MSG_LENGTH_RAW + FC_LENGTH * 3)

    switch (N)
    {
        case MSG_LENGTH_RAW: // No errors stored
            if(generateChecksum((uint8_t*)data, MSG_LENGTH_RAW) != ((uint8_t*)data)[MSG_LENGTH_RAW])
            {
                    return false;
            }
            else
            {
                data->fc1.faultCodeNum = 0xFF;
                data->fc2.faultCodeNum = 0xFF;
                data->fc3.faultCodeNum = 0xFF;
            }
        break;

        case MSG_LENGTH_1: // 1 error stored
            if(generateChecksum((uint8_t*)data, MSG_LENGTH_1) != ((uint8_t*)data)[MSG_LENGTH_1])
            {
                    return false;
            }
            else
            {
                data->fc2.faultCodeNum = 0xFF;
                data->fc3.faultCodeNum = 0xFF;
            }
        break;

        case MSG_LENGTH_2: // 2 errors stored
            if(generateChecksum((uint8_t*)data, MSG_LENGTH_2) != ((uint8_t*)data)[MSG_LENGTH_2])
            {
                    return false;
            }
            else
            {
                data->fc3.faultCodeNum = 0xFF;
            }
        break;

        case MSG_LENGTH_3: // 3 errors stored
            if(generateChecksum((uint8_t*)data, MSG_LENGTH_3) != ((uint8_t*)data)[MSG_LENGTH_3])
            {
                    return false;
            }
        break;
    
        default: // that means message is corrupted, there is even no reason to verify checksum
            return false;
        break;
    }

    return true;
}

char* getPaddedSpeed(uint8_t spd)
{
	static char buf[4];

	*(uint32_t*) buf = 0UL;

	itoa(spd >> 1, buf, 10);
	for (uint8_t i = strlen(buf); i < 3; i++)
	{
		buf[i] = ' ';
	}

	return buf;
}