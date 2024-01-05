#pragma once
#include "ch32v003fun.h"
#include "ch32v003_GPIO_branchless.h"

#define PA1 (GPIOv_from_PORT_PIN(GPIO_port_A, 1))
#define PA2 (GPIOv_from_PORT_PIN(GPIO_port_A, 2))

#define PC0 (GPIOv_from_PORT_PIN(GPIO_port_C, 0))
#define PC1 (GPIOv_from_PORT_PIN(GPIO_port_C, 1))
#define PC2 (GPIOv_from_PORT_PIN(GPIO_port_C, 2))
#define PC3 (GPIOv_from_PORT_PIN(GPIO_port_C, 3))
#define PC4 (GPIOv_from_PORT_PIN(GPIO_port_C, 4))
#define PC5 (GPIOv_from_PORT_PIN(GPIO_port_C, 5))
#define PC6 (GPIOv_from_PORT_PIN(GPIO_port_C, 6))
#define PC7 (GPIOv_from_PORT_PIN(GPIO_port_C, 7))

#define PD0 (GPIOv_from_PORT_PIN(GPIO_port_D, 0))
#define PD1 (GPIOv_from_PORT_PIN(GPIO_port_D, 1))
#define PD2 (GPIOv_from_PORT_PIN(GPIO_port_D, 2))
#define PD3 (GPIOv_from_PORT_PIN(GPIO_port_D, 3))
#define PD4 (GPIOv_from_PORT_PIN(GPIO_port_D, 4))
#define PD5 (GPIOv_from_PORT_PIN(GPIO_port_D, 5))
#define PD6 (GPIOv_from_PORT_PIN(GPIO_port_D, 6))
#define PD7 (GPIOv_from_PORT_PIN(GPIO_port_D, 7))

#define SCL PC2
#define SDA PC1

#define TX PD5
#define RX PD6

#define MISO PC7
#define MOSI PC6
#define SCK  PC5