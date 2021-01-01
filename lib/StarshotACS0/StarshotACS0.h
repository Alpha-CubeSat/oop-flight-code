//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: StarshotACS0.h
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
#ifndef RTW_HEADER_StarshotACS0_h_
#define RTW_HEADER_StarshotACS0_h_
#include <cmath>
#ifndef StarshotACS0_COMMON_INCLUDES_
# define StarshotACS0_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // StarshotACS0_COMMON_INCLUDES_

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// Forward declaration for rtModel
typedef struct tag_RTM RT_MODEL;

// Block signals and states (auto storage) for system '<Root>'
typedef struct {
  real_T DiscreteTimeIntegrator_DSTATE[3];// '<S2>/Discrete-Time Integrator'
  real_T UD_DSTATE[3];                 // '<S5>/UD'
  real_T UD_DSTATE_m;                  // '<S7>/UD'
} DW;

// Invariant block signals (auto storage)
typedef const struct tag_ConstB {
  real_T VectorConcatenate[9];         // '<S10>/Vector Concatenate'
} ConstB;

// Constant parameters (auto storage)
typedef struct {
  // Expression: [1 0 0;0 1 0;0 0 1]
  //  Referenced by: '<S2>/Identity matrix'

  real_T Identitymatrix_Value[9];
} ConstP;

// External inputs (root inport signals with auto storage)
typedef struct {
  real_T angularvelocity[3];           // '<Root>/angularvelocity'
  real_T Bfield_body[3];               // '<Root>/Bfield_body'
} ExtU;

// External outputs (root outports fed by signals with auto storage)
typedef struct {
  real_T detuble[3];                   // '<Root>/detuble'
  real_T point[3];                     // '<Root>/point'
} ExtY;

// Real-time Model Data Structure
struct tag_RTM {
  const char_T * volatile errorStatus;
};

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

extern const ConstB rtConstB;          // constant block i/o

// Constant parameters (auto storage)
extern const ConstP rtConstP;

// Class declaration for model StarshotACS0
class StarshotACS0ModelClass {
  // public data and function members
 public:
  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  StarshotACS0ModelClass();

  // Destructor
  ~StarshotACS0ModelClass();

  // Real-Time Model get method
  RT_MODEL * getRTM();

  // private data and function members
 private:
  // Block signals and states
  DW rtDW;

  // Real-Time Model
  RT_MODEL rtM;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S5>/Data Type Duplicate' : Unused code path elimination
//  Block '<S7>/Data Type Duplicate' : Unused code path elimination
//  Block '<S2>/Gain 1' : Eliminated nontunable gain of 1


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Note that this particular code originates from a subsystem build,
//  and has its own system numbers different from the parent model.
//  Refer to the system hierarchy for this subsystem below, and use the
//  MATLAB hilite_system command to trace the generated code back
//  to the parent model.  For example,
//
//  hilite_system('StarshotAC_DC2d8CLEANED_kane/StarshotACS')    - opens subsystem StarshotAC_DC2d8CLEANED_kane/StarshotACS
//  hilite_system('StarshotAC_DC2d8CLEANED_kane/StarshotACS/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'StarshotAC_DC2d8CLEANED_kane'
//  '<S1>'   : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS'
//  '<S2>'   : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/KD_controller'
//  '<S3>'   : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/Orientation controller'
//  '<S4>'   : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/KD_controller/Cross'
//  '<S5>'   : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/KD_controller/Discrete Derivative'
//  '<S6>'   : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/KD_controller/Skew matrix S(m)1'
//  '<S7>'   : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/Orientation controller/Discrete Derivative'
//  '<S8>'   : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/Orientation controller/Norm2'
//  '<S9>'   : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/Orientation controller/Norm3'
//  '<S10>'  : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/Orientation controller/Skew matrix S(tau_c)1'
//  '<S11>'  : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/Orientation controller/Subsystem1'
//  '<S12>'  : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/Orientation controller/sign_function1'
//  '<S13>'  : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/Orientation controller/Subsystem1/Norm4'
//  '<S14>'  : 'StarshotAC_DC2d8CLEANED_kane/StarshotACS/Orientation controller/Subsystem1/Norm5'

#endif                                 // RTW_HEADER_StarshotACS0_h_

//
// File trailer for generated code.
//
// [EOF]
//
