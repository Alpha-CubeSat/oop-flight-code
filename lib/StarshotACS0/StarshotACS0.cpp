//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: StarshotACS0.cpp
//
// Code generated for Simulink model 'StarshotACS0'.
//
// Model version                  : 1.117
// Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
// C/C++ source code generated on : Wed Apr 25 16:25:06 2018
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#include "StarshotACS0.h"

// Model step function
void StarshotACS0ModelClass::step()
{
  real_T rtb_Gain[3];
  real_T rtb_TSamp[3];
  real_T rtb_VectorConcatenate[9];
  real_T rtb_Saturation3;
  real_T rtb_TrigonometricFunction5;
  real_T rtb_TSamp_k;
  real_T rtb_Gain_0;
  int32_T i;
  real_T rtb_VectorConcatenate_0[9];
  real_T rtb_Product1_g;
  real_T rtb_Gain8_idx_1;
  real_T rtb_Gain8_idx_2;
  real_T rtb_Gain8_idx_0;
  real_T rtb_Product1_o_idx_0;
  real_T rtb_Product1_o_idx_1;
  int32_T tmp;

  // Outputs for Atomic SubSystem: '<Root>/StarshotACS'
  // Gain: '<S2>/Kane damping' incorporates:
  //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
  //   Gain: '<S2>/Gain 2'

  rtb_Gain8_idx_0 = 0.41837 * -rtDW.DiscreteTimeIntegrator_DSTATE[0];

  // Gain: '<S2>/Gain 2' incorporates:
  //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'

  rtb_Product1_o_idx_0 = -rtDW.DiscreteTimeIntegrator_DSTATE[0];

  // Gain: '<S2>/Kane damping' incorporates:
  //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
  //   Gain: '<S2>/Gain 2'

  rtb_Gain8_idx_1 = 0.41837 * -rtDW.DiscreteTimeIntegrator_DSTATE[1];

  // Gain: '<S2>/Gain 2' incorporates:
  //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'

  rtb_Product1_o_idx_1 = -rtDW.DiscreteTimeIntegrator_DSTATE[1];
  rtb_Product1_g = -rtDW.DiscreteTimeIntegrator_DSTATE[2];

  // Gain: '<S2>/Kane damping' incorporates:
  //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
  //   Gain: '<S2>/Gain 2'

  rtb_Gain8_idx_2 = 0.41837 * -rtDW.DiscreteTimeIntegrator_DSTATE[2];

  // S-Function (sdsp2norm2): '<S2>/Normalization' incorporates:
  //   Inport: '<Root>/Bfield_body'

  rtb_Saturation3 = 1.0 / (((rtU.Bfield_body[0] * rtU.Bfield_body[0] +
    rtU.Bfield_body[1] * rtU.Bfield_body[1]) + rtU.Bfield_body[2] *
    rtU.Bfield_body[2]) + 1.0E-10);
  rtb_Gain[0] = rtU.Bfield_body[0] * rtb_Saturation3;

  // SampleTimeMath: '<S5>/TSamp' incorporates:
  //   Inport: '<Root>/angularvelocity'
  //
  //  About '<S5>/TSamp':
  //   y = u * K where K = 1 / ( w * Ts )

  rtb_TSamp[0] = rtU.angularvelocity[0] * 100.0;

  // S-Function (sdsp2norm2): '<S2>/Normalization' incorporates:
  //   Inport: '<Root>/Bfield_body'

  rtb_Gain[1] = rtU.Bfield_body[1] * rtb_Saturation3;

  // SampleTimeMath: '<S5>/TSamp' incorporates:
  //   Inport: '<Root>/angularvelocity'
  //
  //  About '<S5>/TSamp':
  //   y = u * K where K = 1 / ( w * Ts )

  rtb_TSamp[1] = rtU.angularvelocity[1] * 100.0;

  // S-Function (sdsp2norm2): '<S2>/Normalization' incorporates:
  //   Inport: '<Root>/Bfield_body'

  rtb_Gain[2] = rtU.Bfield_body[2] * rtb_Saturation3;

  // SampleTimeMath: '<S5>/TSamp' incorporates:
  //   Inport: '<Root>/angularvelocity'
  //
  //  About '<S5>/TSamp':
  //   y = u * K where K = 1 / ( w * Ts )

  rtb_TSamp[2] = rtU.angularvelocity[2] * 100.0;

  // Product: '<S4>/Product2'
  rtb_TrigonometricFunction5 = rtb_Gain[0];

  // Product: '<S4>/Product4'
  rtb_TSamp_k = rtb_Gain[1];

  // Product: '<S4>/Product5'
  rtb_Gain_0 = rtb_Gain[0];

  // Gain: '<S2>/Gain' incorporates:
  //   Product: '<S4>/Product'
  //   Product: '<S4>/Product1'
  //   Product: '<S4>/Product2'
  //   Product: '<S4>/Product3'
  //   Sum: '<S4>/Sum'
  //   Sum: '<S4>/Sum1'

  rtb_Gain[0] = (rtb_Gain8_idx_1 * rtb_Gain[2] - rtb_Gain8_idx_2 * rtb_Gain[1]) *
    0.025063770565652812;
  rtb_Gain[1] = (rtb_Gain8_idx_2 * rtb_TrigonometricFunction5 - rtb_Gain8_idx_0 *
                 rtb_Gain[2]) * 0.025063770565652812;

  // SignalConversion: '<S6>/ConcatBufferAtVector ConcatenateIn1' incorporates:
  //   Constant: '<S6>/Constant3'
  //   Gain: '<S6>/Gain'
  //   Inport: '<Root>/angularvelocity'

  rtb_VectorConcatenate[0] = 0.0;
  rtb_VectorConcatenate[1] = rtU.angularvelocity[2];
  rtb_VectorConcatenate[2] = -rtU.angularvelocity[1];

  // SignalConversion: '<S6>/ConcatBufferAtVector ConcatenateIn2' incorporates:
  //   Constant: '<S6>/Constant3'
  //   Gain: '<S6>/Gain1'
  //   Inport: '<Root>/angularvelocity'

  rtb_VectorConcatenate[3] = -rtU.angularvelocity[2];
  rtb_VectorConcatenate[4] = 0.0;
  rtb_VectorConcatenate[5] = rtU.angularvelocity[0];

  // SignalConversion: '<S6>/ConcatBufferAtVector ConcatenateIn3' incorporates:
  //   Constant: '<S6>/Constant3'
  //   Gain: '<S6>/Gain2'
  //   Inport: '<Root>/angularvelocity'

  rtb_VectorConcatenate[6] = rtU.angularvelocity[1];
  rtb_VectorConcatenate[7] = -rtU.angularvelocity[0];
  rtb_VectorConcatenate[8] = 0.0;

  // Saturate: '<S2>/Saturation3'
  rtb_Gain8_idx_2 = rtb_Gain[0];

  // Saturate: '<S2>/Saturation4'
  rtb_Saturation3 = rtb_Gain[1];

  // Saturate: '<S2>/Saturation5' incorporates:
  //   Gain: '<S2>/Gain'
  //   Product: '<S4>/Product4'
  //   Product: '<S4>/Product5'
  //   Sum: '<S4>/Sum2'

  rtb_Gain8_idx_0 = (rtb_Gain8_idx_0 * rtb_TSamp_k - rtb_Gain8_idx_1 *
                     rtb_Gain_0) * 0.025063770565652812;

  // Sqrt: '<S8>/Sqrt4' incorporates:
  //   DotProduct: '<S8>/Dot Product6'
  //   Inport: '<Root>/Bfield_body'

  rtb_Gain8_idx_1 = std::sqrt((rtU.Bfield_body[0] * rtU.Bfield_body[0] +
    rtU.Bfield_body[1] * rtU.Bfield_body[1]) + rtU.Bfield_body[2] *
    rtU.Bfield_body[2]);

  // DotProduct: '<S9>/Dot Product6'
  rtb_TSamp_k = 0.0;
  for (i = 0; i < 3; i++) {
    // Product: '<S3>/Product6' incorporates:
    //   Inport: '<Root>/Bfield_body'
    //   Product: '<S3>/Product4'

    rtb_TrigonometricFunction5 = ((rtConstB.VectorConcatenate[i + 3] *
      rtU.Bfield_body[1] + rtConstB.VectorConcatenate[i] * rtU.Bfield_body[0]) +
      rtConstB.VectorConcatenate[i + 6] * rtU.Bfield_body[2]) / rtb_Gain8_idx_1;

    // DotProduct: '<S9>/Dot Product6'
    rtb_TSamp_k += rtb_TrigonometricFunction5 * rtb_TrigonometricFunction5;

    // Product: '<S3>/Product6' incorporates:
    //   Inport: '<Root>/Bfield_body'
    //   Inport: '<Root>/angularvelocity'
    //   Product: '<S11>/Product3'
    //   Product: '<S3>/Product4'

    rtb_Gain[i] = rtU.angularvelocity[i] * rtU.Bfield_body[i];
  }

  // Sqrt: '<S9>/Sqrt4' incorporates:
  //   DotProduct: '<S9>/Dot Product6'

  rtb_TrigonometricFunction5 = std::sqrt(rtb_TSamp_k);

  // Trigonometry: '<S3>/Trigonometric Function5'
  if (rtb_TrigonometricFunction5 > 1.0) {
    rtb_TrigonometricFunction5 = 1.0;
  } else {
    if (rtb_TrigonometricFunction5 < -1.0) {
      rtb_TrigonometricFunction5 = -1.0;
    }
  }

  rtb_TrigonometricFunction5 = std::asin(rtb_TrigonometricFunction5);

  // End of Trigonometry: '<S3>/Trigonometric Function5'

  // SampleTimeMath: '<S7>/TSamp'
  //
  //  About '<S7>/TSamp':
  //   y = u * K where K = 1 / ( w * Ts )

  rtb_TSamp_k = rtb_TrigonometricFunction5 * 100.0;

  // Switch: '<S12>/Switch1' incorporates:
  //   Constant: '<S12>/Constant10'
  //   Constant: '<S12>/Constant9'
  //   Inport: '<Root>/angularvelocity'

  if (rtU.angularvelocity[2] >= 0.0) {
    i = 1;
  } else {
    i = -1;
  }

  // End of Switch: '<S12>/Switch1'

  // Switch: '<S11>/Switch' incorporates:
  //   Constant: '<S11>/Constant3'
  //   Constant: '<S11>/Constant4'
  //   DotProduct: '<S13>/Dot Product6'
  //   DotProduct: '<S14>/Dot Product6'
  //   Inport: '<Root>/Bfield_body'
  //   Inport: '<Root>/angularvelocity'
  //   Product: '<S11>/Divide6'
  //   Sqrt: '<S13>/Sqrt4'
  //   Sqrt: '<S14>/Sqrt4'
  //   Sum: '<S11>/Add'

  if (1.0 / std::sqrt((rtU.angularvelocity[0] * rtU.angularvelocity[0] +
                       rtU.angularvelocity[1] * rtU.angularvelocity[1]) +
                      rtU.angularvelocity[2] * rtU.angularvelocity[2]) *
      ((rtb_Gain[0] + rtb_Gain[1]) + rtb_Gain[2]) / std::sqrt((rtU.Bfield_body[0]
        * rtU.Bfield_body[0] + rtU.Bfield_body[1] * rtU.Bfield_body[1]) +
       rtU.Bfield_body[2] * rtU.Bfield_body[2]) > 0.0) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  // End of Switch: '<S11>/Switch'

  // Product: '<S3>/Product7' incorporates:
  //   Gain: '<S3>/Gain10'
  //   Gain: '<S3>/Gain11'
  //   Product: '<S3>/Product8'
  //   Sum: '<S3>/Sum7'
  //   Sum: '<S7>/Diff'
  //   UnitDelay: '<S7>/UD'
  //
  //  Block description for '<S7>/Diff':
  //
  //   Add in CPU
  //
  //  Block description for '<S7>/UD':
  //
  //   Store in Global RAM

  rtb_Gain8_idx_1 = ((rtb_TSamp_k - rtDW.UD_DSTATE_m) * 7.5058075858287763E-5 +
                     2.0910503844363048E-6 * rtb_TrigonometricFunction5) *
    (real_T)(i * tmp) / rtb_Gain8_idx_1;

  // Sum: '<S2>/Sum10' incorporates:
  //   Constant: '<S2>/Identity matrix'
  //   Product: '<S2>/Product1'

  for (i = 0; i < 3; i++) {
    rtb_VectorConcatenate_0[3 * i] = rtb_VectorConcatenate[3 * i] +
      rtConstP.Identitymatrix_Value[3 * i];
    rtb_VectorConcatenate_0[1 + 3 * i] = rtb_VectorConcatenate[3 * i + 1] +
      rtConstP.Identitymatrix_Value[3 * i + 1];
    rtb_VectorConcatenate_0[2 + 3 * i] = rtb_VectorConcatenate[3 * i + 2] +
      rtConstP.Identitymatrix_Value[3 * i + 2];
  }

  // End of Sum: '<S2>/Sum10'
  for (i = 0; i < 3; i++) {
    // Update for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' incorporates:
    //   Gain: '<S2>/Gain 8'
    //   Gain: '<S2>/Gain 9'
    //   Gain: '<S2>/Id inverse'
    //   Product: '<S2>/Product1'
    //   Sum: '<S2>/Sum8'
    //   Sum: '<S5>/Diff'
    //   UnitDelay: '<S5>/UD'
    //
    //  Block description for '<S5>/Diff':
    //
    //   Add in CPU
    //
    //  Block description for '<S5>/UD':
    //
    //   Store in Global RAM

    rtDW.DiscreteTimeIntegrator_DSTATE[i] += ((0.0 - (rtb_TSamp[i] -
      rtDW.UD_DSTATE[i])) - ((121.13723637508934 * -rtb_Product1_o_idx_0 *
      0.41837 * rtb_VectorConcatenate_0[i] + 121.13723637508934 *
      -rtb_Product1_o_idx_1 * 0.41837 * rtb_VectorConcatenate_0[i + 3]) +
      121.13723637508934 * -rtb_Product1_g * 0.41837 * rtb_VectorConcatenate_0[i
      + 6])) * 0.01;

    // Update for UnitDelay: '<S5>/UD'
    //
    //  Block description for '<S5>/UD':
    //
    //   Store in Global RAM

    rtDW.UD_DSTATE[i] = rtb_TSamp[i];
  }

  // Update for UnitDelay: '<S7>/UD'
  //
  //  Block description for '<S7>/UD':
  //
  //   Store in Global RAM

  rtDW.UD_DSTATE_m = rtb_TSamp_k;

  // Saturate: '<S2>/Saturation3'
  if (rtb_Gain8_idx_2 > 0.00050127541131305623) {
    // Outport: '<Root>/detuble'
    rtY.detuble[0] = 0.00050127541131305623;
  } else if (rtb_Gain8_idx_2 < -0.00050127541131305623) {
    // Outport: '<Root>/detuble'
    rtY.detuble[0] = -0.00050127541131305623;
  } else {
    // Outport: '<Root>/detuble'
    rtY.detuble[0] = rtb_Gain8_idx_2;
  }

  // Saturate: '<S2>/Saturation4'
  if (rtb_Saturation3 > 0.00050127541131305623) {
    // Outport: '<Root>/detuble'
    rtY.detuble[1] = 0.00050127541131305623;
  } else if (rtb_Saturation3 < -0.00050127541131305623) {
    // Outport: '<Root>/detuble'
    rtY.detuble[1] = -0.00050127541131305623;
  } else {
    // Outport: '<Root>/detuble'
    rtY.detuble[1] = rtb_Saturation3;
  }

  // Saturate: '<S2>/Saturation5'
  if (rtb_Gain8_idx_0 > 0.00050127541131305623) {
    // Outport: '<Root>/detuble'
    rtY.detuble[2] = 0.00050127541131305623;
  } else if (rtb_Gain8_idx_0 < -0.00050127541131305623) {
    // Outport: '<Root>/detuble'
    rtY.detuble[2] = -0.00050127541131305623;
  } else {
    // Outport: '<Root>/detuble'
    rtY.detuble[2] = rtb_Gain8_idx_0;
  }

  // Outport: '<Root>/point' incorporates:
  //   Saturate: '<S3>/Saturation3'
  //   Saturate: '<S3>/Saturation4'

  rtY.point[0] = 0.0;
  rtY.point[1] = 0.0;

  // Saturate: '<S3>/Saturation5' incorporates:
  //   Gain: '<S3>/Gain'

  rtb_Gain8_idx_2 = 0.025063770565652812 * rtb_Gain8_idx_1;
  if (rtb_Gain8_idx_2 > 0.00050127541131305623) {
    // Outport: '<Root>/point'
    rtY.point[2] = 0.00050127541131305623;
  } else if (rtb_Gain8_idx_2 < -0.00050127541131305623) {
    // Outport: '<Root>/point'
    rtY.point[2] = -0.00050127541131305623;
  } else {
    // Outport: '<Root>/point'
    rtY.point[2] = rtb_Gain8_idx_2;
  }

  // End of Saturate: '<S3>/Saturation5'
  // End of Outputs for SubSystem: '<Root>/StarshotACS'
}

// Model initialize function
void StarshotACS0ModelClass::initialize()
{
  // (no initialization code required)
}

// Constructor
StarshotACS0ModelClass::StarshotACS0ModelClass()
{
}

// Destructor
StarshotACS0ModelClass::~StarshotACS0ModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL * StarshotACS0ModelClass::getRTM()
{
  return (&rtM);
}

//
// File trailer for generated code.
//
// [EOF]
//
