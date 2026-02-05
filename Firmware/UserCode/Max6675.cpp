#include "Max6675.h"
#include "main.h"
#include "spi.h"


// Internal configuration (hidden from outside)
// Assuming SPI1 and the CS pin named 'TEMP_CS' in CubeMX
static SPI_HandleTypeDef* _hspi = &hspi1; 

// Definition of the CS Pin (matches CubeMX User Label 'TEMP_CS')
#define CS_PORT SPI1_CS_TH_GPIO_Port
#define CS_PIN  SPI1_CS_TH_Pin

void Max6675_Init() {
    // Ensure Chip Select is high (inactive) at start
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

float Max6675_Read() {
    uint8_t data[2] = {0, 0};
    uint16_t raw_val = 0;

    // 1. Start Transmission: Pull CS Low
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);

    // 2. Read 2 Bytes (16 Bits)
    // We use HAL_SPI_Receive. Since it's only 2 bytes, blocking is negligible (< 5us)
    HAL_SPI_Receive(_hspi, data, 2, 50);
    // 3. End Transmission: Pull CS High
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);

    // 4. Combine bytes (High Byte first, then Low Byte)
    raw_val = (data[0] << 8) | data[1];

    // 5. Check for Open Circuit (Bit D2)
    if (raw_val & 0x0004) {
        return -1.0f; // Error: Thermocouple not connected
    }

    // 6. Convert to Temperature
    // Bits D14-D3 hold the value. Resolution 0.25°C.
    // Shift right by 3 to remove dummy bits (D2, D1, D0)
    return (raw_val >> 3) * 0.25f;
		

}
