/* gmp-mparam.h -- Compiler/machine parameter header file.

Copyright 1991, 1993, 1994, 1999, 2000, 2001, 2002, 2003 Free Software
Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#define BITS_PER_MP_LIMB 32
#define BYTES_PER_MP_LIMB 4

/* 1.2 GHz ARM Feroceon */

/* Generated by tuneup.c, 2010-04-02, gcc 4.3 */

#define MUL_KARATSUBA_THRESHOLD          36
#define MUL_TOOM3_THRESHOLD             125
#define MUL_TOOM4_THRESHOLD             178
#define MUL_TOOM8H_THRESHOLD            414

#define SQR_BASECASE_THRESHOLD           12
#define SQR_KARATSUBA_THRESHOLD          82
#define SQR_TOOM3_THRESHOLD             138
#define SQR_TOOM4_THRESHOLD             348
#define SQR_TOOM8_THRESHOLD             381

#define POWM_THRESHOLD                  170

#define GCD_ACCEL_THRESHOLD               6

#define JACOBI_BASE_METHOD                2

#define DIVREM_1_NORM_THRESHOLD           0  /* preinv always */
#define DIVREM_1_UNNORM_THRESHOLD         0  /* always */
#define MOD_1_NORM_THRESHOLD              0  /* always */
#define MOD_1_UNNORM_THRESHOLD            0  /* always */
#define USE_PREINV_DIVREM_1               1  /* preinv always */
#define USE_PREINV_MOD_1                  1  /* preinv always */
#define DIVREM_2_THRESHOLD                0  /* preinv always */
#define DIVEXACT_1_THRESHOLD              0  /* always */
#define MODEXACT_1_ODD_THRESHOLD          0  /* always */
#define MOD_1_1_THRESHOLD                 7
#define MOD_1_2_THRESHOLD               996
#define MOD_1_3_THRESHOLD               996
#define DIVREM_HENSEL_QR_1_THRESHOLD     10
#define RSH_DIVREM_HENSEL_QR_1_THRESHOLD    996
#define DIVREM_EUCLID_HENSEL_THRESHOLD     40

#define ROOTREM_THRESHOLD                 6

#define GET_STR_DC_THRESHOLD             17
#define GET_STR_PRECOMPUTE_THRESHOLD     30
#define SET_STR_DC_THRESHOLD          12643
#define SET_STR_PRECOMPUTE_THRESHOLD  12643

#define MUL_FFT_TABLE  { 400, 1056, 1408, 3584, 10240, 40960, 163840, 655360, 0 }
#define MUL_FFT_MODF_THRESHOLD          416
#define MUL_FFT_FULL_THRESHOLD         1664

#define SQR_FFT_TABLE  { 432, 1120, 1408, 3584, 10240, 40960, 163840, 655360, 0 }
#define SQR_FFT_MODF_THRESHOLD          344
#define SQR_FFT_FULL_THRESHOLD         1664

#define MULLOW_BASECASE_THRESHOLD         0  /* always */
#define MULLOW_DC_THRESHOLD             123
#define MULLOW_MUL_THRESHOLD           2464

#define MULHIGH_BASECASE_THRESHOLD        5
#define MULHIGH_DC_THRESHOLD            122
#define MULHIGH_MUL_THRESHOLD          2664

#define MULMOD_2EXPM1_THRESHOLD          28

#define FAC_UI_THRESHOLD               6838
#define DC_DIV_QR_THRESHOLD             132
#define DC_DIVAPPR_Q_N_THRESHOLD        706
#define INV_DIV_QR_THRESHOLD           1387
#define INV_DIVAPPR_Q_N_THRESHOLD       706
#define DC_DIV_Q_THRESHOLD              734
#define INV_DIV_Q_THRESHOLD            2397
#define DC_DIVAPPR_Q_THRESHOLD          706
#define INV_DIVAPPR_Q_THRESHOLD        9198
#define DC_BDIV_QR_THRESHOLD            140
#define DC_BDIV_Q_THRESHOLD             386
/* Tuneup completed successfully, took 596 seconds */

