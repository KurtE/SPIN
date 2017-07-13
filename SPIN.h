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
    inline void begin() {spi().begin(); }
    inline void usingInterrupt(uint8_t n) {spi().usingInterrupt(n); }
    inline void usingInterrupt(IRQ_NUMBER_t interruptName) {spi().usingInterrupt(interruptName); }
    inline void notUsingInterrupt(IRQ_NUMBER_t interruptName) {spi().notUsingInterrupt(interruptName); }

    // Before using SPI.transfer() or asserting chip select pins,
    // this function is used to gain exclusive access to the SPI bus
    // and configure the correct settings.
    inline void beginTransaction(SPISettings settings)  {spi().beginTransaction(settings);}

    // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
    inline uint8_t transfer(uint8_t data) {return spi().transfer(data);}
    inline uint16_t transfer16(uint16_t data) {return spi().transfer16(data);}
    inline void transfer(void *buf, size_t count) {spi().transfer(buf, count); }

    // After performing a group of transfers and releasing the chip select
    // signal, this function allows others to access the SPI bus
    inline void endTransaction(void)  {spi().endTransaction();}
    inline void end() {spi().end();}

    inline void setBitOrder(uint8_t bitOrder) {spi().setBitOrder(bitOrder);}
    inline void setDataMode(uint8_t dataMode) {spi().setDataMode(dataMode);}
    inline void setClockDivider(uint8_t clockDiv) {spi().setClockDivider(clockDiv);}

    // Teensy 3.x can use alternate pins for these 3 SPI signals.
    inline void setMOSI(uint8_t pin)  {spi().setMOSI(pin);}
    inline void setMISO(uint8_t pin) {spi().setMISO(pin);}
    inline void setSCK(uint8_t pin) {spi().setSCK(pin);}
    // Added ones that are not part of SPI (yet)
    inline bool pinIsMOSI(uint8_t pin)  {return spi().pinIsMOSI(pin);}
    inline bool pinIsMISO(uint8_t pin) {return spi().pinIsMISO(pin);}
    inline bool pinIsSCK(uint8_t pin) {return spi().pinIsSCK(pin);}

    // return true if "pin" has special chip select capability
    inline uint8_t pinIsChipSelect(uint8_t pin) {return spi().pinIsChipSelect(pin);}
    inline bool pinIsChipSelect(uint8_t pin1, uint8_t pin2) {return spi().pinIsChipSelect(pin1, pin2);}
    inline uint8_t setCS(uint8_t pin) {return spi().setCS(pin);}

    // Add helper functions from ILI9341, which manage SPI FIFO
#if defined(KINETISK)
constexpr SPINClass(uintptr_t spiX, uintptr_t myport, uint8_t fifo_size, uint8_t dma_tx_event, uint8_t dma_rx_event)
        : _spi(spiX), _port_addr(myport), _fifo_size(fifo_size), 
        _dma_tx_event(dma_tx_event), _dma_rx_event(dma_rx_event) {
    }

    inline KINETISK_SPI_t & port() { return *(KINETISK_SPI_t *)_port_addr; }

    inline uint8_t sizeFIFO() {return _fifo_size; }
    void waitFifoNotFull(void);
    void waitFifoEmpty(void);
    void waitTransmitComplete(void) ;
    void waitTransmitComplete(uint32_t mcr);

    // return DMA Channel information 
    inline uint8_t dmaTXEvent(void) {return _dma_tx_event;}
    inline uint8_t dmaRXEvent(void) {return _dma_rx_event;}
    inline SPIClass & spi() { return *(SPIClass *)_spi;}
#endif
#if defined(KINETISL)
constexpr SPINClass(uintptr_t spiX, uintptr_t myport)
        : _spi(spiX), _port_addr(myport) {
    }

    inline KINETISL_SPI_t & port() { return *(KINETISL_SPI_t *)_port_addr; }
    inline SPIClass & spi() { return *(SPIClass *)_spi;}
#endif    

private:
    uintptr_t _spi;
#if defined(KINETISK)

    uintptr_t _port_addr;
    uint8_t _fifo_size;
    uint8_t _dma_tx_event;
    uint8_t _dma_rx_event;
#else
    uintptr_t _port_addr;
#endif        

};

extern SPINClass SPIN;

#if defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(KINETISL)
#define SPIN1_OBJECT_CREATED
extern SPINClass SPIN1;
#endif


#if defined(__MK64FX512__) || defined(__MK66FX1M0__)
#define SPIN2_OBJECT_CREATED
extern SPINClass SPIN2;
#endif

#endif
#endif