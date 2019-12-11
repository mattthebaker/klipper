#ifndef __LDC1612_H
#define __LDC1612_H

#include <stdint.h>

#define LDC1612_I2C_ADDR                    0x2A

#define LDC1612_MFG_ID                      0x5449
#define LDC1612_DEV_ID                      0x3055

#define LDC1612_REG_ADDR_DATA0_MSB          0x0
#define LDC1612_REG_ADDR_DATA0_LSB          0x1
#define LDC1612_REG_ADDR_DATA1_MSB          0x2
#define LDC1612_REG_ADDR_DATA1_LSB          0x3
#define LDC1612_REG_ADDR_DATA2_MSB          0x4
#define LDC1612_REG_ADDR_DATA2_LSB          0x5
#define LDC1612_REG_ADDR_DATA3_MSB          0x6
#define LDC1612_REG_ADDR_DATA4_LSB          0x7

#define LDC1612_REG_ADDR_RCOUNT0            0x8
#define LDC1612_REG_ADDR_RCOUNT1            0x9
#define LDC1612_REG_ADDR_RCOUNT2            0xa
#define LDC1612_REG_ADDR_RCOUNT3            0xb

#define LDC1612_REG_ADDR_OFFSET0            0xc
#define LDC1612_REG_ADDR_OFFSET1            0xd
#define LDC1612_REG_ADDR_OFFSET2            0xe
#define LDC1612_REG_ADDR_OFFSET3            0xf

#define LDC1612_REG_ADDR_SETTLECOUNT0       0x10
#define LDC1612_REG_ADDR_SETTLECOUNT1       0x11
#define LDC1612_REG_ADDR_SETTLECOUNT2       0x12
#define LDC1612_REG_ADDR_SETTLECOUNT3       0x13

#define LDC1612_REG_ADDR_CLOCK_DIVIDERS0    0x14
#define LDC1612_REG_ADDR_CLOCK_DIVIDERS1    0x15
#define LDC1612_REG_ADDR_CLOCK_DIVIDERS2    0x16
#define LDC1612_REG_ADDR_CLOCK_DIVIDERS3    0x17

#define LDC1612_REG_ADDR_STATUS             0x18
#define LDC1612_REG_ADDR_ERROR_CONFIG       0x19
#define LDC1612_REG_ADDR_CONFIG             0x1a
#define LDC1612_REG_ADDR_MUX_CONFIG         0x1b
#define LDC1612_REG_ADDR_RESET_DEV          0x1c

#define LDC1612_REG_ADDR_DRIVE_CURRENT0     0x1e
#define LDC1612_REG_ADDR_DRIVE_CURRENT1     0x1f
#define LDC1612_REG_ADDR_DRIVE_CURRENT2     0x20
#define LDC1612_REG_ADDR_DRIVE_CURRENT3     0x21

#define LDC1612_REG_ADDR_MANUFACTURER_ID    0x7e
#define LDC1612_REG_ADDR_DEVICE_ID          0x7f

typedef struct {
    uint32_t    data    :28;
    uint32_t    err_ae  :1;
    uint32_t    err_wd  :1;
    uint32_t    err_or  :1;
    uint32_t    err_ur  :1;
} ldc1612_data_t;

typedef struct {
    uint16_t    fref_divider:10;
    uint16_t    :2;
    uint16_t    fin_divider:4;
} ldc1612_clock_dividers_t;


typedef struct {
    uint16_t    :6;
    uint16_t    init_idrive:5;
    uint16_t    idrive:5;
} ldc1612_drive_current_t;

typedef struct {
    uint16_t            rcount;
    uint16_t            offset;
    uint16_t            settlecount;
    union {
        uint16_t        clock_dividers;
        struct {
            uint16_t    fref_divider:10;
            uint16_t    :2;
            uint16_t    fin_divider:4;
        };
    };
    union {
        uint16_t        drive_current;
        struct {
            uint16_t    :6;
            uint16_t    init_idrive:5;
            uint16_t    idrive:5;
        };
    };
} ldc1612_channel_config_t;

typedef struct {
    uint16_t    unreadconv3:1;
    uint16_t    unreadconv2:1;
    uint16_t    unreadconv1:1;
    uint16_t    unreadconv0:1;
    uint16_t    :2;
    uint16_t    drdy:1;
    uint16_t    :1;
    uint16_t    err_zc:1;
    uint16_t    err_ale:1;
    uint16_t    err_ahe:1;
    uint16_t    err_wd:1;
    uint16_t    err_or:1;
    uint16_t    err_ur:1;
    uint16_t    err_chan:2;
} ldc1612_status_t;

typedef struct {
    uint16_t    drdy_2int:1;
    uint16_t    :1;
    uint16_t    zc_err2int:1;
    uint16_t    al_err2int:1;
    uint16_t    ah_err2int:1;
    uint16_t    wd_err2int:1;
    uint16_t    or_err2int:1;
    uint16_t    ur_err2int:1;
    uint16_t    :3;
    uint16_t    al_err2out:1;
    uint16_t    ah_err2out:1;
    uint16_t    wd_err2out:1;
    uint16_t    or_err2out:1;
    uint16_t    ur_err2out:1;
} ldc1612_error_config_t;

typedef struct {
    uint16_t    reserved:6;
    uint16_t    high_current_drv:1;
    uint16_t    intb_dis:1;
    uint16_t    :1;
    uint16_t    ref_clk_src:1;
    uint16_t    auto_amp_dis:1;
    uint16_t    sensor_activate_sel:1;
    uint16_t    rp_override_en:1;
    uint16_t    sleep_mode_en:1;
    uint16_t    active_chan:2;
} ldc1612_config_t;

typedef struct {
    uint16_t    deglitch:3;
    uint16_t    reserved:10;
    uint16_t    rr_sequence:2;
    uint16_t    autoscan_en:1;
} ldc1612_mux_config_t;

typedef struct {
    uint16_t    :15;
    uint16_t    reset_dev:1;
} ldc1612_reset_dev_t;

void ldc1612_init(void);

#endif

