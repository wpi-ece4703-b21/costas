/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
//!  TI-Design Signal Processing with LEA on MSP430FR5994
//!
//!  William Goh
//!  Texas Instruments Inc.
//!  April 2016
//******************************************************************************

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "dac8311.h"

/*
 * Initializes the SPI module for DAC8311
 */
void DAC8311_init(void) {
    // Configure DAC SYNC on P2.4
    GPIO_setAsOutputPin(DAC8311_SYNC_PORT,
                        DAC8311_SYNC_PIN);

    GPIO_setOutputHighOnPin(DAC8311_SYNC_PORT,
                            DAC8311_SYNC_PIN);

    // USCI_B0 MOSI, MISO, SCLK operation
    // Configure DAC8311_SCLK_PIN
    GPIO_setAsPeripheralModuleFunctionOutputPin(DAC8311_SCLK_PORT,
                                                DAC8311_SCLK_PIN,
                                                DAC8311_SCLK_PIN_FUNCTION);

    // Configure DAC8311_MOSI_PIN
    GPIO_setAsPeripheralModuleFunctionOutputPin(DAC8311_MOSI_PORT,
                                                DAC8311_MOSI_PIN,
                                                DAC8311_MOSI_PIN_FUNCTION);

    eUSCI_SPI_MasterConfig spiMasterConfig =
    {
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,
        SYSTEM_CLK,
        DAC8311_SPI_CLK,
        EUSCI_B_SPI_MSB_FIRST,
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW,
        EUSCI_B_SPI_3PIN
    };

    SPI_initMaster(DAC8311_EUSCI_BASE,
                   &spiMasterConfig);

    SPI_enableModule(DAC8311_EUSCI_BASE);
}

/*
 * Local function to write DAC value into SPI module
 */
static void DAC8311_writeRegister(uint16_t data) {
    // Falling edge to trigger DAC to latch new data
    GPIO_setOutputLowOnPin(DAC8311_SYNC_PORT,
                           DAC8311_SYNC_PIN);

    while (EUSCI_B_SPI_isBusy(DAC8311_EUSCI_BASE)) ;

    EUSCI_B_SPI_transmitData(DAC8311_EUSCI_BASE, data >> 8);

    while (EUSCI_B_SPI_isBusy(DAC8311_EUSCI_BASE)) ;

    EUSCI_B_SPI_transmitData(DAC8311_EUSCI_BASE, data);

    while (EUSCI_B_SPI_isBusy(DAC8311_EUSCI_BASE)) ;

    // Set SYNC back high
    GPIO_setOutputHighOnPin(DAC8311_SYNC_PORT,
                            DAC8311_SYNC_PIN);
}

/*
 * Function that updates the DAC output and forces the device in Normal Mode
 */
void DAC8311_updateDacOut(uint16_t value) {
    // Set DB15 and DB14 to be 0 for normal mode
    value &= ~(0xC000);

    DAC8311_writeRegister(value);
}

/*
 * Function that sets the device in low power mode and clears the DAC value registers.
 */
void DAC8311_setLowPowerMode(uint16_t mode) {
    if ((mode == DAC8311_OUTPUT_1K_TO_GND) || (mode == DAC8311_OUTPUT_100K_TO_GND) || (mode == DAC8311_OUTPUT_HIGHZ))
    {
        DAC8311_writeRegister(mode);
    }
}
