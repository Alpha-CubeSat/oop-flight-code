//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: StarshotACS.h
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
#ifndef RTW_HEADER_StarshotACS_h_
#define RTW_HEADER_StarshotACS_h_
#include "rtwtypes.h"
#include <cmath>

// Model Code Variants

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm) ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val) ((rtm)->errorStatus = (val))
#endif

// Forward declaration for rtModel
typedef struct tag_RTM RT_MODEL;

// Block signals and states (default storage) for system '<Root>'
struct DW
{
    real_T DiscreteTimeIntegrator_DSTATE[3]; // '<S2>/Discrete-Time Integrator'
    real_T UD_DSTATE[3];                     // '<S5>/UD'
    real_T UD_DSTATE_k;                      // '<S7>/UD'
};

// Invariant block signals (default storage)
struct ConstB
{
    real_T VectorConcatenate[9]; // '<S10>/Vector Concatenate'
};

// Constant parameters (default storage)
struct ConstPacs
{
    // Expression: [1 0 0;0 1 0;0 0 1]
    //  Referenced by: '<S2>/Identity matrix'

    real_T Identitymatrix_Value[9];
};

// External inputs (root inport signals with default storage)
struct ExtU
{
    real_T w[3];             // '<Root>/angularvelocity'
    real_T magneticfield[3]; // '<Root>/Bfield_body'
};

// External outputs (root outports fed by signals with default storage)
struct ExtY
{
    real_T detumble[3]; // '<Root>/detumble'
    real_T point[3];    // '<Root>/point'
};

// Real-time Model Data Structure
struct tag_RTM
{
    const char_T *volatile errorStatus;
};

extern const ConstB rtConstB; // constant block i/o

// Constant parameters (default storage)
extern const ConstPacs rtConstPacs;

// Class declaration for model StarshotACS
class StarshotACSModelClass
{
    // public data and function members
public:
    real_T maximum_current;
    real_T magtorqhwvalue;
    real_T m_max;
    real_T damperc;
    real_T Id;
    real_T invId;
    real_T pointing_error;
    real_T time_step;
    // Real-Time Model get method
    RT_MODEL *getRTM();

    // External inputs
    ExtU rtU;

    // External outputs
    ExtY rtY;

    // model initialize function
    void initialize(float t_stp, float kane_damper_c, float kane_Id, float ampFactor, float max_current, float csareamt, float no_loops, float wdx, float wdy, float wdz);

    // model step function
    void step();

    // Constructor
    StarshotACSModelClass();

    // Destructor
    ~StarshotACSModelClass();

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
//  Block '<S5>/Data Type Duplicate' : Unused code path elimination
//  Block '<S2>/Scope' : Unused code path elimination
//  Block '<S2>/To Workspace' : Unused code path elimination
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
//  hilite_system('starshotsimv5/StarshotACS')    - opens subsystem starshotsimv5/StarshotACS
//  hilite_system('starshotsimv5/StarshotACS/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'starshotsimv5'
//  '<S1>'   : 'starshotsimv5/StarshotACS'
//  '<S2>'   : 'starshotsimv5/StarshotACS/KD_controller'
//  '<S3>'   : 'starshotsimv5/StarshotACS/Orientation controller'
//  '<S4>'   : 'starshotsimv5/StarshotACS/KD_controller/Cross'
//  '<S5>'   : 'starshotsimv5/StarshotACS/KD_controller/Discrete Derivative'
//  '<S6>'   : 'starshotsimv5/StarshotACS/KD_controller/Skew matrix S(m)1'
//  '<S7>'   : 'starshotsimv5/StarshotACS/Orientation controller/Discrete Derivative'
//  '<S8>'   : 'starshotsimv5/StarshotACS/Orientation controller/Norm2'
//  '<S9>'   : 'starshotsimv5/StarshotACS/Orientation controller/Norm3'
//  '<S10>'  : 'starshotsimv5/StarshotACS/Orientation controller/Skew matrix S(tau_c)1'
//  '<S11>'  : 'starshotsimv5/StarshotACS/Orientation controller/Subsystem1'
//  '<S12>'  : 'starshotsimv5/StarshotACS/Orientation controller/sign_function1'
//  '<S13>'  : 'starshotsimv5/StarshotACS/Orientation controller/Subsystem1/Norm4'
//  '<S14>'  : 'starshotsimv5/StarshotACS/Orientation controller/Subsystem1/Norm5'

#endif // RTW_HEADER_StarshotACS_h_

//
// File trailer for generated code.
//
// [EOF]
//
