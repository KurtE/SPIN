/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _SPIN_H_INCLUDED
#define _SPIN_H_INCLUDED

#ifdef __cplusplus

#include <Arduino.h>
#include <SPI.h>

/**********************************************************/
/*    SPIN class definition                               */
/**********************************************************/
class SPINClass {
public:
    // Initialize the SPI library
    virtual void begin() = 0;
    virtual void usingInterrupt(uint8_t n) = 0;
    virtual void usingInterrupt(IRQ_NUMBER_t interruptName) = 0;
    virtual void notUsingInterrupt(IRQ_NUMBER_t interruptName) = 0;

    // Before using SPI.transfer() or asserting chip select pins,
    // this function is used to gain exclusive access to the SPI bus
    // and configure the correct settings.
    virtual void beginTransaction(SPISettings settings) = 0;

    // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
    virtual uint8_t transfer(uint8_t data) = 0;
    virtual uint16_t transfer16(uint16_t data) = 0;
    virtual void transfer(void *buf, size_t count) = 0;

    // After performing a group of transfers and releasing the chip select
    // signal, this function allows others to access the SPI bus
    virtual void endTransaction(void) = 0;
    virtual void end() = 0;

    virtual void setBitOrder(uint8_t bitOrder) = 0;
    virtual void setDataMode(uint8_t dataMode) = 0;
    virtual void setClockDivider(uint8_t clockDiv) = 0;

    // Teensy 3.x can use alternate pins for these 3 SPI signals.
    virtual void setMOSI(uint8_t pin) = 0;
    virtual void setMISO(uint8_t pin) = 0;
    virtual void setSCK(uint8_t pin) = 0;
    // Added ones that are not part of SPI (yet)
    virtual bool pinIsMOSI(uint8_t pin) = 0;
    virtual bool pinIsMISO(uint8_t pin) = 0;
    virtual bool pinIsSCK(uint8_t pin) = 0;

    // return true if "pin" has special chip select capability
    virtual uint8_t pinIsChipSelect(uint8_t pin) = 0;
    virtual bool pinIsChipSelect(uint8_t pin1, uint8_t pin2) = 0;
    virtual uint8_t setCS(uint8_t pin) = 0;

    // Add helper functions from ILI9341, which manage SPI FIFO
#if defined(KINETISK)
    virtual KINETISK_SPI_t *kinetisk_spi (void) = 0;

    virtual uint8_t sizeFIFO() = 0;
    virtual void waitFifoNotFull(void) = 0;
    virtual void waitFifoEmpty(void) = 0;
    virtual void waitTransmitComplete(void)  = 0;
    virtual void waitTransmitComplete(uint32_t mcr) = 0;

    // return DMA Channel information 
    virtual uint8_t dmaTXEvent(void) = 0;
    virtual uint8_t dmaRXEvent(void) = 0;
#endif
};

/**********************************************************/
/*   SPIN0                                                */
/**********************************************************/
class SPIN0Class : public SPINClass {
public:
    // Initialize the SPI library
    virtual void begin();
    virtual void usingInterrupt(uint8_t n);
    virtual void usingInterrupt(IRQ_NUMBER_t interruptName);
    virtual void notUsingInterrupt(IRQ_NUMBER_t interruptName);

    // Before using SPI.transfer() or asserting chip select pins,
    // this function is used to gain exclusive access to the SPI bus
    // and configure the correct settings.
    virtual void beginTransaction(SPISettings settings);

    // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
    virtual uint8_t transfer(uint8_t data);
    virtual uint16_t transfer16(uint16_t data);
    virtual void transfer(void *buf, size_t count);

    // After performing a group of transfers and releasing the chip select
    // signal, this function allows others to access the SPI bus
    virtual void endTransaction(void);
    virtual void end();

    virtual void setBitOrder(uint8_t bitOrder);
    virtual void setDataMode(uint8_t dataMode);
    virtual void setClockDivider(uint8_t clockDiv);

    // Teensy 3.x can use alternate pins for these 3 SPI signals.
    virtual void setMOSI(uint8_t pin);
    virtual void setMISO(uint8_t pin);
    virtual void setSCK(uint8_t pin);
    virtual bool pinIsMOSI(uint8_t pin);
    virtual bool pinIsMISO(uint8_t pin);
    virtual bool pinIsSCK(uint8_t pin);

    // return true if "pin" has special chip select capability
    virtual uint8_t pinIsChipSelect(uint8_t pin);
    virtual bool pinIsChipSelect(uint8_t pin1, uint8_t pin2);
    virtual uint8_t setCS(uint8_t pin);

#if defined(KINETISK)
    virtual KINETISK_SPI_t *kinetisk_spi (void);

    virtual uint8_t sizeFIFO();
    virtual void waitFifoNotFull(void);
    virtual void waitFifoEmpty(void);
    virtual void waitTransmitComplete(void);
    virtual void waitTransmitComplete(uint32_t mcr);
    virtual uint8_t dmaTXEvent(void);
    virtual uint8_t dmaRXEvent(void);
#endif
};


#if defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(KINETISL)
#define SPIN1_OBJECT_CREATED
/**********************************************************/
/*   SPIN1                                                */
/**********************************************************/
class SPIN1Class : public SPINClass {
public:
    // Initialize the SPI library
    virtual void begin();
    virtual void usingInterrupt(uint8_t n);
    virtual void usingInterrupt(IRQ_NUMBER_t interruptName);
    virtual void notUsingInterrupt(IRQ_NUMBER_t interruptName);

    // Before using SPI.transfer() or asserting chip select pins,
    // this function is used to gain exclusive access to the SPI bus
    // and configure the correct settings.
    virtual void beginTransaction(SPISettings settings);

    // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
    virtual uint8_t transfer(uint8_t data);
    virtual uint16_t transfer16(uint16_t data);
    virtual void transfer(void *buf, size_t count);

    // After performing a group of transfers and releasing the chip select
    // signal, this function allows others to access the SPI bus
    virtual void endTransaction(void);
    virtual void end();

    virtual void setBitOrder(uint8_t bitOrder);
    virtual void setDataMode(uint8_t dataMode);
    virtual void setClockDivider(uint8_t clockDiv);

    // Teensy 3.x can use alternate pins for these 3 SPI signals.
    virtual void setMOSI(uint8_t pin);
    virtual void setMISO(uint8_t pin);
    virtual void setSCK(uint8_t pin);
    virtual bool pinIsMOSI(uint8_t pin);
    virtual bool pinIsMISO(uint8_t pin);
    virtual bool pinIsSCK(uint8_t pin);

    // return true if "pin" has special chip select capability
    virtual uint8_t pinIsChipSelect(uint8_t pin);
    virtual bool pinIsChipSelect(uint8_t pin1, uint8_t pin2);
    virtual uint8_t setCS(uint8_t pin);

#if defined(KINETISK)
    virtual KINETISK_SPI_t *kinetisk_spi (void);

    virtual uint8_t sizeFIFO();
    virtual void waitFifoNotFull(void);
    virtual void waitFifoEmpty(void);
    virtual void waitTransmitComplete(void);
    virtual void waitTransmitComplete(uint32_t mcr);
    virtual uint8_t dmaTXEvent(void);
    virtual uint8_t dmaRXEvent(void);
#endif
};

extern SPIN1Class SPIN1;
#endif


#if defined(__MK64FX512__) || defined(__MK66FX1M0__)
#define SPIN2_OBJECT_CREATED
/**********************************************************/
/*   SPIN2                                                */
/**********************************************************/
class SPIN2Class : public SPINClass {
public:
    // Initialize the SPI library
    virtual void begin();
    virtual void usingInterrupt(uint8_t n);
    virtual void usingInterrupt(IRQ_NUMBER_t interruptName);
    virtual void notUsingInterrupt(IRQ_NUMBER_t interruptName);

    // Before using SPI.transfer() or asserting chip select pins,
    // this function is used to gain exclusive access to the SPI bus
    // and configure the correct settings.
    virtual void beginTransaction(SPISettings settings);

    // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
    virtual uint8_t transfer(uint8_t data);
    virtual uint16_t transfer16(uint16_t data);
    virtual void transfer(void *buf, size_t count);

    // After performing a group of transfers and releasing the chip select
    // signal, this function allows others to access the SPI bus
    virtual void endTransaction(void);
    virtual void end();

    virtual void setBitOrder(uint8_t bitOrder);
    virtual void setDataMode(uint8_t dataMode);
    virtual void setClockDivider(uint8_t clockDiv);

    // Teensy 3.x can use alternate pins for these 3 SPI signals.
    virtual void setMOSI(uint8_t pin);
    virtual void setMISO(uint8_t pin);
    virtual void setSCK(uint8_t pin);
    virtual bool pinIsMOSI(uint8_t pin);
    virtual bool pinIsMISO(uint8_t pin);
    virtual bool pinIsSCK(uint8_t pin);

    // return true if "pin" has special chip select capability
    virtual uint8_t pinIsChipSelect(uint8_t pin);
    virtual bool pinIsChipSelect(uint8_t pin1, uint8_t pin2);
    virtual uint8_t setCS(uint8_t pin);

#if defined(KINETISK)
    virtual KINETISK_SPI_t *kinetisk_spi (void);

    virtual uint8_t sizeFIFO();
    virtual void waitFifoNotFull(void);
    virtual void waitFifoEmpty(void);
    virtual void waitTransmitComplete(void);
    virtual void waitTransmitComplete(uint32_t mcr);
    virtual uint8_t dmaTXEvent(void);
    virtual uint8_t dmaRXEvent(void);
#endif
};


extern SPIN2Class SPIN2;
#endif

extern SPIN0Class SPIN;
#endif
#endif