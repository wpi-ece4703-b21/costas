#include "xlaudio.h"

#define SYMBOLS 3
#define UPSAMPLE 16

#define NUMCOEF (SYMBOLS * UPSAMPLE + 1)

// This array contains the filter coefficients
// you have computed for the root raised cosine filter
// implementation (3 symbols, 16 samples per symbol, roll-off 0.3)

float32_t coef[NUMCOEF] = {
    -0.042923315,
    0.0,
    0.050726122,
    0.0,
    -0.050086476,
    0.0,
    0.038883134,
    0.0,
    -0.016190898,
    0.0,
    -0.017447588,
    0.0,
    0.05995279,
    0.0,
    -0.10786616,
    0.0,
    0.1567233,
    0.0,
    -0.20159548,
    0.0,
    0.23772369,
    0.0,
    -0.26115217,
    0.0,
    0.26926623,
    0.0,
    -0.26115217,
    0.0,
    0.23772369,
    0.0,
    -0.20159548,
    0.0,
    0.1567233,
    0.0,
    -0.10786616,
    0.0,
    0.05995279,
    0.0,
    -0.017447588,
    0.0,
    -0.016190898,
    0.0,
    0.038883134,
    0.0,
    -0.050086476,
    0.0,
    0.050726122,
    0.0,
    -0.042923315
};

#define NUMTAPS (SYMBOLS+1)
int symboltaps[NUMTAPS];

// this function contains your multirate filter
// parameter phase is the filter phase
// parameter symbol is the symbol (only valid at phase == 0)
// the function returns the filter output for the selected phase

float32_t rrcphase(int phase, float32_t symbol) {

    // At phase = 0, read in a new symbol into the
    // symbol delay line, and shift the delay line
    // At all other phase, keep the delay line intact

    uint32_t i;
    if (phase == 0) {
        for (i=NUMTAPS-1; i>0; i--)
            symboltaps[i] = symboltaps[i-1];
        symboltaps[0] =symbol;
    }

    // Compute the filter output by multiplying symbols with
    // filter taps according to the proper filter phase

    float32_t q = 0.0;
    uint16_t limit = (phase == 0) ? NUMTAPS : NUMTAPS - 1;
    for (i=0; i<limit; i++)
        q += symboltaps[i] * coef[i * UPSAMPLE + phase + 1];

    return q;
}

//------------------------ MAKE NO CHANGES BELOW THIS LINE ---------------------

#define PACKETLEN 2048

int bpsksymbolgenerator() {
    static int packetindex = 0;

    // random symbol is left button is pressed
    if (xlaudio_pushButtonLeftDown())
        return 1 - (rand() % 2)*2;

    // fixed symbol is right button is pressed
    if (xlaudio_pushButtonRightDown())
        return 1;

    packetindex++;
    if (packetindex == PACKETLEN) {
        xlaudio_debugpinhigh();
        packetindex = 0;
    }
    if (packetindex < 256) {
        return 1;
    }
    else if (packetindex < 256) {
        return 1 - (packetindex %2)*2;  // carrier sync
    }
    else if (packetindex < (PACKETLEN - 64)) {
        xlaudio_debugpinlow();
        return 1 - (rand() % 2)*2;      // random data bits
    }
    else if (packetindex < PACKETLEN) {
        return 1;                       // constant '1' as inactive symbol
    }
    return 1;
}

uint16_t processSample(uint16_t x) {
    static int filterphase = 0;
    float32_t result = rrcphase(filterphase, filterphase ? 1 : bpsksymbolgenerator());
    filterphase = (filterphase + 1) % UPSAMPLE;
    return xlaudio_f32_to_dac14(result * 0.1);
}

#include <stdio.h>

int main(void) {
    WDT_A_hold(WDT_A_BASE);

    int c = xlaudio_measurePerfSample(processSample);
    printf("Cycles %d\n", c);

    xlaudio_init_intr(FS_8000_HZ, XLAUDIO_J1_2_IN, processSample);
    xlaudio_run();

    return 1;
}
