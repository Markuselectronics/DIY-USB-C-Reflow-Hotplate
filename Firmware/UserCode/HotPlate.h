#ifndef HotPlate_h
	#define HotPlate_h
	
enum class Events{power_on, PD_ok, PD_fail, none, temp_reached, heating_on, menuDownEdge, menuUpEdge, button_pressed_edge, increaseTemp, decreaseTemp, heating_off, cooling};
enum class States{boot, ready, heating, cooling, error, changeTargetTemp};

	
void updateDisplay();
void HotPlate(float dt);
void setEvent(Events event_);
void changeTargetTemp();
void Buzzer_beep(float duration, float freq, float volume);


#endif