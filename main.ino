  #include "ADC_DEAKIN.h"

  ADC_DEAKIN adc;

  void setup() {
    Serial.begin(9600);
    
    // Configure ADC on port 'A', pin 2 (PA02), 12-bit resolution, single-shot mode
    adc.setup_ADC('A', 2, 12, 0);

    // Enable the ADC
    adc.enable_ADC();

    // Optionally enable interrupt mode
    // adc.setADCInterrupt();
  }

  void loop() {
    int sample;
    
    // Read a sample from the ADC
    if (adc.read_ADC(&sample)) {
      Serial.println(sample);
    }

    delay(1000);  // Polling delay
  }
