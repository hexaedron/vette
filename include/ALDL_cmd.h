#pragma once

// http://easyonlineconverter.com/converters/checksum_converter.html

// Command to get ALDL data
const unsigned char getECUDataCmd[5] = {0xF4, 0x57, 0x01, 0x00, 0xB4};

// Clear malfunction codes ECU
const unsigned char clearCodesCmd[4] = {0xF4, 0x56, 0x0A, 0xAC};

// Silent mode
const unsigned char silentModeCmd[4] = {0xF4, 0x56, 0x08, 0xAE};

// Back to normal mode (enable comm)
const unsigned char silentModeOffCmd[4] = {0xF4, 0x56, 0x09, 0xAD};

// Get ABS data
const unsigned char getABSDataCmd[5] = {0xF9, 0x57, 0x01, 0x00, 0xAF};

// Return from ABS to normal mode
const unsigned char returnFromABSCmd[4] = {0xF9, 0x56, 0x00, 0xB1};

// Clear malfunction codes ABS
const unsigned char clearCodesABSCmd[4] = {0xF9, 0x56, 0x0A, 0xA7};