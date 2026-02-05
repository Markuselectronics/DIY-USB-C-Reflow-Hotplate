#ifndef MenuContent_h
	#define MenuContent_h
	
	#include "MenuHandler.h"
	
	MenuItem mainItems[] = {
    {"Start Heating", nullptr,       nullptr},   
    {"Reflow Profiles",  nullptr, nullptr},     
    {"Status: ",         nullptr,       nullptr},      
		{"Current Temp: ",         nullptr,       nullptr}      

};
MenuPage mainPage = {"MAIN MENU", mainItems, 3, nullptr, 0};
	
	
#endif