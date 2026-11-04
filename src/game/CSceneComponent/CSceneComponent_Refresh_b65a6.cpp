// CSceneComponent's unit: while enabled (+0x44), passes the owner's (+0x18) flag at +0x1b3 to vtable slot
// 0x168. Names are placeholders.
#include "types.h"

struct UnkSceneOwner_b65a6 {
    u8 pad_0x0[0x1b3];
    bool flag;
};

struct UnkSceneComponent_b65a6 {
    virtual void vf_0x0();
    u8 pad_0x8[0x10];
    UnkSceneOwner_b65a6* owner;
    u8 pad_0x20[0x24];
    bool enabled;

    void vf_0x168(bool value) { (*reinterpret_cast<void (***)(UnkSceneComponent_b65a6*, bool)>(this))[0x168 / 8](this, value); }
};

void fn_71009f19e4(UnkSceneComponent_b65a6* self) {
    if (self->enabled)
        self->vf_0x168(self->owner->flag);
}
