// CUsableComponent unit (tools/tu_map.py, unit 1607): takes a weak handle to the user (fn_7100d78378) and, when
// enabled, records three of its values and either forwards to vf_0x168 or requests state 2 from the component's
// state machine at +0x100 ("none" 8). Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);

struct UnkUsableUser_A5_b73 {
    u8 pad_0x0[0x44];
    s32 field_0x44;
    u8 pad_0x48[0x54 - 0x48];
    s32 field_0x54;
    u8 pad_0x58[0x64 - 0x58];
    s32 field_0x64;
};

struct UnkUsable_A5_b73 {
    virtual void vf_0x0();
    void vf_0x168(UnkWeakTarget* user) {
        reinterpret_cast<void (*)(UnkUsable_A5_b73*, UnkWeakTarget*)>((*reinterpret_cast<void***>(this))[0x168 / 8])(
            this, user);
    }
    u8 pad_0x8[0xc8 - 0x8];
    bool field_0xc8;
    u8 pad_0xc9[0xd0 - 0xc9];
    s32 field_0xd0;
    s32 field_0xd4;
    s32 field_0xd8;
    s32 field_0xdc;
    u8 pad_0xe0[0xe8 - 0xe0];
    UnkWeakHandle* user;
    u8 pad_0xf0[0xf8 - 0xf0];
    bool field_0xf8;
    u8 pad_0xf9[0xfb - 0xf9];
    bool field_0xfb;
    u8 pad_0xfc[0x100 - 0xfc];
    UnkSmMachine_A5_b73 machine;
};
static_assert(__builtin_offsetof(UnkUsable_A5_b73, machine) == 0x100, "layout");

bool fn_7100aa1098(UnkUsable_A5_b73* self, UnkWeakTarget* target) {
    AssignRef(&self->user, fn_7100d78378(target));
    if (self->field_0xfb) {
        self->field_0xd0 = 0;
        self->field_0xc8 = true;
        auto* user = reinterpret_cast<UnkUsableUser_A5_b73*>(target);
        s32 a = user->field_0x44;
        s32 b = user->field_0x54;
        s32 c = user->field_0x64;
        self->field_0xd4 = a;
        self->field_0xd8 = b;
        self->field_0xdc = c;
        if (self->field_0xf8)
            self->vf_0x168(target);
        else
            RequestState_A5_b73<15, 8>(&self->machine, 2);
    }
    return true;
}
