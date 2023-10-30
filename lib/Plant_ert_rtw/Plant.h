//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Plant.h
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
#ifndef RTW_HEADER_Plant_h_
#define RTW_HEADER_Plant_h_
#include "rtwtypes.h"
#include <stddef.h>

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm) ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val) ((rtm)->errorStatus = (val))
#endif

// Forward declaration for rtModel
typedef struct tag_RTMplant RT_MODELplant;

// Block signals and states (default storage) for system '<Root>'
struct DWplant
{
  real_T DiscreteTimeIntegrator1_DSTAT_m[4]; // '<S6>/Discrete-Time Integrator1'
  real_T Product_DWORK4[9];                  // '<S11>/Product'
  real_T DiscreteTimeIntegrator_DSTATE;      // '<S1>/Discrete-Time Integrator'
  real_T DiscreteTimeIntegrator1_DSTATE;     // '<S1>/Discrete-Time Integrator1'
  real_T DiscreteTimeIntegrator2_DSTATE;     // '<S1>/Discrete-Time Integrator2'
  real_T DiscreteTimeIntegrator1_DSTAT_l;    // '<S7>/Discrete-Time Integrator1'
  real_T DiscreteTimeIntegrator2_DSTAT_e;    // '<S7>/Discrete-Time Integrator2'
  real_T DiscreteTimeIntegrator3_DSTATE;     // '<S7>/Discrete-Time Integrator3'
  real_T DiscreteTimeIntegrator4_DSTATE;     // '<S7>/Discrete-Time Integrator4'
  real_T DiscreteTimeIntegrator5_DSTATE;     // '<S7>/Discrete-Time Integrator5'
  real_T DiscreteTimeIntegrator6_DSTATE;     // '<S7>/Discrete-Time Integrator6'
};

// Constant parameters (default storage)
struct ConstPplant
{
  // Pooled Parameter (Expression: eye(3,3))
  //  Referenced by:
  //    '<S8>/Matrix Gain'
  //    '<S9>/Matrix Gain'

  real_T pooled6[9];
};

// External inputs (root inport signals with default storage)
struct ExtUplant
{
  real_T current[3]; // '<Root>/current'
};

// External outputs (root outports fed by signals with default storage)
struct ExtYplant
{
  real_T angularvelocity[3]; // '<Root>/angular velocity'
  real_T magneticfield[3];   // '<Root>/magnetic field'
  real_T xyzposition[3];     // '<Root>/xyzposition'
  real_T quaternion[4];      // '<Root>/quaternion'
  real_T pt_error;           // '<Root>/pt_error'
};

// Real-time Model Data Structure
struct tag_RTMplant
{
  const char_T *volatile errorStatus;
};

// Constant parameters (default storage)
extern const ConstPplant rtConstPplant;

//
//  Exported Global Parameters
//
//  Note: Exported global parameters are tunable parameters with an exported
//  global storage class designation.  Code generation will declare the memory for
//  these parameters and exports their symbols.
//

extern real_T Altitude; // Variable: Alt
                        //  Referenced by:
                        //    '<S7>/Discrete-Time Integrator1'
                        //    '<S7>/Discrete-Time Integrator3'
                        //    '<S7>/Discrete-Time Integrator4'
                        //  Cubesat Altitude (km)

extern real_T I[9]; // Variable: I
                    //  Referenced by:
                    //    '<S3>/I'
                    //    '<S3>/I^-1'

extern real_T inclination; // Variable: inclination
                           //  Referenced by:
                           //    '<S7>/Discrete-Time Integrator1'
                           //    '<S7>/Discrete-Time Integrator3'
                           //  ISS inclination

extern real_T m; // Variable: m
                 //  Referenced by: '<S7>/mass gain'
                 //  Cubesat Mass

extern real_T q0[4]; // Variable: q0
                     //  Referenced by: '<S6>/Discrete-Time Integrator1'
                     //  Init Quat (Default Euler Angle = pi/6*[1 2 3])

extern real_T wx; // Variable: wx
                  //  Referenced by: '<S1>/Discrete-Time Integrator'
                  //  init angular vel -x

extern real_T wy; // Variable: wy
                  //  Referenced by: '<S1>/Discrete-Time Integrator1'
                  //  init angular vel -y

extern real_T wz; // Variable: wz
                  //  Referenced by: '<S1>/Discrete-Time Integrator2'
                  //  init angular vel -z

extern "C"
{
  static real_T rtGetNaN(void);
  static real32_T rtGetNaNF(void);
} // extern "C"

#define NOT_USING_NONFINITE_LITERALS 1

extern "C"
{
  extern real_T rtInf;
  extern real_T rtMinusInf;
  extern real_T rtNaN;
  extern real32_T rtInfF;
  extern real32_T rtMinusInfF;
  extern real32_T rtNaNF;
  static void rt_InitInfAndNaN(size_t realSize);
  static boolean_T rtIsInf(real_T value);
  static boolean_T rtIsInfF(real32_T value);
  static boolean_T rtIsNaN(real_T value);
  static boolean_T rtIsNaNF(real32_T value);
  struct BigEndianIEEEDouble
  {
    struct
    {
      uint32_T wordH;
      uint32_T wordL;
    } words;
  };

  struct LittleEndianIEEEDouble
  {
    struct
    {
      uint32_T wordL;
      uint32_T wordH;
    } words;
  };

  struct IEEESingle
  {
    union
    {
      real32_T wordLreal;
      uint32_T wordLuint;
    } wordL;
  };
} // extern "C"

extern "C"
{
  static real_T rtGetInf(void);
  static real32_T rtGetInfF(void);
  static real_T rtGetMinusInf(void);
  static real32_T rtGetMinusInfF(void);
} // extern "C"

// Class declaration for model Plant
class Plant
{
  // public data and function members
public:
  // Real-Time Model get method
  RT_MODELplant *getRTM();

  // External inputs
  ExtUplant rtU;

  // External outputs
  ExtYplant rtY;

  // model initialize function
  void initialize();
  void initialize(double f_step_size_input, double Altitude_input, double I_input[9], double inclination_input, double m_input, double q0_input[4], double wx_input, double wy_input, double wz_input);
  // model step function
  void step();

  // Constructor
  Plant();

  // Destructor
  ~Plant();

  // private data and function members
private:
  // Block states
  DWplant rtDW;

  // private member function(s) for subsystem '<S1>/qtoQ'
  static void qtoQ(const real_T rtu_q[4], real_T rty_Q[9]);

  // Real-Time Model
  RT_MODELplant rtM;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S1>/Scope' : Unused code path elimination
//  Block '<S12>/Reshape' : Reshape block reduction

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
//  hilite_system('starshotsim_dev/Plant')    - opens subsystem starshotsim_dev/Plant
//  hilite_system('starshotsim_dev/Plant/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'starshotsim_dev'
//  '<S1>'   : 'starshotsim_dev/Plant'
//  '<S2>'   : 'starshotsim_dev/Plant/Cross Product'
//  '<S3>'   : 'starshotsim_dev/Plant/Dynamics'
//  '<S4>'   : 'starshotsim_dev/Plant/Magnetic Field Model'
//  '<S5>'   : 'starshotsim_dev/Plant/Point Error'
//  '<S6>'   : 'starshotsim_dev/Plant/Quaternion Integration'
//  '<S7>'   : 'starshotsim_dev/Plant/Tranlational Dynamics'
//  '<S8>'   : 'starshotsim_dev/Plant/qtoQ'
//  '<S9>'   : 'starshotsim_dev/Plant/qtoQ1'
//  '<S10>'  : 'starshotsim_dev/Plant/Dynamics/Cross Product'
//  '<S11>'  : 'starshotsim_dev/Plant/Dynamics/Invert  3x3 Matrix'
//  '<S12>'  : 'starshotsim_dev/Plant/Dynamics/Invert  3x3 Matrix/Determinant of 3x3 Matrix'
//  '<S13>'  : 'starshotsim_dev/Plant/Magnetic Field Model/Dipole->ECI'
//  '<S14>'  : 'starshotsim_dev/Plant/Quaternion Integration/q_normalize'
//  '<S15>'  : 'starshotsim_dev/Plant/Quaternion Integration/qderiv'
//  '<S16>'  : 'starshotsim_dev/Plant/qtoQ/Subsystem'
//  '<S17>'  : 'starshotsim_dev/Plant/qtoQ1/Subsystem'

#endif // RTW_HEADER_Plant_h_

//
// File trailer for generated code.
//
// [EOF]
//
