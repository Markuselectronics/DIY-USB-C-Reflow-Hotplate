/*
 * USB_PD_Handler.h
 * Simple Driver for STUSB4500
 */

#ifndef USB_PD_HANDLER_H
#define USB_PD_HANDLER_H

#include "main.h"

// I2C Address (7-bit 0x28 shifted left)
#define STUSB4500_I2C_ADDR      (0x28 << 1)

// Registers
#define REG_PORT_STATUS_START   0x0D // Start of status regs to clear interrupts
#define REG_RDO_STATUS_0        0x91 // Negotiated Contract Status (PDO Index)

class USB_PD_Handler {
public:
    // Constructor
    USB_PD_Handler(I2C_HandleTypeDef* hi2c);

    // 1. Initialize chip (Clear Interrupts)
    bool init();

    // 2. Simple check: Do we have the High Power Profile (PDO3)?
    bool is20V5A_Active();

private:
    I2C_HandleTypeDef* _hi2c;

    // Internal Helper
    uint8_t readReg(uint8_t reg);
    void readBlock(uint8_t reg, uint8_t* buffer, uint8_t size);
};

// --- C Interface for main.c ---
#ifdef __cplusplus
extern "C" {
#endif

    void USB_PD_Setup(void);
    void USB_PD_Check(void); // Call this in your loop

#ifdef __cplusplus
}
#endif

#endif // USB_PD_HANDLER_H