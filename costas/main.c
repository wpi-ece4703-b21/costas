#include "xlaudio.h"

#define SYMBOLS 3
#define UPSAMPLE 16

#define NUMCOEF (SYMBOLS * UPSAMPLE + 1)

float32_t coef[NUMCOEF] = {
  -0.042923315, -0.047711509,   -0.050726122, -0.05161785,
  -0.050086476, -0.045895194,   -0.038883134, -0.02897551,
  -0.016190898, -0.00064525557,  0.017447588,  0.037779089,
   0.05995279,   0.083494586,    0.10786616,   0.13248111,
   0.1567233,    0.17996659,     0.20159548,   0.22102564,
   0.23772369,   0.2512255,      0.26115217,   0.26722319,
   0.26926623,   0.26722319,     0.26115217,   0.2512255,
   0.23772369,   0.22102564,     0.20159548,   0.17996659,
   0.1567233,    0.13248111,     0.10786616,   0.083494586,
   0.05995279,   0.037779089,    0.017447588, -0.00064525557,
  -0.016190898, -0.02897551,    -0.038883134, -0.045895194,
  -0.050086476, -0.05161785,    -0.050726122, -0.047711509,
  -0.042923315
};

float32_t qtaps[NUMCOEF];
float32_t itaps[NUMCOEF];
float32_t phi = 0.0;

float32_t filter(float32_t input) {
    uint32_t i;

    float32_t qresult = 0.0;
    float32_t iresult = 0.0;

    static unsigned carrier = 0;
    carrier = (carrier + 1) & 3; // carrier = fs/4

    itaps[0] = input * arm_cos_f32(carrier * M_PI /2.0 + phi);
    qtaps[0] = input * arm_sin_f32(carrier * M_PI /2.0 + phi);

    for (i = 0; i< NUMCOEF; i++) {
        qresult += qtaps[i] * coef[i];
        iresult += itaps[i] * coef[i];
    }

    for (i = NUMCOEF-1; i>0; i--) {
        qtaps[i] = qtaps[i-1];
        itaps[i] = itaps[i-1];
    }

    float32_t error = qresult * iresult;
    phi = phi + 0.1 * error;

    if (xlaudio_pushButtonRightDown())
        return error;

    if (xlaudio_pushButtonLeftDown())
        return input;

    return iresult;
}

uint16_t processSample(uint16_t x) {
    float32_t input = xlaudio_adc14_to_f32(x);
    return xlaudio_f32_to_dac14(filter(input));
}

#include <stdio.h>

int main(void) {
    WDT_A_hold(WDT_A_BASE);

    xlaudio_init_intr(FS_8000_HZ, XLAUDIO_J1_2_IN, processSample);

    int c = xlaudio_measurePerfSample(processSample);
    printf("Cycles %d\n", c);

    xlaudio_run();

    return 1;
}
