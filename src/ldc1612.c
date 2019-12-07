#include "ldc1612.h"

DECL_CONSTANT_STR("RESERVE_PINS_probe_safety", "PA0,PB8,PB2");
DECL_CONSTANT_STR("RESERVE_PINS_probe_use", "PB4,PA10,PA9,PB1,PB3,PA15");

typedef struct {
    struct i2c_config i2c;
} ldc1612_settings_t;

static ldc1612_settings_t m_settings;

static void
ldc1612_write(uint8_t reg_addr, uint16_t reg_data)
{
    // pack big endian register write
    uint8_t buffer[3] = {reg_addr, (reg_data >> 8) & 0xFF, reg_data & 0xFF};
    i2c_write(m_settings.i2c, sizeof(buffer), buffer);
}

static uint16_t
ldc1612_read(uint8_t reg_addr)
{
    // unpack big endian register read
    uint16_t buffer;
    i2c_read(m_settings.i2c, 1, &reg_addr, sizeof(buffer), buffer);
    buffer = ((buffer >> 8) & 0xFF) | ((buffer & 0xFF) << 8);
    return buffer;
}

static void
ldc1612_channel_config_write(uint8_t ch, ldc1612_channel_config_t *config_ch)
{
    ldc1612_write(LDC1612_REG_ADDR_RCOUNT0          + ch,
                    config_ch->rcount);

    ldc1612_write(LDC1612_REG_ADDR_OFFSET0          + ch,
                    config_ch->offset);

    ldc1612_write(LDC1612_REG_ADDR_SETTLECOUNT0     + ch,
                    config_ch->settlecount);

    ldc1612_write(LDC1612_REG_ADDR_CLOCK_DIVIDERS0  + ch,
                    config_ch->clock_dividers);

    ldc1612_write(LDC1612_REG_ADDR_DRIVE_CURRENT0   + ch,
                    config_ch->drive_current);
}

void
ldc1612_init(void)
{
    ldc1612_channel_config_t config_ch0 = {
        .rcount         = 0x04FF;
        .offset         = 0;
        .settlecount    = 0x10;
        .fin_divider    = 1;
        .fref_divider   = 1;
        .idrive         = 0;
        .init_idrive    = 0;
    };

    ldc1612_error_config_t config_error = {
        .drdy_2int      = 1;
    };

    ldc1612_config_t config = {
        .active_chan    = 0;
        .sleep_mode_en  = 0;
        .ref_clk_src    = 1;
        .reserved       = 1;
    };

    ldc1612_mux_config_t config_mux = {
        .deglitch       = 4;
        .reserved       = 0x41;
    };

    memset(&m_settings, 0, sizeof(m_settings));

    m_settings.i2c = i2c_setup(0, 0, LDC1612_I2C_ADDR);

    ldc1612_channel_config_write(0, &config_ch0);
    ldc1612_write(LDC1612_REG_ADDR_ERROR_CONFIG, (uint16_t)config_error);
    ldc1612_write(LDC1612_REG_ADDR_MUX_CONFIG, (uint16_t)config_mux);
    ldc1612_write(LDC1612_REG_ADDR_CONFIG, (uint16_t)config);

}
