#pragma once
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
#include <string.h>    // strlen

#include "include/cube_defs.h"

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

class ALDLParser
{
       public:
       void attach(A172ALDL*);
       void parse();
       void resetCounter();
       uint8_t getErrCount();
       char** getErrTexts();
       char*  getLBLMPct(); 
       char*  getRBLMPct();
       char*  getLINTPct();
       char*  getRINTPct();
       char*  getBLMCell(); 
       char*  getKnockrtdDeg();
       char*  getCoolC();    
       char*  getMatTempC();  
       char*  getOilTempC();  
       char*  getRPM();
       char*  getDesiredIdleRPM();
       char*  getTPSPct();
       char*  getEGRDutyPct();
       char*  getMAPkpa();
       char*  getBarometerkpa();   
       char*  getVoltage();
       bool   validateChecksum();   

       private:
       uint8_t errCNT = 0;
       A172ALDL* data;
       char* errors[40] = {0};
       char ret_buf[8];
       void makeFloatStr(int32_t inValx10, char symbol);
       uint8_t generateChecksum(uint8_t *buf, size_t len);
};