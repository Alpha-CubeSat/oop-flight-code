#include "RockblockCommand.hpp"
#include "sfr.hpp"

void CameraFragmentCommand::execute()
{
    sfr::camera::fragment_requested = true;
    sfr::camera::serial_requested = f_arg_1;
    sfr::camera::fragment_number_requested = f_arg_2;
}

bool CameraFragmentCommand::isValid()
{
    return f_arg_1 < sfr::camera::images_written;
}

void DeployCommand::execute()
{
    sfr::mission::current_mode = sfr::mission::transmitDeployment;
}

bool DeployCommand::isValid()
{
    return sfr::mission::current_mode->get_phase()->get_id() == sfr::mission::standby->get_id();
}

void ArmCommand::execute()
{
    sfr::mission::current_mode = sfr::mission::transmitArmed;
}

bool ArmCommand::isValid()
{
    return sfr::mission::current_mode->get_phase()->get_id() == sfr::mission::deployment->get_id();
}

void FireCommand::execute()
{
    sfr::mission::current_mode = sfr::mission::transmitInSun;
}

bool FireCommand::isValid()
{
    return sfr::mission::current_mode->get_phase()->get_id() == sfr::mission::armed->get_id();
}

void MissionModeOverrideCommand::execute()
{
    switch (f_arg_1) {
    case 0: {
        sfr::mission::current_mode = sfr::mission::boot;
        break;
    }
    case 1: {
        sfr::mission::current_mode = sfr::mission::aliveSignal;
        break;
    }
    case 2: {
        sfr::mission::current_mode = sfr::mission::detumbleSpin;
        break;
    }
    case 3: {
        sfr::mission::current_mode = sfr::mission::lowPowerDetumbleSpin;
        break;
    }
    case 4: {
        sfr::mission::current_mode = sfr::mission::normal;
        break;
    }
    case 5: {
        sfr::mission::current_mode = sfr::mission::transmit;
        break;
    }
    case 6: {
        sfr::mission::current_mode = sfr::mission::lowPower;
        break;
    }
    case 7: {
        sfr::mission::current_mode = sfr::mission::normalDeployment;
        break;
    }
    case 8: {
        sfr::mission::current_mode = sfr::mission::transmitDeployment;
        break;
    }
    case 9: {
        sfr::mission::current_mode = sfr::mission::lowPowerDeployment;
        break;
    }
    case 10: {
        sfr::mission::current_mode = sfr::mission::normalArmed;
        break;
    }
    case 11: {
        sfr::mission::current_mode = sfr::mission::transmitArmed;
        break;
    }
    case 12: {
        sfr::mission::current_mode = sfr::mission::lowPowerArmed;
        break;
    }
    case 13: {
        sfr::mission::current_mode = sfr::mission::normalInSun;
        break;
    }
    case 14: {
        sfr::mission::current_mode = sfr::mission::transmitInSun;
        break;
    }
    case 15: {
        sfr::mission::current_mode = sfr::mission::lowPowerInSun;
        break;
    }
    case 16: {
        sfr::mission::current_mode = sfr::mission::voltageFailureInSun;
        break;
    }
    case 17: {
        sfr::mission::current_mode = sfr::mission::bootSensors;
        break;
    }
    case 18: {
        sfr::mission::current_mode = sfr::mission::captureIMU;
        break;
    }
    case 19: {
        sfr::mission::current_mode = sfr::mission::mandatoryBurns;
        break;
    }
    case 20: {
        sfr::mission::current_mode = sfr::mission::regularBurns;
        break;
    }
    case 21: {
        sfr::mission::current_mode = sfr::mission::deploymentVerification;
        break;
    }
    }
}

bool MissionModeOverrideCommand::isValid()
{
    return (f_arg_1 >= sfr::mission::boot->get_id() && f_arg_1 <= sfr::mission::deploymentVerification->get_id());
}

void EEPROMResetCommand::execute()
{
    // Unpack args
    uint8_t boot_count = (constants::masks::uint32_byte1_mask & f_arg_1) >> 24;
    bool light_switch = !!((constants::masks::uint32_byte2_mask & f_arg_1) >> 16);

    uint16_t sfr_data_addr = (constants::masks::uint32_byte3_mask | constants::masks::uint32_byte4_mask) & f_arg_1;

    uint8_t sfr_data_age = (constants::masks::uint32_byte1_mask & f_arg_2) >> 24;
    uint8_t dynamic_data_age = (constants::masks::uint32_byte2_mask & f_arg_2) >> 16;

    uint16_t dynamic_data_addr = (constants::masks::uint32_byte3_mask | constants::masks::uint32_byte4_mask) & f_arg_2;

    sfr::eeprom::boot_counter = boot_count;
    sfr::eeprom::light_switch = light_switch;
    sfr::eeprom::dynamic_data_addr = dynamic_data_addr;
    sfr::eeprom::sfr_data_addr = sfr_data_addr;
    sfr::eeprom::dynamic_data_age = round(map(dynamic_data_age, 0, 255, 0, constants::eeprom::write_age_limit));
    sfr::eeprom::sfr_data_age = round(map(sfr_data_age, 0, 255, 0, constants::eeprom::write_age_limit));
    sfr::eeprom::error_mode = false;
    sfr::eeprom::boot_restarted = false;

    // Write to EEPROM
    EEPROM.put(constants::eeprom::boot_time_loc1, sfr::eeprom::boot_counter.get());
    EEPROM.put(constants::eeprom::boot_time_loc2, sfr::eeprom::boot_counter.get());

    EEPROM.put(constants::eeprom::light_switch_loc1, sfr::eeprom::light_switch.get());
    EEPROM.put(constants::eeprom::light_switch_loc2, sfr::eeprom::light_switch.get());

    EEPROM.put(constants::eeprom::dynamic_data_addr_loc1, sfr::eeprom::dynamic_data_addr.get());
    EEPROM.put(constants::eeprom::dynamic_data_addr_loc2, sfr::eeprom::dynamic_data_addr.get());

    EEPROM.put(constants::eeprom::sfr_data_addr_loc1, sfr::eeprom::sfr_data_addr.get());
    EEPROM.put(constants::eeprom::sfr_data_addr_loc2, sfr::eeprom::sfr_data_addr.get());

    // Write to the relevant SFRFields
}

bool EEPROMResetCommand::isValid()
{
    return sfr::eeprom::error_mode.get();
}