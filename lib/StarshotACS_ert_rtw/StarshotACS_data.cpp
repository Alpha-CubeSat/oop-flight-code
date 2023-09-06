//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: StarshotACS_data.cpp
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

// Invariant block signals (default storage)
const ConstB rtConstB = {
  {
    0.0,
    1.0,
    -0.0,
    -1.0,
    0.0,
    0.0,
    0.0,
    -0.0,
    0.0
  }
  // '<S11>/Vector Concatenate'
};

// Constant parameters (default storage)
const ConstP rtConstP = {
  // Expression: [1 0 0;0 1 0;0 0 1]
  //  Referenced by: '<S2>/Identity matrix'

  { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 },

  // Pooled Parameter (Mixed Expressions)
  //  Referenced by:
  //    '<S2>/Discrete-Time Integrator'
  //    '<S3>/e_z'
  //    '<S4>/Constant'

  { 0.0, 0.0, 1.0 }
};

//
// File trailer for generated code.
//
// [EOF]
//
