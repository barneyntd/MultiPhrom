//
//  hardwaremap.h
//  MultiPhrom
//
//  Created by Barney Hilken on 20/03/2020.
//  Copyright © 2020 Barney Hilken. All rights reserved.
//


#ifndef HARDWAREMAP_H_
#define HARDWAREMAP_H_

// Global includes
#include <avr/io.h>

// Some useful definitions
#define FALSE	0
#define TRUE	1
#define NULL    0


// TMS6100 IO Pins

// M0 (PC5)
#define M0_BITMASK          (1 << 5)
#define SET_M0_INPUT        DDRC &= ~M0_BITMASK
#define SET_M0_NOPULLUP     PORTC &= ~M0_BITMASK
#define M0                  ((PINC & M0_BITMASK) != 0)

// M1 (PD0)
#define M1_BITMASK          (1 << 0)
#define SET_M1_INPUT        DDRD &= ~M1_BITMASK
#define SET_M1_NOPULLUP     PORTD &= ~M1_BITMASK
#define M1                  ((PIND & M1_BITMASK) != 0)

// ROMCLK (PC4)
#define CLK_BITMASK         (1 << 4)
#define SET_CLK_INPUT       DDRC &= ~CLK_BITMASK
#define SET_CLK_NOPULLUP    PORTC &= ~CLK_BITMASK
#define CLK                 ((PINC & CLK_BITMASK) != 0)

// ADD (PC0-3)
#define ADD1_BIT            0
#define ADD_BITMASK         (0x0F << ADD1_BIT)
#define SET_ADD_INPUT       DDRC &= ~ADD_BITMASK
#define SET_ADD_NOPULLUP    PORTC &= ~ADD_BITMASK
#define ADD                 ((PINC & ADD_BITMASK) >> ADD1_BIT)

// ADD8 (ADD >> 3)
#define ADD8_BITMASK        (1 << (ADD1_BIT + 3))
#define ADD8                ((PINC & ADD8_BITMASK) != 0)

// DATA (PD2), connected to ADD8 via resistor
#define DATA_BITMASK        (1 << 2)
#define SET_DATA_INPUT      DDRD &= ~DATA_BITMASK
#define SET_DATA_OUTPUT     DDRD |= DATA_BITMASK
#define SET_DATA(b)         PORTD = (PORTD & ~DATA_BITMASK) | ((b) ? DATA_BITMASK : 0)

// SPI interface

// MISO (PB4)
#define MISO_BITMASK        (1 << 4)
#define SET_MISO_INPUT      DDRB &= ~MISO_BITMASK
#define SET_MISO_NOPULLUP   PORTB &= ~MISO_BITMASK
#define MISO                ((PINB & MISO_BITMASK) != 0)

// MOSI (PB3)
#define MOSI_BIT            3
#define MOSI_BITMASK        (1 << MOSI_BIT)
#define SET_MOSI_OUTPUT     DDRB |= MOSI_BITMASK
#define SET_MOSI(b)         PORTB = (PORTB & ~MOSI_BITMASK) | ((b) ? MOSI_BITMASK : 0)

// SCK (PB5)
#define SCK_BIT             5
#define SCK_BITMASK         (1 << SCK_BIT)
#define SET_SCK_OUTPUT      DDRB |= SCK_BITMASK
#define SET_SCK(b)          PORTB = (PORTB & ~SCK_BITMASK) | ((b) ? SCK_BITMASK : 0)

// CHIP (PB0)
#define CHIP_BIT            0
#define CHIP_BITMASK        (1 << CHIP_BIT)
#define SET_CHIP_OUTPUT     DDRB |= CHIP_BITMASK
#define SET_CHIP(b)         PORTB = (PORTB & ~CHIP_BITMASK) | ((b) ? CHIP_BITMASK : 0)

// CARD (PB2)
#define CARD_BIT            2
#define CARD_BITMASK        (1 << CARD_BIT)
#define SET_CARD_OUTPUT     DDRB |= CARD_BITMASK
#define SET_CARD(b)         PORTB = (PORTB & ~CARD_BITMASK) | ((b) ? CARD_BITMASK : 0)

// Other IO pins

// LED (PD1)
#define LED_BIT             1
#define LED_BITMASK         (1 << LED_BIT)
#define SET_LED_OUTPUT      DDRD |= LED_BITMASK
#define SET_LED(b)          PORTD = (PORTD & ~LED_BITMASK) | ((b) ? LED_BITMASK : 0)

// DTCT (PD5)
#define DTCT_BITMASK        (1 << 5)
#define SET_DTCT_INPUT      DDRD &= ~DTCT_BITMASK
#define SET_DTCT_PULLUP     PORTD |= DTCT_BITMASK
#define DTCT                ((PIND & DTCT_BITMASK) != 0)
#define DTCT_INT_vect       PCINT2_vect
#define DTCT_INT_enable     PCMSK2 |= (1 << PCINT21); PCICR |= (1 << PCIE2)



// turn off unused features
#define UNUSED_OFF          PRR = (1 << PRADC) | (1 << PRUSART0) | (1 << PRTIM1) | (1 << PRTIM0) | (1 << PRTIM2) | (1 << PRTWI)

#endif /* HARDWAREMAP_H_ */
