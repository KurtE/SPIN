# SPIN
Simple wrapper SPI library, that creates a new SPI base class, with all virtual functions, and then creates sub-classes for each of the SPI busses on the processor.  

This allows me to pass in the base class to other code, which allows this code to work on multiple SPI busses without source code changes. 

# Motivation: Teensy 3.4/3.5 Hardware 

This code is setup for me for testing stuff on the Teensy K66 beta.  I used it to create a modified version of the ILI9341_t3 library which I renamed ILI9341_t3n, which among other things allows you to be able to use the display on all three of the SPI buses of the new hardware


# WARNING

This is WIP.  So use at your own risk as there is no Guarantees!

# Overview
The main class is called SPIN, all of it's methods are pure virtual.  Most of the methods, are exact duplicates of the standard SPI object, except their methods are virtual.  

In addition I added methods, that were part of the ILI9341_t3 library that have to do with using the FIFO queues and a method to query the FIFO size. 

I then created a subclass for each of the SPI busses supported by the hardware. 

# Methods
Sorry I know the below does not describe the methods, but the majority of them are described either in the standard SPI documentation or in data on PJRC websites.

## Direct from SPI object:
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

    // return true if "pin" has special chip select capability
    virtual uint8_t pinIsChipSelect(uint8_t pin) = 0;
    virtual bool pinIsChipSelect(uint8_t pin1, uint8_t pin2) = 0;
    virtual uint8_t setCS(uint8_t pin) = 0;

## Methods that should be part of SPI
The low level SPI code has query functions to find out if a pin is a CS pin and if the two CS pins are both CS pins and not the same channel.  It would be great if they also had methods to see if a pin was a valid MISO, or MOSI, or SCK pin.  So I added them at this level

    // Added ones that are not part of SPI (yet)
    virtual bool pinIsMOSI(uint8_t pin) = 0;
    virtual bool pinIsMISO(uint8_t pin) = 0;
    virtual bool pinIsSCK(uint8_t pin) = 0;

## Methods for working with SPI registers and deal with Queue
Most of these methods come directly from the ILI9341_T3 code, at least for the SPI0 case

    virtual KINETISK_SPI_t *kinetisk_spi (void) = 0;

    virtual uint8_t sizeFIFO() = 0;
    virtual void waitFifoNotFull(void) = 0;
    virtual void waitFifoEmpty(void) = 0;
    virtual void waitTransmitComplete(void)  = 0;
    virtual void waitTransmitComplete(uint32_t mcr) = 0;

The first method, returns a pointer to the SPI register structure. This is important as code that is ported to this needs to change from using direct registers, to instead go through this pointer. Example in ILI9341_t3 library, the writecommand_cont function looks like:

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
  
# Again Warning
Again this is a WIP!  So far this code is only used in ili9341_t3n library, so there could be holes that I have not run into.  