// game::logic::collision::CBroadphaseObject's unit: sets the object's flag at +0x6a; a change runs
// vf_0x98 and then vf_0x48 (game/UnkBroadphaseObject.h). Names and layouts are placeholders.
#include "game/UnkBroadphaseObject.h"
#include "types.h"

struct UnkBroadphaseObject {
    void** vtable;
    u8 pad_0x8[0x62];
    bool field_0x6a;

    void vf_0x48() { reinterpret_cast<void (*)(UnkBroadphaseObject*)>(vtable[0x48 / 8])(this); }
    void vf_0x98() { reinterpret_cast<void (*)(UnkBroadphaseObject*)>(vtable[0x98 / 8])(this); }
};

static inline void SetFlag(UnkBroadphaseObject* self, bool value) {
    if (self->field_0x6a != value) {
        self->field_0x6a = value;
        self->vf_0x98();
        self->vf_0x48();
    }
}

void fn_7100d35a98(UnkEntry* entry, bool flag) {
    SetFlag(reinterpret_cast<UnkBroadphaseObject*>(entry), flag);
}
