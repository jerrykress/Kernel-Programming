/* Copyright (C) 2017 Daniel Page <csdsp@bristol.ac.uk>
 *
 * Use of this source code is restricted per the CC BY-NC-ND license, a copy of
 * which can be found via http://creativecommons.org (and should be included as
 * LICENSE.txt within the associated archive or repository).
 */

#ifndef __HILEVEL_H
#define __HILEVEL_H

// Include functionality relating to newlib (the standard C library).

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "mutex.h"

// Include functionality relating to the platform.

#include "GIC.h"
#include "PL011.h"
#include "SP804.h"
#include "PL050.h"
#include "PL111.h"
#include "SYS.h"
#include "scanCodeTable.h"

// Include functionality relating to the   kernel.

#include "lolevel.h"
#include "int.h"

typedef int pid_t;

typedef enum
{
    STATUS_CREATED,
    STATUS_READY,
    STATUS_EXECUTING,
    STATUS_WAITING,
    STATUS_TERMINATED
} status_t;

typedef struct
{
    uint32_t cpsr, pc, gpr[13], sp, lr;
} ctx_t;

typedef struct
{
    pid_t pid;
    status_t status;
    ctx_t ctx;
    int priority;
    int runtime;
} pcb_t;

typedef struct
{
    int status;
    int data;
    int owner;
} mutex;

uint16_t font_black[128][16] = {
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0000 (nul)
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0001
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0002
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0003
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0004
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0005
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0006
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0007
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0008
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0009
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+000A
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+000B
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+000C
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+000D
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+000E
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+000F
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0010
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0011
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0012
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0013
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0014
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0015
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0016
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0017
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0018
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0019
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+001A
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+001B
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+001C
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+001D
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+001E
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+001F
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // U+0020 (space)
    {0,0,0, 0, 0, 124, 13311, 13311, 124, 0, 0, 0, 0, 0,0,0},                                     // '!'  33
	{0,0,0, 0, 60, 60, 0, 0, 60, 60, 0, 0, 0, 0,0,0},                                             // '"'  34
	{0,0,512, 7696, 8080, 1008, 638, 7710, 8080, 1008, 638, 30, 16, 0,0,0},                       // '#'  35
	{0,0,0, 1144, 3324, 3276, 16383, 16383, 3276, 4044, 1928, 0, 0, 0,0,0},                       // '$'  36
	{0,0,12288, 14392, 7224, 3640, 1792, 896, 448, 14560, 14448, 14392, 28, 0,0,0},               // '%'  37
	{0,0,0, 7936, 16312, 12796, 8646, 14306, 7742, 7196, 13824, 8704, 0, 0,0,0},                  // '&'  38
	{0,0,0, 0, 0, 39, 63, 31, 0, 0, 0, 0, 0, 0,0,0},                                              // '''  39
	{0,0,0, 0, 1008, 4092, 8190, 14343, 8193, 8193, 0, 0, 0, 0,0,0},                              // '('  40
	{0,0,0, 0, 8193, 8193, 14343, 8190, 4092, 1008, 0, 0, 0, 0,0,0},                              // ')'  41
	{0,0,0, 3224, 3768, 992, 4088, 4088, 992, 3768, 3224, 0, 0, 0,0,0},                           // '*'  42
	{0,0,0, 384, 384, 384, 4080, 4080, 384, 384, 384, 0, 0, 0,0,0},                               // '+'  43
	{0,0,0, 0, 0, 47104, 63488, 30720, 0, 0, 0, 0, 0, 0,0,0},                                     // ','  44
	{0,0,0, 384, 384, 384, 384, 384, 384, 384, 384, 0, 0, 0,0,0},                                 // '-'  45
	{0,0,0, 0, 0, 14336, 14336, 14336, 0, 0, 0, 0, 0, 0,0,0},                                     // '.'  46
	{0,0,6144, 7168, 3584, 1792, 896, 448, 224, 112, 56, 28, 14, 0,0,0},                          // '/'  47
	{0,0,2040, 8190, 7686, 13059, 12675, 12483, 12387, 12339, 6174, 8190, 2040, 0,0,0},           // '0'  48
    {0, 0, 0, 0, 12300, 12300, 12302, 16383, 16383, 12288, 12288, 12288, 0, 0, 0, 0},             // U+0031 (1)
    {0, 0, 12316, 14366, 15367, 15875, 14083, 13187, 12739, 12515, 12407, 12350, 12316, 0, 0, 0}, // U+0032 (2)
    {0,0,3084, 7182, 14343, 12483, 12483, 12483, 12483, 12483, 14823, 8062, 3644, 0,0,0},         // U+0033 (3)
    {0,0,960, 992, 880, 824, 796, 782, 775, 16383, 16383, 768, 768, 0,0,0},                       // U+0034 (4)
    {0,0,3135, 7295, 14435, 12387, 12387, 12387, 12387, 12387, 14563, 8131, 3971,0,0,0},          // U+0035 (5)
    {0,0,4032, 8176, 14840, 12508, 12494, 12487, 12483, 12483, 14787, 8064, 3840, 0,0,0},         // '6'  54
	{0,0,3, 3, 3, 12291, 15363, 3843, 963, 243, 63, 15, 3, 0,0,0},                                // '7'  55
	{0,0,3840, 8124, 14846, 12519, 12483, 12483, 12483, 12519, 14846, 8124, 3840, 0,0,0},         // '8'  56
	{0,0,60, 126, 12519, 12483, 12483, 14531, 7363, 3779, 2023, 1022, 252, 0,0,0},                // '9'  57
	{0,0,0, 0, 0, 7280, 7280, 7280, 0, 0, 0, 0, 0, 0,0,0},                                        // ':'  58
	{0,0,0, 0, 0, 40048, 64624, 31856, 0, 0, 0, 0, 0, 0,0,0},                                     // ';'  59
	{0,0,0, 192, 480, 1008, 1848, 3612, 7182, 14343, 12291, 0, 0, 0,0,0},                         // '<'  60
	{0,0,0, 1632, 1632, 1632, 1632, 1632, 1632, 1632, 1632, 1632, 0, 0,0,0},                      // '='  61
	{0,0,0, 12291, 14343, 7182, 3612, 1848, 1008, 480, 192, 0, 0, 0,0,0},                         // '>'  62
	{0,0,28, 30, 7, 3, 14211, 14275, 227, 119, 62, 28, 0, 0,0,0},                                 // '?'  63
	{0,0,4088, 8190, 6151, 13299, 14331, 13851, 14331, 14331, 13831, 1022, 504, 0,0,0},           // '@'  64
	{0,0,14336, 16128, 2016, 1788, 1567, 1567, 1788, 2016, 16128, 14336, 0, 0,0,0},               // 'A'  65
	{0,0,16383, 16383, 12483, 12483, 12483, 12483, 12519, 14846, 8124, 3840, 0, 0,0,0},           // 'B'  66
	{0,0,1008, 4092, 7182, 14343, 12291, 12291, 12291, 14343, 7182, 3084, 0, 0,0,0},              // 'C'  67
	{0,0,16383, 16383, 12291, 12291, 12291, 12291, 14343, 7182, 4092, 1008, 0, 0,0,0},            // 'D'  68
	{0,0,16383, 16383, 12483, 12483, 12483, 12483, 12483, 12483, 12291, 12291, 0, 0,0,0},         // 'E'  69
	{0,0,16383, 16383, 195, 195, 195, 195, 195, 195, 3, 3, 0, 0,0,0},                             // 'F'  70
	{0,0,1008, 4092, 7182, 14343, 12291, 12483, 12483, 12483, 16327, 16326, 0, 0,0,0},            // 'G'  71
	{0,0,16383, 16383, 192, 192, 192, 192, 192, 192, 16383, 16383, 0, 0,0,0},                     // 'H'  72
	{0,0,0, 0, 12291, 12291, 16383, 16383, 12291, 12291, 0, 0, 0, 0,0,0},                         // 'I'  73
	{0,0,3584, 7680, 14336, 12288, 12288, 12288, 12288, 14336, 8191, 2047, 0, 0,0,0},             // 'J'  74
	{0,0,16383, 16383, 192, 480, 1008, 1848, 3612, 7182, 14343, 12291, 0, 0,0,0},                 // 'K'  75
	{0,0,16383, 16383, 12288, 12288, 12288, 12288, 12288, 12288, 12288, 12288, 0, 0,0,0},         // 'L'  76
	{0,0,16383, 16383, 30, 120, 480, 480, 120, 30, 16383, 16383, 0, 0,0,0},                       // 'M'  77
	{0,0,16383, 16383, 14, 56, 240, 960, 1792, 7168, 16383, 16383, 0, 0,0,0},                     // 'N'  78
	{0,0,1008, 4092, 7182, 14343, 12291, 12291, 14343, 7182, 4092, 1008, 0, 0,0,0},               // 'O'  79
	{0,0,16383, 16383, 387, 387, 387, 387, 387, 455, 254, 124, 0, 0,0,0},                         // 'P'  80
	{0,0,1008, 4092, 7182, 14343, 12291, 13827, 15879, 7182, 16380, 13296, 0, 0,0,0},             // 'Q'  81
	{0,0,16383, 16383, 387, 387, 899, 1923, 3971, 7623, 14590, 12412, 0, 0,0,0},                  // 'R'  82
	{0,0,3132, 7294, 14567, 12483, 12483, 12483, 12483, 14791, 8078, 3852, 0, 0,0,0},             // 'S'  83
	{0,0,0, 3, 3, 3, 16383, 16383, 3, 3, 3, 0, 0, 0,0,0},                                         // 'T'  84
	{0,0,2047, 8191, 14336, 12288, 12288, 12288, 12288, 14336, 8191, 2047, 0, 0,0,0},             // 'U'  85
	{0,0,7, 63, 504, 4032, 15872, 15872, 4032, 504, 63, 7, 0, 0,0,0},                             // 'V'  86
	{0,0,16383, 16383, 7168, 1536, 896, 896, 1536, 7168, 16383, 16383, 0, 0,0,0},                 // 'W'  87
	{0,0,12291, 15375, 3612, 816, 480, 480, 816, 3612, 15375, 12291, 0, 0,0,0},                   // 'X'  88
	{0,0,3, 15, 60, 240, 16320, 16320, 240, 60, 15, 3, 0, 0,0,0},                                 // 'Y'  89
	{0,0,12291, 15363, 15875, 13059, 12739, 12515, 12339, 12319, 12303, 12291, 0, 0,0,0},         // 'Z'  90
	{0,0,0, 0, 16383, 16383, 12291, 12291, 12291, 12291, 0, 0, 0, 0,0,0},                         // '['  91
	{0,0,14, 28, 56, 112, 224, 448, 896, 1792, 3584, 7168, 6144, 0,0,0},                          // '\'  92
	{0,0,0, 0, 12291, 12291, 12291, 12291, 16383, 16383, 0, 0, 0, 0,0,0},                         // ']'  93
	{0,0,96, 112, 56, 28, 14, 7, 14, 28, 56, 112, 96, 0,0,0},                                     // '^'  94
	{0,0,49152, 49152, 49152, 49152, 49152, 49152, 49152, 49152, 49152, 49152, 49152, 0,0,0},     // '_'  95
	{0,0,0, 0, 0, 0, 62, 126, 78, 0, 0, 0, 0, 0,0,0},                                             // '`'  96
	{0,0,7168, 15936, 13152, 13152, 13152, 13152, 13152, 13152, 16352, 16320, 0, 0,0,0},          // 'a'  97
	{0,0,16383, 16383, 12480, 12384, 12384, 12384, 12384, 14560, 8128, 3968, 0, 0,0,0},           // 'b'  98
	{0,0,3968, 8128, 14560, 12384, 12384, 12384, 12384, 12384, 6336, 2176, 0, 0,0,0},             // 'c'  99
	{0,0,3968, 8128, 14560, 12384, 12384, 12384, 12512, 12480, 16383, 16383, 0, 0,0,0},           // 'd' 100
	{0,0,3968, 8128, 15328, 13152, 13152, 13152, 13152, 13152, 5056, 384, 0, 0,0,0},              // 'e' 101
	{0,0,192, 192, 16380, 16382, 199, 195, 195, 3, 0, 0, 0, 0,0,0},                               // 'f' 102
	{0,0,896, 51136, 52960, 52320, 52320, 52320, 52320, 58976, 32736, 16352, 0, 0,0,0},           // 'g' 103
	{0,0,16383, 16383, 192, 96, 96, 96, 224, 16320, 16256, 0, 0, 0,0,0},                          // 'h' 104
	{0,0,0, 0, 12288, 12384, 16364, 16364, 12288, 12288, 0, 0, 0, 0,0,0},                         // 'i' 105
	{0,0,0, 0, 24576, 57344, 49152, 49248, 65516, 32748, 0, 0, 0, 0,0,0},                         // 'j' 106
	{0,0,0, 16383, 16383, 768, 1920, 4032, 7392, 14432, 12288, 0, 0, 0,0,0},                      // 'k' 107
	{0,0,0, 0, 12288, 12291, 16383, 16383, 12288, 12288, 0, 0, 0, 0,0,0},                         // 'l' 108
	{0,0,16352, 16320, 224, 224, 16320, 16320, 224, 224, 16320, 16256, 0, 0,0,0},                 // 'm' 109
	{0,0,0, 16352, 16352, 96, 96, 96, 96, 224, 16320, 16256, 0, 0,0,0},                           // 'n' 110
	{0,0,3968, 8128, 14560, 12384, 12384, 12384, 12384, 14560, 8128, 3968, 0, 0,0,0},             // 'o' 111
	{0,0,65504, 65504, 3168, 6240, 6240, 6240, 6240, 7392, 4032, 1920, 0, 0,0,0},                 // 'p' 112
	{0,0,1920, 4032, 7392, 6240, 6240, 6240, 6240, 3168, 65504, 65504, 0, 0,0,0},                 // 'q' 113
	{0,0,0, 16352, 16352, 192, 96, 96, 96, 96, 224, 192, 0, 0,0,0},                               // 'r' 114
	{0,0,4544, 13280, 13152, 13152, 13152, 13152, 16224, 7744, 0, 0, 0, 0,0,0},                   // 's' 115
	{0,0,96, 96, 8190, 16382, 12384, 12384, 12384, 12288, 0, 0, 0, 0,0,0},                        // 't' 116
	{0,0,4064, 8160, 14336, 12288, 12288, 12288, 12288, 6144, 16352, 16352, 0, 0,0,0},            // 'u' 117
	{0,0,96, 480, 1920, 7680, 14336, 14336, 7680, 1920, 480, 96, 0, 0,0,0},                       // 'v' 118
	{0,0,2016, 8160, 14336, 7168, 4064, 4064, 7168, 14336, 8160, 2016, 0, 0,0,0},                 // 'w' 119
	{0,0,12384, 14560, 7616, 3968, 1792, 3968, 7616, 14560, 12384, 0, 0, 0,0,0},                  // 'x' 120
	{0,0,0, 96, 33248, 59264, 32256, 7680, 1920, 480, 96, 0, 0, 0,0,0},                           // 'y' 121
	{0,0,12384, 14432, 15456, 13920, 13152, 12768, 12512, 12384, 12320, 0, 0, 0,0,0},             // 'z' 122
	{0,0,0, 128, 448, 8188, 16254, 28679, 24579, 24579, 24579, 0, 0, 0,0,0},                      // '{' 123
	{0,0,0, 0, 0, 0, 16383, 16383, 0, 0, 0, 0, 0, 0,0,0},									     // '|' 124
	{0,0,0, 24579, 24579, 24579, 28679, 16254, 8188, 448, 128, 0, 0, 0,0,0},                      // '}' 125
	{0,0,16, 24, 12, 4, 12, 24, 16, 24, 12, 4, 0, 0,0,0},                                         // '~' 126
};

uint16_t font_white[128][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0000 (nul)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0001
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0002
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0003
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0004
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0005
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0006
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0007
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0008
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0009
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0010
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0011
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0012
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0013
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0014
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0015
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0016
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0017
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0018
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0019
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0020 (space)
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (//)
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+007F
};

uint16_t cursor[16] = {1798, 2249, 4401, 4161, 9218, 10500, 8716, 24580, 36868, 32776, 17424, 9184, 4352, 2304, 1536, 0};

uint16_t cursor_click[16] = {49193, 50966, 2505, 53297, 4161, 9218, 10501, 8716, 24580, 36868, 33800, 16400, 9184, 4352, 2304, 1536};

uint16_t abort_button[16] = {61455, 57351, 50499, 34113, 5440, 5440, 4432, 8144, 8144, 8144, 8176, 8176, 36833, 51139, 57351, 61455};

uint16_t plus_button[16] = {61455, 49155, 32769, 33153, 384, 384, 384, 8184, 8184, 384, 384, 384, 33153, 32769, 49155, 61455};

uint16_t right_arrow[16] = {61455, 49155, 32769, 32769, 0, 768, 896, 992, 992, 896, 512, 0, 32769, 32769, 49155, 61455};

uint16_t left_arrow[16] = {61455, 49155, 32769, 32769, 0, 64, 448, 960, 960, 448, 64, 0, 32769, 32769, 49155, 61455};

uint16_t nice_button[16] = {65151, 64575, 64575, 63519, 57351, 0, 0, 0, 32769, 49155, 57351, 57351, 57351, 49155, 49539, 50115};

uint16_t showAllButton[16] = {65535, 65535, 65535, 65523, 65505, 65472, 53120, 34561, 771, 7, 32783, 49167, 57375, 61503, 63615, 64767};

#endif