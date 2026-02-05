#ifndef UserMain_h
#define UserMain_h
#include "main.h"
#include "tim.h"

	#ifdef __cplusplus
	extern "C"{
	#endif
	#define DO_set(x) HAL_GPIO_WritePin(x##_GPIO_Port, x##_Pin, GPIO_PIN_SET);
	#define DO_reset(x) HAL_GPIO_WritePin(x##_GPIO_Port, x##_Pin, GPIO_PIN_RESET);
		
	void loop();
	void setup();
	void Task_1(float dt);
	
#ifdef __cplusplus
}
#endif

#endif