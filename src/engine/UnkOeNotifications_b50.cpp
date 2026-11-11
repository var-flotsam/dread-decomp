// The system notification loop (unit 110 of tools/tu_map.py): drains nn::oe's notification messages,
// refreshing the operation mode (message 30, operation mode changed) and raising the exit flag
// (message 4, exit requested); fn_71001327c0 runs a per-frame callback until exit is requested.
// Names are placeholders.
#include <nn/oe.h>

#include "game/malloc.h"
#include "types.h"

extern bool g_unk_710b371010;  // exit requested
extern void (*g_unk_710b371008)();  // the per-frame callback

static inline void DrainNotifications() {
    u32 message;
    while (nn::oe::TryPopNotificationMessage(&message)) {
        if (message == 4)
            g_unk_710b371010 = true;
        else if (message == 30)
            g_unk_710b371068 = nn::oe::GetOperationMode();
    }
}

void fn_7100132890() {
    DrainNotifications();
}

void fn_71001327c0() {
    while (g_unk_710b371008 && !g_unk_710b371010) {
        g_unk_710b371008();
        DrainNotifications();
    }
}
