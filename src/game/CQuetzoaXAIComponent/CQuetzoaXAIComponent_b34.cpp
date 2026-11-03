#include "types.h"

struct UnkQuetzoaXToggle {
    bool enabled;
    u32 timer;
};

// Sets the toggle at 0x14bc8 and restarts its timer when the value changes.
void fn_71006407e0(char* self, bool value) {
    UnkQuetzoaXToggle* t = reinterpret_cast<UnkQuetzoaXToggle*>(self + 0x14bc8);
    if (t->enabled != value) {
        t->enabled = value;
        t->timer = 0;
    }
}
