//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: StarshotACS.cpp
//
// Code generated for Simulink model 'StarshotACS'.
//
// Model version                  : 13.5
// Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
// C/C++ source code generated on : Wed Jul 26 17:42:42 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#include "StarshotACS.h"

// Exported block parameters
real_T A = 4.0E-5; // Variable: A
                   //  Referenced by:
                   //    '<S2>/Gain'
                   //    '<S2>/Saturation1'
                   //    '<S2>/Saturation2'
                   //    '<S2>/Saturation6'
                   //    '<S3>/Gain'
                   //    '<S3>/Saturation1'
                   //    '<S3>/Saturation2'
                   //    '<S3>/Saturation6'
                   //  Cross Section Area For magnetorquer

real_T Id = 0.0021; // Variable: Id
                    //  Referenced by: '<S2>/Id inverse'
                    //  Id for the Detumble

real_T Kd = 0.0007935279615795299; // Variable: Kd
                                   //  Referenced by: '<S3>/Kd Gain'
                                   //  Kd for the pointing

real_T Kp = 5.2506307629097953E-10; // Variable: Kp
                                    //  Referenced by: '<S3>/Kp Gain'
                                    //  Kp for the pointing

real_T c = 0.004; // Variable: c
                  //  Referenced by:
                  //    '<S2>/Gain 8'
                  //    '<S2>/Kane damping'
                  //  c for the Detumble

real_T i_max = 0.25; // Variable: i_max
                     //  Referenced by:
                     //    '<S2>/Saturation1'
                     //    '<S2>/Saturation2'
                     //    '<S2>/Saturation6'
                     //    '<S3>/Saturation1'
                     //    '<S3>/Saturation2'
                     //    '<S3>/Saturation6'
                     //  Max Current for each magnetorquer

real_T k = 13.5; // Variable: k
                 //  Referenced by:
                 //    '<S2>/Gain'
                 //    '<S2>/Saturation1'
                 //    '<S2>/Saturation2'
                 //    '<S2>/Saturation6'
                 //    '<S3>/Gain'
                 //    '<S3>/Saturation1'
                 //    '<S3>/Saturation2'
                 //    '<S3>/Saturation6'
                 //  Gain for magnetorquer

real_T n = 500.0; // Variable: n
                  //  Referenced by:
                  //    '<S2>/Gain'
                  //    '<S2>/Saturation1'
                  //    '<S2>/Saturation2'
                  //    '<S2>/Saturation6'
                  //    '<S3>/Gain'
                  //    '<S3>/Saturation1'
                  //    '<S3>/Saturation2'
                  //    '<S3>/Saturation6'
                  //  Wire Turn For magnetorquer
real_T step_size = 0.25;

// Model step function
void StarshotACS::step()
{
  real_T rtb_VectorConcatenate[9];
  real_T rtb_VectorConcatenate_0[9];
  real_T rtb_Normalization[3];
  real_T rtb_Product1[3];
  real_T acc;
  real_T rtb_Gain2_0;
  real_T rtb_Gain2_idx_0;
  real_T rtb_Gain2_idx_1;
  real_T rtb_Normalization_0;
  real_T rtb_Saturation2;
  real_T rtb_Saturation6;
  real_T rtb_Sqrt4;
  real_T rtb_TSamp_o;
  real_T rtb_TrigonometricFunction5;
  real_T rtb_TrigonometricFunction5_k;
  real_T tmp;
  real_T tmp_0;
  real_T tmp_1;
  real_T u2_tmp;
  int32_T i;
  int32_T idx1;

  // Outputs for Atomic SubSystem: '<Root>/StarshotACS'
  // Gain: '<S2>/Gain 2' incorporates:
  //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'

  rtb_Gain2_idx_0 = -rtDW.DiscreteTimeIntegrator_DSTATE[0];

  // Gain: '<S2>/Kane damping' incorporates:
  //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
  //   Gain: '<S2>/Gain 2'

  rtb_Product1[0] = c * -rtDW.DiscreteTimeIntegrator_DSTATE[0];

  // Gain: '<S2>/Gain 2' incorporates:
  //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'

  rtb_Gain2_idx_1 = -rtDW.DiscreteTimeIntegrator_DSTATE[1];

  // Gain: '<S2>/Kane damping' incorporates:
  //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
  //   Gain: '<S2>/Gain 2'

  rtb_Product1[1] = c * -rtDW.DiscreteTimeIntegrator_DSTATE[1];

  // Gain: '<S2>/Gain 2' incorporates:
  //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'

  rtb_Gain2_0 = -rtDW.DiscreteTimeIntegrator_DSTATE[2];

  // Gain: '<S2>/Kane damping' incorporates:
  //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
  //   Gain: '<S2>/Gain 2'

  rtb_Product1[2] = c * -rtDW.DiscreteTimeIntegrator_DSTATE[2];

  // S-Function (sdsp2norm2): '<S2>/Normalization' incorporates:
  //   DotProduct: '<S9>/Dot Product6'
  //   Inport: '<Root>/Bfield_body'

  rtb_Saturation6 = (rtU.Bfield_body[0] * rtU.Bfield_body[0] + rtU.Bfield_body[1] * rtU.Bfield_body[1]) + rtU.Bfield_body[2] *
                                                                                                              rtU.Bfield_body[2];
  acc = 1.0 / (rtb_Saturation6 + 1.0E-10);
  rtb_Normalization[0] = rtU.Bfield_body[0] * acc;
  rtb_Normalization[1] = rtU.Bfield_body[1] * acc;
  rtb_Normalization[2] = rtU.Bfield_body[2] * acc;

  // Sum: '<S5>/Sum' incorporates:
  //   Product: '<S5>/Product'
  //   Product: '<S5>/Product1'

  rtb_Sqrt4 = rtb_Product1[1] * rtb_Normalization[2] - rtb_Normalization[1] *
                                                           rtb_Product1[2];

  // Sum: '<S5>/Sum1' incorporates:
  //   Product: '<S5>/Product2'
  //   Product: '<S5>/Product3'

  rtb_Saturation2 = rtb_Normalization[0] * rtb_Product1[2] - rtb_Product1[0] *
                                                                 rtb_Normalization[2];

  // Sum: '<S5>/Sum2' incorporates:
  //   Product: '<S5>/Product4'
  //   Product: '<S5>/Product5'

  rtb_TrigonometricFunction5_k = rtb_Product1[0] * rtb_Normalization[1] -
                                 rtb_Normalization[0] * rtb_Product1[1];

  // Gain: '<S2>/Gain'
  tmp = 1.0 / (k * A * n);

  // Gain: '<S2>/Id inverse'
  tmp_0 = 1.0 / Id;

  // SignalConversion generated from: '<S7>/Vector Concatenate' incorporates:
  //   Constant: '<S7>/Constant3'
  //   Gain: '<S7>/Gain'
  //   Inport: '<Root>/angularvelocity'

  rtb_VectorConcatenate[0] = 0.0;
  rtb_VectorConcatenate[1] = rtU.w[2];
  rtb_VectorConcatenate[2] = -rtU.w[1];

  // SignalConversion generated from: '<S7>/Vector Concatenate' incorporates:
  //   Constant: '<S7>/Constant3'
  //   Gain: '<S7>/Gain1'
  //   Inport: '<Root>/angularvelocity'

  rtb_VectorConcatenate[3] = -rtU.w[2];
  rtb_VectorConcatenate[4] = 0.0;
  rtb_VectorConcatenate[5] = rtU.w[0];

  // SignalConversion generated from: '<S7>/Vector Concatenate' incorporates:
  //   Constant: '<S7>/Constant3'
  //   Gain: '<S7>/Gain2'
  //   Inport: '<Root>/angularvelocity'

  rtb_VectorConcatenate[6] = rtU.w[1];
  rtb_VectorConcatenate[7] = -rtU.w[0];
  rtb_VectorConcatenate[8] = 0.0;

  // SampleTimeMath: '<S6>/TSamp' incorporates:
  //   Inport: '<Root>/angularvelocity'
  //
  //  About '<S6>/TSamp':
  //   y = u * K where K = 1 / ( w * Ts )

  rtb_Product1[0] = rtU.w[0] * (1.0 / step_size);
  rtb_Product1[1] = rtU.w[1] * (1.0 / step_size);
  rtb_Product1[2] = rtU.w[2] * (1.0 / step_size);

  // Sqrt: '<S9>/Sqrt4'
  rtb_Saturation6 = std::sqrt(rtb_Saturation6);

  // DotProduct: '<S10>/Dot Product6'
  rtb_Normalization_0 = 0.0;

  // DotProduct: '<S14>/Dot Product6'
  u2_tmp = 0.0;

  // DotProduct: '<S15>/Dot Product6'
  tmp_1 = 0.0;

  // S-Function (sdsp2norm2): '<S4>/Normalization'
  idx1 = 0;
  acc = 0.0;
  for (i = 0; i < 3; i++)
  {
    // Product: '<S3>/Product6' incorporates:
    //   Concatenate: '<S11>/Vector Concatenate'
    //   Inport: '<Root>/Bfield_body'
    //   Product: '<S3>/Product4'

    rtb_TrigonometricFunction5 = ((rtConstB.VectorConcatenate[i + 3] *
                                       rtU.Bfield_body[1] +
                                   rtConstB.VectorConcatenate[i] * rtU.Bfield_body[0]) +
                                  rtConstB.VectorConcatenate[i + 6] * rtU.Bfield_body[2]) /
                                 rtb_Saturation6;

    // DotProduct: '<S10>/Dot Product6'
    rtb_Normalization_0 += rtb_TrigonometricFunction5 *
                           rtb_TrigonometricFunction5;

    // Product: '<S12>/Product3' incorporates:
    //   Inport: '<Root>/Bfield_body'
    //   Inport: '<Root>/angularvelocity'

    rtb_TrigonometricFunction5 = rtU.w[i];
    rtb_Normalization[i] = rtb_TrigonometricFunction5 * rtU.Bfield_body[i];

    // DotProduct: '<S14>/Dot Product6'
    u2_tmp += rtb_TrigonometricFunction5 * rtb_TrigonometricFunction5;

    // DotProduct: '<S15>/Dot Product6' incorporates:
    //   Inport: '<Root>/Bfield_body'

    tmp_1 += rtU.Bfield_body[i] * rtU.Bfield_body[i];

    // S-Function (sdsp2norm2): '<S4>/Normalization' incorporates:
    //   Inport: '<Root>/Bfield_body'

    acc += rtU.Bfield_body[idx1] * rtU.Bfield_body[idx1];
    idx1++;
  }

  // Sqrt: '<S10>/Sqrt4' incorporates:
  //   DotProduct: '<S10>/Dot Product6'

  rtb_Normalization_0 = std::sqrt(rtb_Normalization_0);

  // Trigonometry: '<S3>/Trigonometric Function5'
  if (rtb_Normalization_0 > 1.0)
  {
    rtb_Normalization_0 = 1.0;
  }

  rtb_TrigonometricFunction5 = std::asin(rtb_Normalization_0);

  // End of Trigonometry: '<S3>/Trigonometric Function5'

  // SampleTimeMath: '<S8>/TSamp'
  //
  //  About '<S8>/TSamp':
  //   y = u * K where K = 1 / ( w * Ts )

  rtb_TSamp_o = rtb_TrigonometricFunction5 * (1.0 / step_size);

  // Switch: '<S13>/Switch1' incorporates:
  //   Constant: '<S13>/Constant10'
  //   Constant: '<S13>/Constant9'
  //   Inport: '<Root>/angularvelocity'

  if (rtU.w[2] >= 0.0)
  {
    idx1 = 1;
  }
  else
  {
    idx1 = -1;
  }

  // Switch: '<S12>/Switch' incorporates:
  //   Constant: '<S12>/Constant3'
  //   Constant: '<S12>/Constant4'
  //   DotProduct: '<S14>/Dot Product6'
  //   DotProduct: '<S15>/Dot Product6'
  //   Product: '<S12>/Divide6'
  //   Sqrt: '<S14>/Sqrt4'
  //   Sqrt: '<S15>/Sqrt4'
  //   Sum: '<S12>/Add'

  if (((rtb_Normalization[0] + rtb_Normalization[1]) + rtb_Normalization[2]) *
          (1.0 / std::sqrt(u2_tmp)) / std::sqrt(tmp_1) >
      0.0)
  {
    i = 1;
  }
  else
  {
    i = -1;
  }

  // Product: '<S3>/Product7' incorporates:
  //   Gain: '<S3>/Kd Gain'
  //   Gain: '<S3>/Kp Gain'
  //   Product: '<S3>/Product8'
  //   Sum: '<S3>/Sum7'
  //   Sum: '<S8>/Diff'
  //   Switch: '<S12>/Switch'
  //   Switch: '<S13>/Switch1'
  //   UnitDelay: '<S8>/UD'
  //
  //  Block description for '<S8>/Diff':
  //
  //   Add in CPU
  //
  //  Block description for '<S8>/UD':
  //
  //   Store in Global RAM

  rtb_Normalization_0 = ((rtb_TSamp_o - rtDW.UD_DSTATE_k) * Kd + Kp *
                                                                     rtb_TrigonometricFunction5) *
                        static_cast<real_T>(idx1 * i) /
                        rtb_Saturation6;

  // Saturate: '<S3>/Saturation6' incorporates:
  //   Saturate: '<S2>/Saturation1'
  //   Saturate: '<S2>/Saturation2'
  //   Saturate: '<S2>/Saturation6'
  //   Saturate: '<S3>/Saturation1'

  rtb_Saturation6 = -k * i_max * A * n;
  u2_tmp = k * i_max * A * n;

  // Gain: '<S3>/Gain'
  rtb_TrigonometricFunction5 = 1.0 / (A * n * k);

  // S-Function (sdsp2norm2): '<S4>/Normalization' incorporates:
  //   Inport: '<Root>/Bfield_body'

  acc = 1.0 / (std::sqrt(acc) + 1.0E-10);
  rtb_Normalization[0] = rtU.Bfield_body[0] * acc;
  rtb_Normalization[1] = rtU.Bfield_body[1] * acc;
  rtb_Normalization[2] = rtU.Bfield_body[2] * acc;

  // Sum: '<S2>/Sum10' incorporates:
  //   Concatenate: '<S7>/Vector Concatenate'
  //   Constant: '<S2>/Identity matrix'

  for (idx1 = 0; idx1 < 9; idx1++)
  {
    rtb_VectorConcatenate_0[idx1] = rtb_VectorConcatenate[idx1] +
                                    rtConstP.Identitymatrix_Value[idx1];
  }

  // End of Sum: '<S2>/Sum10'

  // Update for UnitDelay: '<S8>/UD'
  //
  //  Block description for '<S8>/UD':
  //
  //   Store in Global RAM

  rtDW.UD_DSTATE_k = rtb_TSamp_o;

  // Saturate: '<S2>/Saturation1'
  if (rtb_Sqrt4 > u2_tmp)
  {
    rtb_Sqrt4 = u2_tmp;
  }
  else if (rtb_Sqrt4 < rtb_Saturation6)
  {
    rtb_Sqrt4 = rtb_Saturation6;
  }

  // Outport: '<Root>/detumble' incorporates:
  //   Gain: '<S2>/Gain'
  //   Saturate: '<S2>/Saturation1'

  rtY.detumble[0] = tmp * rtb_Sqrt4;

  // Saturate: '<S2>/Saturation2'
  if (rtb_Saturation2 > u2_tmp)
  {
    rtb_Saturation2 = u2_tmp;
  }
  else if (rtb_Saturation2 < rtb_Saturation6)
  {
    rtb_Saturation2 = rtb_Saturation6;
  }

  // Outport: '<Root>/detumble' incorporates:
  //   Gain: '<S2>/Gain'
  //   Saturate: '<S2>/Saturation2'

  rtY.detumble[1] = tmp * rtb_Saturation2;

  // Saturate: '<S2>/Saturation6'
  if (rtb_TrigonometricFunction5_k > u2_tmp)
  {
    rtb_TrigonometricFunction5_k = u2_tmp;
  }
  else if (rtb_TrigonometricFunction5_k < rtb_Saturation6)
  {
    rtb_TrigonometricFunction5_k = rtb_Saturation6;
  }

  // Outport: '<Root>/detumble' incorporates:
  //   Gain: '<S2>/Gain'
  //   Saturate: '<S2>/Saturation6'

  rtY.detumble[2] = tmp * rtb_TrigonometricFunction5_k;

  // Saturate: '<S3>/Saturation1'
  if (u2_tmp < 0.0)
  {
    // Saturate: '<S3>/Saturation2'
    rtb_Sqrt4 = u2_tmp;
  }
  else if (rtb_Saturation6 > 0.0)
  {
    // Saturate: '<S3>/Saturation2'
    rtb_Sqrt4 = rtb_Saturation6;
  }
  else
  {
    // Saturate: '<S3>/Saturation2'
    rtb_Sqrt4 = 0.0;
  }

  // Gain: '<S3>/Gain' incorporates:
  //   Saturate: '<S3>/Saturation1'

  acc = rtb_TrigonometricFunction5 * rtb_Sqrt4;

  // Outport: '<Root>/point' incorporates:
  //   Gain: '<S3>/Gain'

  rtY.point[0] = acc;
  rtY.point[1] = acc;

  // Saturate: '<S3>/Saturation6'
  if (rtb_Normalization_0 > u2_tmp)
  {
    rtb_Normalization_0 = u2_tmp;
  }
  else if (rtb_Normalization_0 < rtb_Saturation6)
  {
    rtb_Normalization_0 = rtb_Saturation6;
  }

  // Outport: '<Root>/point' incorporates:
  //   Gain: '<S3>/Gain'
  //   Saturate: '<S3>/Saturation6'

  rtY.point[2] = rtb_TrigonometricFunction5 * rtb_Normalization_0;

  // DotProduct: '<S4>/Dot Product'
  rtb_Normalization_0 = 0.0;
  for (i = 0; i < 3; i++)
  {
    // Sum: '<S6>/Diff' incorporates:
    //   Product: '<S2>/Product1'
    //
    //  Block description for '<S6>/Diff':
    //
    //   Add in CPU

    acc = rtb_Product1[i];

    // Update for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' incorporates:
    //   Gain: '<S2>/Gain 2'
    //   Gain: '<S2>/Gain 8'
    //   Gain: '<S2>/Gain 9'
    //   Gain: '<S2>/Id inverse'
    //   Product: '<S2>/Product1'
    //   Sum: '<S2>/Sum8'
    //   Sum: '<S6>/Diff'
    //   UnitDelay: '<S6>/UD'
    //
    //  Block description for '<S6>/Diff':
    //
    //   Add in CPU
    //
    //  Block description for '<S6>/UD':
    //
    //   Store in Global RAM

    rtDW.DiscreteTimeIntegrator_DSTATE[i] += ((0.0 - (acc - rtDW.UD_DSTATE[i])) - ((tmp_0 * -rtb_Gain2_idx_0 * c * rtb_VectorConcatenate_0[i] + tmp_0 *
                                                                                                                                                    -rtb_Gain2_idx_1 * c * rtb_VectorConcatenate_0[i + 3]) +
                                                                                   tmp_0 *
                                                                                       -rtb_Gain2_0 * c * rtb_VectorConcatenate_0[i + 6])) *
                                             step_size;

    // Update for UnitDelay: '<S6>/UD'
    //
    //  Block description for '<S6>/UD':
    //
    //   Store in Global RAM

    rtDW.UD_DSTATE[i] = acc;

    // DotProduct: '<S4>/Dot Product' incorporates:
    //   Constant: '<S4>/Constant'

    rtb_Normalization_0 += rtb_Normalization[i] * rtConstP.pooled1[i];
  }

  // Saturate: '<S4>/Saturation3' incorporates:
  //   DotProduct: '<S4>/Dot Product'

  if (rtb_Normalization_0 > 1.0)
  {
    rtb_Normalization_0 = 1.0;
  }
  else if (rtb_Normalization_0 < -1.0)
  {
    rtb_Normalization_0 = -1.0;
  }

  // Outport: '<Root>/pt_error' incorporates:
  //   Gain: '<S4>/Multiply'
  //   Saturate: '<S4>/Saturation3'
  //   Trigonometry: '<S4>/Acos'

  rtY.pt_error = 57.295779513082323 * std::acos(rtb_Normalization_0);

  // End of Outputs for SubSystem: '<Root>/StarshotACS'
}

// Model initialize function
// void StarshotACS::initialize()
// {
//   // SystemInitialize for Atomic SubSystem: '<Root>/StarshotACS'
//   // InitializeConditions for DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
//   rtDW.DiscreteTimeIntegrator_DSTATE[0] = 0.0;
//   rtDW.DiscreteTimeIntegrator_DSTATE[1] = 0.0;
//   rtDW.DiscreteTimeIntegrator_DSTATE[2] = 1.0;

//   // End of SystemInitialize for SubSystem: '<Root>/StarshotACS'
// }

void StarshotACS::initialize(double step_size_input, double A_input, double Id_input, double Kd_input, double Kp_input, double c_input, double i_max_input, double k_input, double n_input)
{
  // SystemInitialize for Atomic SubSystem: '<Root>/StarshotACS'
  // InitializeConditions for DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
  rtDW.DiscreteTimeIntegrator_DSTATE[0] = 0.0;
  rtDW.DiscreteTimeIntegrator_DSTATE[1] = 0.0;
  rtDW.DiscreteTimeIntegrator_DSTATE[2] = 1.0;
  
  step_size = step_size_input;
  A = A_input;
  Id = Id_input;
  Kd = Kd_input;
  Kp = Kp_input;
  c = c_input;
  i_max = i_max_input;
  k = k_input;
  n = n_input;

  // End of SystemInitialize for SubSystem: '<Root>/StarshotACS'
}

// Constructor
StarshotACS::StarshotACS() : rtU(),
                             rtY(),
                             rtDW(),
                             rtM()
{
  // Currently there is no constructor body generated.
}

// Destructor
StarshotACS::~StarshotACS()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL *StarshotACS::getRTM()
{
  return (&rtM);
}

//
// File trailer for generated code.
//
// [EOF]
//
