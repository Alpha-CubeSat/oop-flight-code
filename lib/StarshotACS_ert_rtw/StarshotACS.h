//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: StarshotACS.h
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
#ifndef RTW_HEADER_StarshotACS_h_
#define RTW_HEADER_StarshotACS_h_
#include "rtwtypes.h"
#include <cmath>

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Forward declaration for rtModel
typedef struct tag_RTM RT_MODEL;

// Block signals and states (default storage) for system '<Root>'
struct DW {
  real_T DiscreteTimeIntegrator_DSTATE[3];// '<S2>/Discrete-Time Integrator'
  real_T UD_DSTATE[3];                 // '<S6>/UD'
  real_T UD_DSTATE_k;                  // '<S8>/UD'
};

// Invariant block signals (default storage)
struct ConstB {
  real_T VectorConcatenate[9];         // '<S11>/Vector Concatenate'
};

// Constant parameters (default storage)
struct ConstP {
  // Expression: [1 0 0;0 1 0;0 0 1]
  //  Referenced by: '<S2>/Identity matrix'

  real_T Identitymatrix_Value[9];

  // Pooled Parameter (Mixed Expressions)
  //  Referenced by:
  //    '<S2>/Discrete-Time Integrator'
  //    '<S3>/e_z'
  //    '<S4>/Constant'

  real_T pooled1[3];
};

// External inputs (root inport signals with default storage)
struct ExtU {
  real_T w[3];                         // '<Root>/angularvelocity'
  real_T Bfield_body[3];               // '<Root>/Bfield_body'
};

// External outputs (root outports fed by signals with default storage)
struct ExtY {
  real_T detumble[3];                  // '<Root>/detumble'
  real_T point[3];                     // '<Root>/point'
  real_T pt_error;                     // '<Root>/pt_error'
};

// Real-time Model Data Structure
struct tag_RTM {
  const char_T * volatile errorStatus;
};

extern const ConstB rtConstB;          // constant block i/o

// Constant parameters (default storage)
extern const ConstP rtConstP;

//
//  Exported Global Parameters
//
//  Note: Exported global parameters are tunable parameters with an exported
//  global storage class designation.  Code generation will declare the memory for
//  these parameters and exports their symbols.
//

extern real_T A;                       // Variable: A
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

extern real_T Id;                      // Variable: Id
                                          //  Referenced by: '<S2>/Id inverse'
                                          //  Id for the Detumble

extern real_T Kd;                      // Variable: Kd
                                          //  Referenced by: '<S3>/Kd Gain'
                                          //  Kd for the pointing

extern real_T Kp;                      // Variable: Kp
                                          //  Referenced by: '<S3>/Kp Gain'
                                          //  Kp for the pointing

extern real_T c;                       // Variable: c
                                          //  Referenced by:
                                          //    '<S2>/Gain 8'
                                          //    '<S2>/Kane damping'
                                          //  c for the Detumble

extern real_T i_max;                   // Variable: i_max
                                          //  Referenced by:
                                          //    '<S2>/Saturation1'
                                          //    '<S2>/Saturation2'
                                          //    '<S2>/Saturation6'
                                          //    '<S3>/Saturation1'
                                          //    '<S3>/Saturation2'
                                          //    '<S3>/Saturation6'
                                          //  Max Current for each magnetorquer

extern real_T k;                       // Variable: k
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

extern real_T n;                       // Variable: n
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


// Class declaration for model StarshotACS
class StarshotACS
{
  // public data and function members
 public:
  // Real-Time Model get method
  RT_MODEL * getRTM();

  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // model initialize function
//   void initialize();
  void initialize(double step_size_input, double A_input, double Id_input, double Kd_input, double Kp_input, double c_input, double i_max_input, double k_input, double n_input);

      // model step function
  void step();

  // Constructor
  StarshotACS();

  // Destructor
  ~StarshotACS();

  // private data and function members
 private:
  // Block states
  DW rtDW;

  // Real-Time Model
  RT_MODEL rtM;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S6>/Data Type Duplicate' : Unused code path elimination
//  Block '<S8>/Data Type Duplicate' : Unused code path elimination
//  Block '<S3>/Gain1' : Unused code path elimination
//  Block '<S3>/Scope' : Unused code path elimination
//  Block '<S3>/Scope1' : Unused code path elimination
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
//  hilite_system('starshotsim_dev/StarshotACS')    - opens subsystem starshotsim_dev/StarshotACS
//  hilite_system('starshotsim_dev/StarshotACS/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'starshotsim_dev'
//  '<S1>'   : 'starshotsim_dev/StarshotACS'
//  '<S2>'   : 'starshotsim_dev/StarshotACS/KD_controller'
//  '<S3>'   : 'starshotsim_dev/StarshotACS/Orientation controller'
//  '<S4>'   : 'starshotsim_dev/StarshotACS/Point Error'
//  '<S5>'   : 'starshotsim_dev/StarshotACS/KD_controller/Cross'
//  '<S6>'   : 'starshotsim_dev/StarshotACS/KD_controller/Discrete Derivative'
//  '<S7>'   : 'starshotsim_dev/StarshotACS/KD_controller/Skew matrix S(m)1'
//  '<S8>'   : 'starshotsim_dev/StarshotACS/Orientation controller/Discrete Derivative'
//  '<S9>'   : 'starshotsim_dev/StarshotACS/Orientation controller/Norm2'
//  '<S10>'  : 'starshotsim_dev/StarshotACS/Orientation controller/Norm3'
//  '<S11>'  : 'starshotsim_dev/StarshotACS/Orientation controller/Skew matrix S(tau_c)1'
//  '<S12>'  : 'starshotsim_dev/StarshotACS/Orientation controller/Subsystem1'
//  '<S13>'  : 'starshotsim_dev/StarshotACS/Orientation controller/sign_function1'
//  '<S14>'  : 'starshotsim_dev/StarshotACS/Orientation controller/Subsystem1/Norm4'
//  '<S15>'  : 'starshotsim_dev/StarshotACS/Orientation controller/Subsystem1/Norm5'

#endif                                 // RTW_HEADER_StarshotACS_h_

//
// File trailer for generated code.
//
// [EOF]
//
