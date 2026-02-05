#include "HotPlate.h"
#include "SI_Units.h"
#include "main.h"
#include "tim.h"
#include "Max6675.h"
#include "UserMain.h"
#include "MenuContent.h"
#include "stdio.h"

// --- Global Variables ---
States state;
Events event;
Events m_event;

bool flag_DrawMenu;

float currentTemp;
float targetTemp;
float time;
float buzzer_timeLeft_ms = 0;
float t;

#define ENCODER_STEPS_PER_CLICK 4
#define guro

#ifdef guro

void HotPlate(float dt_ms){

    static bool ini_ok = false;

    // --- Hardware Input: Encoder & Button ---
    static uint32_t enc_last_counter = 0;
    uint32_t enc_current_counter = __HAL_TIM_GET_COUNTER(&htim2);
    const bool button_pressed = !HAL_GPIO_ReadPin(button_notPressed_GPIO_Port, button_notPressed_Pin);
    
		//time duration since last event
    if(!ini_ok)                        t = dt_ms/2;
    else if(event == Events::none)     t = t + dt_ms;
    else                               t = dt_ms/2;
    
    
    // --- Edge Detection: Button ---
    static bool m_button_pressed = true;
    bool button_pressed_edge = button_pressed && !m_button_pressed;
    m_button_pressed = button_pressed;
    
    // --- Edge Detection: Encoder ---
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
        
    // --- Sensor: Read Temp every 1.0 sec ---
    if(!ini_ok)     Max6675_Init();
    static float                                         			t_ms;
    if(!ini_ok)                                          			t_ms=dt_ms/2;
    else if(t_ms + dt_ms < 1*sec)    t_ms = t_ms + dt_ms;
    else{                                                    	currentTemp = Max6675_Read();
                                                              t_ms = dt_ms/2;    }
              



																															
    // Debug info                                                        
    time = t_ms;
    if(event != Events::none) m_event = event;
    
    // --- State Machine: Transition Logic ---
    if(!ini_ok)                                                                         {event = Events::power_on;                            state = States::ready;}
    else if(state == States::ready && rot_right_edge && t>100*ms)                       {event = Events::menuDownEdge;                        state = States::ready;}        
    else if(state == States::ready && rot_left_edge && t>100*ms)                        {event = Events::menuUpEdge;                          state = States::ready;}        
    else if(state == States::ready && button_pressed_edge&& t>300*ms)                		{event = Events::button_pressed_edge;            			state = States::ready;}        
    else if(state == States::changeTargetTemp && rot_right_edge)                        {event = Events::increaseTemp;                        state = States::changeTargetTemp;}        
    else if(state == States::changeTargetTemp && rot_left_edge)                         {event = Events::decreaseTemp;                        state = States::changeTargetTemp;}    
    else if(state == States::changeTargetTemp && button_pressed_edge)                		{event = Events::none;                            		state = States::ready;}    
    else if(state == States::ready && event == Events::heating_on)                    	{event = Events::heating_on;                         	state = States::heating;}
    else if(state == States::heating && button_pressed_edge)                            {event = Events::heating_off;                       	state = States::ready;}
    else                                                                               	{event = Events::none;}    
    
    // Default target temp on startup
    if(!ini_ok)            targetTemp = 250;

    // --- Execution: Handle Events ---
    // Menu Navigation
    if(event == Events::menuDownEdge)                    menu.next();
    else if(event == Events::menuUpEdge)                 menu.prev();
    else if(event == Events::button_pressed_edge)        menu.enter();    
    
    // Change Temperature Value
    else if(state == States::changeTargetTemp && event == Events::increaseTemp)            targetTemp++;                
    else if(state == States::changeTargetTemp && event == Events::decreaseTemp)            targetTemp--;                
    
    // Feedback Beep
    if(event != Events::none) Buzzer_beep(100*ms, 400, 3);
    
    
		
		
		
   
		
		
		
		
		
    // --- Output Control: Heater & Text ---
    if (state == States::heating) {
        // Bang-bang controller
        if (currentTemp < targetTemp) {
            HAL_GPIO_WritePin(HeatingPlate_on_GPIO_Port, HeatingPlate_on_Pin, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(HeatingPlate_on_GPIO_Port, HeatingPlate_on_Pin, GPIO_PIN_RESET);
        }
        sprintf(heatingText_, "STOP HEATING");
    } else {
        // Safe state: Heater OFF
        HAL_GPIO_WritePin(HeatingPlate_on_GPIO_Port, HeatingPlate_on_Pin, GPIO_PIN_RESET);
        sprintf(heatingText_, "START HEATING");
    }
        
    // --- Display: Refresh Logic (200ms) ---
    float updateTime = 200*ms;
    static float                                                          	t_update;
    if(!ini_ok)                                                             t_update=dt_ms/2;
    else if(t_update + dt_ms < updateTime)     t_update = t_update + dt_ms;
    else{                                                            
                                                                                        flag_DrawMenu = true;
                                                                                        t_update = dt_ms/2;    }    
    
    // Update string buffers for display
    sprintf(currentTemp_, "Curr. Temp: %.0f C", currentTemp);
    sprintf(targetTemp_, "Targ. Temp: %.0f C", targetTemp);


    // --- Buzzer: Handle Duration ---
    if(buzzer_timeLeft_ms > 0){
        
        buzzer_timeLeft_ms -= dt_ms; 
        
        if(buzzer_timeLeft_ms <= 0){
            buzzer_timeLeft_ms = 0;
            HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
        }
    }

    ini_ok = true;
    #endif
}

// --- Helper: Render Display ---
void updateDisplay(){
    static bool ini_ok = false;
    if(!ini_ok)     ssd1306_Init();
    if (flag_DrawMenu) {
        menu.draw();        
        flag_DrawMenu = false; 
    }
    ini_ok = true;
}

// --- Helper: Set Event Externally ---
void setEvent(Events event_){
    event = event_;
}

// --- Helper: Change State Externally ---
void changeTargetTemp(){
    state = States::changeTargetTemp;
}

// --- Helper: Start Buzzer PWM ---
void Buzzer_beep(float duration, float freq, float volume){
    if(freq < 50) freq = 50; 
    if(volume > 50) volume = 50; 
    if(volume < 1) volume = 1;

    uint32_t period = (1000000 / (uint32_t)freq) - 1;
    uint32_t pulse = (period * (uint32_t)volume) / 100;

    __HAL_TIM_SET_AUTORELOAD(&htim3, period);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, pulse); 

    buzzer_timeLeft_ms = duration; 
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}