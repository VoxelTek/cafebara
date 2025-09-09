#pragma once

#include <stdint.h>

typedef uint16_t bq25895_iin_max_t;

typedef uint16_t bq25895_batt_volt_t;

typedef uint16_t bq25895_vin_max_t;

typedef uint16_t bq25895_vsys_min_t;

typedef enum {
    BQ_CHG_CONFIG_DISABLE   = 0b00U,
    BQ_CHG_CONFIG_ENABLE    = 0b01U,
    BQ_CHG_CONFIG_OTG       = 0b10U,
} bq25895_chg_config_t;

typedef uint16_t bq25895_chg_current_t;

typedef uint16_t bq25895_term_current_t;

typedef uint16_t bq25895_prechg_current_t;

typedef enum {
    BQ_VRECHG_100MV = 0U,
    BQ_VRECHG_200MV = 1U,
} bq25895_vrechg_offset_t;

typedef enum {
    BQ_VBATLOW_2800MV = 0U,
    BQ_VBATLOW_3000MV = 1U,
} bq25895_vbatlow_t;

typedef uint16_t bq25895_vchg_max_t;

typedef enum {
    BQ_CHG_TIMER_5H     = 0b00U,
    BQ_CHG_TIMER_8H     = 0b01U,
    BQ_CHG_TIMER_12H    = 0b10U,
    BQ_CHG_TIMER_20H    = 0b11U,
} bq25895_chg_timer_t;

typedef enum {
    BQ_WATCHDOG_DISABLE     = 0b00U,
    BQ_WATCHDOG_40S         = 0b01U,
    BQ_WATCHDOG_80S         = 0b10U,
    BQ_WATCHDOG_160S        = 0b11U,
} bq25895_watchdog_conf_t;

typedef enum {
    BQ_MAX_TEMP_60C     = 0b00U,
    BQ_MAX_TEMP_80C     = 0b01U,
    BQ_MAX_TEMP_100C    = 0b10U,
    BQ_MAX_TEMP_120C    = 0b11U,
} bq_24292i_max_temp_t;

typedef uint8_t bq25895_clamp_voltage_t;

typedef uint8_t bq25895_comp_resistor_t;

typedef enum {
    BQ_INT_NONE                 = 0b00U,
    BQ_INT_BAT_FAULT            = 0b01U,
    BQ_INT_CHRG_FAULT           = 0b10U,
    BQ_INT_BAT_AND_CHRG_FAULT   = 0b11U,
} bq25895_interrupt_mask_t;

typedef enum {
    BQ_STATE_NOT_CHARGING   = 0b00U,
    BQ_STATE_PRECHARGE      = 0b01U,
    BQ_STATE_FAST_CHARGE    = 0b10U,
    BQ_STATE_TERMINATED     = 0b11U,
} bq25895_charge_state_t;

typedef enum {
    BQ_SOURCE_NONE      = 0b000U,
    BQ_SOURCE_USB_SDP   = 0b001U,
    BQ_SOURCE_USB_CDP   = 0b010U,
    BQ_SOURCE_USB_DCP   = 0b011U,
    BQ_SOURCE_ADJ       = 0b100U,
    BQ_SOURCE_UNKNOWN   = 0b101U,
    BQ_SOURCE_NSTD      = 0b110U,
    BQ_SOURCE_OTG       = 0b111U,
} bq25895_source_type_t;

enum {
    BQ_FAULT_NONE       = 0x00U,
    BQ_FAULT_THERM      = 0x07U,
    BQ_FAULT_BAT        = 0x08U,
    BQ_FAULT_CHG        = 0x30U,
    BQ_FAULT_BOOST      = 0x40U,
    BQ_FAULT_WATCHDOG   = 0x80U,
};
typedef uint8_t bq25895_fault_t;
