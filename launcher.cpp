#include "launcher.h"

app_func_t app_launcher() {
    d->setFont(&fonts::Font2);
    d->setTextDatum(top_left);

    int8_t choice = ui_picker(appNames, itemCount);
    
    if (choice >= 0 && choice < itemCount) {
        return apps[choice].func; 
    }
    
    return nullptr;
}
