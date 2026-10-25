// Two checks (fn_710054b120 with the sub-object at 0x14780, fn_710054b2b0 with the one at
// 0x14880) of whether an ignore-perception track may be started: true while the master flag
// at 0x27720 is clear; otherwise only when the flag at 0x14c14 is set, neither timer (0x29350,
// 0x29418) is running, the owner's object isn't an ignore-perception track and the flag at
// 0x29f26 is clear; then the sub-object decides (fn_7100573d54).
#include "base/reflection/UnkClassGetters.h"
#include "types.h"

struct UnkTrackOwner {
    u8 unk_0x0[0x220];
    void* field_0x220;
};
struct UnkSelf {
    u8 unk_0x0[0x30];
    UnkTrackOwner* owner;
    u8 unk_0x38[0x14748];
    u8 sub_0x14780[0x100];
    u8 sub_0x14880[0x394];
    u8 field_0x14c14;
    u8 unk_0x14c15[0x12b0b];
    u8 field_0x27720;
    u8 unk_0x27721[0x1c2f];
    u8 timer_0x29350[0xc8];
    u8 timer_0x29418[0xb0e];
    u8 field_0x29f26;
};

bool fn_710002fb70(void* timer);
bool fn_710117b348(void* object, const base::reflection::CClass* type);
bool fn_7100573d54(void* sub, void* a1);

__attribute__((always_inline)) static inline bool Check(UnkSelf* self, void* sub, void* a1) {
    if (!self->field_0x27720)
        return true;
    if (!self->field_0x14c14 || fn_710002fb70(self->timer_0x29350))
        return false;
    void* object = self->owner->field_0x220;
    if (!object || fn_710117b348(object, fn_7100c78d04()) ||
        fn_710002fb70(self->timer_0x29418) || self->field_0x29f26)
        return false;
    return fn_7100573d54(sub, a1);
}

bool fn_710054b120(UnkSelf* self, void* a1) { return Check(self, self->sub_0x14780, a1); }
bool fn_710054b2b0(UnkSelf* self, void* a1) { return Check(self, self->sub_0x14880, a1); }
