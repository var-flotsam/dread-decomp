// From unit 2730 of tools/tu_map.py (no reflected class): tells every listener (an array of
// callback and context pairs at +0x30, count at +0x38) about an event, 4, 5 or 6, passing the
// argument on. Names and layouts are placeholders.
#include "types.h"

struct UnkEventListener {
    void* context;
    void (*callback)(void* context, void* arg, u32 event);
};

struct UnkEventSource {
    u8 pad_0x0[0x30];
    UnkEventListener* listeners;
    u32 count;
};

template <u32 Event>
__attribute__((always_inline)) static inline void Notify(UnkEventSource* self, void* arg) {
    for (UnkEventListener *it = self->listeners, *end = self->listeners + self->count; it != end; ++it)
        it->callback(it->context, arg, Event);
}

void fn_7101197d2c(UnkEventSource* self, void* arg) { Notify<4>(self, arg); }
void fn_7101197d80(UnkEventSource* self, void* arg) { Notify<5>(self, arg); }
void fn_7101197dd8(UnkEventSource* self, void* arg) { Notify<6>(self, arg); }
