// CMissileMovement's unit (tools/tu_map.py unit 1618). Names and layouts are placeholders.
#include "game/UnkTU341.h"
#include "types.h"

// An intrusively counted result (count at +8, deleted through its slot 2).
struct UnkMissileMoveResult_ovb0070 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkMissileMoveResultRef_ovb0070 {
    UnkMissileMoveResult_ovb0070* ptr = nullptr;
    void Release() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkMissileOwner_ovb0070 {
    u8 pad_0x0[0x1b5];
    bool frozen;  // 0x1b5
};

struct UnkMissileUpdate_ovb0070 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkMissileOwner_ovb0070* owner;  // 0x30
    u8 pad_0x38[0x4c];
    u8 field_0x84[0x4a0];
    bool field_0x524;

    bool vf_0x2b0(void* a, void* arg, bool b, bool c, UnkMissileMoveResultRef_ovb0070* out) {
        return reinterpret_cast<bool (*)(UnkMissileUpdate_ovb0070*, void*, void*, bool, bool,
                                         UnkMissileMoveResultRef_ovb0070*)>(vtable[0x2b0 / 8])(this, a, arg, b, c,
                                                                                               out);
    }
};
static_assert(__builtin_offsetof(UnkMissileUpdate_ovb0070, field_0x524) == 0x524, "layout");

// The missile's move (as fn_7100ac3efc moves a projectile): unless the owner is frozen, runs the
// move (virtual slot 0x2b0) and, when it moved and +0x524 is clear, hands the owner to
// fn_7100232c68.
bool fn_7100abae3c(UnkMissileUpdate_ovb0070* self, void* arg) {
    if (self->owner->frozen)
        return false;
    UnkMissileMoveResultRef_ovb0070 result;
    bool moved = self->vf_0x2b0(self->field_0x84, arg, false, true, &result);
    result.Release();
    if (!moved)
        return false;
    if (!self->field_0x524)
        fn_7100232c68(self->owner);
    return true;
}
