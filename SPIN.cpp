/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SPIN.h"
#include "pins_arduino.h"

////////////////////////
/**********************************************************/
/*   SPIN0                                                */
/**********************************************************/

void SPIN0Class::begin() {SPI.begin(); }
void SPIN0Class::usingInterrupt(uint8_t n) {SPI.usingInterrupt(n); }
void SPIN0Class::usingInterrupt(IRQ_NUMBER_t interruptName) {SPI.usingInterrupt(interruptName); }
void SPIN0Class::notUsingInterrupt(IRQ_NUMBER_t interruptName) {SPI.notUsingInterrupt(interruptName); }

// Before using SPI.transfer() or asserting chip select pins,
// this function is used to gain exclusive access to the SPI bus
// and configure the correct settings.
void SPIN0Class::beginTransaction(SPISettings settings) {SPI.beginTransaction(settings);}

// Write to the SPI bus (MOSI pin) and also receive (MISO pin)
uint8_t SPIN0Class::transfer(uint8_t data) {return SPI.transfer(data);}
uint16_t SPIN0Class::transfer16(uint16_t data) {return SPI.transfer16(data);}
void SPIN0Class::transfer(void *buf, size_t count) {SPI.transfer(buf, count); }

// After performing a group of transfers and releasing the chip select
// signal, this function allows others to access the SPI bus
void SPIN0Class::endTransaction(void) {SPI.endTransaction();}
void SPIN0Class::end() {SPI.end();}

void SPIN0Class::setBitOrder(uint8_t bitOrder) {SPI.setBitOrder(bitOrder);}
void SPIN0Class::setDataMode(uint8_t dataMode) {SPI.setDataMode(dataMode);}
void SPIN0Class::setClockDivider(uint8_t clockDiv) {SPI.setClockDivider(clockDiv);}

// Teensy 3.x can use alternate pins for these 3 SPI signals.
void SPIN0Class::setMOSI(uint8_t pin) {SPI.setMOSI(pin);}
void SPIN0Class::setMISO(uint8_t pin) {SPI.setMISO(pin);}
void SPIN0Class::setSCK(uint8_t pin) {SPI.setSCK(pin);}

bool SPIN0Class::pinIsMOSI(uint8_t pin)
{
    if ((pin == 11) || (pin == 7)) return true;
    return false;
}

bool SPIN0Class::pinIsMISO(uint8_t pin)
{
    if ((pin == 12) || (pin == 8)) return true;
    return false;
}

bool SPIN0Class::pinIsSCK(uint8_t pin)
{
    if ((pin == 13) || (pin == 14)) return true;
    return false;
}


// return true if "pin" has special chip select capability
uint8_t SPIN0Class::pinIsChipSelect(uint8_t pin) {return SPI.pinIsChipSelect(pin);}
bool SPIN0Class::pinIsChipSelect(uint8_t pin1, uint8_t pin2) {return SPI.pinIsChipSelect(pin1, pin2);}
uint8_t SPIN0Class::setCS(uint8_t pin) {return SPI.setCS(pin);}


// Helper functions for managing queue
#if defined(KINETISK)
uint8_t SPIN0Class::sizeFIFO() {return 4;}

KINETISK_SPI_t *SPIN0Class::kinetisk_spi (void) {
    return &KINETISK_SPI0;
}


void SPIN0Class::waitFifoNotFull(void) {
    uint32_t sr;
    uint32_t tmp __attribute__((unused));
    do {
        sr = KINETISK_SPI0.SR;
        if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
    } while ((sr & (15 << 12)) > ((4-1) << 12));
}
void SPIN0Class::waitFifoEmpty(void) {
    uint32_t sr;
    uint32_t tmp __attribute__((unused));
    do {
        sr = KINETISK_SPI0.SR;
        if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
    } while ((sr & 0xF0F0) > 0);             // wait both RX & TX empty
}
void SPIN0Class::waitTransmitComplete(void)  {
    uint32_t tmp __attribute__((unused));
    while (!(KINETISK_SPI0.SR & SPI_SR_TCF)) ; // wait until final output done
    tmp = KINETISK_SPI0.POPR;                  // drain the final RX FIFO word
}
void SPIN0Class::waitTransmitComplete(uint32_t mcr) {
    uint32_t tmp __attribute__((unused));
    while (1) {
        uint32_t sr = KINETISK_SPI0.SR;
        if (sr & SPI_SR_EOQF) break;  // wait for last transmit
        if (sr &  0xF0) tmp = KINETISK_SPI0.POPR;
    }
    KINETISK_SPI0.SR = SPI_SR_EOQF;
    KINETISK_SPI0.MCR = mcr;
    while (KINETISK_SPI0.SR & 0xF0) {
        tmp = KINETISK_SPI0.POPR;
    }
}
#endif


SPIN0Class SPIN;


#if defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(KINETISL)
/**********************************************************/
/*   SPIN1                                               */
/**********************************************************/

void SPIN1Class::begin() {SPI1.begin(); }
void SPIN1Class::usingInterrupt(uint8_t n) {SPI1.usingInterrupt(n); }
void SPIN1Class::usingInterrupt(IRQ_NUMBER_t interruptName) {SPI1.usingInterrupt(interruptName); }
void SPIN1Class::notUsingInterrupt(IRQ_NUMBER_t interruptName) {SPI1.notUsingInterrupt(interruptName); }

// Before using SPI1.transfer() or asserting chip select pins,
// this function is used to gain exclusive access to the SPI bus
// and configure the correct settings.
void SPIN1Class::beginTransaction(SPISettings settings) {SPI1.beginTransaction(settings);}

// Write to the SPI bus (MOSI pin) and also receive (MISO pin)
uint8_t SPIN1Class::transfer(uint8_t data) {return SPI1.transfer(data);}
uint16_t SPIN1Class::transfer16(uint16_t data) {return SPI1.transfer16(data);}
void SPIN1Class::transfer(void *buf, size_t count) {SPI1.transfer(buf, count); }

// After performing a group of transfers and releasing the chip select
// signal, this function allows others to access the SPI bus
void SPIN1Class::endTransaction(void) {SPI1.endTransaction();}
void SPIN1Class::end() {SPI1.end();}

void SPIN1Class::setBitOrder(uint8_t bitOrder) {SPI1.setBitOrder(bitOrder);}
void SPIN1Class::setDataMode(uint8_t dataMode) {SPI1.setDataMode(dataMode);}
void SPIN1Class::setClockDivider(uint8_t clockDiv) {SPI1.setClockDivider(clockDiv);}

// Teensy 3.x can use alternate pins for these 3 SPI signals.
void SPIN1Class::setMOSI(uint8_t pin) {SPI1.setMOSI(pin);}
void SPIN1Class::setMISO(uint8_t pin) {SPI1.setMISO(pin);}
void SPIN1Class::setSCK(uint8_t pin) {SPI1.setSCK(pin);}
bool SPIN1Class::pinIsMOSI(uint8_t pin)
{
    switch (pin) {
    case 0: 
    case 21:
    case 61:
    case 59:
        //Serial.printf("Valid MOSI %d\n\r", pin);
        return true;
    }
    return false;

}

bool SPIN1Class::pinIsMISO(uint8_t pin)
{
    switch (pin) {
    case 1: 
    case 5: 
    case 61:
    case 59:
        //Serial.printf("Valid MISO %d\n\r", pin);
        return true;
    }
    return false;
}

bool SPIN1Class::pinIsSCK(uint8_t pin)
{
    switch (pin) {
    case 20:
    case 32:
    case 60: 
        //Serial.printf("Valid SCK %d\n\r", pin);
        return true;
    }
    return false;
}

// return true if "pin" has special chip select capability
uint8_t SPIN1Class::pinIsChipSelect(uint8_t pin) {return SPI1.pinIsChipSelect(pin);}
bool SPIN1Class::pinIsChipSelect(uint8_t pin1, uint8_t pin2) {return SPI1.pinIsChipSelect(pin1, pin2);}
uint8_t SPIN1Class::setCS(uint8_t pin) {return SPI1.setCS(pin);}

// Helper functions for managing queue
#if defined(KINETISK)
uint8_t SPIN1Class::sizeFIFO() {return 1;}
KINETISK_SPI_t *SPIN1Class::kinetisk_spi (void) {
    return &KINETISK_SPI1;
}


void SPIN1Class::waitFifoNotFull(void) {
    uint32_t sr;
    uint32_t tmp __attribute__((unused));
    do {
        sr = KINETISK_SPI1.SR;
        if (sr & 0xF0) tmp = KINETISK_SPI1.POPR;  // drain RX FIFO
    } while ((sr & (15 << 12)) > ((1-1) << 12));
}
void SPIN1Class::waitFifoEmpty(void) {
    uint32_t sr;
    uint32_t tmp __attribute__((unused));
    do {
        sr = KINETISK_SPI1.SR;
        if (sr & 0xF0) tmp = KINETISK_SPI1.POPR;  // drain RX FIFO
    } while ((sr & 0xF0F0) > 0);             // wait both RX & TX empty
}
void SPIN1Class::waitTransmitComplete(void) {
    uint32_t tmp __attribute__((unused));
    while (!(KINETISK_SPI1.SR & SPI_SR_TCF)) ; // wait until final output done
    tmp = KINETISK_SPI1.POPR;                  // drain the final RX FIFO word
}
void SPIN1Class::waitTransmitComplete(uint32_t mcr)  {
    uint32_t tmp __attribute__((unused));
    while (1) {
        uint32_t sr = KINETISK_SPI1.SR;
        if (sr & SPI_SR_EOQF) break;  // wait for last transmit
        if (sr &  0xF0) tmp = KINETISK_SPI1.POPR;
    }
    KINETISK_SPI1.SR = SPI_SR_EOQF;
    KINETISK_SPI1.MCR = mcr;
    while (KINETISK_SPI1.SR & 0xF0) {
        tmp = KINETISK_SPI1.POPR;
    }
}
#endif


SPIN1Class SPIN1;
#endif


#if defined(__MK64FX512__) || defined(__MK66FX1M0__)
/**********************************************************/
/*   SPIN2                                               */
/**********************************************************/

void SPIN2Class::begin() {SPI2.begin(); }
void SPIN2Class::usingInterrupt(uint8_t n) {SPI2.usingInterrupt(n); }
void SPIN2Class::usingInterrupt(IRQ_NUMBER_t interruptName) {SPI2.usingInterrupt(interruptName); }
void SPIN2Class::notUsingInterrupt(IRQ_NUMBER_t interruptName) {SPI2.notUsingInterrupt(interruptName); }

// Before using SPI2.transfer() or asserting chip select pins,
// this function is used to gain exclusive access to the SPI bus
// and configure the correct settings.
void SPIN2Class::beginTransaction(SPISettings settings) {SPI2.beginTransaction(settings);}

// Write to the SPI bus (MOSI pin) and also receive (MISO pin)
uint8_t SPIN2Class::transfer(uint8_t data) {return SPI2.transfer(data);}
uint16_t SPIN2Class::transfer16(uint16_t data) {return SPI2.transfer16(data);}
void SPIN2Class::transfer(void *buf, size_t count) {SPI2.transfer(buf, count); }

// After performing a group of transfers and releasing the chip select
// signal, this function allows others to access the SPI bus
void SPIN2Class::endTransaction(void) {SPI2.endTransaction();}
void SPIN2Class::end() {SPI2.end();}

void SPIN2Class::setBitOrder(uint8_t bitOrder) {SPI2.setBitOrder(bitOrder);}
void SPIN2Class::setDataMode(uint8_t dataMode) {SPI2.setDataMode(dataMode);}
void SPIN2Class::setClockDivider(uint8_t clockDiv) {SPI2.setClockDivider(clockDiv);}

// Teensy 3.x can use alternate pins for these 3 SPI signals.
void SPIN2Class::setMOSI(uint8_t pin) {SPI2.setMOSI(pin);}
void SPIN2Class::setMISO(uint8_t pin) {SPI2.setMISO(pin);}
void SPIN2Class::setSCK(uint8_t pin) {SPI2.setSCK(pin);}
bool SPIN2Class::pinIsMOSI(uint8_t pin)
{
    switch (pin) {
    case 44: 
    case 52:
        return true;
    }
    return false;

}

bool SPIN2Class::pinIsMISO(uint8_t pin)
{
    switch (pin) {
    case 45: 
    case 51: 
        return true;
    }
    return false;
}

bool SPIN2Class::pinIsSCK(uint8_t pin)
{
    switch (pin) {
    case 46:
    case 53:
        return true;
    }
    return false;
}

// return true if "pin" has special chip select capability
uint8_t SPIN2Class::pinIsChipSelect(uint8_t pin) {return SPI2.pinIsChipSelect(pin);}
bool SPIN2Class::pinIsChipSelect(uint8_t pin1, uint8_t pin2) {return SPI2.pinIsChipSelect(pin1, pin2);}
uint8_t SPIN2Class::setCS(uint8_t pin) {return SPI2.setCS(pin);}

// Helper functions for managing queue
#if defined(KINETISK)
uint8_t SPIN2Class::sizeFIFO() {return 1;}
KINETISK_SPI_t *SPIN2Class::kinetisk_spi (void) {
    return &KINETISK_SPI2;
}


void SPIN2Class::waitFifoNotFull(void) {
    uint32_t sr;
    uint32_t tmp __attribute__((unused));
    do {
        sr = KINETISK_SPI2.SR;
        if (sr & 0xF0) tmp = KINETISK_SPI2.POPR;  // drain RX FIFO
    } while ((sr & (15 << 12)) > ((1-1) << 12));
}
void SPIN2Class::waitFifoEmpty(void) {
    uint32_t sr;
    uint32_t tmp __attribute__((unused));
    do {
        sr = KINETISK_SPI2.SR;
        if (sr & 0xF0) tmp = KINETISK_SPI2.POPR;  // drain RX FIFO
    } while ((sr & 0xF0F0) > 0);             // wait both RX & TX empty
}
void SPIN2Class::waitTransmitComplete(void) {
    uint32_t tmp __attribute__((unused));
    while (!(KINETISK_SPI2.SR & SPI_SR_TCF)) ; // wait until final output done
    tmp = KINETISK_SPI2.POPR;                  // drain the final RX FIFO word
}
void SPIN2Class::waitTransmitComplete(uint32_t mcr)  {
    uint32_t tmp __attribute__((unused));
    while (1) {
        uint32_t sr = KINETISK_SPI2.SR;
        if (sr & SPI_SR_EOQF) break;  // wait for last transmit
        if (sr &  0xF0) tmp = KINETISK_SPI2.POPR;
    }
    KINETISK_SPI2.SR = SPI_SR_EOQF;
    KINETISK_SPI2.MCR = mcr;
    while (KINETISK_SPI2.SR & 0xF0) {
        tmp = KINETISK_SPI2.POPR;
    }
}
#endif


SPIN2Class SPIN2;
#endif
