// From unit 2702 of tools/tu_map.py (CAIComponent): starts something once: not while one is held at
// +0x1148, nor while the object at +0x30's +0x278 reports busy (fn_7101136308); otherwise takes the
// one vf_0x218 gives and clears the two flags after it. Names and layouts are placeholders.
#include "types.h"

struct UnkBusy_71011058a8;
bool fn_7101136308(void* object);

struct UnkOwnerData_71011058a8 {
    u8 pad_0x0[0x278];
    UnkBusy_71011058a8* busy;  // +0x278
};

struct UnkComponent_71011058a8;
struct UnkComponentVtbl_71011058a8 {
    void* slots[0x218 / 8];
    void* (*vf_0x218)(UnkComponent_71011058a8* self);
};

struct UnkComponent_71011058a8 {
    UnkComponentVtbl_71011058a8* vtbl;
    u8 pad_0x8[0x28];
    UnkOwnerData_71011058a8* owner;  // +0x30
    u8 pad_0x38[0x1148 - 0x38];
    void* current;  // +0x1148
    bool flag_0x1150;
    u8 pad_0x1151[7];
    bool flag_0x1158;
};

bool fn_71011058a8(UnkComponent_71011058a8* self) {
    if (self->current)
        return false;
    if (UnkBusy_71011058a8* busy = self->owner->busy)
        if (fn_7101136308(busy))
            return false;
    void* r = self->vtbl->vf_0x218(self);
    if (!r)
        return false;
    self->current = r;
    self->flag_0x1150 = false;
    self->flag_0x1158 = false;
    return true;
}
