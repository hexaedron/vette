#include "include/ABSALDL.h"

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

//void    ABSParser::attach(ABSALDL* ABSdata) { this->data = ABSdata; };

const char* ABSParser::getABSMessage(uint8_t msg)
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

bool ABSParser::fixAndCheckABSData(ABSALDL* data)
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

char* ABSParser::getPaddedSpeed(uint8_t spd)
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