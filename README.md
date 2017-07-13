# SPIN
Simple wrapper SPI library, that creates a new SPI base class, with all virtual functions, and then creates sub-classes for each of the SPI busses on the processor.  

This allows me to pass in the base class to other code, which allows this code to work on multiple SPI busses without source code changes. 

# Motivation: Teensy 3.4/3.5 Hardware 

Originally the Spin class was created as part of my testing for the Teensy K66 (3.6) beta.  It was setup such that I could test out SPI code on 
all SPI busses that a Teensy processor may have.  For the Teensy 3.5 and 3.6 they have 3 busses and the Teensy LC has 2. 

The Original test hardware for this was the ili9341 displays and the first test was to modify the ILI9341_t3 library which I renamed ILI9341_t3n.

While doing this I found a more primary motivation was the ability to modify different libraries that use SPI to be able to use a library that supports all the SPI busses, such that if a user wishes for that library to use SPI1 instead of SPI, they don't have to manually edit that library and change all of the references from SPI to SPI1, but instead potentially simply pass in which buss they wish to use into the constructor or begin for the object. 


# WARNING

With the release of Teensyduino 1.37, the SPI class was updated such that all of the SPI objects now derive from one Class SPIClass.  This is consistent with some of the other Arduino implementations for other boards like the DUE and M0, and as such any library that wishes to modify themselves to be able to use SPI on any of the buses, should first see if they can do it using the main SPI class instead of SPIN. 

The Teensyduino 1.37 release also added some of the methods that I had earlier added to SPIN, to make it easier to work with the multiple busses. 
Things like asking is a pin a valid MISO pin, or a MOSI pin... 

# Overview

The original version of spin had a base class, where all of the methods were pure virtual and then had subclasses for each SPI buss.  

This new version, I updated it to be like SPI and only have one class, where the constructor allows me to pass in the parameters that are specific for each buss.  

Most of the methods, are exact duplicates of the standard SPI object and they simply call off to the corresponding  method in the SPI object that was passed to it in the constructor.  These methods are defined as inline so hopefully they do not have very much overhead.  

In addition I added methods, that were part of the ILI9341_t3 library that have to do with using the FIFO queues and a method to query the FIFO size.  Plus methods to return some additional information, like a reference to the SPI object and a reference to the SPI hardware registers. 

I then created a subclass for each of the SPI busses supported by the hardware. 

# Methods
Sorry I know the below does not describe the methods, but the majority of them are described either in the standard SPI documentation or in data on PJRC websites.

## Direct from SPI object:
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


## Methods for working with SPI registers and deal with Queue
Most of these methods come directly from the ILI9341_T3 code, at least for the SPI0 case
Warning: I renamed a few methods here to be consistent with the newer SPI class, in particular the method kinetisk_spi was renamed to port and it now returns a reference instead of a pointer. 

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

The first method, returns a handle to the SPI register structure. This is important as code that is ported to this needs to change from using direct registers, to instead go through this pointer. Example in ILI9341_t3 library, the writecommand_cont function looks like:

    void writecommand_cont(uint8_t c) __attribute__((always_inline)) {
        KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
        waitFifoNotFull();
    }

The version that works with all three queue (ili9341_t3n) looks like (Note: _pkinetisk_spi is a cached copy of what was returned by the above method:

    void writecommand_cont(uint8_t c) __attribute__((always_inline)) {
        _pkinetisk_spi->PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
        _pspin->waitFifoNotFull();
    }

As mentioned above SPI1 and SPI2 queue size is different(1) than SPI(4), so several of the above methods have to be different.  Example the waitFifoNotFull.  Which needs to check for different values:

Example for SPI0:

    void SPIN0Class::waitFifoNotFull(void) {
        uint32_t sr;
        uint32_t tmp __attribute__((unused));
        do {
            sr = KINETISK_SPI0.SR;
            if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
        } while ((sr & (15 << 12)) > ((4-1) << 12));
    }

Where for SPI1 it looks like:

    void SPIN1Class::waitFifoNotFull(void) {
        uint32_t sr;
        uint32_t tmp __attribute__((unused));
        do {
            sr = KINETISK_SPI1.SR;
            if (sr & 0xF0) tmp = KINETISK_SPI1.POPR;  // drain RX FIFO
        } while ((sr & (15 << 12)) > ((1-1) << 12));
    }

I could probably could combine, but...

#Usage

The library creates an object for each of the SPI busses: SPIN, SPIN1, SPIN2,

My ILI9341_t3n creator method has an optional point parameter (default &SPIN).  SO to create an instance of my class to run on the SPI1 buss you can pass in &SPI1
