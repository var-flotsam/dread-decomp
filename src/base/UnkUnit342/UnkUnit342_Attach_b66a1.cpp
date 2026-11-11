// tools/tu_map.py unit 342 (no class hint): attaches a source once, records a flag from a virtual
// and starts both values from the source's current one. Names and layouts are placeholders.
#include "game/CActorComponentDef.h"
#include "types.h"

struct UnkAttach342_b66a1 {
    virtual void vf_0x0();
    u8 pad_0x8[0x8];
    void* source;  // 0x10
    u8 pad_0x18[0x8];
    bool flag;      // 0x20
    f32 current;    // 0x24
    f32 target;     // 0x28

    bool Attach(void** source);
};

bool fn_710024f8ac(UnkAttach342_b66a1* self, void** source) {
    if (self->source != nullptr) {
        return false;
    }
    self->source = *source;
    typedef bool (*Fn)(UnkAttach342_b66a1*);
    self->flag = reinterpret_cast<Fn>((*reinterpret_cast<void***>(self))[0x120 / 8])(self);
    f32 value = fn_7100253ad4(self->source);
    self->target = value;
    self->current = value;
    return true;
}
