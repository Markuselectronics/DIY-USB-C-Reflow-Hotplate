#include "UserMain.h"
#include "USB_PD_Handler.h"
#include "i2c.h"
#include "HotPlate.h"
#include "SI_Units.h"

//#include "MenuContent.h"


void setup(){

	// NOTE: This logic executes every time the timer overflows (interrupt context).
	htim4.PeriodElapsedCallback = [] (TIM_HandleTypeDef *htim){Task_1(1*ms);};
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

}

// Periodic task called by the Timer ISR
// param dt: Delta time in seconds since the last call (for integral/derivative calculations)

void Task_1(float dt){
	
	HotPlate(dt);

}

void loop(){
	
updateDisplay();
	
}
	
