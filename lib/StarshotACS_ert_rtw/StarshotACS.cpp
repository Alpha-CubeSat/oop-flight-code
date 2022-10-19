//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: StarshotACS.cpp
//
// Code generated for Simulink model 'StarshotACS'.
//
// Model version                  : 10.28
// Simulink Coder version         : 9.6 (R2021b) 14-May-2021
// C/C++ source code generated on : Sun Jun  5 03:57:16 2022
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#include "StarshotACS.h"

// Model step function
void StarshotACSModelClass::step()
{
    real_T rtb_VectorConcatenate[9];
    real_T rtb_VectorConcatenate_0[9];
    real_T rtb_Gain[3];
    real_T rtb_TSamp[3];
    real_T rtb_Gain8_idx_0;
    real_T rtb_Gain8_idx_1;
    real_T rtb_Gain8_idx_2;
    real_T rtb_Gain_0;
    real_T rtb_Gain_1;
    real_T rtb_Gain_2;
    real_T rtb_Gain_3;
    real_T rtb_Product1;
    real_T rtb_Product1_idx_0;
    real_T rtb_Product1_idx_1;
    real_T rtb_Product7;
    real_T rtb_Saturation3;
    real_T rtb_TSamp_o;
    real_T rtb_TrigonometricFunction5;
    int32_T i;
    int32_T tmp;

    // Outputs for Atomic SubSystem: '<Root>/StarshotACS'
    // Gain: '<S2>/Kane damping' incorporates:
    //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
    //   Gain: '<S2>/Gain 2'

    rtb_Gain8_idx_0 = 1.0E-5 * -rtDW.DiscreteTimeIntegrator_DSTATE[0];

    // Gain: '<S2>/Gain 2' incorporates:
    //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'

    rtb_Product1_idx_0 = -rtDW.DiscreteTimeIntegrator_DSTATE[0];

    // Gain: '<S2>/Kane damping' incorporates:
    //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
    //   Gain: '<S2>/Gain 2'

    rtb_Gain8_idx_1 = 1.0E-5 * -rtDW.DiscreteTimeIntegrator_DSTATE[1];

    // Gain: '<S2>/Gain 2' incorporates:
    //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'

    rtb_Product1_idx_1 = -rtDW.DiscreteTimeIntegrator_DSTATE[1];
    rtb_Product1 = -rtDW.DiscreteTimeIntegrator_DSTATE[2];

    // Gain: '<S2>/Kane damping' incorporates:
    //   DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
    //   Gain: '<S2>/Gain 2'

    rtb_Gain8_idx_2 = 1.0E-5 * -rtDW.DiscreteTimeIntegrator_DSTATE[2];

    // S-Function (sdsp2norm2): '<S2>/Normalization' incorporates:
    //   DotProduct: '<S8>/Dot Product6'
    //   Inport: '<Root>/magneticfield'

    rtb_Product7 = (rtU.magneticfield[0] * rtU.magneticfield[0] + rtU.magneticfield[1] *
                                                                      rtU.magneticfield[1]) +
                   rtU.magneticfield[2] * rtU.magneticfield[2];
    rtb_Saturation3 = 1.0 / (rtb_Product7 + 1.0E-10);

    // SignalConversion generated from: '<S6>/Vector Concatenate' incorporates:
    //   Constant: '<S6>/Constant3'
    //   Gain: '<S6>/Gain'
    //   Inport: '<Root>/angularvelocity'

    rtb_VectorConcatenate[0] = 0.0;
    rtb_VectorConcatenate[1] = rtU.w[2];
    rtb_VectorConcatenate[2] = -rtU.w[1];

    // SignalConversion generated from: '<S6>/Vector Concatenate' incorporates:
    //   Constant: '<S6>/Constant3'
    //   Gain: '<S6>/Gain1'
    //   Inport: '<Root>/angularvelocity'

    rtb_VectorConcatenate[3] = -rtU.w[2];
    rtb_VectorConcatenate[4] = 0.0;
    rtb_VectorConcatenate[5] = rtU.w[0];

    // SignalConversion generated from: '<S6>/Vector Concatenate' incorporates:
    //   Constant: '<S6>/Constant3'
    //   Gain: '<S6>/Gain2'
    //   Inport: '<Root>/angularvelocity'

    rtb_VectorConcatenate[6] = rtU.w[1];
    rtb_VectorConcatenate[7] = -rtU.w[0];
    rtb_VectorConcatenate[8] = 0.0;

    // S-Function (sdsp2norm2): '<S2>/Normalization' incorporates:
    //   Inport: '<Root>/Bfield_body'

    rtb_Gain[0] = rtU.magneticfield[0] * rtb_Saturation3;

    // SampleTimeMath: '<S5>/TSamp' incorporates:
    //   Inport: '<Root>/angularvelocity'
    //
    //  About '<S5>/TSamp':
    //   y = u * K where K = 1 / ( w * Ts )

    // rtb_TSamp[0] = rtU.w[0] * 1000.0;
    rtb_TSamp[0] = rtU.w[0] * 1.0 / (time_step);

    // S-Function (sdsp2norm2): '<S2>/Normalization' incorporates:
    //   Inport: '<Root>/Bfield_body'

    rtb_Gain[1] = rtU.magneticfield[1] * rtb_Saturation3;

    // SampleTimeMath: '<S5>/TSamp' incorporates:
    //   Inport: '<Root>/angularvelocity'
    //
    //  About '<S5>/TSamp':
    //   y = u * K where K = 1 / ( w * Ts )

    // rtb_TSamp[1] = rtU.w[1] * 1000.0;
    rtb_TSamp[1] = rtU.w[1] * 1.0 / (time_step);

    // S-Function (sdsp2norm2): '<S2>/Normalization' incorporates:
    //   Inport: '<Root>/Bfield_body'

    rtb_Gain[2] = rtU.magneticfield[2] * rtb_Saturation3;

    // SampleTimeMath: '<S5>/TSamp' incorporates:
    //   Inport: '<Root>/angularvelocity'
    //
    //  About '<S5>/TSamp':
    //   y = u * K where K = 1 / ( w * Ts )

    // rtb_TSamp[2] = rtU.w[2] * 1000.0;
    rtb_TSamp[2] = rtU.w[2] * 1.0 / (time_step);

    // Product: '<S4>/Product'
    rtb_Gain_0 = rtb_Gain[2];

    // Product: '<S4>/Product1'
    rtb_Gain_1 = rtb_Gain[1];

    // Product: '<S4>/Product2'
    rtb_Gain_2 = rtb_Gain[0];

    // Product: '<S4>/Product3'
    rtb_TrigonometricFunction5 = rtb_Gain[2];

    // Product: '<S4>/Product4'
    rtb_Gain_3 = rtb_Gain[1];

    // Product: '<S4>/Product5'
    rtb_TSamp_o = rtb_Gain[0];

    // Saturate: '<S2>/Saturation3' incorporates:
    //   Gain: '<S2>/Gain'
    //   Product: '<S4>/Product'
    //   Product: '<S4>/Product1'
    //   Sum: '<S4>/Sum'

    rtb_Saturation3 = (rtb_Gain8_idx_1 * rtb_Gain_0 - rtb_Gain_1 * rtb_Gain8_idx_2) * 50.0;

    // Saturate: '<S2>/Saturation4' incorporates:
    //   Gain: '<S2>/Gain'
    //   Product: '<S4>/Product2'
    //   Product: '<S4>/Product3'
    //   Sum: '<S4>/Sum1'

    rtb_Gain8_idx_2 = (rtb_Gain_2 * rtb_Gain8_idx_2 - rtb_Gain8_idx_0 *
                                                          rtb_TrigonometricFunction5) *
                      50.0;

    // Saturate: '<S2>/Saturation5' incorporates:
    //   Gain: '<S2>/Gain'
    //   Product: '<S4>/Product4'
    //   Product: '<S4>/Product5'
    //   Sum: '<S4>/Sum2'

    rtb_Gain8_idx_0 = (rtb_Gain8_idx_0 * rtb_Gain_3 - rtb_TSamp_o *
                                                          rtb_Gain8_idx_1) *
                      50.0;

    // Sqrt: '<S8>/Sqrt4'
    rtb_Product7 = std::sqrt(rtb_Product7);

    // DotProduct: '<S9>/Dot Product6'
    rtb_Gain_0 = 0.0;

    // DotProduct: '<S13>/Dot Product6'
    rtb_Gain8_idx_1 = 0.0;

    // DotProduct: '<S14>/Dot Product6'
    rtb_Gain_3 = 0.0;
    for (i = 0; i < 3; i++) {
        // Product: '<S3>/Product6' incorporates:
        //   Concatenate: '<S10>/Vector Concatenate'
        //   Inport: '<Root>/Bfield_body'
        //   Product: '<S3>/Product4'

        rtb_Gain_1 = ((rtConstB.VectorConcatenate[i + 3] * rtU.magneticfield[1] +
                       rtConstB.VectorConcatenate[i] * rtU.magneticfield[0]) +
                      rtConstB.VectorConcatenate[i + 6] * rtU.magneticfield[2]) /
                     rtb_Product7;

        // DotProduct: '<S9>/Dot Product6'
        rtb_Gain_0 += rtb_Gain_1 * rtb_Gain_1;

        // DotProduct: '<S13>/Dot Product6' incorporates:
        //   Inport: '<Root>/angularvelocity'

        rtb_Gain8_idx_1 += rtU.w[i] * rtU.w[i];

        // DotProduct: '<S14>/Dot Product6' incorporates:
        //   Inport: '<Root>/Bfield_body'

        rtb_Gain_3 += rtU.magneticfield[i] * rtU.magneticfield[i];

        // Product: '<S3>/Product6' incorporates:
        //   Inport: '<Root>/Bfield_body'
        //   Inport: '<Root>/angularvelocity'
        //   Product: '<S11>/Product3'
        //   Product: '<S3>/Product4'

        rtb_Gain[i] = rtU.w[i] * rtU.magneticfield[i];
    }

    // Sqrt: '<S9>/Sqrt4' incorporates:
    //   DotProduct: '<S9>/Dot Product6'

    rtb_Gain_0 = std::sqrt(rtb_Gain_0);

    // Trigonometry: '<S3>/Trigonometric Function5'
    if (rtb_Gain_0 > 1.0) {
        rtb_Gain_0 = 1.0;
    } else if (rtb_Gain_0 < -1.0) {
        rtb_Gain_0 = -1.0;
    }

    rtb_TrigonometricFunction5 = std::asin(rtb_Gain_0);
    pointing_error = rtb_TrigonometricFunction5;

    // End of Trigonometry: '<S3>/Trigonometric Function5'

    // SampleTimeMath: '<S7>/TSamp'
    //
    //  About '<S7>/TSamp':
    //   y = u * K where K = 1 / ( w * Ts )

    // rtb_TSamp_o = rtb_TrigonometricFunction5 * 1000.0;
    rtb_TSamp_o = rtb_TrigonometricFunction5 * 1.0 / (time_step);

    // Sum: '<S11>/Add'
    rtb_Gain_0 = rtb_Gain[0];
    rtb_Gain_1 = rtb_Gain[1];
    rtb_Gain_2 = rtb_Gain[2];

    // Gain: '<S3>/Gain' incorporates:
    //   Constant: '<S11>/Constant3'
    //   Constant: '<S11>/Constant4'
    //   Constant: '<S12>/Constant10'
    //   Constant: '<S12>/Constant9'
    //   DotProduct: '<S13>/Dot Product6'
    //   DotProduct: '<S14>/Dot Product6'
    //   Gain: '<S3>/Gain10'
    //   Gain: '<S3>/Gain11'
    //   Inport: '<Root>/angularvelocity'
    //   Product: '<S11>/Divide6'
    //   Product: '<S3>/Product7'
    //   Product: '<S3>/Product8'
    //   Sqrt: '<S13>/Sqrt4'
    //   Sqrt: '<S14>/Sqrt4'
    //   Sum: '<S11>/Add'
    //   Sum: '<S3>/Sum7'
    //   Sum: '<S7>/Diff'
    //   Switch: '<S11>/Switch'
    //   Switch: '<S12>/Switch1'
    //   UnitDelay: '<S7>/UD'
    //
    //  Block description for '<S7>/Diff':
    //
    //   Add in CPU
    //
    //  Block description for '<S7>/UD':
    //
    //   Store in Global RAM

    if (rtU.w[2] >= 0.0) {
        i = 1;
    } else {
        i = -1;
    }

    if (((rtb_Gain_0 + rtb_Gain_1) + rtb_Gain_2) * (1.0 / std::sqrt(rtb_Gain8_idx_1)) / std::sqrt(rtb_Gain_3) > 0.0) {
        tmp = 1;
    } else {
        tmp = -1;
    }

    rtb_Gain[2] = ((rtb_TSamp_o - rtDW.UD_DSTATE_k) * 1.6243830496592611E-6 +
                   5.2506360319823134E-10 * rtb_TrigonometricFunction5) *
                  static_cast<real_T>(i * tmp) / rtb_Product7 * 50.0;

    // End of Gain: '<S3>/Gain'

    // Sum: '<S2>/Sum10' incorporates:
    //   Concatenate: '<S6>/Vector Concatenate'
    //   Constant: '<S2>/Identity matrix'

    for (i = 0; i < 9; i++) {
        rtb_VectorConcatenate_0[i] = rtb_VectorConcatenate[i] +
                                     rtConstPacs.Identitymatrix_Value[i];
    }

    // End of Sum: '<S2>/Sum10'
    for (i = 0; i < 3; i++) {
        // Sum: '<S5>/Diff' incorporates:
        //   Product: '<S2>/Product1'
        //
        //  Block description for '<S5>/Diff':
        //
        //   Add in CPU

        rtb_Gain_0 = rtb_TSamp[i];

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

        // rtDW.DiscreteTimeIntegrator_DSTATE[i] += ((0.0 - (rtb_Gain_0 -
        //   rtDW.UD_DSTATE[i])) - ((invId * -rtb_Product1_idx_0 * damperc *
        //   rtb_VectorConcatenate_0[i] + invId * -rtb_Product1_idx_1 *
        //   damperc * rtb_VectorConcatenate_0[i + 3]) + invId *
        //   -rtb_Product1 * damperc * rtb_VectorConcatenate_0[i + 6])) * 0.001;

        rtDW.DiscreteTimeIntegrator_DSTATE[i] += ((0.0 - (rtb_Gain_0 -
                                                          rtDW.UD_DSTATE[i])) -
                                                  ((invId * -rtb_Product1_idx_0 * damperc *
                                                        rtb_VectorConcatenate_0[i] +
                                                    invId * -rtb_Product1_idx_1 *
                                                        damperc * rtb_VectorConcatenate_0[i + 3]) +
                                                   invId *
                                                       -rtb_Product1 * damperc * rtb_VectorConcatenate_0[i + 6])) *
                                                 time_step;

        // Update for UnitDelay: '<S5>/UD'
        //
        //  Block description for '<S5>/UD':
        //
        //   Store in Global RAM

        rtDW.UD_DSTATE[i] = rtb_Gain_0;
    }

    // Update for UnitDelay: '<S7>/UD'
    //
    //  Block description for '<S7>/UD':
    //
    //   Store in Global RAM

    rtDW.UD_DSTATE_k = rtb_TSamp_o;

    // Saturate: '<S2>/Saturation3'
    if (rtb_Saturation3 > maximum_current) {
        // Outport: '<Root>/detumble'
        rtY.detumble[0] = maximum_current;
    } else if (rtb_Saturation3 < -maximum_current) {
        // Outport: '<Root>/detumble'
        rtY.detumble[0] = -maximum_current;
    } else {
        // Outport: '<Root>/detumble'
        rtY.detumble[0] = rtb_Saturation3;
    }

    // Saturate: '<S2>/Saturation4'
    if (rtb_Gain8_idx_2 > maximum_current) {
        // Outport: '<Root>/detumble'
        rtY.detumble[1] = maximum_current;
    } else if (rtb_Gain8_idx_2 < -maximum_current) {
        // Outport: '<Root>/detumble'
        rtY.detumble[1] = -maximum_current;
    } else {
        // Outport: '<Root>/detumble'
        rtY.detumble[1] = rtb_Gain8_idx_2;
    }

    // Saturate: '<S2>/Saturation5'
    if (rtb_Gain8_idx_0 > maximum_current) {
        // Outport: '<Root>/detumble'
        rtY.detumble[2] = maximum_current;
    } else if (rtb_Gain8_idx_0 < -maximum_current) {
        // Outport: '<Root>/detumble'
        rtY.detumble[2] = -maximum_current;
    } else {
        // Outport: '<Root>/detumble'
        rtY.detumble[2] = rtb_Gain8_idx_0;
    }

    // Outport: '<Root>/point' incorporates:
    //   Saturate: '<S3>/Saturation3'
    //   Saturate: '<S3>/Saturation4'
    // 3.375 = starshot.magnetorq.m_max_x/(starshot.magnetorque.A*starshot.magnetorq.n)

    rtY.point[0] = 0.0;
    rtY.point[1] = 0.0;

    // Saturate: '<S3>/Saturation5'
    if (rtb_Gain[2] > maximum_current) {
        // Outport: '<Root>/point'
        rtY.point[2] = maximum_current;
    } else if (rtb_Gain[2] < -maximum_current) {
        // Outport: '<Root>/point'
        rtY.point[2] = -maximum_current;
    } else {
        // Outport: '<Root>/point'
        rtY.point[2] = rtb_Gain[2];
    }

    // End of Saturate: '<S3>/Saturation5'
    // End of Outputs for SubSystem: '<Root>/StarshotACS'
}

// Model initialize function
void StarshotACSModelClass::initialize(float t_stp, float kane_damper_c, float kane_Id, float ampFactor, float max_current, float csareamt, float no_loops, float wdx, float wdy, float wdz)
{
    // SystemInitialize for Atomic SubSystem: '<Root>/StarshotACS'
    // InitializeConditions for DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
    rtDW.DiscreteTimeIntegrator_DSTATE[0] = wdx;
    rtDW.DiscreteTimeIntegrator_DSTATE[1] = wdy;
    rtDW.DiscreteTimeIntegrator_DSTATE[2] = wdz;
    damperc = kane_damper_c;
    Id = kane_Id;
    invId = 1 / kane_Id;
    // End of SystemInitialize for SubSystem: '<Root>/StarshotACS'
    m_max = ampFactor * max_current * csareamt * no_loops;
    magtorqhwvalue = m_max / (csareamt * no_loops);
    maximum_current = max_current;
    time_step = t_stp;
}
// 3.375 = starshot.magnetorq.m_max_x/(starshot.magnetorque.A*starshot.magnetorq.n)
//  Constructor
StarshotACSModelClass::StarshotACSModelClass() : rtU(),
                                                 rtY(),
                                                 rtDW(),
                                                 rtM()
{
    // Currently there is no constructor body generated.
}

// Destructor
StarshotACSModelClass::~StarshotACSModelClass()
{
    // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL *StarshotACSModelClass::getRTM()
{
    return (&rtM);
}

//
// File trailer for generated code.
//
// [EOF]
//
