# SPIN
Simple wrapper SPI library, that creates a new SPI base class, with all virtual functions, and then creates sub-classes for each of the SPI busses on the processor.  

This allows me to pass in the base class to other code, which allows this code to work on multiple SPI busses without source code changes. 

# Teensy 3.4/3.5 Hardware Beta

This code is setup for me for testing stuff on the Teensy K66 beta.  I am using it with a modified version of the ILI9341_t3 library which I renamed ILI9341_t3n and will also upload. 

# WARNING

This is WIP and may be temporary.  So use at your own risk. 

There are no Guarantees!