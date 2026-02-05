#include "MenuHandler.h"


// --- Constructor ---
MenuHandler::MenuHandler(MenuPage* startMenu) {
    activeMenu = startMenu;
    // Ensure parent of main menu is null
    activeMenu->parentMenu = nullptr; 
}

// --- Navigation Logic ---

void MenuHandler::next() {
    
    // Only move down if we haven't reached the bottom yet.
    if (activeMenu->currentSelection < (activeMenu->itemCount - 1)) {
        activeMenu->currentSelection++;
    }
}

void MenuHandler::prev() {
    
    // Only move up if we are not at 0 yet.
    if (activeMenu->currentSelection > 0) {
        activeMenu->currentSelection--;
    }
}

void MenuHandler::enter() {
    // Get the item currently pointed to by cursor
    MenuItem* item = &activeMenu->items[activeMenu->currentSelection];

    // CASE 1: Item is a link to a Sub-Menu
    if (item->subMenu != nullptr) {
        // Link the new menu back to the current one (so we can go back)
        item->subMenu->parentMenu = activeMenu;
        
        // Switch active menu
        activeMenu = item->subMenu;
        
        // Reset cursor to top of new menu
        activeMenu->currentSelection = 0;
    }
    
    // CASE 2: Item is an Action (Function)
    else if (item->actionFunc != nullptr) {
        // Execute the function
        item->actionFunc();
    }
}

void MenuHandler::back() {
    // Check if there is a parent menu
    if (activeMenu->parentMenu != nullptr) {
        activeMenu = activeMenu->parentMenu;
    }
}

// --- Rendering Logic ---

void MenuHandler::draw() {
    // 1. Clear the display buffer
    ssd1306_Fill(Black);

    // 2. Draw the Title Bar
    // Adjust position (0,0) and Font as needed by your library
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString((char*)activeMenu->title, Font_7x10, White);
    
    // Optional: Draw a line under the title
    // ssd1306_Line(0, 12, 128, 12, White); 

    // 3. Loop through items
    for (int i = 0; i < activeMenu->itemCount; i++) {
        // Calculate Y position
        // e.g., Title is 12px high, each line is 12px
        int yPos = 14 + (i * 12); 

        // Stop drawing if we run out of screen space
        if (yPos > 54) break; 

        // Check if this is the selected item
        if (i == activeMenu->currentSelection) {
            // Draw Cursor (>)
            ssd1306_SetCursor(2, yPos);
            ssd1306_WriteString(">", Font_7x10, White);
        }

        // Draw Item Name
        // Indent X by 12 pixels to make room for cursor
        ssd1306_SetCursor(12, yPos);
        ssd1306_WriteString((char*)activeMenu->items[i].name, Font_7x10, White);
    }

    // 4. Push buffer to OLED hardware
    ssd1306_UpdateScreen();
}