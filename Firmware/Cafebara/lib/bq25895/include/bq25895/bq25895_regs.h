#pragma once

// BQ25895 i2c address
#define BQ_ADDR 0x6AU

// BQ25895 registers
#define BQ_REG00   0x00U    // HI-Z, ILIM, max input current
#define BQ_REG01   0x01U    // Boost temp, input voltage offset
#define BQ_REG02   0x02U    // Misc control options
#define BQ_REG03   0x03U    // More misc control, min sys voltage
#define BQ_REG04   0x04U    // Max charge current
#define BQ_REG05   0x05U    // Precharge and termination current limits
#define BQ_REG06   0x06U    // Max charge voltage, BATLOWV, VRECHG
#define BQ_REG07   0x07U    // Misc charge options
#define BQ_REG08   0x08U    // IR compensation, thermal regulation
#define BQ_REG09   0x09U    // Misc control options, shipping mode
#define BQ_REG0A   0x0AU    // Boost voltage
#define BQ_REG0B   0x0BU    // Power status registers
#define BQ_REG0C   0x0CU    // Error status registers
#define BQ_REG0D   0x0DU    // VINDPM settings
#define BQ_REG0E   0x0EU    // ADC battery voltage
#define BQ_REG0F   0x0FU    // ADC system voltage
#define BQ_REG10   0x10U    // ADC TS voltage
#define BQ_REG11   0x11U    // ADC VBUS voltage
#define BQ_REG12   0x12U    // ADC charge current
#define BQ_REG13   0x13U    // Misc status registers
#define BQ_REG14   0x14U    // Chip info
/*
#define BQ_INPUT_SRC_CTRL 0x00U            // R/W: Input source control
#define BQ_PWR_ON_CONF 0x01U               // R/W: Power-on configuration
#define BQ_CHRG_CURRENT_CTRL 0x02U         // R/W: Charge current control
#define BQ_PRECHRG_TERM_CURRENT_CTRL 0x03U // R/W: Pre-charge/termination current control
#define BQ_CHRG_VOLTAGE_CTRL 0x04U         // R/W: Charge voltage control
#define BQ_CHRG_TERM_TIMER_CTRL 0x05U      // R/W: Charge termination/timer control
#define BQ_IR_COMP_THERMAL_REG_CTRL 0x06U  // R/W: IR compensation / thermal regulation control
#define BQ_MISC_OPERATION_CTRL 0x07U       // R/W: Misc operation control
#define BQ_SYSTEM_STATUS 0x08U             // R/-: System status
#define BQ_FAULT 0x09U                     // R/-: Fault (cleared after read)
#define BQ_VENDOR_PART_REV_STATUS 0x0AU    // R/-: Vendor / part / revision status
*/

// BQ25895 masks/constants
#define BQ_EN_ILIM_POS 6U
#define BQ_EN_ILIM_MSK (0x01U << BQ_EN_ILIM_POS) // DONE

#define BQ_IIN_MAX_POS 0U   // DONE
#define BQ_IIN_MAX_MSK (0x3FU << BQ_IIN_MAX_POS)
#define BQ_IIN_MAX_OFFSET 100U
#define BQ_IIN_MAX_INCR 50U

#define BQ_VIN_FORCE_POS 7U // DONE?
#define BQ_VIN_FORCE_MSK (0x01U << BQ_VIN_FORCE_POS)

#define BQ_VIN_MAX_POS 0U // DONE?
#define BQ_VIN_MAX_MSK (0x7FU << BQ_VIN_MAX_POS)
#define BQ_VIN_MAX_OFFSET 2600U
#define BQ_VIN_MAX_INCR 100U

#define BQ_ADC_START_POS 7U
#define BQ_ADC_START_MSK (0x01U << BQ_ADC_START_POS)
#define BQ_ADC_RATE_POS 6U
#define BQ_ADC_RATE_MSK (0x01U << BQ_ADC_RATE_POS)

#define BQ_BST_FRQ_POS 5U
#define BQ_BST_FRQ_MSK (0x01U << BQ_BST_FRQ_POS)

#define BQ_DPDM_FORCE_POS 1U
#define BQ_DPDM_FORCE_MSK (0x01U << BQ_DPDM_FORCE_POS)
#define BQ_DPDM_EN_POS 0U
#define BQ_DPDM_EN_MSK (0x01U << BQ_DPDM_EN_POS)

#define BQ_BAT_LOAD_POS 7U
#define BQ_BAT_LOAD_MSK (0x01 << BQ_BAT_LOAD_POS)

#define BQ_VSYS_MIN_POS 1U
#define BQ_VSYS_MIN_MSK (0x07U << BQ_VSYS_MIN_POS)
#define BQ_VSYS_MIN_OFFSET 3000U
#define BQ_VSYS_MIN_INCR 100U

#define BQ_CHG_CONFIG_POS 4U
#define BQ_CHG_CONFIG_MSK (0x03U << BQ_CHG_CONFIG_POS)

#define BQ_WDT_POS 6U
#define BQ_WDT_MSK (0x01U << BQ_WDT_POS)

#define BQ_ICHG_POS 0U
#define BQ_ICHG_MSK (0x7FU << BQ_ICHG_POS)
#define BQ_ICHG_OFFSET 0U
#define BQ_ICHG_INCR 64U

#define BQ_ITERM_POS 0U
#define BQ_ITERM_MSK (0x0FU << BQ_ITERM_POS)
#define BQ_ITERM_OFFSET 64U
#define BQ_ITERM_INCR 64U

#define BQ_IPRECHG_POS 4U
#define BQ_IPRECHG_MSK (0x0FU << BQ_IPRECHG_POS)
#define BQ_IPRECHG_OFFSET 64U
#define BQ_IPRECHG_INCR 64U

#define BQ_VRECHG_POS 0U
#define BQ_VRECHG_MSK (0x03U << BQ_VRECHG_POS)

#define BQ_VBATLOW_POS 1U
#define BQ_VBATLOW_MSK (0x01 << BQ_VBATLOW_POS)

#define BQ_VCHG_MAX_POS 2U
#define BQ_VCHG_MAX_MSK (0x3FU << BQ_VCHG_MAX_POS)
#define BQ_VCHG_MAX_OFFSET 3840U
#define BQ_VCHG_MAX_INCR 16U

#define BQ_TERM_EN_POS 7U
#define BQ_TERM_EN_MSK (0x01U << BQ_TERM_EN_POS)

#define BQ_CHG_TIMER_POS 1U
#define BQ_CHG_TIMER_MSK (0x03U << BQ_CHG_TIMER_POS)
#define BQ_CHG_TIMER_EN_POS 3U
#define BQ_CHG_TIMER_EN_MSK (0x01U << BQ_CHG_TIMER_EN_POS)

#define BQ_WDT_CONF_POS 4U
#define BQ_WDT_CONF_MSK (0x03U << BQ_WDT_CONF_POS)

#define BQ_THERMAL_REG_POS 0U
#define BQ_THERMAL_REG_MSK (0x03U << BQ_THERMAL_REG_POS)

#define BQ_VCLAMP_POS 2U
#define BQ_VCLAMP_MSK (0x07U << BQ_VCLAMP_POS)
#define BQ_VCLAMP_OFFSET 0U
#define BQ_VCLAMP_INCR 32U

#define BQ_BAT_COMP_POS 5U
#define BQ_BAT_COMP_MSK (0x07U << BQ_BAT_COMP_POS)
#define BQ_BAT_COMP_OFFSET 0U
#define BQ_BAT_COMP_INCR 20U

#define BQ_INT_MASK_POS 0U
#define BQ_INT_MASK_MSK (0x03U << BQ_INT_MASK_POS)

#define BQ_BATFET_POS 5U
#define BQ_BATFET_MSK (0x01U << BQ_BATFET_POS)

#define BQ_VSYS_STAT_POS 0U
#define BQ_VSYS_STAT_MSK (0x01U << BQ_VSYS_STAT_POS)

#define BQ_THERM_STAT_POS 7U
#define BQ_THERM_STAT_MSK (0x01U << BQ_THERM_STAT_POS)

#define BQ_PG_STAT_POS 2U
#define BQ_PG_STAT_MSK (0x01U << BQ_PG_STAT_POS)

#define BQ_DPM_STAT_POS 1U
#define BQ_DPM_STAT_MSK (0x01U << BQ_DPM_STAT_POS)

#define BQ_CHRG_STAT_POS 3U
#define BQ_CHRG_STAT_MSK (0x03U << BQ_CHRG_STAT_POS)

#define BQ_VBUS_STAT_POS 5U
#define BQ_VBUS_STAT_MSK (0x07U << BQ_VBUS_STAT_POS)

#define BQ_ADC_VAL_POS 0U
#define BQ_ADC_VAL_MSK (0x7FU << BQ_ADC_VAL_POS)
#define BQ_ADC_VAL_OFFSET 2304U
#define BQ_ADC_VAL_INCR 20U

#define BQ_PART_NUMBER_POS 3U
#define BQ_PART_NUMBER_MSK (0x07U << BQ_PART_NUMBER_POS)
#define BQ_PART_NUMBER 0b111U
