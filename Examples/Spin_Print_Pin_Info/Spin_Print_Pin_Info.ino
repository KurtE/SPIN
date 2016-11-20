#include <SPIN.h>
#include <SPI.h>

void setup() {
  // put your setup code here, to run once:
  while (!Serial) ;
  Serial.begin(115200);
  delay(250);
  Serial.println("Spin - print processor SPI Pin information");
#if defined(__MK20DX128__)
  Serial.print("Teensy 3.0 - ");
#elif defined(__MK20DX256__)
  Serial.print("Teensy 3.1/3.2 - ");
#elif defined(__MKL26Z64__)
  Serial.print("Teensy 3.5 - ");
#elif defined(__MK64FX512__)
  Serial.print("Teensy 3.0 - ");
#elif defined(__MK66FX1M0__)
  Serial.print("Teensy 3.6 - ");
#else
  Serial.print("Teensy ??? - ");
#endif
  Serial.print(CORE_NUM_TOTAL_PINS, DEC);
  Serial.println(" pins");
}
void PrintSpinInfo(int spi_num, SPINClass *pspin) {
  Serial.printf("\nSerial pin information for BUSS %d\n", spi_num);

  for (uint8_t pin = 0; pin < CORE_NUM_TOTAL_PINS; pin++) {
    bool something_printed = false;
    if (pspin->pinIsMOSI(pin)) {
      if (!something_printed) {
        Serial.printf("%d:", pin);
        something_printed = true;
      }
      Serial.print(" MOSI");
    }

    if (pspin->pinIsMISO(pin)) {
      if (!something_printed) {
        Serial.printf("%d:", pin);
        something_printed = true;
      }
      Serial.print(" MISO");
    }
    if (pspin->pinIsSCK(pin)) {
      if (!something_printed) {
        Serial.printf("%d:", pin);
        something_printed = true;
      }
      Serial.print(" SCK");
    }
    uint8_t csMask;
    if ((csMask = pspin->pinIsChipSelect(pin))) {
      if (!something_printed) {
        Serial.printf("%d:", pin);
        something_printed = true;
      }
      uint8_t csNum = 0;
      for (csNum = 0; (csMask & 1) == 0; csNum++) {
        csMask >>= 1; // shift down
      }
      Serial.printf(" CS%d-%d", spi_num, csNum);
    }
    if (something_printed) {
      Serial.println();
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  PrintSpinInfo(0, &SPIN);
#ifdef SPIN1_OBJECT_CREATED
  PrintSpinInfo(1, &SPIN1);
#endif

#ifdef SPIN2_OBJECT_CREATED
  PrintSpinInfo(2, &SPIN2);
#endif
  Serial.println("*** End SPIN - Enter anything to repeat ***");
  while (Serial.read() == -1) ;
  while (Serial.read() != -1) ;
}
