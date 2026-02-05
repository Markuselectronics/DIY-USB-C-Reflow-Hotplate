#ifndef MenuHandler_h
#define MenuHandler_h

// --- 1. Include C Libraries ---
// We use extern "C" so the C++ compiler understands these are C files.
#ifdef __cplusplus
extern "C" {
#endif

#include "ssd1306.h"
#include "ssd1306_fonts.h" // Ensure fonts are included

#ifdef __cplusplus
}
#endif

// --- 2. Data Structures ---

// Forward declaration so MenuItem knows MenuPage exists
struct MenuPage;

/**
 * Represents a single line in the menu.
 * It can either open a sub-menu OR execute a function.
 */
struct MenuItem {
    const char* name;          // Text displayed on screen
    MenuPage* subMenu;         // Pointer to a sub-menu (or nullptr)
    void (*actionFunc)();      // Pointer to a function (or nullptr)
};

/**
 * Represents a full screen page of menu items.
 */
struct MenuPage {
    const char* title;         // Title at the top of the screen
    MenuItem* items;           // Array of menu items
    int itemCount;             // Number of items in the array
    MenuPage* parentMenu;      // Pointer to previous menu (for Back button)
    int currentSelection;      // Saves cursor position
};

// --- 3. The Class Definition ---

class MenuHandler {
private:
    MenuPage* activeMenu;      // The menu page currently visible

public:
    // Constructor: Needs the main menu to start
    MenuHandler(MenuPage* startMenu);

    // Navigation methods
    void next();    // Move cursor down
    void prev();    // Move cursor up
    void enter();   // specific action: Enter sub-menu or run function
    void back();    // Go back to parent menu

    // Rendering method
    void draw();    // Draws the current state to the OLED
};

#endif