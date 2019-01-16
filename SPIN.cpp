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


#if defined(KINETISK)
void SPINClass::waitFifoNotFull(void) {
    uint32_t sr;
    uint32_t tmp __attribute__((unused));
    do {
        sr = port().SR;
        if (sr & 0xF0) tmp = port().POPR;  // drain RX FIFO
    } while ((uint32_t)(sr & (15 << 12)) > (uint32_t)((_fifo_size-1) << 12));
}
void SPINClass::waitFifoEmpty(void) {
    uint32_t sr;
    uint32_t tmp __attribute__((unused));
    do {
        sr = port().SR;
        if (sr & 0xF0) tmp = port().POPR;  // drain RX FIFO
    } while ((sr & 0xF0F0) > 0);             // wait both RX & TX empty
}
void SPINClass::waitTransmitComplete(void)  {
    uint32_t tmp __attribute__((unused));
    while (!(port().SR & SPI_SR_TCF)) ; // wait until final output done
    tmp = port().POPR;                  // drain the final RX FIFO word
}
void SPINClass::waitTransmitComplete(uint32_t mcr) {
    uint32_t tmp __attribute__((unused));
    while (1) {
        uint32_t sr = port().SR;
        if (sr & SPI_SR_EOQF) break;  // wait for last transmit
        if (sr &  0xF0) tmp = port().POPR;
    }
    port().SR = SPI_SR_EOQF;
    port().MCR = mcr;
    while (port().SR & 0xF0) {
        tmp = port().POPR;
    }
}


SPINClass SPIN((uintptr_t)&SPI, (uintptr_t)&KINETISK_SPI0, 4, DMAMUX_SOURCE_SPI0_TX, DMAMUX_SOURCE_SPI0_RX);

#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
void SPINClass::waitFifoNotFull(void) {
    uint32_t tmp __attribute__((unused));
    do {
        if ((port().RSR & LPSPI_RSR_RXEMPTY) == 0)  {
            tmp = port().RDR;  // Read any pending RX bytes in
        }
    } while ((port().SR & LPSPI_SR_TDF) == 0) ;
}
void SPINClass::waitFifoEmpty(void) {
    uint32_t tmp __attribute__((unused));
    do {
        if ((port().RSR & LPSPI_RSR_RXEMPTY) == 0)  {
            tmp = port().RDR;  // Read any pending RX bytes in
        }
    } while ((port().SR & LPSPI_SR_TCF) == 0) ;
}
void SPINClass::waitTransmitComplete(void)  {
    uint32_t tmp __attribute__((unused));
//    digitalWriteFast(2, HIGH);
    do {
        if ((port().RSR & LPSPI_RSR_RXEMPTY) == 0)  {
            tmp = port().RDR;  // Read any pending RX bytes in
        }
    } while ((port().SR & LPSPI_SR_MBF) == 0) ;
    uint16_t loop_cnts_timeout = 0xff;   // don't check more than so many times
    while (--loop_cnts_timeout && ((port().SR & LPSPI_SR_FCF) == 0) ) ;
    port().CR = LPSPI_CR_MEN | LPSPI_CR_RRF;       // Clear RX FIFO
//    digitalWriteFast(2, LOW);
}

void SPINClass::waitTransmitComplete(uint32_t mcr) {
    // BUGBUG:: figure out if needed...
    waitTransmitComplete();
}


SPINClass SPIN((uintptr_t)&SPI, (uintptr_t)&IMXRT_LPSPI4_S, 16, DMAMUX_SOURCE_LPSPI4_TX, DMAMUX_SOURCE_LPSPI4_RX);
#else
// Teensy LC
SPINClass SPIN((uintptr_t)&SPI, (uintptr_t)&KINETISL_SPI0);
#endif

// Define SPI1 and SPI2 for those boards who support them. 
#if defined(__MK64FX512__) 
// Teensy 3.5
// Warning T3.5 has only one DMAMUX source for SPI1 and SPI2 which can be RX or TX 
SPINClass SPIN1((uintptr_t)&SPI1, (uintptr_t)&KINETISK_SPI1, 1, 0, DMAMUX_SOURCE_SPI1);
SPINClass SPIN2((uintptr_t)&SPI2, (uintptr_t)&KINETISK_SPI2, 1, 0, DMAMUX_SOURCE_SPI2);

#elif defined(__MK66FX1M0__) 
// Teensy 3.6
SPINClass SPIN1((uintptr_t)&SPI1, (uintptr_t)&KINETISK_SPI1, 1, DMAMUX_SOURCE_SPI1_TX, DMAMUX_SOURCE_SPI1_RX);
SPINClass SPIN2((uintptr_t)&SPI2, (uintptr_t)&KINETISK_SPI2, 1, DMAMUX_SOURCE_SPI2_TX, DMAMUX_SOURCE_SPI2_RX);

#elif defined(KINETISL)
// Teensy LC
SPINClass SPIN1((uintptr_t)&SPI1, (uintptr_t)&KINETISL_SPI1);
#endif
