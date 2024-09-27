  #ifndef ADC_DEAKIN_H
  #define ADC_DEAKIN_H

  #include <Arduino.h>

  class ADC_DEAKIN {
  public:
      // Constructor
      ADC_DEAKIN();

      // Method to configure the ADC
      bool setup_ADC(char PortNum, char PinNum, int bits, int mode);

      // Method to read an ADC sample
      bool read_ADC(int *sample);

      // Method to enable the ADC
      bool enable_ADC();

      // Method to disable the ADC
      bool disable_ADC();

      // Method to enable ADC interrupt
      bool setADCInterrupt();

  private:
      // Internal methods to configure the ADC hardware
      void configure_pin(PortGroup *port, char PinNum);
      void configure_resolution(int bits);
      void configure_mode(int mode);
  };

  #endif
    