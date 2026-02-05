#include "MenuContent.h"
	#include "SI_Units.h"

#include "main.h"
#include "tim.h"


#define ENCODER_STEPS_PER_CLICK 4




bool flag_DrawMenu;

void Display(float dt_ms){
	
	
	static bool ini_ok         		= false;
	
	
	//Rotary Encoder
	static uint32_t enc_last_counter = 0;
  uint32_t enc_current_counter = __HAL_TIM_GET_COUNTER(&htim2);
	const bool button_pressed 		= !HAL_GPIO_ReadPin(button_notPressed_GPIO_Port, button_notPressed_Pin);
	
	//Rotary Encoder ----------
	static bool m_button_pressed = true;
	bool button_pressed_edge = button_pressed && !m_button_pressed;
	m_button_pressed = button_pressed;
	
	if(!ini_ok) enc_last_counter = enc_current_counter;
	bool rot_right_edge = false;
  bool rot_left_edge  = false;
	int16_t enc_diff = (int16_t)(enc_current_counter - enc_last_counter);

    if (enc_diff >= ENCODER_STEPS_PER_CLICK) {
        rot_right_edge = true;
        enc_last_counter = enc_current_counter; 
    } 
    else if (enc_diff <= -ENCODER_STEPS_PER_CLICK) {
        rot_left_edge = true;
        enc_last_counter = enc_current_counter; 
    }
	//Rotary Encoder------------------
	
	
		
		if(!ini_ok)																															{event = Events::power_on;								state = States::ready;}
		else if(state == States::ready && rot_right_edge)												{event = Events::menuDownEdge;						state = States::ready;}		
		else if(state == States::ready && rot_left_edge)												{event = Events::menuUpEdge;							state = States::ready;}		
		else if(state == States::ready && button_pressed_edge)									{event = Events::button_pressed_edge;			state = States::ready;}		
		
	
	
	
	
		//Update Display		 								
	float updateTime = 200*ms;
	static float 										 					t_update;
	if(!ini_ok)											 					t_update=dt_ms/2;
	else if(t_update + dt_ms < updateTime)    t_update = t_update + dt_ms;
	else																			flag_DrawMenu = true;														
					
	

	ini_ok = true;
}


	
		char currentTemp_[20];
		char targetTemp_[20];  


	
	void back() {
    menu.back();
}

char heatingText_[20] = "START HEATING"; 

void heating_on(){
	setEvent(Events::heating_on);
}

	MenuItem profileItems[] = {
    {"SAC305 (LeadFree)", nullptr, nullptr},
    {"SnPb (Leaded)",     nullptr, nullptr},
    {"Back",              nullptr, back} 
};
	MenuPage profilePage = {"SELECT PROFILE", profileItems, 3, nullptr, 0};
	
	
	
MenuItem manualItems[] = {
		{heatingText_,  		nullptr, 		heating_on},
    {targetTemp_,   		nullptr, 		changeTargetTemp},
		{currentTemp_, 	 	nullptr, 		nullptr},
    {"Back",           		nullptr, 		back}
};
	MenuPage manualPage = {"MANUAL MODE", manualItems, 4, nullptr, 0};
	

	
	
	MenuItem mainItems[] = {
    {"Start Heating",  		&manualPage,       nullptr},   
    {"Reflow Profiles",   &profilePage,			 nullptr},     
    {"Settings: ",        nullptr,       		 nullptr},      
		{"Current Temp: ",    nullptr,           nullptr}      

};
	MenuPage mainPage = {"MAIN MENU", mainItems, 3, nullptr, 0}; 

	MenuHandler menu(&mainPage);
	
	
	
	
	void updateDisplay(){
	static bool ini_ok = false;
	if(!ini_ok) 	ssd1306_Init();
    if (flag_DrawMenu) {
        menu.draw();        
        flag_DrawMenu = false; 
    }
		ini_ok = true;
	}