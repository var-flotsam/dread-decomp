// GUI::CTutorialComposition (unit 1727 of tools/tu_map.py). Names and layouts are placeholders.
#include "game/GUI/CArchivesInspectorComposition.h"
#include "game/GUI/CTutorialComposition.h"
#include "types.h"

struct UnkTutorialRequest {
    u8 pad_0x0[0x1a];
    bool handled;  // 0x1a
};

struct UnkTutorialComposition {
    virtual void vf_0x0();
    u8 pad_0x8[0xb4 - 0x8];
    bool shown;  // 0xb4
    u8 pad_0xb5[0x230 - 0xb5];
    u8 on_exit[0x8];  // 0x230: deferred callbacks run with the request

    void vf_0x1e8(const char* event, u32 arg) {
        (*reinterpret_cast<void (**)(UnkTutorialComposition*, const char*, u32)>(*reinterpret_cast<char**>(this) + 0x1e8))(this, event, arg);
    }
};

// Leaves the tutorial unless the request was already handled: sends "OnExit", runs the exit
// callbacks with the request and marks it handled.
void fn_7100b8903c(UnkTutorialComposition* self, UnkTutorialRequest* request) {
    if (request && request->handled) return;
    if (!self->shown) return;
    UnkTutorialRequest* arg = request;
    self->vf_0x1e8("OnExit", 0);
    fn_7100af3364(self->on_exit, &arg);
    if (request) request->handled = true;
}
