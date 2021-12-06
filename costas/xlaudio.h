/* xlaudio_lib
   Written by Patrick Schaumont (pschaumont@wpi.edu) at Worcester Polytechnic Institute in 2020

   xlaudio_lib is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3, or (at your option) any later
   version.

   xlaudio_lib is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with msp432_boostxl; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */


#ifndef XLAUDIO_H_
#define XLAUDIO_H_

#include <stdlib.h>
#include <stdio.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "xlaudio_armdsp.h"
#include "msp432p401r.h"

typedef enum {
    FS_8000_HZ,
    FS_11025_HZ,
    FS_16000_HZ,
    FS_22050_HZ,
    FS_24000_HZ,
    FS_32000_HZ,
    FS_44100_HZ,
    FS_48000_HZ
} fs_enum_t;

typedef enum {
    XLAUDIO_MIC_IN,
    XLAUDIO_J1_2_IN
} xlaudio_in_enum_t;

typedef enum {
    BUFLEN_8,
    BUFLEN_16,
    BUFLEN_32,
    BUFLEN_64,
    BUFLEN_128
} buflen_enum_t;

typedef uint16_t (*xlaudio_sample_process_t)(uint16_t);
typedef void     (*xlaudio_buffer_process_t)(uint16_t *, uint16_t *);

void xlaudio_init();

void xlaudio_init_poll(xlaudio_in_enum_t  _audioin,
                       xlaudio_sample_process_t _cb
                       );

void xlaudio_init_intr(fs_enum_t          _fs,
                       xlaudio_in_enum_t  _audioin,
                       xlaudio_sample_process_t _cb
                       );

void xlaudio_init_dma (fs_enum_t          _fs,
                       xlaudio_in_enum_t  _audioin,
                       buflen_enum_t      _pplen,
                       xlaudio_buffer_process_t _cb
                       );

void xlaudio_run();

void xlaudio_errorledon();
void xlaudio_errorledoff();

void xlaudio_colorledred();
void xlaudio_colorledgreen();
void xlaudio_colorledblue();
void xlaudio_colorledoff();

void xlaudio_debugpinhigh();
void xlaudio_debugpinlow();

uint32_t xlaudio_measurePerfSample(xlaudio_sample_process_t _cb);
uint32_t xlaudio_measurePerfBuffer(xlaudio_buffer_process_t _cb);

int xlaudio_pushButtonLeftUp();
int xlaudio_pushButtonLeftDown();
int xlaudio_pushButtonRightUp();
int xlaudio_pushButtonRightDown();

void xlaudio_delay(uint32_t cycles);

#endif /* XLAUDIO_H_ */
