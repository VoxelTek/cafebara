#pragma once

#include "bq25895/bq25895_defs.h"

#include <stdbool.h>
#include <stddef.h>

typedef bool (*bq25895_write_t)(
  uint16_t addr, uint8_t reg, void const* buf, size_t len, void* context);
typedef bool (*bq25895_read_t)(uint16_t addr, uint8_t reg, void* buf, size_t len, void* context);

typedef struct {
    bq25895_write_t write;
    bq25895_read_t read;
    void* context;
} bq25895_t;

bool bq25895_is_present(bq25895_t const* dev);

bool bq25895_set_iin_max(bq25895_t const* dev, bq25895_iin_max_t ma);
bool bq25895_get_iin_max(bq25895_t const* dev, bq25895_iin_max_t* ma);

bool bq25895_set_vin_max(bq25895_t const* dev, bq25895_vin_max_t mv);
bool bq25895_get_vin_max(bq25895_t const* dev, bq25895_vin_max_t* mv);

bool bq25895_set_vsys_min(bq25895_t const* dev, bq25895_vsys_min_t mv);
bool bq25895_get_vsys_min(bq25895_t const* dev, bq25895_vsys_min_t* mv);

bool bq25895_set_charge_config(bq25895_t const* dev, bq25895_chg_config_t conf);
bool bq25895_get_charge_config(bq25895_t const* dev, bq25895_chg_config_t* conf);

bool bq25895_reset_wdt(bq25895_t const* dev);

bool bq25895_set_charge_current(bq25895_t const* dev, bq25895_chg_current_t ma);
bool bq25895_get_charge_current(bq25895_t const* dev, bq25895_chg_current_t* ma);

bool bq25895_set_term_current(bq25895_t const* dev, bq25895_term_current_t ma);
bool bq25895_get_term_current(bq25895_t const* dev, bq25895_term_current_t* ma);

bool bq25895_set_precharge_current(bq25895_t const* dev, bq25895_prechg_current_t ma);
bool bq25895_get_precharge_current(bq25895_t const* dev, bq25895_prechg_current_t* ma);

bool bq25895_set_recharge_offset(bq25895_t const* dev, bq25895_vrechg_offset_t offset);
bool bq25895_get_recharge_offset(bq25895_t const* dev, bq25895_vrechg_offset_t* offset);

bool bq25895_set_batlow_voltage(bq25895_t const* dev, bq25895_vbatlow_t mv);
bool bq25895_get_batlow_voltage(bq25895_t const* dev, bq25895_vbatlow_t* mv);

bool bq25895_set_max_charge_voltage(bq25895_t const* dev, bq25895_vchg_max_t mv);
bool bq25895_get_max_charge_voltage(bq25895_t const* dev, bq25895_vchg_max_t* mv);

bool bq25895_set_charge_timer(bq25895_t const* dev, bq25895_chg_timer_t conf);
bool bq25895_get_charge_timer(bq25895_t const* dev, bq25895_chg_timer_t* conf);

bool bq25895_set_wdt_config(bq25895_t const* dev, bq25895_watchdog_conf_t conf);
bool bq25895_get_wdt_config(bq25895_t const* dev, bq25895_watchdog_conf_t* conf);

bool bq25895_set_charge_termination(bq25895_t const* dev, bool enable);
bool bq25895_get_charge_termination(bq25895_t const* dev, bool* enable);

bool bq25895_set_max_temp(bq25895_t const* dev, bq_24292i_max_temp_t temp);
bool bq25895_get_max_temp(bq25895_t const* dev, bq_24292i_max_temp_t* temp);

bool bq25895_set_voltage_clamp(bq25895_t const* dev, bq25895_clamp_voltage_t mv);
bool bq25895_get_voltage_clamp(bq25895_t const* dev, bq25895_clamp_voltage_t* mv);

bool bq25895_set_comp_resistor(bq25895_t const* dev, bq25895_comp_resistor_t mohms);
bool bq25895_get_comp_resistor(bq25895_t const* dev, bq25895_comp_resistor_t* mohms);

/*
bool bq25895_set_interrupt_mask(bq25895_t const* dev, bq25895_interrupt_mask_t mask);
bool bq25895_get_interrupt_mask(bq25895_t const* dev, bq25895_interrupt_mask_t* mask);
*/

bool bq25895_set_batfet_enabled(bq25895_t const* dev, bool enable);
bool bq25895_get_batfet_enabled(bq25895_t const* dev, bool* enable);

//bool bq25895_is_vsys_boosted(bq25895_t const* dev, bool* result);

bool bq25895_is_overtemp(bq25895_t const* dev, bool* result);

bool bq25895_is_charger_connected(bq25895_t const* dev, bool* result);

bool bq25895_is_in_dpm(bq25895_t const* dev, bool* result);

bool bq25895_get_charge_state(bq25895_t const* dev, bq25895_charge_state_t* state);

bool bq25895_get_source_type(bq25895_t const* dev, bq25895_source_type_t* source);

bool bq25895_check_faults(bq25895_t const* dev, bq25895_fault_t* faults);

bool bq25895_trigger_adc_read(bq25895_t const* dev);
bool bq25895_get_adc_batt(bq25895_t const* dev, bq25895_batt_volt_t* voltage);
bool bq25895_set_adc_cont(bq25895_t const* dev, bool enabled);
