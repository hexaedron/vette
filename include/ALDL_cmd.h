#pragma once

// http://easyonlineconverter.com/converters/checksum_converter.html

// Command to initiate ECM connection
const unsigned char pokeECMCmd[4] = {0xF4, 0x56, 0x00, 0xB6};

#define POKE_ECM_RESPONSE_FAST 0xB60056F4

// Command to get ALDL data
const unsigned char getECMDataCmd[5] = {0xF4, 0x57, 0x01, 0x00, 0xB4};

// Clear malfunction codes ECM
const unsigned char clearCodesCmd[4] = {0xF4, 0x56, 0x0A, 0xAC};

// Silent mode
//const unsigned char silentModeCmd[4] = {0xF4, 0x56, 0x08, 0xAE};
const unsigned char silentModeCmd[4] = {0xF1, 0x56, 0x08, 0xB1};

// Back to normal mode (enable comm)
//const unsigned char silentModeOffCmd[4] = {0xF4, 0x56, 0x09, 0xAD};
const unsigned char silentModeOffCmd[4] = {0xF1, 0x56, 0x09, 0xB0};

// Get ABS data
const unsigned char getABSDataCmd[5] = {0xF9, 0x57, 0x01, 0x00, 0xAF};

// Return from ABS to normal mode
const unsigned char returnFromABSCmd[4] = {0xF9, 0x56, 0x00, 0xB1};

// Clear malfunction codes ABS
const unsigned char clearCodesABSCmd[4] = {0xF9, 0x56, 0x0A, 0xA7};