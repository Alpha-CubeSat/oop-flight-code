//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Plant.cpp
//
// Code generated for Simulink model 'Plant'.
//
// Model version                  : 13.5
// Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
// C/C++ source code generated on : Wed Jul 26 17:41:01 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#include "Plant.h"
#include "rtwtypes.h"
#include <cmath>
#include <cstring>
#include <stddef.h>
#define NumBitsPerChar                 8U

// Exported block parameters
real_T Altitude = 400.0;               // Variable: Alt
                                          //  Referenced by:
                                          //    '<S7>/Discrete-Time Integrator1'
                                          //    '<S7>/Discrete-Time Integrator3'
                                          //    '<S7>/Discrete-Time Integrator4'
                                          //  Cubesat Altitude (km)

real_T I[9] = { 0.00195761450869, -5.836632382E-5, 2.27638093E-6,
  -5.836632382E-5, 0.00196346658902, 8.8920475E-7, 2.27638093E-6, 8.8920475E-7,
  0.00204697265884 } ;                 // Variable: I
                                          //  Referenced by:
                                          //    '<S3>/I'
                                          //    '<S3>/I^-1'


real_T inclination = 0.90058989402907408;// Variable: inclination
                                            //  Referenced by:
                                            //    '<S7>/Discrete-Time Integrator1'
                                            //    '<S7>/Discrete-Time Integrator3'
                                            //  ISS inclination

real_T m = 1.3;                        // Variable: m
                                          //  Referenced by: '<S7>/mass gain'
                                          //  Cubesat Mass

real_T q0[4] = { 0.49999999999999994, 0.5, -0.18301270189221924,
  0.6830127018922193 } ;               // Variable: q0
                                          //  Referenced by: '<S6>/Discrete-Time Integrator1'
                                          //  Init Quat (Default Euler Angle = pi/6*[1 2 3])


real_T wx = 0;                       // Variable: wx
                                          //  Referenced by: '<S1>/Discrete-Time Integrator'
                                          //  init angular vel -x

real_T wy = 0;                       // Variable: wy
                                          //  Referenced by: '<S1>/Discrete-Time Integrator1'
                                          //  init angular vel -y

real_T wz =1;                       // Variable: wz
                                          //  Referenced by: '<S1>/Discrete-Time Integrator2'
                                          //  init angular vel -z

extern real_T rt_atan2d_snf(real_T u0, real_T u1);
extern void rt_invd3x3_snf(const real_T u[9], real_T y[9]);

//===========*
//  Constants *
// ===========
#define RT_PI                          3.14159265358979323846
#define RT_PIF                         3.1415927F
#define RT_LN_10                       2.30258509299404568402
#define RT_LN_10F                      2.3025851F
#define RT_LOG10E                      0.43429448190325182765
#define RT_LOG10EF                     0.43429449F
#define RT_E                           2.7182818284590452354
#define RT_EF                          2.7182817F

//
//  UNUSED_PARAMETER(x)
//    Used to specify that a function parameter (argument) is required but not
//    accessed by the function body.

#ifndef UNUSED_PARAMETER
#if defined(__LCC__)
#define UNUSED_PARAMETER(x)                                      // do nothing
#else

//
//  This is the semi-ANSI standard way of indicating that an
//  unused function parameter is required.

#define UNUSED_PARAMETER(x)            (void) (x)
#endif
#endif

extern "C"
{
  real_T rtInf;
  real_T rtMinusInf;
  real_T rtNaN;
  real32_T rtInfF;
  real32_T rtMinusInfF;
  real32_T rtNaNF;
}

//=========*
//  Asserts *
// =========
#ifndef utAssert
#if defined(DOASSERTS)
#if !defined(PRINT_ASSERTS)
#include <assert.h>
#define utAssert(exp)                  assert(exp)
#else
#include <stdio.h>

static void _assert(char_T *statement, char_T *file, int line)
{
  printf("%s in %s on line %d\n", statement, file, line);
}

#define utAssert(_EX)                  ((_EX) ? (void)0 : _assert(#_EX, __FILE__, __LINE__))
#endif

#else
#define utAssert(exp)                                            // do nothing
#endif
#endif

extern "C"
{
  //
  // Initialize rtNaN needed by the generated code.
  // NaN is initialized as non-signaling. Assumes IEEE.
  //
  static real_T rtGetNaN(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T nan = 0.0;
    if (bitsPerReal == 32U) {
      nan = rtGetNaNF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0xFFF80000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      nan = tmpVal.fltVal;
    }

    return nan;
  }

  //
  // Initialize rtNaNF needed by the generated code.
  // NaN is initialized as non-signaling. Assumes IEEE.
  //
  static real32_T rtGetNaNF(void)
  {
    IEEESingle nanF = { { 0.0F } };

    nanF.wordL.wordLuint = 0xFFC00000U;
    return nanF.wordL.wordLreal;
  }
}

extern "C"
{
  //
  // Initialize the rtInf, rtMinusInf, and rtNaN needed by the
  // generated code. NaN is initialized as non-signaling. Assumes IEEE.
  //
  static void rt_InitInfAndNaN(size_t realSize)
  {
    (void) (realSize);
    rtNaN = rtGetNaN();
    rtNaNF = rtGetNaNF();
    rtInf = rtGetInf();
    rtInfF = rtGetInfF();
    rtMinusInf = rtGetMinusInf();
    rtMinusInfF = rtGetMinusInfF();
  }

  // Test if value is infinite
  static boolean_T rtIsInf(real_T value)
  {
    return (boolean_T)((value==rtInf || value==rtMinusInf) ? 1U : 0U);
  }

  // Test if single-precision value is infinite
  static boolean_T rtIsInfF(real32_T value)
  {
    return (boolean_T)(((value)==rtInfF || (value)==rtMinusInfF) ? 1U : 0U);
  }

  // Test if value is not a number
  static boolean_T rtIsNaN(real_T value)
  {
    boolean_T result = (boolean_T) 0;
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    if (bitsPerReal == 32U) {
      result = rtIsNaNF((real32_T)value);
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.fltVal = value;
      result = (boolean_T)((tmpVal.bitVal.words.wordH & 0x7FF00000) ==
                           0x7FF00000 &&
                           ( (tmpVal.bitVal.words.wordH & 0x000FFFFF) != 0 ||
                            (tmpVal.bitVal.words.wordL != 0) ));
    }

    return result;
  }

  // Test if single-precision value is not a number
  static boolean_T rtIsNaNF(real32_T value)
  {
    IEEESingle tmp;
    tmp.wordL.wordLreal = value;
    return (boolean_T)( (tmp.wordL.wordLuint & 0x7F800000) == 0x7F800000 &&
                       (tmp.wordL.wordLuint & 0x007FFFFF) != 0 );
  }
}

extern "C"
{
  //
  // Initialize rtInf needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  static real_T rtGetInf(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T inf = 0.0;
    if (bitsPerReal == 32U) {
      inf = rtGetInfF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0x7FF00000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      inf = tmpVal.fltVal;
    }

    return inf;
  }

  //
  // Initialize rtInfF needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  static real32_T rtGetInfF(void)
  {
    IEEESingle infF;
    infF.wordL.wordLuint = 0x7F800000U;
    return infF.wordL.wordLreal;
  }

  //
  // Initialize rtMinusInf needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  static real_T rtGetMinusInf(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T minf = 0.0;
    if (bitsPerReal == 32U) {
      minf = rtGetMinusInfF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0xFFF00000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      minf = tmpVal.fltVal;
    }

    return minf;
  }

  //
  // Initialize rtMinusInfF needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  static real32_T rtGetMinusInfF(void)
  {
    IEEESingle minfF;
    minfF.wordL.wordLuint = 0xFF800000U;
    return minfF.wordL.wordLreal;
  }
}

//
// Output and update for atomic system:
//    '<S1>/qtoQ'
//    '<S1>/qtoQ1'
//
void Plant::qtoQ(const real_T rtu_q[4], real_T rty_Q[9])
{
  real_T rtu_q_0[9];
  real_T tmp[9];
  real_T rtb_Sum1;
  real_T rtu_q_1;

  // Product: '<S8>/qTq' incorporates:
  //   Math: '<S8>/T2'

  rtu_q_1 = 0.0;
  for (int32_T i = 0; i < 3; i++) {
    // Math: '<S8>/T2'
    rtb_Sum1 = rtu_q[i];
    rtu_q_1 += rtb_Sum1 * rtb_Sum1;

    // Product: '<S8>/qqT' incorporates:
    //   Math: '<S8>/T1'
    //   Math: '<S8>/T2'

    rtu_q_0[3 * i] = rtu_q[0] * rtu_q[i];
    rtu_q_0[3 * i + 1] = rtu_q[1] * rtu_q[i];
    rtu_q_0[3 * i + 2] = rtu_q[2] * rtu_q[i];
  }

  // Sum: '<S8>/Sum1' incorporates:
  //   Product: '<S8>/Product1'
  //   Product: '<S8>/qTq'

  rtb_Sum1 = rtu_q[3] * rtu_q[3] - rtu_q_1;

  // Reshape: '<S16>/3x3' incorporates:
  //   Constant: '<S16>/diag 0 '
  //   Gain: '<S16>/Gain'
  //   Gain: '<S16>/Gain1'
  //   Gain: '<S16>/Gain2'

  tmp[0] = 0.0;
  tmp[1] = rtu_q[2];
  tmp[2] = -rtu_q[1];
  tmp[3] = -rtu_q[2];
  tmp[4] = 0.0;
  tmp[5] = rtu_q[0];
  tmp[6] = rtu_q[1];
  tmp[7] = -rtu_q[0];
  tmp[8] = 0.0;

  // Product: '<S8>/Product'
  rtu_q_1 = rtu_q[3];

  // Sum: '<S8>/Sum8' incorporates:
  //   Gain: '<S8>/Gain1'
  //   Gain: '<S8>/Gain2'
  //   Gain: '<S8>/Matrix Gain'
  //   Product: '<S8>/Product'

  for (int32_T i = 0; i < 9; i++) {
    rty_Q[i] = (2.0 * rtu_q_0[i] - tmp[i] * rtu_q_1 * 2.0) + rtConstPplant.pooled6[i]
      * rtb_Sum1;
  }

  // End of Sum: '<S8>/Sum8'
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int32_T tmp;
    int32_T tmp_0;
    if (u0 > 0.0) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    if (u1 > 0.0) {
      tmp_0 = 1;
    } else {
      tmp_0 = -1;
    }

    y = std::atan2(static_cast<real_T>(tmp), static_cast<real_T>(tmp_0));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = std::atan2(u0, u1);
  }

  return y;
}

void rt_invd3x3_snf(const real_T u[9], real_T y[9])
{
  real_T x[9];
  real_T absx11;
  real_T absx21;
  real_T absx31;
  int32_T p1;
  int32_T p2;
  int32_T p3;
  std::memcpy(&x[0], &u[0], 9U * sizeof(real_T));
  p1 = 1;
  p2 = 3;
  p3 = 6;
  absx11 = std::abs(u[0]);
  absx21 = std::abs(u[1]);
  absx31 = std::abs(u[2]);
  if ((absx21 > absx11) && (absx21 > absx31)) {
    p1 = 4;
    p2 = 0;
    x[0] = u[1];
    x[1] = u[0];
    x[3] = u[4];
    x[4] = u[3];
    x[6] = u[7];
    x[7] = u[6];
  } else if (absx31 > absx11) {
    p1 = 7;
    p3 = 0;
    x[2] = x[0];
    x[0] = u[2];
    x[5] = x[3];
    x[3] = u[5];
    x[8] = x[6];
    x[6] = u[8];
  }

  absx31 = x[1] / x[0];
  x[1] = absx31;
  absx11 = x[2] / x[0];
  x[2] = absx11;
  x[4] -= absx31 * x[3];
  x[5] -= absx11 * x[3];
  x[7] -= absx31 * x[6];
  x[8] -= absx11 * x[6];
  if (std::abs(x[5]) > std::abs(x[4])) {
    int32_T itmp;
    itmp = p2;
    p2 = p3;
    p3 = itmp;
    x[1] = absx11;
    x[2] = absx31;
    absx11 = x[4];
    x[4] = x[5];
    x[5] = absx11;
    absx11 = x[7];
    x[7] = x[8];
    x[8] = absx11;
  }

  absx31 = x[5] / x[4];
  x[8] -= absx31 * x[7];
  absx11 = (x[1] * absx31 - x[2]) / x[8];
  absx21 = -(x[7] * absx11 + x[1]) / x[4];
  y[p1 - 1] = ((1.0 - x[3] * absx21) - x[6] * absx11) / x[0];
  y[p1] = absx21;
  y[p1 + 1] = absx11;
  absx11 = -absx31 / x[8];
  absx21 = (1.0 - x[7] * absx11) / x[4];
  y[p2] = -(x[3] * absx21 + x[6] * absx11) / x[0];
  y[p2 + 1] = absx21;
  y[p2 + 2] = absx11;
  absx11 = 1.0 / x[8];
  absx21 = -x[7] * absx11 / x[4];
  y[p3] = -(x[3] * absx21 + x[6] * absx11) / x[0];
  y[p3 + 1] = absx21;
  y[p3 + 2] = absx11;
}

// Model step function
void Plant::step()
{
  real_T rtb_Sum8[9];
  real_T rtb_Elementproduct[6];
  real_T rtb_Product[4];
  real_T rtb_Normalization[3];
  real_T rtb_Product_j[3];
  real_T rtb_Sum[3];
  real_T acc;
  real_T rtb_DiscreteTimeIntegrator1;
  real_T rtb_DiscreteTimeIntegrator2;
  real_T rtb_DiscreteTimeIntegrator3;
  real_T rtb_Product_n;
  real_T rtb_Product_p_tmp;
  real_T rtb_qd2;
  real_T rtb_qd3;
  real_T rtb_qd4;
  int32_T i;
  int32_T k;

  // Outputs for Atomic SubSystem: '<Root>/Plant'
  // Gain: '<S1>/Gain' incorporates:
  //   Inport: '<Root>/current'

  rtb_Product_j[0] = 0.27 * rtU.current[0];
  rtb_Product_j[1] = 0.27 * rtU.current[1];
  rtb_Product_j[2] = 0.27 * rtU.current[2];

  // Outport: '<Root>/angular velocity' incorporates:
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator2'

  rtY.angularvelocity[0] = rtDW.DiscreteTimeIntegrator_DSTATE;
  rtY.angularvelocity[1] = rtDW.DiscreteTimeIntegrator1_DSTATE;
  rtY.angularvelocity[2] = rtDW.DiscreteTimeIntegrator2_DSTATE;

  // Outputs for Atomic SubSystem: '<S1>/Quaternion Integration'
  // DotProduct: '<S14>/Dot Product' incorporates:
  //   DiscreteIntegrator: '<S6>/Discrete-Time Integrator1'

  acc = ((rtDW.DiscreteTimeIntegrator1_DSTAT_m[0] *
          rtDW.DiscreteTimeIntegrator1_DSTAT_m[0] +
          rtDW.DiscreteTimeIntegrator1_DSTAT_m[1] *
          rtDW.DiscreteTimeIntegrator1_DSTAT_m[1]) +
         rtDW.DiscreteTimeIntegrator1_DSTAT_m[2] *
         rtDW.DiscreteTimeIntegrator1_DSTAT_m[2]) +
    rtDW.DiscreteTimeIntegrator1_DSTAT_m[3] *
    rtDW.DiscreteTimeIntegrator1_DSTAT_m[3];

  // Math: '<S14>/Math Function' incorporates:
  //   DotProduct: '<S14>/Dot Product'
  //
  //  About '<S14>/Math Function':
  //   Operator: sqrt

  if (acc < 0.0) {
    rtb_Product_n = -std::sqrt(std::abs(acc));
  } else {
    rtb_Product_n = std::sqrt(acc);
  }

  // End of Math: '<S14>/Math Function'

  // Product: '<S14>/Product' incorporates:
  //   DiscreteIntegrator: '<S6>/Discrete-Time Integrator1'

  rtb_Product[0] = rtDW.DiscreteTimeIntegrator1_DSTAT_m[0] / rtb_Product_n;
  rtb_Product[1] = rtDW.DiscreteTimeIntegrator1_DSTAT_m[1] / rtb_Product_n;
  rtb_Product[2] = rtDW.DiscreteTimeIntegrator1_DSTAT_m[2] / rtb_Product_n;
  rtb_Product[3] = rtDW.DiscreteTimeIntegrator1_DSTAT_m[3] / rtb_Product_n;

  // Update for DiscreteIntegrator: '<S6>/Discrete-Time Integrator1' incorporates:
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator2'
  //   Fcn: '<S15>/qd1'
  //   Fcn: '<S15>/qd2'
  //   Fcn: '<S15>/qd3'
  //   Fcn: '<S15>/qd4'

  rtb_Product_n = rtDW.DiscreteTimeIntegrator1_DSTAT_m[0];
  rtb_qd2 = rtDW.DiscreteTimeIntegrator1_DSTAT_m[1];
  rtb_qd3 = rtDW.DiscreteTimeIntegrator1_DSTAT_m[2];
  acc = rtDW.DiscreteTimeIntegrator1_DSTAT_m[3];
  rtDW.DiscreteTimeIntegrator1_DSTAT_m[0] = ((rtb_Product[3] *
    rtDW.DiscreteTimeIntegrator_DSTATE - rtb_Product[2] *
    rtDW.DiscreteTimeIntegrator1_DSTATE) + rtb_Product[1] *
    rtDW.DiscreteTimeIntegrator2_DSTATE) / 2.0 * 0.001 + rtb_Product_n;
  rtDW.DiscreteTimeIntegrator1_DSTAT_m[1] = ((rtb_Product[2] *
    rtDW.DiscreteTimeIntegrator_DSTATE + rtb_Product[3] *
    rtDW.DiscreteTimeIntegrator1_DSTATE) - rtb_Product[0] *
    rtDW.DiscreteTimeIntegrator2_DSTATE) / 2.0 * 0.001 + rtb_qd2;
  rtDW.DiscreteTimeIntegrator1_DSTAT_m[2] = ((-rtb_Product[1] *
    rtDW.DiscreteTimeIntegrator_DSTATE + rtb_Product[0] *
    rtDW.DiscreteTimeIntegrator1_DSTATE) + rtb_Product[3] *
    rtDW.DiscreteTimeIntegrator2_DSTATE) / 2.0 * 0.001 + rtb_qd3;
  rtDW.DiscreteTimeIntegrator1_DSTAT_m[3] = ((-rtb_Product[0] *
    rtDW.DiscreteTimeIntegrator_DSTATE - rtb_Product[1] *
    rtDW.DiscreteTimeIntegrator1_DSTATE) - rtb_Product[2] *
    rtDW.DiscreteTimeIntegrator2_DSTATE) / 2.0 * 0.001 + acc;

  // End of Outputs for SubSystem: '<S1>/Quaternion Integration'

  // Outputs for Atomic SubSystem: '<S1>/qtoQ1'
  qtoQ(rtb_Product, rtb_Sum8);

  // End of Outputs for SubSystem: '<S1>/qtoQ1'

  // Outputs for Atomic SubSystem: '<S1>/Tranlational Dynamics'
  // DiscreteIntegrator: '<S7>/Discrete-Time Integrator1'
  rtb_DiscreteTimeIntegrator1 = rtDW.DiscreteTimeIntegrator1_DSTAT_l;

  // DiscreteIntegrator: '<S7>/Discrete-Time Integrator2'
  rtb_DiscreteTimeIntegrator2 = rtDW.DiscreteTimeIntegrator2_DSTAT_e;

  // DiscreteIntegrator: '<S7>/Discrete-Time Integrator3'
  rtb_DiscreteTimeIntegrator3 = rtDW.DiscreteTimeIntegrator3_DSTATE;

  // DiscreteIntegrator: '<S7>/Discrete-Time Integrator4'
  rtb_Product_n = rtDW.DiscreteTimeIntegrator4_DSTATE;

  // DiscreteIntegrator: '<S7>/Discrete-Time Integrator5'
  rtb_qd2 = rtDW.DiscreteTimeIntegrator5_DSTATE;

  // DiscreteIntegrator: '<S7>/Discrete-Time Integrator6'
  rtb_qd3 = rtDW.DiscreteTimeIntegrator6_DSTATE;

  // Sqrt: '<S7>/Sqrt' incorporates:
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator4'
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator5'
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator6'
  //   DotProduct: '<S7>/Dot Product'
  //   DotProduct: '<S7>/Dot Product1'
  //   DotProduct: '<S7>/Dot Product2'
  //   Sum: '<S7>/Sum'

  rtb_qd4 = std::sqrt((rtDW.DiscreteTimeIntegrator4_DSTATE *
                       rtDW.DiscreteTimeIntegrator4_DSTATE +
                       rtDW.DiscreteTimeIntegrator5_DSTATE *
                       rtDW.DiscreteTimeIntegrator5_DSTATE) +
                      rtDW.DiscreteTimeIntegrator6_DSTATE *
                      rtDW.DiscreteTimeIntegrator6_DSTATE);

  // Product: '<S7>/Divide' incorporates:
  //   Constant: '<S7>/Constant'
  //   Gain: '<S7>/Gain1'
  //   Gain: '<S7>/mass gain'
  //   Math: '<S7>/Math Function'

  acc = 1.0 / (rtb_qd4 * rtb_qd4) * (m * -3.983324E+14);

  // Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator1' incorporates:
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator5'
  //   DotProduct: '<S7>/Dot Product4'
  //   Product: '<S7>/Divide1'

  rtDW.DiscreteTimeIntegrator1_DSTAT_l += 1.0 / rtb_qd4 *
    rtDW.DiscreteTimeIntegrator5_DSTATE * acc * 0.001;

  // Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator2' incorporates:
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator4'
  //   DotProduct: '<S7>/Dot Product5'
  //   Product: '<S7>/Divide1'

  rtDW.DiscreteTimeIntegrator2_DSTAT_e += 1.0 / rtb_qd4 *
    rtDW.DiscreteTimeIntegrator4_DSTATE * acc * 0.001;

  // Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator3' incorporates:
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator6'
  //   DotProduct: '<S7>/Dot Product3'
  //   Product: '<S7>/Divide1'

  rtDW.DiscreteTimeIntegrator3_DSTATE += 1.0 / rtb_qd4 *
    rtDW.DiscreteTimeIntegrator6_DSTATE * acc * 0.001;

  // Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator4'
  rtDW.DiscreteTimeIntegrator4_DSTATE += 0.001 * rtb_DiscreteTimeIntegrator2;

  // Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator5'
  rtDW.DiscreteTimeIntegrator5_DSTATE += 0.001 * rtb_DiscreteTimeIntegrator1;

  // Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator6'
  rtDW.DiscreteTimeIntegrator6_DSTATE += 0.001 * rtb_DiscreteTimeIntegrator3;

  // End of Outputs for SubSystem: '<S1>/Tranlational Dynamics'

  // Outputs for Atomic SubSystem: '<S1>/Magnetic Field Model'
  // Sum: '<S4>/Sum4' incorporates:
  //   Math: '<S4>/Math Function5'
  //   Math: '<S4>/Math Function6'
  //   Sum: '<S4>/Sum3'

  rtb_DiscreteTimeIntegrator1 = rtb_Product_n * rtb_Product_n + rtb_qd2 *
    rtb_qd2;

  // Product: '<S4>/Divide2' incorporates:
  //   Constant: '<S4>/Constant1'
  //   Math: '<S4>/Math Function7'
  //   Sqrt: '<S4>/Sqrt3'
  //   Sum: '<S4>/Sum4'

  acc = 6.371E+6 / std::sqrt(rtb_qd3 * rtb_qd3 + rtb_DiscreteTimeIntegrator1);

  // Gain: '<S4>/Gain4' incorporates:
  //   Math: '<S4>/Math Function8'
  //   Product: '<S4>/Product'

  acc = acc * acc * acc * -3.12E-5;

  // Sum: '<S4>/Sum5' incorporates:
  //   Constant: '<S4>/Constant8'
  //   Sqrt: '<S4>/Sqrt2'
  //   Trigonometry: '<S4>/Trigonometric Function3'

  rtb_qd4 = 1.5707963267948966 - rt_atan2d_snf(rtb_qd3, std::sqrt
    (rtb_DiscreteTimeIntegrator1));

  // Product: '<S4>/Divide3' incorporates:
  //   Gain: '<S4>/Gain5'
  //   Trigonometry: '<S4>/Trigonometric Function2'

  rtb_DiscreteTimeIntegrator1 = 2.0 * acc * std::cos(rtb_qd4);

  // Product: '<S4>/Divide4' incorporates:
  //   Trigonometry: '<S4>/Trigonometric Function4'

  rtb_DiscreteTimeIntegrator2 = acc * std::sin(rtb_qd4);

  // Outputs for Atomic SubSystem: '<S4>/Dipole->ECI'
  // Trigonometry: '<S13>/Trigonometric Function8' incorporates:
  //   Gain: '<S4>/Gain6'
  //   Trigonometry: '<S13>/Trigonometric Function5'

  rtb_DiscreteTimeIntegrator3 = std::cos(-rtb_qd4);

  // Trigonometry: '<S13>/Trigonometric Function7' incorporates:
  //   Gain: '<S4>/Gain6'
  //   Trigonometry: '<S13>/Trigonometric Function6'

  rtb_Product_p_tmp = std::sin(-rtb_qd4);

  // Sum: '<S13>/Sum6' incorporates:
  //   Product: '<S13>/Product3'
  //   Product: '<S13>/Product4'
  //   Trigonometry: '<S13>/Trigonometric Function7'
  //   Trigonometry: '<S13>/Trigonometric Function8'

  acc = rtb_DiscreteTimeIntegrator3 * rtb_DiscreteTimeIntegrator2 +
    rtb_Product_p_tmp * rtb_DiscreteTimeIntegrator1;

  // Trigonometry: '<S13>/Trigonometric Function'
  rtb_qd4 = rt_atan2d_snf(rtb_qd2, rtb_Product_n);

  // SignalConversion generated from: '<S1>/Matrix Multiply' incorporates:
  //   Product: '<S13>/Divide'
  //   Product: '<S13>/Divide1'
  //   Product: '<S13>/Product1'
  //   Product: '<S13>/Product2'
  //   Sum: '<S13>/Sum5'
  //   Trigonometry: '<S13>/Trigonometric Function1'
  //   Trigonometry: '<S13>/Trigonometric Function2'

  rtb_Normalization[0] = acc * std::cos(rtb_qd4);
  rtb_Normalization[1] = acc * std::sin(rtb_qd4);
  rtb_Normalization[2] = rtb_DiscreteTimeIntegrator3 *
    rtb_DiscreteTimeIntegrator1 - rtb_Product_p_tmp *
    rtb_DiscreteTimeIntegrator2;

  // End of Outputs for SubSystem: '<S4>/Dipole->ECI'
  // End of Outputs for SubSystem: '<S1>/Magnetic Field Model'

  // Product: '<S1>/Matrix Multiply1' incorporates:
  //   Sum: '<S8>/Sum8'

  acc = rtb_Normalization[1];
  rtb_DiscreteTimeIntegrator1 = rtb_Normalization[0];
  rtb_DiscreteTimeIntegrator2 = rtb_Normalization[2];
  for (i = 0; i < 3; i++) {
    rtb_Sum[i] = (rtb_Sum8[i + 3] * acc + rtb_Sum8[i] *
                  rtb_DiscreteTimeIntegrator1) + rtb_Sum8[i + 6] *
      rtb_DiscreteTimeIntegrator2;
  }

  // End of Product: '<S1>/Matrix Multiply1'

  // Product: '<S2>/Element product'
  rtb_Elementproduct[0] = rtb_Product_j[1] * rtb_Sum[2];
  rtb_Elementproduct[1] = rtb_Sum[0] * rtb_Product_j[2];
  rtb_Elementproduct[2] = rtb_Product_j[0] * rtb_Sum[1];
  rtb_Elementproduct[3] = rtb_Sum[1] * rtb_Product_j[2];
  rtb_Elementproduct[4] = rtb_Product_j[0] * rtb_Sum[2];
  rtb_Elementproduct[5] = rtb_Sum[0] * rtb_Product_j[1];

  // Outputs for Atomic SubSystem: '<S1>/Dynamics'
  for (i = 0; i < 3; i++) {
    // Sum: '<S2>/Add3'
    rtb_Sum[i] = rtb_Elementproduct[i] - rtb_Elementproduct[i + 3];

    // Product: '<S3>/Product' incorporates:
    //   Constant: '<S3>/I'
    //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
    //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'
    //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator2'

    rtb_Product_j[i] = (I[i + 3] * rtDW.DiscreteTimeIntegrator1_DSTATE + I[i] *
                        rtDW.DiscreteTimeIntegrator_DSTATE) + I[i + 6] *
      rtDW.DiscreteTimeIntegrator2_DSTATE;
  }

  // Assertion: '<S11>/Assertion' incorporates:
  //   Constant: '<S3>/I^-1'
  //   Product: '<S12>/Product'
  //   Product: '<S12>/Product1'
  //   Product: '<S12>/Product2'
  //   Product: '<S12>/Product3'
  //   Product: '<S12>/Product4'
  //   Product: '<S12>/Product5'
  //   Sum: '<S12>/Sum'

  utAssert(((((I[0] * I[4] * I[8] - I[0] * I[5] * I[7]) - I[1] * I[3] * I[8]) +
             I[2] * I[3] * I[7]) + I[1] * I[5] * I[6]) - I[2] * I[4] * I[6] !=
           0.0);

  // Product: '<S11>/Product' incorporates:
  //   Constant: '<S3>/I^-1'
  //   Sum: '<S8>/Sum8'

  rt_invd3x3_snf(I, rtb_Sum8);

  // Sum: '<S3>/Sum' incorporates:
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator2'
  //   Product: '<S10>/Element product'
  //   Sum: '<S10>/Add3'

  acc = rtb_Sum[0] - (rtDW.DiscreteTimeIntegrator1_DSTATE * rtb_Product_j[2] -
                      rtDW.DiscreteTimeIntegrator2_DSTATE * rtb_Product_j[1]);
  rtb_DiscreteTimeIntegrator1 = rtb_Sum[1] -
    (rtDW.DiscreteTimeIntegrator2_DSTATE * rtb_Product_j[0] -
     rtDW.DiscreteTimeIntegrator_DSTATE * rtb_Product_j[2]);
  rtb_DiscreteTimeIntegrator2 = rtb_Sum[2] - (rtDW.DiscreteTimeIntegrator_DSTATE
    * rtb_Product_j[1] - rtDW.DiscreteTimeIntegrator1_DSTATE * rtb_Product_j[0]);

  // Product: '<S3>/Product1' incorporates:
  //   Sum: '<S8>/Sum8'

  for (i = 0; i < 3; i++) {
    rtb_Product_j[i] = (rtb_Sum8[i + 3] * rtb_DiscreteTimeIntegrator1 +
                        rtb_Sum8[i] * acc) + rtb_Sum8[i + 6] *
      rtb_DiscreteTimeIntegrator2;
  }

  // End of Product: '<S3>/Product1'
  // End of Outputs for SubSystem: '<S1>/Dynamics'

  // Outputs for Atomic SubSystem: '<S1>/qtoQ'
  qtoQ(rtb_Product, rtb_Sum8);

  // End of Outputs for SubSystem: '<S1>/qtoQ'

  // S-Function (sdsp2norm2): '<S5>/Normalization'
  i = 0;
  acc = 0.0;
  for (k = 0; k < 3; k++) {
    // Outport: '<Root>/magnetic field' incorporates:
    //   Product: '<S1>/Matrix Multiply'
    //   Sum: '<S8>/Sum8'

    rtY.magneticfield[k] = (rtb_Sum8[k + 3] * rtb_Normalization[1] + rtb_Sum8[k]
      * rtb_Normalization[0]) + rtb_Sum8[k + 6] * rtb_Normalization[2];

    // S-Function (sdsp2norm2): '<S5>/Normalization'
    acc += rtb_Normalization[i] * rtb_Normalization[i];
    i++;
  }

  // S-Function (sdsp2norm2): '<S5>/Normalization'
  acc = 1.0 / (std::sqrt(acc) + 1.0E-10);
  rtb_Normalization[0] *= acc;
  rtb_Normalization[1] *= acc;
  rtb_Normalization[2] *= acc;

  // Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
  rtDW.DiscreteTimeIntegrator_DSTATE += 0.001 * rtb_Product_j[0];

  // Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'
  rtDW.DiscreteTimeIntegrator1_DSTATE += 0.001 * rtb_Product_j[1];

  // Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator2'
  rtDW.DiscreteTimeIntegrator2_DSTATE += 0.001 * rtb_Product_j[2];

  // End of Outputs for SubSystem: '<Root>/Plant'

  // Outport: '<Root>/xyzposition'
  rtY.xyzposition[0] = rtb_Product_n;
  rtY.xyzposition[1] = rtb_qd2;
  rtY.xyzposition[2] = rtb_qd3;

  // Outport: '<Root>/quaternion'
  rtY.quaternion[0] = rtb_Product[0];
  rtY.quaternion[1] = rtb_Product[1];
  rtY.quaternion[2] = rtb_Product[2];
  rtY.quaternion[3] = rtb_Product[3];

  // Outputs for Atomic SubSystem: '<Root>/Plant'
  // DotProduct: '<S5>/Dot Product' incorporates:
  //   Constant: '<S5>/Constant'

  acc = (rtb_Normalization[0] * 0.0 + rtb_Normalization[1] * 0.0) +
    rtb_Normalization[2];

  // Saturate: '<S5>/Saturation3' incorporates:
  //   DotProduct: '<S5>/Dot Product'

  if (acc > 1.0) {
    acc = 1.0;
  } else if (acc < -1.0) {
    acc = -1.0;
  }

  // Outport: '<Root>/pt_error' incorporates:
  //   Gain: '<S5>/Multiply'
  //   Saturate: '<S5>/Saturation3'
  //   Trigonometry: '<S5>/Acos'

  rtY.pt_error = 57.295779513082323 * std::acos(acc);

  // End of Outputs for SubSystem: '<Root>/Plant'
}

// Model initialize function
void Plant::initialize()
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  {
    real_T DiscreteTimeIntegrator1_DSTAT_l;

    // SystemInitialize for Atomic SubSystem: '<Root>/Plant'
    // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' 
    rtDW.DiscreteTimeIntegrator_DSTATE = wx;

    // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator1' 
    rtDW.DiscreteTimeIntegrator1_DSTATE = wy;

    // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator2' 
    rtDW.DiscreteTimeIntegrator2_DSTATE = wz;

    // SystemInitialize for Atomic SubSystem: '<S1>/Quaternion Integration'
    // InitializeConditions for DiscreteIntegrator: '<S6>/Discrete-Time Integrator1' 
    rtDW.DiscreteTimeIntegrator1_DSTAT_m[0] = q0[0];
    rtDW.DiscreteTimeIntegrator1_DSTAT_m[1] = q0[1];
    rtDW.DiscreteTimeIntegrator1_DSTAT_m[2] = q0[2];
    rtDW.DiscreteTimeIntegrator1_DSTAT_m[3] = q0[3];

    // End of SystemInitialize for SubSystem: '<S1>/Quaternion Integration'

    // SystemInitialize for Atomic SubSystem: '<S1>/Tranlational Dynamics'
    // InitializeConditions for DiscreteIntegrator: '<S7>/Discrete-Time Integrator1' incorporates:
    //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator3'

    DiscreteTimeIntegrator1_DSTAT_l = std::sqrt(398600.0 / (Altitude + 6371.0)) *
      1000.0;
    rtDW.DiscreteTimeIntegrator1_DSTAT_l = DiscreteTimeIntegrator1_DSTAT_l * std::
      cos(inclination);

    // InitializeConditions for DiscreteIntegrator: '<S7>/Discrete-Time Integrator3' 
    rtDW.DiscreteTimeIntegrator3_DSTATE = DiscreteTimeIntegrator1_DSTAT_l * std::
      tan(inclination) * std::sin(inclination);

    // InitializeConditions for DiscreteIntegrator: '<S7>/Discrete-Time Integrator4' 
    rtDW.DiscreteTimeIntegrator4_DSTATE = Altitude * 1000.0 + 6.371E+6;

    // End of SystemInitialize for SubSystem: '<S1>/Tranlational Dynamics'
    // End of SystemInitialize for SubSystem: '<Root>/Plant'
  }
}
// Model initialize function
void Plant::initialize(double Altitude_input, double I_input[9], double inclination_input, double m_input, double q0_input[4], double wx_input, double wy_input, double wz_input)
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  {
    real_T DiscreteTimeIntegrator1_DSTAT_l;

    Altitude = Altitude_input;

    for (int i = 0; i < 9; i++)
    {
      I[i] = I_input[i];
    }

    inclination = inclination_input;

    m = m_input;

    for (int i = 0; i < 4; i++)
    {
      q0[i] = q0_input[i];
    }

    wx = wx_input;
    wy = wy_input;
    wz = wz_input;

    // SystemInitialize for Atomic SubSystem: '<Root>/Plant'
    // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
    rtDW.DiscreteTimeIntegrator_DSTATE = wx_input;

    // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'
    rtDW.DiscreteTimeIntegrator1_DSTATE = wy_input;

    // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator2'
    rtDW.DiscreteTimeIntegrator2_DSTATE = wz_input;

    // SystemInitialize for Atomic SubSystem: '<S1>/Quaternion Integration'
    // InitializeConditions for DiscreteIntegrator: '<S5>/Discrete-Time Integrator1'
    rtDW.DiscreteTimeIntegrator1_DSTAT_m[0] = q0_input[0];
    rtDW.DiscreteTimeIntegrator1_DSTAT_m[1] = q0_input[1];
    rtDW.DiscreteTimeIntegrator1_DSTAT_m[2] = q0_input[2];
    rtDW.DiscreteTimeIntegrator1_DSTAT_m[3] = q0_input[3];

    // End of SystemInitialize for SubSystem: '<S1>/Quaternion Integration'

    // SystemInitialize for Atomic SubSystem: '<S1>/Tranlational Dynamics'
    // InitializeConditions for DiscreteIntegrator: '<S6>/Discrete-Time Integrator1' incorporates:
    //   DiscreteIntegrator: '<S6>/Discrete-Time Integrator3'

    DiscreteTimeIntegrator1_DSTAT_l = std::sqrt(398600.0 / (Altitude_input + 6371.0)) *
                                      1000.0;
    rtDW.DiscreteTimeIntegrator1_DSTAT_l = DiscreteTimeIntegrator1_DSTAT_l * std::cos(inclination_input);

    // InitializeConditions for DiscreteIntegrator: '<S6>/Discrete-Time Integrator3'
    rtDW.DiscreteTimeIntegrator3_DSTATE = DiscreteTimeIntegrator1_DSTAT_l * std::tan(inclination_input) * std::sin(inclination_input);

    // InitializeConditions for DiscreteIntegrator: '<S6>/Discrete-Time Integrator4'
    rtDW.DiscreteTimeIntegrator4_DSTATE = Altitude_input * 1000.0 + 6.371E+6;

    // End of SystemInitialize for SubSystem: '<S1>/Tranlational Dynamics'
    // End of SystemInitialize for SubSystem: '<Root>/Plant'
  }
}

// Constructor
Plant::Plant() :
  rtU(),
  rtY(),
  rtDW(),
  rtM()
{
  // Currently there is no constructor body generated.
}

// Destructor
Plant::~Plant()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODELplant * Plant::getRTM()
{
  return (&rtM);
}

//
// File trailer for generated code.
//
// [EOF]
//
