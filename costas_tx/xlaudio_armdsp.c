#include "xlaudio_armdsp.h"


void xlaudio_adc14_to_q15_vec(const uint16_t *v, q15_t *q, uint32_t sz) {
    while (sz--)
        *q++ = xlaudio_adc14_to_q15(*v++);
}

void xlaudio_adc14_to_q31_vec(const uint16_t *v, q31_t *q, uint32_t sz) {
    while (sz--)
        *q++ = xlaudio_adc14_to_q31(*v++);
}

void xlaudio_adc14_to_f32_vec(const uint16_t *v, float32_t *q, uint32_t sz) {
    while (sz--)
        *q++ = xlaudio_adc14_to_f32(*v++);
}

void xlaudio_q15_to_dac14_vec(const q15_t *v, uint16_t *q, uint32_t sz) {
    while (sz--)
        *q++ = xlaudio_q15_to_dac14(*v++);
}

void xlaudio_q31_to_dac14_vec(const q31_t *v, uint16_t *q, uint32_t sz) {
    while (sz--)
        *q++ = xlaudio_q31_to_dac14(*v++);
}

void xlaudio_f32_to_dac14_vec(const float32_t *v, uint16_t *q, uint32_t sz) {
    while (sz--)
        *q++ = xlaudio_f32_to_dac14(*v++);
}

q15_t     xlaudio_adc14_to_q15(uint16_t v) {
    return (q15_t) ((v - 0x2000));
}

q31_t     xlaudio_adc14_to_q31(uint16_t v) {
    return (q31_t) ((v - 0x2000) << 16);
}

float32_t xlaudio_adc14_to_f32(uint16_t v) {
    return (float32_t) (((q15_t) (v - 0x2000)) / 32768.0f);
}

uint16_t  xlaudio_q15_to_dac14(q15_t v) {
    return (uint16_t) ((v + 0x2000) & 0x3fff);
}

uint16_t  xlaudio_q31_to_dac14(q31_t v) {
    return (uint16_t) (((v >> 16) + 0x2000) & 0x3fff);
}

uint16_t  xlaudio_f32_to_dac14(float32_t v) {
    return (uint16_t) ((((q15_t) (v * 32768.0f)) + 0x2000) & 0x3fff);
}
