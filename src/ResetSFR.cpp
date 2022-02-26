#include "ResetSFR.hpp"

void ResetSFR::capture_default_sfr()
{
  // call this at the BEGINNING of the test suite, before sfr values change
  photoresistor_covered = sfr::photoresistor::covered;

  mission_mode = sfr::mission::mode;
  mission_low_power_eligible = sfr::mission::low_power_eligible;

  burnwire_fire = sfr::burnwire::fire;
  burnwire_arm = sfr::burnwire::arm;
  burnwire_mode = sfr::burnwire::mode;
  burnwire_attempts = sfr::burnwire::attempts;
  burnwire_start_time = sfr::burnwire::start_time;
  burnwire_burn_time = sfr::burnwire::burn_time;
  burnwire_armed_time = sfr::burnwire::armed_time;
}

void ResetSFR::restore_default_sfr()
{
    // call this at the END of the test suite, after sfr values change
    sfr::photoresistor::covered = photoresistor_covered;

    sfr::mission::mode = mission_mode;
    sfr::mission::low_power_eligible = mission_low_power_eligible;

    sfr::burnwire::fire = burnwire_fire;
    sfr::burnwire::arm = burnwire_arm;
    sfr::burnwire::mode = burnwire_mode;
    sfr::burnwire::attempts = burnwire_attempts;
    sfr::burnwire::start_time = burnwire_start_time;
    sfr::burnwire::armed_time = burnwire_armed_time;

    // Use setPinState to reset pinMap

}