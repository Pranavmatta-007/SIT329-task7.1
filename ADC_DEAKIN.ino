#include "ADC_DEAKIN.h"
#include <Arduino.h>

// Constructor
ADC_DEAKIN::ADC_DEAKIN() {
    // Initialization code if required
}

// Method to setup ADC
bool ADC_DEAKIN::setup_ADC(char PortNum, char PinNum, int bits, int mode) {
    PortGroup *port;

    // Select the appropriate PortGroup based on PortNum ('A' or 'B')
    if (PortNum == 'A') {
        port = &PORT->Group[0];
    } else if (PortNum == 'B') {
        port = &PORT->Group[1];
    } else {
        return false;
    }

    // Configure the pin for ADC usage
    configure_pin(port, PinNum);

    // Set resolution
    configure_resolution(bits);

    // Set mode (e.g., single shot or free run)
    configure_mode(mode);

    return true;
}

// Method to read an ADC sample
bool ADC_DEAKIN::read_ADC(int *sample) {
    if (!sample) return false;

    // Start ADC conversion
    ADC->SWTRIG.bit.START = 1;

    // Wait for conversion to be complete
    while (ADC->INTFLAG.bit.RESRDY == 0);

    // Read the result and store it in the provided variable
    *sample = ADC->RESULT.reg;

    return true;
}

// Method to enable ADC
bool ADC_DEAKIN::enable_ADC() {
    ADC->CTRLA.bit.ENABLE = 1;   // Enable ADC
    return true;
}

// Method to disable ADC
bool ADC_DEAKIN::disable_ADC() {
    ADC->CTRLA.bit.ENABLE = 0;   // Disable ADC
    return true;
}

// Method to set ADC interrupt
bool ADC_DEAKIN::setADCInterrupt() {
    // Enable ADC interrupt when result is ready
    ADC->INTENSET.bit.RESRDY = 1;

    // Enable NVIC for ADC interrupts
    NVIC_SetPriority(ADC_IRQn, 0);
    NVIC_EnableIRQ(ADC_IRQn);
    __enable_irq();   // Globally enable interrupts

    return true;
}

// Internal method to configure the ADC pin
void ADC_DEAKIN::configure_pin(PortGroup *port, char PinNum) {
    // Set pin direction as input
    port->DIRCLR.reg = (1 << PinNum);

    // Enable the pin multiplexer for analog input
    port->PINCFG[PinNum].bit.PMUXEN = 1;

    // Set the pin multiplexer to analog (PMUXE or PMUXO depending on pin number)
    if (PinNum % 2 == 0) {
        port->PMUX[PinNum / 2].bit.PMUXE = 1;  // Even pin, set as analog
    } else {
        port->PMUX[PinNum / 2].bit.PMUXO = 1;  // Odd pin, set as analog
    }
}

// Internal method to set ADC resolution
void ADC_DEAKIN::configure_resolution(int bits) {
    // Set resolution (0 = 12-bit, 1 = 16-bit, 2 = 8-bit)
    switch(bits) {
        case 12:
            ADC->CTRLB.bit.RESSEL = 0;
            break;
        case 16:
            ADC->CTRLB.bit.RESSEL = 1;
            break;
        case 8:
            ADC->CTRLB.bit.RESSEL = 2;
            break;
        default:
            ADC->CTRLB.bit.RESSEL = 0;  // Default to 12-bit
            break;
    }
}

// Internal method to set ADC mode (single shot, free run)
void ADC_DEAKIN::configure_mode(int mode) {
    if (mode == 0) {
        ADC->CTRLB.bit.FREERUN = 0;  // Single shot
    } else {
        ADC->CTRLB.bit.FREERUN = 1;  // Free run
    }
}

// Interrupt service routine for ADC
extern "C" void ADC_Handler() {
    // Handle ADC interrupt (e.g., store data, clear flags)
    if (ADC->INTFLAG.bit.RESRDY) {
        int result = ADC->RESULT.reg;
        ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY;  // Clear interrupt flag
    }
}
