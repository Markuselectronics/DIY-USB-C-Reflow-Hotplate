#ifndef MenuContent_h
#define MenuContent_h
	
	#include "MenuHandler.h"
	#include "HotPlate.h"
	#include "stdio.h"
	
	extern MenuHandler menu;
	
		char currentTemp_[20];
		char targetTemp_[20];  
	
	void back() {
    menu.back();
}
	

char heatingText_[20] = "START HEATING"; 

void heating_on(){
	setEvent(Events::heating_on);
}

	MenuItem SACItems[] = {
		{heatingText_,  		nullptr, 		heating_on},
    {targetTemp_,   		nullptr, 		changeTargetTemp},
		{currentTemp_, 	 		nullptr, 		nullptr},
    {"Back",           	nullptr, 		back}
};
	MenuPage SACProfile = {"SAC305 PROFILE", SACItems, 4, nullptr, 0};

	
		MenuItem SnPbItems[] = {
		{heatingText_,  		nullptr, 		heating_on},
    {targetTemp_,   		nullptr, 		changeTargetTemp},
		{currentTemp_, 	 		nullptr, 		nullptr},
    {"Back",           	nullptr, 		back}
};
	MenuPage SnPbProfile = {"SnPb PROFILE", SnPbItems, 4, nullptr, 0};

	

	MenuItem profileItems[] = {
    {"SAC305 (LeadFree)", &SACProfile, nullptr},
    {"SnPb (Leaded)",     &SnPbProfile, nullptr},
    {"Back",              nullptr, back} 
};
	MenuPage profilePage = {"SELECT PROFILE", profileItems, 3, nullptr, 0};
	
	
	
MenuItem manualItems[] = {
		{heatingText_,  		nullptr, 		heating_on},
    {targetTemp_,   		nullptr, 		changeTargetTemp},
		{currentTemp_, 	 		nullptr, 		nullptr},
    {"Back",           	nullptr, 		back}
};
	MenuPage manualPage = {"MANUAL MODE", manualItems, 4, nullptr, 0};
	
	
	

	

	
	
	MenuItem mainItems[] = {
    {"Start Heating",  		&manualPage,       nullptr},       
    //{"Settings: ",        nullptr,       		 nullptr}     
		      

};
	MenuPage mainPage = {"MAIN MENU", mainItems, 1, nullptr, 0}; 


	



	
	
	MenuHandler menu(&mainPage);
	
	
#endif