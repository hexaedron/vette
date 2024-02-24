//                        DATA STREAM A172  SPECIFICATION
//
//       SPECIFICATIONS FOR  DATA STREAM INFORMATION
//       -------------------------------------------
//
//       ENGINE   USAGE:
//              5.7L MFI H.O. LT1 (VIN = P) 1992 1Y CAR
//              5.7L MFI H.O. LT1 (VIN = P) 1993 1Y 1,2F
//       DATA PIN  : READ DATA ON PIN "M" OF ALDL CONNECTOR
//
//       BAUD RATE : 8192 BAUD - BI - DIRECTIONAL
//
//
//        MODE 0 (INPUT MESSAGE)
//                ALDL REQUEST:
//                - MESSAGE ID     = $F4
//                - MESSAGE LENGTH = $56
//                - MODE           = $00
//                - SUM CHECK
//
//                ECM RESPONSE: THE ECM WILL TRANSMIT THE FOLLOWING MESSAGE.
//                - MESSAGE ID     = $F4
//                - MESSAGE LENGTH = $56
//                - MODE           = $00
//                - SUM CHECK
//
//        MODE 1 (TRANSMIT FIXED DATA STREAM) MESSAGE 0
//                ALDL REQUEST:
//                - MESSAGE ID     = $F4
//                - MESSAGE LENGTH = $57
//                - MODE           = $01
//*               - MESSAGE        = $00
//                - SUM CHECK
//
//                THE ECM WILL RESPOND WITH THE FOLLOWING MESSAGE:
//                - MESSAGE ID     = $F4
//                - MESSAGE LENGTH = $95
//                - MODE           = $01
//                - DATA BYTE 1
//                  .
//                  .
//                - DATA BYTE 63
//                - SUM CHECK
//
//
//
//
//        MODE 10 (CLEAR MALFUNCTION CODES)
//                ALDL REQUEST:
//                - MESSAGE ID     = $F4
//                - MESSAGE LENGTH = $56
//                - MODE           = $0A
//                - CHECKSUM
//
//
//
//
//
//ALDL DATA LIST
//NUMBER OF DATA WORDS - 63
//ALDL MODE 1 DATA LIST
//WORD #   DATA NAME            DESCRIPTION

#include <stdint.h>
#include <stdlib.h> // itoa

typedef struct 
{
    uint8_t msgID;
    uint8_t msgLen;
    uint8_t msgMode;
    uint16_t PROMID;      //PROMID (MSB)
           //PROMIDA+1    PROMID (LSB)
                          //ID = (MSB * 256 + LSB)
    uint8_t MALFFLG1;     //MALFUNCTION WORD 1
         //0      CODE 23     MAT SENSOR LOW
         //1      CODE 22     THROTTLE SENSOR POSITION LOW
         //2      CODE 21     THROTTLE SENSOR POSITION HIGH
         //3      CODE 16     LO RES. PULSE FAILURE
         //4      CODE 15     COOLANT SENSOR LOW TEMPERATURE
         //5      CODE 14     COOLANT SENSOR HIGH TEMPERATURE
         //6      CODE 13     LEFT OXYGEN SENSOR FAILED
         //7      CODE 12     NO REFERENCE PULSES
    uint8_t MALFFLG2;      //MALFUNCTION WORD 2
         //0      CODE 34     MAP SENSOR LOW
         //1      CODE 33     MAP SENSOR HIGH
         //2      CODE 32     EGR DIAGNOSTIC
         //3      CODE 28     QDM #3
         //4      CODE 27     QDM #3
         //5      CODE 26     QDM #3
         //6      CODE 25     MAT SENSOR HIGH
         //7      CODE 24     VEHICLE SPEED SENSOR
    uint8_t MALFFLG3;      //MALFUNCTION WORD 3
         //0      CODE 51     PROM ERROR
         //1      CODE 46     FUEL ENABLE FAILURE
         //2      CODE 45     LEFT OXYGEN SENSOR RICH
         //3      CODE 44     LEFT OXYGEN SENSOR LEAN
         //4      CODE 43     ESC FAILURE
         //5      CODE 42     EST GROUNDED
         //6      CODE 41     EST OPEN
         //7      CODE 36     HI RES. PULSE FAILURE
    uint8_t MALFFLG4;      //MALFUNCTION WORD 4
         //0      CODE 63     RIGHT OXYGEN SENSOR FAILURE
         //1      CODE 62     OIL TEMPERATURE SENSOR HIGH
         //2      CODE 61     C.V. IV A/C COMPRESSOR FAULT
         //3      CODE 56     NOT USED
         //4      CODE 55     FUEL LEAN MALFUNCTION
         //5      CODE 54     NOT USED
         //6      CODE 53     SYSTEM VOLTAGE PROBLEM
         //7      CODE 52     OIL TEMPERATURE SENSOR LOW
   uint8_t MALFFLG5;      //MALFUNCTION WORD 5
         //0      CODE 72     GEAR SWITCH FAILURE
         //1      CODE 71     A/C EVAPORATOR TEMP SENSOR FAILURE
         //2      CODE 69     A/C CLUTCH CIRCUIT OPEN
         //3      CODE 68     A/C RELAY SHORTED
         //4      CODE 67     A/C PRESSURE TRANSDUCER DEGRADED
         //5      CODE 66     A/C PRESSURE TRANSDUCER OPEN OR SHORTED
         //6      CODE 65     RIGHT OXYGEN SENSOR RICH
         //7      CODE 64     RIGHT OXYGEN SENSOR LEAN

   uint8_t TCCMW;         //TCC MODE WORD
         //0      1 = TCC LOCKED
         //1      1 = KCOASTS COAST RELEASE TPS THRESHOLD IN USE (HI MPH)
         //2      1 = 4-3/4-2 DOWNSHIFT RELEASE IN PROGRESS
         //3      1 = STATUS OF FOURTH GEAR LAST PASS
         //4      1 = TCC LOCKED FOR PASS-BY NOISE
         //5      1 = 1-2 UPSHIFT IN PROGRESS
         //6      1 = 2-3 UPSHIFT IN PROGRESS
         //7      1 = IN FOURTH GEAR         
  uint8_t NVMW2;         //NON - VOLITILE MODE WORD
         //0      NOT USED
         //1      1 = CLOSED LOOP (FUEL) TIMER TIMED OUT
         //2      NOT USED
         //3      1 = PLUGGABLE MEMORY FAILURE (M51)
         //4      NOT USED
         //5      1 = FUEL ENABLED VIA FEDS OR FREQUENCY
         //6      NOT USED
         //7      1 = ESC ENABLED BY DELTA COOLANT
  uint8_t CARSMW;        //MODE FOR COMPUTER AIDED RATIO SELECTION LOGIC
         //0      1 = CARS DISABLED DUE TO LOW BAROMETRIC PRESSURE
         //1      1 = CARS ACTIVE
         //2      NOT USED
         //3      NOT USED
         //4      1 = TRANSMISSION IN FOURTH GEAR
         //5      1 = TRANSMISSION IN FIRST GEAR
         //6      1 = WAIT FOR VEHICLE SPEED RESET
         //7      1 = TRANSMISSION IN SECOND GEAR
  uint8_t IODPORTC;      //IOD PORT C OUTPUT CONTROL WORD
         //0      1 = IN DRIVE    (0 = P/N)
         //1      1 = IN 3RD OR 4TH GEAR
         //2      0 = IN 4TH GEAR
         //3      1 = IN LOW, 3RD, OR 4TH GEAR
         //4      1 = PSPS NOT CRAMPED (NOT USED)
         //5      1 = A/C CLUTCH ENGAGED
         //6      0 = A/C REQUESTED (Y - CAR)
         //7      0 = TRACTION CONTROL ACTIVE
  uint8_t IODPORTB;      //IOD PORT B INPUT DATA/OUTPUT CONTROL WORD
         //0      1 = CHECK ENGINE LIGHT ON
         //1      1 = QDM FAULT 1 NOT INDICATED
         //2      1 = QDM FAULT 2 NOT INDICATED
         //3      1 = QDM FAULT 3 NOT INDICATED
         //4      1 = SXR TRANSMISSION ENABLED
         //5      1 = C.A.R.S. LIGHT ON
         //6      1 = D, R CAR A/C REQUEST (Y-CAR OPTION NOT SELECTED)
         //7      1 = NOT USED
  uint8_t MW1;           //MODE WORD 1
         //0      1 = FIRST REFERENCE PULSE HAS BEEN DETECTED
         //1      1 = IDLE SPARK HIGH GAIN MODE ENABLED
         //2      1 = IDLE SPARK LOW GAIN MODE ENABLED
         //3      1 = UNDERSPEED (USED BY IDLE SPARK LOGIC)
         //4      1 = ROAD SPEED 1ST PULSE DETECTED
         //5      0 = A/C CLUTCH ON
         //6      1 = 1ST SPI ERROR DETECTED (2ND IS FATAL)
         //7      1 = ENGINE RUNNING (SPARK)

  uint8_t MW2;           //MODE WORD 2
         //0      1 = MORE THAN 100 MSECS. FOR SPI MESSAGE TO COMPLETE
         //1      1 = RUN SPARK ENABLED
         //2      1 = REFERENCE PULSE OCCURRED (6.25 MSEC. CHECK)
         //3      1 = ELITE DELAY STARTED
         //4      0 = REVERSE INHIBITED (F-CAR ONLY)
         //5      1 = A/C DISABLED DUE TO COMPRESSOR FAULT DETECTED
         //6      1 = HIGH BATTERY VOLTAGE
         //7      1 = USE LO A/C OFF EVAP. TEMP THRESH DUE TO WARM START

  uint8_t PORTASD;       //PORT A DATA REGISTOR
         //0      FUEL TRIGGER
         //1      LOW RESOLUTION REFERENCE PULSE INPUT
         //2      EST FEEDBACK
         //3      INJECTOR 2 FAULT
         //4      INJECTOR 1 FAULT
         //5      INJECTOR DRIVER 1
         //6      INJECTOR DRIVER 2
         //7      SPI HANDSHAKE LINE
  uint8_t LCCPMW;       //LCC & CCP MODE WORD
         //0      1 = CCP ENABLED
         //1      1 = CCMREVS FLIP-FLOP
         //2      1 = MALFS HAVE OCCURRED
         //3      1 = ALDL MODE 4 BYPASS FUEL COMMANDED
         //4      SECOND TIME COOLANT
         //5      1 = FALSE REFERENCE PULSE DETECTED
         //6      1 = KWARM KICKDOWN REQUESTED
         //7      1 = CLOSED THROTTLE SPARK ACTIVE
  uint8_t DIAGMW5;       //DIAGNOSTIC MODE WORD 5
         //     0      1 = REFERENCE PULSE OCCURRED THIS 100 MSECS. (M36 LOGIC)
         //     1      1 = MALF 41 (OPEN EST) DETECTED
         //     2      1 = MALF 42 (GROUNDED EST) DETECTED
         //     3      1 = MALF 16 FIRST PASS HAS OCCURRED
         //     4      NOT USED
         // *   5      1 = MALF 43A PRESENT (ONE ESC SENSOR OPEN)(not used in f-car)
         // *   6      1 = MALF 43C PRESENT (BOTH ESC SENSORS OPEN) Y-CAR ONLY
         //            1 = MALF 43C PRESENT (ESC SENSOR OPEN) F-CAR ONLY
         // *   7      1 = MALF 43D PRESENT (BOTH ESC SENSORS GROUNDED) Y-CAR ONLY
         //            1 = MALF 43D PRESENT (ESC SENSOR GROUNDED) F-CAR ONLY
  uint8_t AIRMW;         //MAJOR LOOP MODE WORD
         //0      1 = IAC MOTOR MOVING (LAST PASS)
         //1      1 = AIR TO PORTS OR CONVERTER,   0 = AIR DEVERTED
         //2      1 = AIR TO EXHAUST PORTS,        0 = AIR TO CONVERTER
         //3      1 = IAC MOTOR RESET PREVIOUSLY VIA SLEWING
         //4      1 = A/C REQUESTED (F - CAR)
         //5      1 = IAC MOTOR EXTENDING          (0 = RETRACTING)
         //6      1 = LAST 3.125 MSEC. IRQ WAS IAC 320 HZ INTERRUPT
         //7      1 = DECEL ENLEANMENT ENABLED

  uint8_t MWFA1;         //FUEL - AIR MODE WORD
         //0      1 = AIR DIVERTED
         //1      1 = LEARN CONTROL ENABLED
         //2      1 = LOW RPM HYSTERESIS FLAG IN CRANKING
         //3      1 = LEFT BANK BLM CHANGE INTEGRATOR RESET FLAG
         //4      1 = RIGHT BANK BLM CHANGE INTEGRATOR RESET FLAG
         //5      NOT USED
         //6      1 = CATALYTIC CONVERTER OVERTEMP ENABLED
         //7      1 = ENGINE IS CLOSED LOOP (BOTH SENSORS C.L.)

  uint8_t ADACPRES;    //A/D RESULT FOR A/C PRESSURE TRANSDUCER
                       //PSI = (N * 1.875)-18.75

  uint8_t DIAGMW3;       //DIAGNOSTIC MODE WORD 3
         //0      1 = INTEGRATORS BEING FORCED TO 128 (USED IN M32)
         //1      MALF 44 OR 45 HAS BEEN DETECTED
         //2      1 = BYPASS O2 MALF LOGIC - INT. BEING RESET
         //3      1 = M53 ACTIVE (VOLTAGE > 17 OR VOLTAGE < 8)
         //4      1 = MALF 13 DETECTED
         //5      1 = IN DIAGNOSTIC MODE LAST PASS (FOR NVM CLEAR)
         //6      1 = MALF 36 INDICATED AND LATCHED (HI REF. FAULT)
         //7      1 = DIAGNOSTIC MODE HAS BEEN ENTERED

  uint8_t COOLDEGA;    //COOLANT TEMPERATURE LINEARIZED  (NON-DEFAULTED)
                       //DEGREES C = (N*.75 - 40)
                       //DEGS. F = (1.35N - 40)
  uint8_t ADMAP;       //A/D RESULT FOR MANIFOLD PRESSURE SENSOR INPUT
                       //VOLTS = N*5/256
                       //(kpa = (N + 28.06)/2.71)
  uint8_t ADTHROT;     //A/D RESULT FOR TPS INPUT
                       //VOLTS = N*(5/256)
  uint8_t ADBAT;       //A/D RESULT FOR IGNITION VOLTAGE INPUT
                       //VOLTS = N/10
  uint8_t ADOILTMP;    //A/D RESULT FOR OIL TEMPERATURE SENSOR INPUT
                       //SEE OIL TEMP. LOOK-UP TABLE (NON-DEFAULTED)
  uint8_t NVADBARO;    //NON - VOLITILE COMPUTED A/D BAROMETER
                       //VOLTS = N*(5/256)
                       //KPA = (N + 28.06)/2.71)
  uint8_t ADMAT;       //A/D RESULT FOR MANIFOLD TEMPERATURE INPUT
                       //SEE MAT LOOK-UP TABLE  (DEFAULTED)
  uint8_t ADLO2;       //A/D RESULT FOR LEFT O2 SENSOR INPUT
                       //MV = N*4.42
  uint8_t ADRO2;       //A/D RESULT FOR RIGHT O2 SENSOR INPUT
                       //MV = N*4.42
  uint8_t LBLM;        //LEFT BANK BLOCK LEARN MULTIPLIER
                       //N = COUNTS
  uint8_t RBLM;        //RIGHT BANK BLOCK LEARN MULTIPLIER
                       //N = COUNTS
  uint8_t BLMCELL;     //BLOCK LEARN MULTIPLIER CELL NUMBER
                       //N = NUMBER
  uint8_t RESETCTR;    //RESET COUNTER FOR MALF 51

  uint8_t CYLSD;       //COPY OF LOWER BYTE OF CYL SERIAL DATA

  uint8_t LINT;        //INTEGRATOR VALUE FOR LEFT O2 SENSOR
                       //N = COUNTS
  uint8_t RINT;        //INTEGRATOR VALUE FOR RIGHT O2 SENSOR
                       //N = COUNTS
  uint8_t ISMNMPKA;    //KEEP - ALIVE IAC MINIMUM MOTOR POSITION
                       //N = IAC STEPS
  uint8_t PURGEDC;     //PURGE DUTY CYCLE
                       //% DC = N/2.56
  uint8_t ISSPMP;      //IAC PRESET MOTOR POSITION
                       //N = IAC STEPS
  uint8_t ISESDD;      //DESIRED IDLE SPEED
                       //RPM = N * 12.5
  uint16_t NEWRFPER;   //PRESENT MINOR LOOP REFERENCE PERIOD (MSB)
  //43     NEWRFPER+1   PRESENT MINOR LOOP REFERENCE PERIOD (LSB)
                       //MSEC = (MSB*256 + LSB)/65.536
  uint8_t SAREF;       //LIMITED SPARK RELATIVE TO REF. PULSE
                       //DEGREES = N
  uint8_t ADACEVAP;    //A/D RESULT FROM A/C EVAPORATOR TEMP.
                       //SEE A/C LOOK-UP TABLE
  uint8_t NTRPMX;      //ENGINE SPEED
                       //RPM = N * 25
  uint16_t TIME;       //ENGINE RUNNING TIME IN SECONDS (MSB)
  //48     TIME+1        ENGINE RUNNING TIME IN SECONDS (LSB)
                       //SECONDS = (MSB*256 + LSB)
  uint8_t NOCKRTD;     //RETARD DUE TO KNOCK
                       //DEGREES = N/2
  uint8_t OLDESCCT;    //ECU ESC COUNTER VALUE FROM LAST MINOR LOOP (MSB)
  uint8_t OLDESCCT_1;  //(LSB) NOT USED
                       //N = (MSB)
  uint8_t MPH;         //VEHICLE SPEED
                       //MPH = N
                       //KPH = (1.61 * N)
  uint8_t NTPSLD;      //THROTTLE LOAD AXIS VARIABLE
                       //% THROTTLE = N/2.56
  uint16_t ACTVOL36;   //100 MSEC . OLD HI RES. PULSE COUNT (M36 LOGIC) (MSB)
  //55     ACTVOL36+1    100 MSEC . OLD HI RES. PULSE COUNT (M36 LOGIC) (LSB)

  uint16_t LBPINJC;    //LEFT BANK LOW VOLTAGE CORRECTED BPW (MSB)
  //57     LBPINJC+1   //LEFT BANK LOW VOLTAGE CORRECTED BPW (LSB)
                       //MSECS. = (MSB * 256 + LSB)/131
  uint16_t RBPINJC;    //RIGHT BANK LOW VOLTAGE CORRECTED BPW (MSB)
  //59     RBPINJC+1   //RIGHT BANK LOW VOLTAGE CORRECTED BPW (LSB)
                       //MSECS. = (MSB * 256 + LSB)/131
  uint16_t REFPERSD;   //COPY OF REFPER FOR SERIAL DATA (MSB)
  //61     REFPERSD+1  //COPY OF REFPER FOR SERIAL DATA (LSB)
                       //MSECS. = (MSB * 256 + LSB)/65.535
  uint8_t EGRDC;       //EGR DUTY CYCLE
                       //DC = uint8_t 2.56

  uint8_t FANMW;       //FAN CONTROL MODE WORD
         //0      1 = FAN 1 ENABLED (ALL PID FAN 1 STEPS ADDED)
         //1      1 = SPI FAILED (USED FOR ALDL)
         //2      1 = FAN 1 REQUESTED
         //3      1 = FAN 2 ENABLED (ALL PID FAN 2 STEPS ADDED)
         //4      1 = FAN 2 REQUESTED
         //5      1 = FAN 1 ENABLED BY HIGH OIL TEMPERATURE
         //6      1 = HIGH RPM FAN ENABLE
         //7      1 = NOT USED
  uint8_t checksum;

} __attribute__ ((__packed__)) A172ALDL;

const char* MALFFLG_codes[40] =
{
       //MALFFLG1:
       "MAT sensor low",
       "Thr sens pos low",
       "Thr sens pos high",
       "Lo res pulse fail",
       "Clnt sens low temp",
       "Clnt sens hi temp",
       "Left ox sens fail",
       "No ref. pulses",
       
       //MALFFLG2:
       "MAP sensor low",
       "MAP sensor high",
       "EGR diagnostic",
       "QDM #3",
       "QDM #3",
       "QDM #3",
       "MAT sensor high",
       "Speed sens fail",

       //MALFFLG3:
       "PROM error",
       "Fuel enable fail",
       "Left ox sensor rich",
       "Left ox sensor lean",
       "ESC failure",
       "EST grounded",
       "EST open",
       "Hi res pulse fail",

       //MALFFLG4:
       "Right ox sens fail",
       "Oil temp sens high",
       "A/C compr fault",
       "",
       "Fuel lean malf",
       "",
       "Sys volt problem",
       "Oil temp sens low",

       //MALFFLG5:
       "Gear switch fail",
       "AC evap tmp sen fail",
       "AC clutch circ open",
       "AC relay shorted",
       "AC transducer degr",
       "A/C trans opn/short",
       "Right ox sensor rich",
       "Right ox sensor lean"
};

const char* FANMW_codes[8] =
{
       //FAN CONTROL MODE WORD
       "Fan 1 enabled",
       "SPI failed",
       "Fan 1 requested",
       "Fan 2 enabled",
       "Fan 2 requested",
       "Fan 1 by oil temp",
       "High rpm fan",
       ""
};

const int16_t MAT_temp_celsius_x10[256]
{ 
  1513, 1499, 1486, 1473, 1460, 1447, 1434, 1421, 1408, 1394, 1381, 1368, 1355, 1342, 1329, 1316, 
  1303, 1289, 1276, 1263, 1250, 1237, 1224, 1211, 1198, 1184, 1171, 1158, 1145, 1132, 1119, 1106, 
  1093, 1082, 1071, 1060, 1049, 1039, 1028, 1017, 1006, 995, 985, 974, 963, 952, 942, 931, 920, 
  913, 906, 899, 892, 885, 878, 871, 864, 857, 850, 843, 836, 829, 822, 815, 808, 801, 795, 789, 
  783, 777, 771, 765, 759, 753, 747, 740, 734, 728, 722, 716, 710, 705, 700, 695, 689, 684, 679, 
  674, 669, 664, 658, 653, 648, 643, 638, 633, 628, 623, 619, 615, 611, 606, 602, 598, 594, 590, 
  585, 581, 577, 573, 568, 564, 560, 556, 553, 549, 545, 541, 538, 534, 530, 526, 523, 519, 515, 
  511, 508, 504, 500, 496, 492, 487, 483, 479, 475, 470, 466, 462, 458, 454, 449, 445, 441, 437, 
  433, 429, 425, 421, 418, 414, 410, 406, 403, 399, 395, 391, 388, 384, 380, 376, 373, 369, 365, 
  361, 358, 354, 350, 346, 343, 339, 335, 331, 328, 324, 320, 316, 313, 308, 303, 298, 294, 289, 
  284, 280, 275, 270, 266, 261, 256, 252, 247, 242, 238, 233, 228, 223, 219, 214, 209, 205, 200, 
  195, 191, 186, 181, 177, 172, 167, 163, 156, 149, 143, 136, 130, 123, 117, 110, 103, 97, 90, 
  84, 77, 71, 64, 58, 49, 40, 31, 22, 13, 4, -5, -14, -23, -32, -40, -49, -58, -67, -76, -85, 
  -105, -124, -144, -164, -183, -203, -223, -242, -262, -282, -302, -321, -341, -361, -380
};

const int16_t oil_temp_celsius_x10[256]
{ 
  1513, 1513, 1513, 1513, 1513, 1513, 1513, 1513, 1513, 1513, 1513, 1513, 1513, 1513, 1513, 1513, 
  1513, 1510, 1508, 1505, 1503, 1501, 1498, 1496, 1494, 1491, 1489, 1487, 1484, 1482, 1480, 1477, 
  1475, 1463, 1451, 1438, 1426, 1414, 1402, 1390, 1378, 1365, 1353, 1341, 1329, 1317, 1304, 1292, 
  1280, 1271, 1262, 1253, 1244, 1235, 1227, 1218, 1209, 1200, 1191, 1182, 1173, 1164, 1155, 1146, 
  1138, 1131, 1124, 1118, 1111, 1105, 1098, 1092, 1085, 1078, 1072, 1065, 1059, 1052, 1046, 1039, 
  1033, 1026, 1020, 1014, 1008, 1002, 996, 990, 984, 978, 972, 965, 959, 953, 947, 941, 935, 930, 
  925, 920, 914, 909, 904, 899, 894, 889, 883, 878, 873, 868, 863, 858, 853, 848, 843, 838, 834, 
  829, 824, 820, 815, 810, 806, 801, 796, 792, 787, 782, 778, 773, 768, 763, 759, 754, 749, 745, 
  740, 735, 731, 726, 721, 717, 712, 707, 703, 698, 693, 688, 684, 679, 674, 670, 665, 660, 656, 
  651, 646, 642, 637, 632, 628, 623, 618, 613, 609, 604, 599, 595, 590, 585, 581, 576, 571, 567, 
  562, 557, 553, 547, 542, 537, 532, 527, 522, 516, 511, 506, 501, 496, 491, 485, 480, 475, 470, 
  464, 458, 452, 446, 440, 433, 427, 421, 415, 409, 403, 397, 391, 385, 379, 373, 365, 358, 350, 
  343, 335, 328, 320, 313, 305, 298, 290, 283, 275, 268, 260, 253, 240, 228, 216, 204, 192, 179, 
  167, 155, 143, 131, 118, 106, 94, 82, 70, 58, 29, 0, -28, -57, -85, -114, -143, -171, -200, 
  -228, -257, -286, -314, -343, -371
};

const int16_t evap_temp_celsius_x10[256]
{
  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  
  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  
  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  322,  
  322,  322,  322,  322,  322,  322,  322,  322,  322,  319,  314,  308,  303,  297,  292,  289,  
  283,  278,  272,  269,  264,  258,  256,  250,  247,  242,  236,  233,  228,  225,  219,  217,  
  211,  208,  206,  200,  197,  192,  189,  186,  181,  178,  172,  169,  167,  161,  158,  156,  
  153,  147,  144,  142,  136,  133,  131,  128,  122,  119,  117,  114,  111,  106,  103,  100,  
  97,  94,  89,  86,  83,  81,  78,  75,  69,  67,  64,  61,  58,  56,  53,  47,  44,  42,  39,  
  36,  33,  31,  28,  22,  19,  17,  14,  11,  8,  6,  3,  -3,  -6,  -8,  -11,  -14,  -17,  -19,  
  -22,  -25,  -31,  -33,  -36,  -39,  -42,  -44,  -47,  -50,  -56,  -58,  -61,  -64,  -67,  -67,  
  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  
  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  
  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  
  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  
  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  
  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67
};


const int16_t AC_evap_temp_celsius_x10[256]
{
 525, 519, 516, 513, 508, 505, 502, 497, 494, 491, 486, 483, 480, 477, 472, 469, 466, 461, 458, 
 455, 450, 447, 444, 441, 436, 433, 430, 425, 422, 419, 413, 411, 408, 405, 400, 397, 394, 388, 
 386, 383, 377, 375, 372, 369, 363, 361, 358, 352, 350, 347, 341, 338, 336, 333, 327, 325, 322, 
 319, 313, 308, 302, 297, 291, 288, 283, 277, 272, 269, 263, 258, 255, 250, 247, 241, 236, 233, 
 227, 225, 219, 216, 211, 208, 205, 200, 197, 191, 188, 186, 180, 177, 172, 169, 166, 161, 158, 
 155, 152, 147, 144, 141, 136, 133, 130, 127, 122, 119, 116, 113, 111, 105, 102, 100, 97, 94, 88, 
 86, 83, 80, 77, 75, 69, 66, 63, 61, 58, 55, 52, 47, 44, 41, 38, 36, 33, 30, 27, 22, 19, 16, 13, 
 11, 8, 5, 2, -3, -6, -9, -12, -14, -17, -20, -23, -25, -31, -34, -37, -39, -42, -45, -48, -50, 
 -56, -59, -62, -64, -70, -73, -75, -81, -84, -87, -92, -95, -98, -100, -106, -109, -112, -117, 
 -120, -123, -128, -131, -134, -137, -142, -145, -148, -153, -156, -159, -164, -167, -170, -173, 
 -178, -181, -184, -189, -192, -195, -200, -203, -206, -209, -214, -217, -220, -225, -228, -231, 
 -237, -239, -242, -245, -250, -253, -256, -262, -264, -267, -273, -275, -278, -281, -287, -289, 
 -292, -298, -300, -303, -309, -312, -314, -317, -323, -325, -328, -334, -337, -339, -345, -348, 
 -350, -353, -359, -362, -364, -370, -373, -375, -381, -384, -387, -389, -395, -398
};

class ALDLParser
{
       public:
       void attach(A172ALDL* ALDLdata) { this->data = ALDLdata; };
       void parse();
       void resetCounter() { this->errCNT = 0; }
       uint8_t getErrCount() { return this->errCNT; }
       char** getErrTexts() { return this->errors; }
       char*  getLBLMPct(); 
       char*  getRBLMPct(); 
       char*  getKnockrtdDeg();
       char*  getCoolC();    
       char*  getMatTempC();  
       char*  getOilTempC();  
       char*  getRPM();      

       private:
       uint8_t errCNT = 0;
       A172ALDL* data;
       char* errors[40] = {0};
       char ret_buf[8];
       void makeFloatStr(int32_t inValx10);
       
};

void ALDLParser::makeFloatStr(int32_t inValx10)
{
       // Clear buffer
       *(uint64_t*)this->ret_buf = 0ULL;

       if(inValx10 == 0)
       {
              this->ret_buf[0] = '0';
              this->ret_buf[1] = '.';
              this->ret_buf[2] = '0';
              this->ret_buf[3] = '\0';
       }
       else
       {
              char buf[7];
              itoa(inValx10, buf, 10);
              uint8_t len = strlen(buf);

              // Copy everything except last digit
              for(uint8_t i = 0; i < len - 1; i++)
              {
                     this->ret_buf[i] = buf[i];
              }

              // Add decimal dot 1234
              this->ret_buf[len - 1] = '.';

              // Add last digit
              this->ret_buf[len] = buf[len - 1];
       }
}

//LEFT BANK BLOCK LEARN MULTIPLIER
//N = COUNTS
char* ALDLParser::getLBLMPct()
{
       int32_t LBLMx10 = (uint32_t)this->data->LBLM * 10L;
       LBLMx10 = (LBLMx10 * 100) >> 7; // (LBLMx10 * 100) / 128
       this->makeFloatStr(LBLMx10);
       return this->ret_buf;   
}

//RIGHT BANK BLOCK LEARN MULTIPLIER
//N = COUNTS
char* ALDLParser::getRBLMPct()
{        
       int32_t RBLMx10 = (uint32_t)this->data->RBLM * 10L;
       RBLMx10 = (RBLMx10 * 100) >> 7; // (RBLMx10 * 100) / 128
       this->makeFloatStr(RBLMx10);
       return this->ret_buf;
}

//RETARD DUE TO KNOCK
//DEGREES = N/2
char* ALDLParser::getKnockrtdDeg()     
{
       int32_t retx10 = (int32_t)this->data->NOCKRTD * 10L;
       this->makeFloatStr(retx10);
       return this->ret_buf;    
}

//COOLANT TEMPERATURE LINEARIZED  (NON-DEFAULTED)
//DEGREES C = (N*.75 - 40)
char* ALDLParser::getCoolC()
{
       int32_t tempx10 = (int32_t)this->data->COOLDEGA * 750L / 100L - 400L;
       //tempx10 = ((tempx10 >> 2) - 40) * 10L;
       this->makeFloatStr(tempx10);
       return this->ret_buf;
}

//A/D RESULT FOR MANIFOLD TEMPERATURE INPUT
//SEE MAT LOOK-UP TABLE  (DEFAULTED)
char* ALDLParser::getMatTempC()
{      
       int32_t tempx10 = MAT_temp_celsius_x10[this->data->ADMAT];
       this->makeFloatStr(tempx10);
       return this->ret_buf;
}

//A/D RESULT FOR OIL TEMPERATURE SENSOR INPUT
//SEE OIL TEMP. LOOK-UP TABLE (NON-DEFAULTED)
char* ALDLParser::getOilTempC()
{
       int32_t tempx10 = oil_temp_celsius_x10[this->data->ADOILTMP];
       this->makeFloatStr(tempx10);
       return this->ret_buf;
}

//ENGINE SPEED
//RPM = N * 25
char* ALDLParser::getRPM()
{      
       uint16_t RPM = (uint16_t)this->data->NTRPMX * (uint16_t)25;
       itoa(RPM, this->ret_buf, 10);
       return this->ret_buf;
}

void ALDLParser::parse()
{       
       this->resetCounter();
       
       for (uint8_t i = 0; i < 40; i++)
       {
              switch (i)
              {
                     case 0 ... 7:
                            if (bitRead(this->data->MALFFLG1, i))
                            {
                                   this->errors[this->errCNT] = (char*)MALFFLG_codes[i];
                                   this->errCNT++;
                            }                     
                     break;

                     case 8 ... 15:
                            if (bitRead(this->data->MALFFLG2, i - 8))
                            {
                                   this->errors[this->errCNT] = (char*)MALFFLG_codes[i];
                                   this->errCNT++;
                            }                     
                     break;

                     case 16 ... 23:
                            if (bitRead(this->data->MALFFLG3, i - 16))
                            {
                                   this->errors[this->errCNT] = (char*)MALFFLG_codes[i];
                                   this->errCNT++;
                            }                     
                     break;

                     case 24 ... 31:
                            if (bitRead(this->data->MALFFLG4, i - 24))
                            {
                                   this->errors[this->errCNT] = (char*)MALFFLG_codes[i];
                                   this->errCNT++;
                            }                     
                     break;

                     case 32 ... 39:
                            if (bitRead(this->data->MALFFLG5, i - 32))
                            {
                                   this->errors[this->errCNT] = (char*)MALFFLG_codes[i];
                                   this->errCNT++;
                            }                     
                     break;
              
              default:
                     break;
              }
       }
}