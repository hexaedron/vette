#include "include/A172ALDL.h"

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

void    ALDLParser::attach(A172ALDL* ALDLdata) { this->data = ALDLdata; };
void    ALDLParser::resetCounter() { this->errCNT = 0; }
uint8_t ALDLParser::getErrCount() { return this->errCNT; }
char**  ALDLParser::getErrTexts() { return this->errors; }

uint8_t ALDLParser::generateChecksum(uint8_t *buf, size_t len) 
{
       uint8_t x = 0;
       uint8_t sum = 0;
       for(x = 0; x < len; x++) sum += buf[x];
       return ( UINT8_MAX - sum + 1);
};

bool   ALDLParser::validateChecksum()
{
       return this->data->checksum == this->generateChecksum((uint8_t*)this->data, sizeof(A172ALDL) - 1);
};

void ALDLParser::makeFloatStr(int32_t inValx10, char symbol)
{
       // Clear buffer
       *(uint64_t*)this->ret_buf = 0ULL;

       if(inValx10 == 0)
       {
              this->ret_buf[0] = '0';
              this->ret_buf[1] = '.';
              this->ret_buf[2] = '0';
              this->ret_buf[3] = symbol;
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

              // Add symbol
              this->ret_buf[len+1] = symbol;
       }
}

//A/D RESULT FOR IGNITION VOLTAGE INPUT
//VOLTS = N/10
char* ALDLParser::getVoltage()
{
       int32_t Voltx10 = (int32_t)this->data->ADBAT;
       this->makeFloatStr(Voltx10, 'V');
       return this->ret_buf;   
}

//BLOCK LEARN MULTIPLIER CELL NUMBER
//N = NUMBER
char* ALDLParser::getBLMCell()
{
       itoa(this->data->BLMCELL, this->ret_buf, 10);
       return this->ret_buf;   
}

//LEFT BANK BLOCK LEARN MULTIPLIER
//N = COUNTS
char* ALDLParser::getLBLMPct()
{
       int32_t LBLMx10 = (int32_t)this->data->LBLM * 10L;
       LBLMx10 = ((LBLMx10 * 100) >> 7) - 1000L; // (LBLMx10 * 100) / 128
       this->makeFloatStr(LBLMx10, '\0');
       return this->ret_buf;   
}

//RIGHT BANK BLOCK LEARN MULTIPLIER
//N = COUNTS
char* ALDLParser::getRBLMPct()
{        
       int32_t RBLMx10 = (int32_t)this->data->RBLM * 10L;
       RBLMx10 = ((RBLMx10 * 100) >> 7) - 1000L; // (RBLMx10 * 100) / 128
       this->makeFloatStr(RBLMx10, '\0');
       return this->ret_buf;
}

//INTEGRATOR VALUE FOR LEFT O2 SENSOR
//N = COUNTS
char* ALDLParser::getLINTPct()
{
       int32_t LINTx10 = (int32_t)this->data->LINT * 10L;
       LINTx10 = ((LINTx10 * 100) >> 7) - 1000L; // (LINTx10 * 100) / 128
       this->makeFloatStr(LINTx10, '\0');
       return this->ret_buf;   
}

//INTEGRATOR VALUE FOR RIGHT O2 SENSOR
//N = COUNTS
char* ALDLParser::getRINTPct()
{
       int32_t RINTx10 = (int32_t)this->data->RINT * 10L;
       RINTx10 = ((RINTx10 * 100) >> 7) - 1000L; // (RINTx10 * 100) / 128
       this->makeFloatStr(RINTx10, '\0');
       return this->ret_buf;   
}

//THROTTLE LOAD AXIS VARIABLE
//% THROTTLE = N/2.56
char* ALDLParser::getTPSPct()
{
       int32_t TPSx10 = (int32_t)this->data->NTPSLD * 10L + 10L;
       TPSx10 = ((TPSx10 * 100) >> 8); // (TPSx10 * 100) / 256
       this->makeFloatStr(TPSx10, '\0');
       return this->ret_buf;   
}

//A/D RESULT FOR MANIFOLD PRESSURE SENSOR INPUT
//(kpa = (N + 28.06)/2.71)
char* ALDLParser::getMAPkpa()
{
       uint32_t MAPx100 = (uint32_t)this->data->ADMAP * 100L;
       MAPx100 = (MAPx100 + 2806) * 10 / 271;
       this->makeFloatStr(MAPx100 / 10, '\0');
       return this->ret_buf;   
}

//NON - VOLITILE COMPUTED A/D BAROMETER
//KPA = (N + 28.06)/2.71)
char* ALDLParser::getBarometerkpa()
{
       uint32_t barometerx100 = (uint32_t)this->data->NVADBARO * 100L;
       barometerx100 = (barometerx100 + 2806) * 10 / 271;
       this->makeFloatStr(barometerx100 / 10, '\0');
       return this->ret_buf;   
}

//EGR DUTY CYCLE
//DC = uint8_t 2.56
char* ALDLParser::getEGRDutyPct()
{
       int32_t EGRx10 = (int32_t)this->data->EGRDC * 10L;
       EGRx10 = ((EGRx10 * 100) >> 8); // (EGRx10 * 100) / 256
       this->makeFloatStr(EGRx10, '\0');
       return this->ret_buf;   
}

//RETARD DUE TO KNOCK
//DEGREES = N/2
char* ALDLParser::getKnockrtdDeg()     
{
       int32_t retx10 = (int32_t)this->data->NOCKRTD * 10L;
       this->makeFloatStr(retx10, '\0');
       return this->ret_buf;    
}

//COOLANT TEMPERATURE LINEARIZED  (NON-DEFAULTED)
//DEGREES C = (N*.75 - 40)
char* ALDLParser::getCoolC()
{
       int32_t tempx10 = (int32_t)this->data->COOLDEGA * 750L / 100L - 400L;
       this->makeFloatStr(tempx10, '\0');
       return this->ret_buf;
}

//A/D RESULT FOR MANIFOLD TEMPERATURE INPUT
//SEE MAT LOOK-UP TABLE  (DEFAULTED)
char* ALDLParser::getMatTempC()
{      
       int32_t tempx10 = MAT_temp_celsius_x10[this->data->ADMAT];
       this->makeFloatStr(tempx10, '\0');
       return this->ret_buf;
}

//A/D RESULT FOR OIL TEMPERATURE SENSOR INPUT
//SEE OIL TEMP. LOOK-UP TABLE (NON-DEFAULTED)
char* ALDLParser::getOilTempC()
{
       int32_t tempx10 = oil_temp_celsius_x10[this->data->ADOILTMP];
       this->makeFloatStr(tempx10, '\0');
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

//DESIRED IDLE SPEED
//RPM = N * 12.5
char* ALDLParser::getDesiredIdleRPM()
{
       uint32_t RPM = (uint32_t)this->data->ISESDD * 12UL + ((uint32_t)this->data->ISESDD >> 1);
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