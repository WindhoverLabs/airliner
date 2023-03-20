/****************************************************************************
*
*   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name Windhover Labs nor the names of its 
*    contributors may be used to endorse or promote products derived 
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#include "cfe.h"
#include "rcin_app.h"

#include "rcin_custom_hooks.h"

#include <string.h>
#include <time.h>
#include <inttypes.h>


uint32  SEDLIB_ReadMsg_Cnt = 0;
uint32  Measure_False_Cnt = 0;

uint8 Ut_RxBuffer_1Frame_Nominal[UT_TEST_BUF_SIZE_1FRAME] = {
    15, 164, 169,  76,  26,  98,  97,  17, 206,  73, 115, 161,  86, 229, 132, 245, 223, 251, 117,  25,  64,  50,  54, 217,  0
};

uint8 Ut_RxBuffer_2Frame_Nominal[UT_TEST_BUF_SIZE_2FRAME] = {
    15, 164, 169,  76,  26,  98,  97,  17, 206,  73, 115, 161,  86, 229, 132, 245, 223, 251, 117,  25,  64,  50,  54, 217,  0,
    15, 173, 165,  56,  54,   3, 190, 148, 166,  60,  32, 104, 252, 226,  29, 248, 168, 235, 140,  99,  86, 102, 188,   3,  0
};

uint8 Ut_RxBuffer_2Frame_RcLost[UT_TEST_BUF_SIZE_2FRAME] = {
    15, 164, 169,  76,  26,  98,  97,  17, 206,  73, 115, 161,  86, 229, 132, 245, 223, 251, 117,  25,  64,  50,  54, 217,  0,
    15, 243, 156,   9, 243, 143, 143, 170, 122, 141,  25,  21, 167, 166, 103, 199,  37, 119,  64, 127, 155, 135,  83, 159,  0
};

uint8 Ut_RxBuffer_2Frame_1NoFooter[UT_TEST_BUF_SIZE_2FRAME] = {
    15, 243, 156,   9, 243, 143, 143, 170, 122, 141,  25,  21, 167, 166, 103, 199,  37, 119,  64, 127, 155, 135,  83, 159,  0,
    15,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155, 10
};

uint8 Ut_RxBuffer_2Frame_1NoHdr[UT_TEST_BUF_SIZE_2FRAME] = {
    15, 229, 137, 165, 171, 140,  33,  64,  25,  96,  65, 229,  69,  65,  88,   9, 207,  38, 211,  84, 190, 189,   3, 104,  0,
    42,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155,  0
};

uint8 Ut_RxBuffer_10Frame_1NoHdr1NoFooter[UT_TEST_BUF_SIZE_10FRAME] = {
    15, 229, 137, 165, 171, 140,  33,  64,  25,  96,  65, 229,  69,  65,  88,   9, 207,  38, 211,  84, 190, 189,   3, 104,  0,
    /* Missing header */
    42,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155,  0,
    15, 119,   4,  42, 244, 130, 183, 138,  50,  10, 142, 136,  17,  97,  22,  11,  67,  19, 247, 162, 103, 116, 168,  44,  0,
    15, 230,  35,  63, 195, 226, 255, 126,  74, 180, 238,  22, 133, 136, 183,  12,  50,  15,  62, 253, 226,   6, 252,  76,  0,
    15, 149, 112,  47,  47,  90, 230,  69,  77,  43,  60, 211, 243, 181,  58,  69,  55, 244, 190, 190,  98, 236,  73,  84,  0,
    15, 237, 202,  89, 163, 234,  27, 154,  65, 219, 175, 231, 218, 185,   5, 219, 149,  25, 116,  94,  77,  40,  48, 138,  0,
    /* Missing footer */
    15,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155, 10,
    15,   8,  90,  24,  40, 159, 113, 100, 190,  17,  15, 211, 189,  82,  61,  22, 168,  51,   8, 133,  50, 238, 205, 255,  0,
    15, 206, 200, 132,  55, 226,  14, 196, 216, 163,  94, 123, 113, 253,  45,  94,  86, 229, 153, 234, 239, 225, 242,  54,  0,
    15, 205, 104,   0, 128,  86, 231,  15, 112, 130, 133, 203,  34, 241, 250,   1, 163, 158,  36,  99, 152, 114,  89,  70,  0
};

uint8 Ut_RxBuffer_Multiple_1NoHdr1NoFooter[UT_TEST_BUF_SIZE_30FRAME] = {
    15, 229, 137, 165, 171, 140,  33,  64,  25,  96,  65, 229,  69,  65,  88,   9, 207,  38, 211,  84, 190, 189,   3, 104,  0,
    15, 145, 151, 234, 183, 149, 129, 152, 128,  77,  31, 243, 239,  58, 197, 162, 177, 117, 186,  25,  39, 140, 122, 106,  0,
    15,  88, 168, 102, 150,  71,  49,  59,  46,  76, 114, 181,  23, 226, 144,  17, 160, 253, 126,  47, 184, 202, 225,  47,  0,
    15, 215, 191,  83,  10, 166, 171, 186, 143, 160, 234, 141, 223, 111,  17,  31, 102, 251,  92, 223, 200, 251, 191, 228,  0,
    15, 205, 104,   0, 128,  86, 231,  15, 112, 130, 133, 203,  34, 241, 250,   1, 163, 158,  36,  99, 152, 114,  89,  70,  0,
    /* Missing header */
    42,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155,  0,
    15, 164, 169,  76,  26,  98,  97,  17, 206,  73, 115, 161,  86, 229, 132, 245, 223, 251, 117,  25,  64,  50,  54, 217,  0,
    15, 173, 165,  56,  54,   3, 190, 148, 166,  60,  32, 104, 252, 226,  29, 248, 168, 235, 140,  99,  86, 102, 188,   3,  0,
    15, 243, 156,   9, 243, 143, 143, 170, 122, 141,  25,  21, 167, 166, 103, 199,  37, 119,  64, 127, 155, 135,  83, 159,  0,
    15, 201,  79, 134, 180, 124, 146, 222, 242, 250, 170,  94,  55, 122, 247,  87, 172,  96,  85, 122,  76, 220,  37,  72,  0,
    15,  47, 166,  30, 156,   6,  16,  73,  73, 152,  30,  25, 202,  47, 171, 191, 194, 253, 161,  16, 112, 126,   7, 216,  0,
    /* Missing footer */
    15,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155, 10,
    15,  46,  56,  58, 198,  30, 131,  81,   9, 146, 166,  73,  57, 219,   9,  92, 200,  29, 145, 205,  20,  16, 106, 239,  0,
    15, 157,  44,  76, 182, 157, 149,   0, 236, 236, 193, 230, 139, 166,  96, 160,  53, 160,  94,  90, 117, 251,   5, 104,  0,
    15, 236,  31, 119, 239, 174, 162, 247, 126, 238, 254,  73,  61, 138, 213, 149,  35,  20, 148, 224, 199,  99,  39,  45,  0,
    15, 206, 200, 132,  55, 226,  14, 196, 216, 163,  94, 123, 113, 253,  45,  94,  86, 229, 153, 234, 239, 225, 242,  54,  0,
    15, 230,  35,  63, 195, 226, 255, 126,  74, 180, 238,  22, 133, 136, 183,  12,  50,  15,  62, 253, 226,   6, 252,  76,  0,
    15, 149, 112,  47,  47,  90, 230,  69,  77,  43,  60, 211, 243, 181,  58,  69,  55, 244, 190, 190,  98, 236,  73,  84,  0,
    15, 237, 202,  89, 163, 234,  27, 154,  65, 219, 175, 231, 218, 185,   5, 219, 149,  25, 116,  94,  77,  40,  48, 138,  0,
    15,  65, 246, 214,  13, 182, 171, 190,  33, 231, 136,  38, 113,   2,  23,  16, 115, 152,  76,  67, 131, 180, 119,  82,  0,
    15, 126, 11,  198, 176,  78, 230, 239,  19, 110,  48, 216,  18, 183, 178,  23,  76, 124,  32,  94, 191,  97, 160, 106,  0,
    15, 180, 230,  15, 150,  24, 147, 117, 161,  95,  54,  59,  88, 145, 132,  32, 214, 189, 255,  74, 204, 180, 225, 144,  0,
    15, 138, 118,  76,  72, 239, 127, 112, 233,  77, 169,  85, 132, 152, 123, 182,  93, 226, 191, 107, 131, 185, 158, 239,  0,
    15,   8,  90,  24,  40, 159, 113, 100, 190,  17,  15, 211, 189,  82,  61,  22, 168,  51,   8, 133,  50, 238, 205, 255,  0,
    15, 119,   4,  42, 244, 130, 183, 138,  50,  10, 142, 136,  17,  97,  22,  11,  67,  19, 247, 162, 103, 116, 168,  44,  0,
    15,  86,  37,  99, 220, 243,   6, 104, 185, 158, 202,  40, 201, 171, 246, 141,  49, 250,  97,  14, 194, 192,  32, 129,  0,
    15,  75, 218, 130, 113, 126, 231, 229, 226, 171, 125,  73, 230,  62, 170, 228, 120,  30, 118, 102, 181, 230, 215, 185,  0,
    15,   3, 137,  23, 166,  56,  27, 117,  37,  44,  81, 176,  95, 166, 161, 235,  24,  16,  81, 252, 147,   5, 109, 143,  0,
    15,  50, 104,  34,  64, 248, 148, 150, 113, 131, 123, 219, 116,  45, 214, 226, 105, 110, 122,  64, 227,  66,  66,  99,  0,
    15, 229, 137, 165, 171, 140,  33,  64,  25,  96,  65, 229,  69,  65,  88,   9, 207,  38, 211,  84, 190, 189,   3, 104,  0
};

uint8 Ut_RxBuffer_Multiple_25NoHdr[UT_TEST_BUF_SIZE_30FRAME] = {
    15, 229, 137, 165, 171, 140,  33,  64,  25,  96,  65, 229,  69,  65,  88,   9, 207,  38, 211,  84, 190, 189,   3, 104,  0,
    15, 145, 151, 234, 183, 149, 129, 152, 128,  77,  31, 243, 239,  58, 197, 162, 177, 117, 186,  25,  39, 140, 122, 106,  0,
    15, 229, 137, 165, 171, 140,  33,  64,  25,  96,  65, 229,  69,  65,  88,   9, 207,  38, 211,  84, 190, 189,   3, 104,  0,
    15, 215, 191,  83,  10, 166, 171, 186, 143, 160, 234, 141, 223, 111,  17,  31, 102, 251,  92, 223, 200, 251, 191, 228,  0,
    15, 205, 104,   0, 128,  86, 231,  15, 112, 130, 133, 203,  34, 241, 250,   1, 163, 158,  36,  99, 152, 114,  89,  70,  0,
    /* 25 Missing header */
    42,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155,  0,
    30, 164, 169,  76,  26,  98,  97,  17, 206,  73, 115, 161,  86, 229, 132, 245, 223, 251, 117,  25,  64,  50,  54, 217,  0,
    20, 173, 165,  56,  54,   3, 190, 148, 166,  60,  32, 104, 252, 226,  29, 248, 168, 235, 140,  99,  86, 102, 188,   3,  0,
    10, 243, 156,   9, 243, 143, 143, 170, 122, 141,  25,  21, 167, 166, 103, 199,  37, 119,  64, 127, 155, 135,  83, 159,  0,
     5, 201,  79, 134, 180, 124, 146, 222, 242, 250, 170,  94,  55, 122, 247,  87, 172,  96,  85, 122,  76, 220,  37,  72,  0,
    73, 230,  35,  63, 195, 226, 255, 126,  74, 180, 238,  22, 133, 136, 183,  12,  50, 150,  62, 253, 226,   6, 252,  76,  0,
    23,  47, 166,  30, 156,   6,  16,  73,  73, 152,  30,  25, 202,  47, 171, 191, 194, 253, 161,  16, 112, 126,   7, 216,  0,
    16,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155,  0,
    41,  46,  56,  58, 198,  30, 131,  81,   9, 146, 166,  73,  57, 219,   9,  92, 200,  29, 145, 205,  20,  16, 106, 239,  0,
    60, 157,  44,  76, 182, 157, 149,   0, 236, 236, 193, 230, 139, 166,  96, 160,  53, 160,  94,  90, 117, 251,   5, 104,  0,
    84, 236,  31, 119, 239, 174, 162, 247, 126, 238, 254,  73,  61, 138, 213, 149,  35,  20, 148, 224, 199,  99,  39,  45,  0,
    90, 206, 200, 132,  55, 226,  14, 196, 216, 163,  94, 123, 113, 253,  45,  94,  86, 229, 153, 234, 239, 225, 242,  54,  0,
    93, 149, 112,  47,  47,  90, 230,  69,  77,  43,  60, 211, 243, 181,  58,  69,  55, 244, 190, 190,  98, 236,  73,  84,  0,
    29, 237, 202,  89, 163, 234,  27, 154,  65, 219, 175, 231, 218, 185,   5, 219, 149,  25, 116,  94,  77,  40,  48, 138,  0,
    57,  65, 246, 214,  13, 182, 171, 190,  33, 231, 136,  38, 113,   2,  23,  16, 115, 152,  76,  67, 131, 180, 119,  82,  0,
    11, 126, 11,  198, 176,  78, 230, 239,  19, 110,  48, 216,  18, 183, 178,  23,  76, 124,  32,  94, 191,  97, 160, 106,  0,
    65, 180, 230, 105, 150,  24, 147, 117, 161,  95,  54,  59,  88, 145, 132,  32, 214, 189, 255,  74, 204, 180, 225, 144,  0,
    37, 138, 118,  76,  72, 239, 127, 112, 233,  77, 169,  85, 132, 152, 123, 182,  93, 226, 191, 107, 131, 185, 158, 239,  0,
    74,   8,  90,  24,  40, 159, 113, 100, 190,  17, 125, 211, 189,  82,  61,  22, 168,  51,   8, 133,  50, 238, 205, 255,  0,
    96, 119,   4,  42, 244, 130, 183, 138,  50,  10, 142, 136,  17,  97,  22,  11,  67,  19, 247, 162, 103, 116, 168,  44,  0,
    79,  86,  37,  99, 220, 243,   6, 104, 185, 158, 202,  40, 201, 171, 246, 141,  49, 250,  97,  14, 194, 192,  32, 129,  0,
     9,  75, 218, 130, 113, 126, 231, 229, 226, 171, 125,  73, 230,  62, 170, 228, 120,  30, 118, 102, 181, 230, 215, 185,  0,
    39,   3, 137,  23, 166,  56,  27, 117,  37,  44,  81, 176,  95, 166, 161, 235,  24,  16,  81, 252, 147,   5, 109, 143,  0,
    50,  50, 104,  34,  64, 248, 148, 150, 113, 131, 123, 219, 116,  45, 214, 226, 105, 110, 122,  64, 227,  66,  66,  99,  0,
    48, 229, 137, 165, 171, 140,  33,  64,  25,  96,  65, 229,  69,  65,  88,   9, 207,  38, 211,  84, 190, 189,   3, 104,  0
};

uint8 Ut_RxBuffer_Multiple_25Bytes[UT_TEST_BUF_SIZE_30FRAME] = {
    15, 229, 137, 165, 171, 140,  33,  64,  25,  96,  65, 229,  69,  65,  88,   9, 207,  38, 211,  84, 190, 189,   3, 104,  0,
    15, 145, 151, 234, 183, 149, 129, 152, 128,  77,  31, 243, 239,  58, 197, 162, 177, 117, 186,  25,  39, 140, 122, 106,  0,
    15,  88, 168, 102, 150,  71,  49,  59,  46,  76, 114, 181,  23, 226, 144,  17, 160, 253, 126,  47, 184, 202, 225,  47,  0,
    15, 215, 191,  83,  10, 166, 171, 186, 143, 160, 234, 141, 223, 111,  17,  31, 102, 251,  92, 223, 200, 251, 191, 228,  0,
    15,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155, 10,
    15, 205, 104,   0, 128,  86, 231,  15, 112, 130, 133, 203,  34, 241, 250,   1, 163, 158,  36,  99, 152, 114,  89,  70,  0,
    15, 164, 169,  76,  26,  98,  97,  17, 206,  73, 115, 161,  86, 229, 132, 245, 223, 251, 117,  25,  64,  50,  54, 217,  0,
    15, 173, 165,  56,  54,   3, 190, 148, 166,  60,  32, 104, 252, 226,  29, 248, 168, 235, 140,  99,  86, 102, 188,   3,  0,
    15, 243, 156,   9, 243, 143, 143, 170, 122, 141,  25,  21, 167, 166, 103, 199,  37, 119,  64, 127, 155, 135,  83, 159,  0,
    15, 201,  79, 134, 180, 124, 146, 222, 242, 250, 170,  94,  55, 122, 247,  87, 172,  96,  85, 122,  76, 220,  37,  72,  0,
    15,  47, 166,  30, 156,   6,  16,  73,  73, 152,  30,  25, 202,  47, 171, 191, 194, 253, 161,  16, 112, 126,   7, 216,  0,
    15,  46,  56,  58, 198,  30, 131,  81,   9, 146, 166,  73,  57, 219,   9,  92, 200,  29, 145, 205,  20,  16, 106, 239,  0,
    15, 157,  44,  76, 182, 157, 149,   0, 236, 236, 193, 230, 139, 166,  96, 160,  53, 160,  94,  90, 117, 251,   5, 104,  0,
    15, 236,  31, 119, 239, 174, 162, 247, 126, 238, 254,  73,  61, 138, 213, 149,  35,  20, 148, 224, 199,  99,  39,  45,  0,
    15, 206, 200, 132,  55, 226,  14, 196, 216, 163,  94, 123, 113, 253,  45,  94,  86, 229, 153, 234, 239, 225, 242,  54,  0,
    15, 230,  35,  63, 195, 226, 255, 126,  74, 180, 238,  22, 133, 136, 183,  12,  50,  15,  62, 253, 226,   6, 252,  76,  0,
    15, 149, 112,  47,  47,  90, 230,  69,  77,  43,  60, 211, 243, 181,  58,  69,  55, 244, 190, 190,  98, 236,  73,  84,  0,
    15, 237, 202,  89, 163, 234,  27, 154,  65, 219, 175, 231, 218, 185,   5, 219, 149,  25, 116,  94,  77,  40,  48, 138,  0,
    15,  65, 246, 214,  13, 182, 171, 190,  33, 231, 136,  38, 113,   2,  23,  16, 115, 152,  76,  67, 131, 180, 119,  82,  0,
    15, 126,  11, 198, 176,  78, 230, 239,  19, 110,  48, 216,  18, 183, 178,  23,  76, 124,  32,  94, 191,  97, 160, 106,  0,
    15, 180, 230,  15, 150,  24, 147, 117, 161,  95,  54,  59,  88, 145, 132,  32, 214, 189, 255,  74, 204, 180, 225, 144,  0,
    15, 138, 118,  76,  72, 239, 127, 112, 233,  77, 169,  85, 132, 152, 123, 182,  93, 226, 191, 107, 131, 185, 158, 239,  0,
    15,   8,  90,  24,  40, 159, 113, 100, 190,  17,  15, 211, 189,  82,  61,  22, 168,  51,   8, 133,  50, 238, 205, 255,  0,
    15, 119,   4,  42, 244, 130, 183, 138,  50,  10, 142, 136,  17,  97,  22,  11,  67,  19, 247, 162, 103, 116, 168,  44,  0,
    15,  86,  37,  99, 220, 243,   6, 104, 185, 158, 202,  40, 201, 171, 246, 141,  49, 250,  97,  14, 194, 192,  32, 129,  0,
    15,  75, 218, 130, 113, 126, 231, 229, 226, 171, 125,  73, 230,  62, 170, 228, 120,  30, 118, 102, 181, 230, 215, 185,  0,
    15,   3, 137,  23, 166,  56,  27, 117,  37,  44,  81, 176,  95, 166, 161, 235,  24,  16,  81, 252, 147,   5, 109, 143,  0,
    15,  50, 104,  34,  64, 248, 148, 150, 113, 131, 123, 219, 116,  45, 214, 226, 105, 110, 122,  64, 227,  66,  66,  99,  0,
    15, 164, 169,  76,  26,  98,  97,  17, 206,  73, 115, 161,  86, 229, 132, 245, 223, 251, 117,  25,  64,  50,  54, 217,  0,
    15, 173, 165,  56,  54,   3, 190, 148, 166,  60,  32, 104, 252, 226,  29, 248, 168, 235, 140,  99,  86, 102, 188,   3,  0
};

/* Same data with Ut_RxBuffer_Multiple_25Bytes */
uint8 Ut_RxBuffer_Multiple_12Bytes[UT_TEST_BUF_SIZE_30FRAME] = {
     15, 229, 137, 165, 171, 140,  33,  64,  25,  96,  65, 229,
     69,  65,  88,   9, 207,  38, 211,  84, 190, 189,   3, 104,
      0,  15, 145, 151, 234, 183, 149, 129, 152, 128,  77,  31,
    243, 239,  58, 197, 162, 177, 117, 186,  25,  39, 140, 122,
    106,   0,  15,  88, 168, 102, 150,  71,  49,  59,  46,  76,
    114, 181,  23, 226, 144,  17, 160, 253, 126,  47, 184, 202,
    225,  47,   0,  15, 215, 191,  83,  10, 166, 171, 186, 143,
    160, 234, 141, 223, 111,  17,  31, 102, 251,  92, 223, 200,
    251, 191, 228,   0,  15,  60,   8,  46,  25, 208,  91,  76,
    234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,
      9,  90, 112, 155,  10,  15, 205, 104,   0, 128,  86, 231,
     15, 112, 130, 133, 203,  34, 241, 250,   1, 163, 158,  36,
     99, 152, 114,  89,  70,   0,  15, 164, 169,  76,  26,  98,
     97,  17, 206,  73, 115, 161,  86, 229, 132, 245, 223, 251,
    117,  25,  64,  50,  54, 217,   0,  15, 173, 165,  56,  54,
      3, 190, 148, 166,  60,  32, 104, 252, 226,  29, 248, 168,
    235, 140,  99,  86, 102, 188,   3,   0,  15, 243, 156,   9,
    243, 143, 143, 170, 122, 141,  25,  21, 167, 166, 103, 199,
     37, 119,  64, 127, 155, 135,  83, 159,   0,  15, 201,  79,
    134, 180, 124, 146, 222, 242, 250, 170,  94,  55, 122, 247,
     87, 172,  96,  85, 122,  76, 220,  37,  72,   0,  15,  47,
    166,  30, 156,   6,  16,  73,  73, 152,  30,  25, 202,  47,
    171, 191, 194, 253, 161,  16, 112, 126,   7, 216,   0,  15,
     46,  56,  58, 198,  30, 131,  81,   9, 146, 166,  73,  57,
    219,   9,  92, 200,  29, 145, 205,  20,  16, 106, 239,   0,
     15, 157,  44,  76, 182, 157, 149,   0, 236, 236, 193, 230,
    139, 166,  96, 160,  53, 160,  94,  90, 117, 251,   5, 104,
      0,  15, 236,  31, 119, 239, 174, 162, 247, 126, 238, 254,
     73,  61, 138, 213, 149,  35,  20, 148, 224, 199,  99,  39,
     45,   0,  15, 206, 200, 132,  55, 226,  14, 196, 216, 163,
     94, 123, 113, 253,  45,  94,  86, 229, 153, 234, 239, 225,
    242,  54,   0,  15, 230,  35,  63, 195, 226, 255, 126,  74,
    180, 238,  22, 133, 136, 183,  12,  50,  15,  62, 253, 226,
      6, 252,  76,   0,  15, 149, 112,  47,  47,  90, 230,  69,
     77,  43,  60, 211, 243, 181,  58,  69,  55, 244, 190, 190,
     98, 236,  73,  84,   0,  15, 237, 202,  89, 163, 234,  27,
    154,  65, 219, 175, 231, 218, 185,   5, 219, 149,  25, 116,
     94,  77,  40,  48, 138,   0,  15,  65, 246, 214,  13, 182,
    171, 190,  33, 231, 136,  38, 113,   2,  23,  16, 115, 152,
     76,  67, 131, 180, 119,  82,   0,  15, 126,  11, 198, 176,
     78, 230, 239,  19, 110,  48, 216,  18, 183, 178,  23,  76,
    124,  32,  94, 191,  97, 160, 106,   0,  15, 180, 230,  15,
    150,  24, 147, 117, 161,  95,  54,  59,  88, 145, 132,  32,
    214, 189, 255,  74, 204, 180, 225, 144,   0,  15, 138, 118,
     76,  72, 239, 127, 112, 233,  77, 169,  85, 132, 152, 123,
    182,  93, 226, 191, 107, 131, 185, 158, 239,   0,  15,   8,
     90,  24,  40, 159, 113, 100, 190,  17,  15, 211, 189,  82,
     61,  22, 168,  51,   8, 133,  50, 238, 205, 255,   0,  15,
    119,   4,  42, 244, 130, 183, 138,  50,  10, 142, 136,  17,
     97,  22,  11,  67,  19, 247, 162, 103, 116, 168,  44,   0,
     15,  86,  37,  99, 220, 243,   6, 104, 185, 158, 202,  40,
    201, 171, 246, 141,  49, 250,  97,  14, 194, 192,  32, 129,
      0,  15,  75, 218, 130, 113, 126, 231, 229, 226, 171, 125,
     73, 230,  62, 170, 228, 120,  30, 118, 102, 181, 230, 215,
    185,   0,  15,   3, 137,  23, 166,  56,  27, 117,  37,  44,
     81, 176,  95, 166, 161, 235,  24,  16,  81, 252, 147,   5,
    109, 143,   0,  15,  50, 104,  34,  64, 248, 148, 150, 113,
    131, 123, 219, 116,  45, 214, 226, 105, 110, 122,  64, 227,
     66,  66,  99,   0,  15, 164, 169,  76,  26,  98,  97,  17,
    206,  73, 115, 161,  86, 229, 132, 245, 223, 251, 117,  25,
     64,  50,  54, 217,   0,  15, 173, 165,  56,  54,   3, 190,
    148, 166,  60,  32, 104, 252, 226,  29, 248, 168, 235, 140,
     99,  86, 102, 188,   3,   0
};


int32 RCIN_Util_Stream_Emulator(void)
{
    int32 iStatus = CFE_SUCCESS;

    while(SEDLIB_ReadMsg_Cnt > 0)
    {
        SEDLIB_ReturnCode_t rc = (SEDLIB_ReturnCode_t)0;

        /* SBUS data should be 25 bytes every 7ms (highspeed mode). */
        OS_TaskDelay(7);

        rc = SEDLIB_ReadMsg(RCIN_AppCustomData.MsgPortHandle,
                            (CFE_SB_MsgPtr_t)&RCIN_AppCustomData.UartStatusTlm);
        if(SEDLIB_MSG_FRESH_OK == rc)
        {
            /* Fresh data. */
            RCIN_Custom_Parse((uint8 *)RCIN_AppCustomData.UartStatusTlm.RxBuffer,
                              RCIN_AppCustomData.UartStatusTlm.Hdr.BytesInBuffer);
        }
        else if(SEDLIB_MSG_STALE_OK == rc)
        {
            /* No data. */
            RCIN_Custom_RC_Lost(TRUE);
            continue;
        }
        else
        {
            /* An error occurred. We can't read from the incoming port. */
            RCIN_Custom_RC_Lost(TRUE);
            continue;
        }
    } /* end while loop */

    if(RCIN_CUSTOM_STREAMING != RCIN_AppCustomData.Status)
    {
        iStatus = RCIN_ERROR;
    }

    if (RCIN_AppCustomData.Measure.RcFailsafe == TRUE &&
       RCIN_AppCustomData.Measure.RcLost == TRUE)
    {
        iStatus = RCIN_ERROR;
    }

    return iStatus;
}


SEDLIB_ReturnCode_t SEDLIB_GetPipeHook(char *PipeName,
                                       uint32 Size, uint32 *PipeHandle)
{
    return SEDLIB_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_1Frame_Nominal(uint32 PipeHandle,
                                                    CFE_SB_MsgPtr_t Msg)
{
    UART_StatusTlm_t  *pMsg;

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, Ut_RxBuffer_1Frame_Nominal,
           UT_TEST_BUF_SIZE_1FRAME);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUF_SIZE_1FRAME;

    SEDLIB_ReadMsg_Cnt --;

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Frame_Nominal(uint32 PipeHandle,
                                                    CFE_SB_MsgPtr_t Msg)
{
    UART_StatusTlm_t  *pMsg;

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, Ut_RxBuffer_2Frame_Nominal,
           UT_TEST_BUF_SIZE_2FRAME);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUF_SIZE_2FRAME;

    SEDLIB_ReadMsg_Cnt --;

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Frame_RcLost(uint32 PipeHandle,
                                                  CFE_SB_MsgPtr_t Msg)
{
    UART_StatusTlm_t  *pMsg;

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, Ut_RxBuffer_2Frame_RcLost,
           UT_TEST_BUF_SIZE_2FRAME);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUF_SIZE_2FRAME;

    SEDLIB_ReadMsg_Cnt --;

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Frame_1NoFooter(uint32 PipeHandle,
                                                  CFE_SB_MsgPtr_t Msg)
{
    UART_StatusTlm_t  *pMsg;

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, Ut_RxBuffer_2Frame_1NoFooter,
           UT_TEST_BUF_SIZE_2FRAME);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUF_SIZE_2FRAME;

    SEDLIB_ReadMsg_Cnt --;

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Frame_1NoHdr(uint32 PipeHandle,
                                                 CFE_SB_MsgPtr_t Msg)
{
    UART_StatusTlm_t  *pMsg;

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, Ut_RxBuffer_2Frame_1NoHdr,
           UT_TEST_BUF_SIZE_2FRAME);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUF_SIZE_2FRAME;

    SEDLIB_ReadMsg_Cnt --;

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_10Frame_1NoHdr1NoFooter(
                            uint32 PipeHandle, CFE_SB_MsgPtr_t Msg)
{
    UART_StatusTlm_t  *pMsg;

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, Ut_RxBuffer_10Frame_1NoHdr1NoFooter,
           UT_TEST_BUF_SIZE_10FRAME);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUF_SIZE_10FRAME;

    SEDLIB_ReadMsg_Cnt --;

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_Multiple_1NoHdr1NoFooter(
                            uint32 PipeHandle, CFE_SB_MsgPtr_t Msg)
{
    static int        index1 = 0;
    UART_StatusTlm_t  *pMsg;

printf("!!!SEDLIB_ReadMsgHook_Multiple: index1(%d), SEDLIB_ReadMsg_Cnt(%ld)\n", index1, SEDLIB_ReadMsg_Cnt);
    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer,
           &Ut_RxBuffer_Multiple_1NoHdr1NoFooter[index1],
           UT_TEST_BUF_SIZE_1FRAME);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUF_SIZE_1FRAME;
    index1 += UT_TEST_BUF_SIZE_1FRAME;
    if (index1 >= UT_TEST_BUF_SIZE_30FRAME)
    {
        index1 = 0;
    }

    SEDLIB_ReadMsg_Cnt --;

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_Multiple_25NoHdr(
                            uint32 PipeHandle, CFE_SB_MsgPtr_t Msg)
{
    static int        index25 = 0;
    UART_StatusTlm_t  *pMsg;

printf("!!!SEDLIB_ReadMsgHook_Multiple_25NoHdr: index25(%d), SEDLIB_ReadMsg_Cnt(%ld)\n", index25, SEDLIB_ReadMsg_Cnt);
    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, &Ut_RxBuffer_Multiple_25NoHdr[index25],
           UT_TEST_BUF_SIZE_1FRAME);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUF_SIZE_1FRAME;
    index25 += UT_TEST_BUF_SIZE_1FRAME;
    if (index25 >= UT_TEST_BUF_SIZE_30FRAME)
    {
        index25 = 0;
    }

    SEDLIB_ReadMsg_Cnt --;

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_Multiple_25Bytes(
                            uint32 PipeHandle, CFE_SB_MsgPtr_t Msg)
{
    static int        index = 0;
    UART_StatusTlm_t  *pMsg;

printf("!!!SEDLIB_ReadMsgHook_Multiple_25Bytes: index(%d), SEDLIB_ReadMsg_Cnt(%ld)\n", index, SEDLIB_ReadMsg_Cnt);
    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, &Ut_RxBuffer_Multiple_25Bytes[index],
           UT_TEST_BUF_SIZE_1FRAME);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUF_SIZE_1FRAME;
    index += UT_TEST_BUF_SIZE_1FRAME;
    if (index >= UT_TEST_BUF_SIZE_30FRAME)
    {
        index = 0;
    }

    SEDLIB_ReadMsg_Cnt --;

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_Multiple_12Bytes(
                            uint32 PipeHandle, CFE_SB_MsgPtr_t Msg)
{
    static int        index12 = 0;
    int               length;
    UART_StatusTlm_t  *pMsg;

printf("!!!SEDLIB_ReadMsgHook_Multiple_12Bytes: index12(%d), SEDLIB_ReadMsg_Cnt(%ld)\n", index12, SEDLIB_ReadMsg_Cnt);
    if ((UT_TEST_BUF_SIZE_30FRAME - index12) < UT_TEST_BUF_SIZE_12BYTES)
    {
        length = UT_TEST_BUF_SIZE_30FRAME - index12;
    }
    else
    {
        length = UT_TEST_BUF_SIZE_12BYTES;
    }

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, &Ut_RxBuffer_Multiple_12Bytes[index12],
           length);
    pMsg->Hdr.BytesInBuffer = length;
    index12 += length;
    if (index12 >= UT_TEST_BUF_SIZE_30FRAME)
    {
        index12 = 0;
    }
for (int i = 0; i < length; i++)
{
    printf("%u ", pMsg->RxBuffer[i]);
}
printf("\n");

    SEDLIB_ReadMsg_Cnt --;

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_NoData(uint32 PipeHandle,
                                              CFE_SB_MsgPtr_t Msg)
{
printf("!!!SEDLIB_ReadMsgHook_NoData: SEDLIB_ReadMsg_Cnt(%ld)\n", SEDLIB_ReadMsg_Cnt);
    SEDLIB_ReadMsg_Cnt --;

    return SEDLIB_MSG_STALE_OK;
}


void GetMeasureValues(uint8 *data, uint16 *Values)
{
    Values[0] = (uint16)(((data[1] | data[2] << 8)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[1] = (uint16)(((data[2] >> 3 | data[3] << 5)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[2] = (uint16)(((data[3] >> 6 | data[4] << 2
          | data[5] << 10) & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f)
          + RCIN_SBUS_SCALE_OFFSET;
    Values[3] = (uint16)(((data[5] >> 1 | data[6] << 7)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[4] = (uint16)(((data[6] >> 4 | data[7] << 4)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[5] = (uint16)(((data[7] >> 7 | data[8] << 1
          | data[9] << 9) & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f)
          + RCIN_SBUS_SCALE_OFFSET;
    Values[6] = (uint16)(((data[9] >> 2 | data[10] << 6)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[7] = (uint16)(((data[10] >> 5 | data[11] << 3)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[8] = (uint16)(((data[12] | data[13] << 8)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[9] = (uint16)(((data[13] >> 3 | data[14] << 5)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[10] = (uint16)(((data[14] >> 6 | data[15] << 2
          | data[16] << 10) & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f)
          + RCIN_SBUS_SCALE_OFFSET;
    Values[11] = (uint16)(((data[16] >> 1 | data[17] << 7)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[12] = (uint16)(((data[17] >> 4 | data[18] << 4)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[13] = (uint16)(((data[18] >> 7 | data[19] << 1
          | data[20] << 9) & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f)
          + RCIN_SBUS_SCALE_OFFSET;
    Values[14] = (uint16)(((data[20] >> 2 | data[21] << 6)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[15] = (uint16)(((data[21] >> 5 | data[22] << 3)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;

    return;
}


uint32 GetChecksum_1Frame_Nominal(void)
{
    int     i;
    uint32  checksum;
    uint8   *data;
    uint16  Values[PX4_RC_INPUT_MAX_CHANNELS];

    data = Ut_RxBuffer_1Frame_Nominal;

    GetMeasureValues(data, Values);

    checksum = 0;
    for (i = 0; i < RCIN_SBUS_CHANNEL_COUNT; i++)
    {
        checksum += Values[i];
    }

    return (checksum);
}


uint32 GetChecksum_2Frame_Nominal(void)
{
    int     i;
    uint32  checksum;
    uint8   *data;
    uint16  Values[PX4_RC_INPUT_MAX_CHANNELS];

    data = &Ut_RxBuffer_2Frame_Nominal[UT_TEST_BUF_SIZE_2FRAME/2];

    GetMeasureValues(data, Values);

    checksum = 0;
    for (i = 0; i < RCIN_SBUS_CHANNEL_COUNT; i++)
    {
        checksum += Values[i];
    }

    return (checksum);
}


uint32 GetChecksum_2Frame_RcLost(void)
{
    int     i;
    uint32  checksum;
    uint8   *data;
    uint16  Values[PX4_RC_INPUT_MAX_CHANNELS];

    data = &Ut_RxBuffer_2Frame_RcLost[UT_TEST_BUF_SIZE_2FRAME/2];

    GetMeasureValues(data, Values);

    checksum = 0;
    for (i = 0; i < RCIN_SBUS_CHANNEL_COUNT; i++)
    {
        checksum += Values[i];
    }

    return (checksum);
}


CFE_TIME_SysTime_t  CFE_TIME_GetTimeHook(void)
{
    int                 iStatus;
    CFE_TIME_SysTime_t  CfeTime;
    struct timespec     time;

    iStatus = clock_gettime(CLOCK_REALTIME, &time);
    if (iStatus == 0)
    {
        CfeTime.Seconds = time.tv_sec;
        CfeTime.Subseconds = time.tv_nsec / 1000;
    }

    return CfeTime;
}


void CFE_SB_TimeStampMsgHook(CFE_SB_MsgPtr_t MsgPtr)
{
    CFE_SB_SetMsgTime(MsgPtr, CFE_TIME_GetTime());

    return;
}


void CFE_PSP_GetTimeHook(OS_time_t *LocalTime)
{
    int              iStatus;
    struct timespec  time;

    iStatus = clock_gettime(CLOCK_REALTIME, &time);
    if (iStatus == 0)
    {
        LocalTime->seconds = time.tv_sec;
        LocalTime->microsecs = time.tv_nsec / 1000;
    }

    return;
}


int32 OS_TaskDelayHook(uint32 millisecond)
{
    struct timespec  start_time;
    struct timespec  end_time;
    uint64           elapsed_time;

    clock_gettime(CLOCK_REALTIME, &start_time);
    do
    {
        clock_gettime(CLOCK_REALTIME, &end_time);
        elapsed_time = ((end_time.tv_sec * 1000) + (end_time.tv_nsec / 1000000))
               - ((start_time.tv_sec * 1000) + (start_time.tv_nsec / 1000000));
    } while(elapsed_time < millisecond);
printf("!!!OS_TaskDelayHook: elapsed_time(%" PRId64")\n", elapsed_time);
}
