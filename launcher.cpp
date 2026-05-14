#include "launcher.h"

app_func_t launcher() {
    int choice = ui_picker(appNames, itemCount);
    
    if (choice >= 0 && choice < itemCount) return apps[choice].func; 
    
    return nullptr;
}
