// Unit 1445 (CCharacterMovement): copies another entity's character movement settings into this one
// (two assigned slots, thirteen words and a weak reference), when that entity's movement is a
// CCharacterMovement. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAssignedSlot.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// CCharacterMovement's class (what fn_7100975b3c returns).
__attribute__((visibility("hidden"))) extern base::reflection::CClass g_unk_717273a930;

struct UnkMovementSettings_l976140 {
    UnkAssignedSlot slot_a;  // 0x0
    UnkAssignedSlot slot_b;  // 0x8
    u32 values[13];          // 0x10

    UnkMovementSettings_l976140& operator=(const UnkMovementSettings_l976140& other) {
        if (this != &other) {
            slot_a = other.slot_a;
            slot_b = other.slot_b;
            for (u32 i = 0; i < 13; ++i)
                values[i] = other.values[i];
        }
        return *this;
    }
};

// A weak reference to an object (through its weak handle).
struct UnkWeakRef_l976140 {
    UnkWeakHandle* handle;
};

static inline void AssignWeakRef(UnkWeakRef_l976140& dst, const UnkWeakRef_l976140& src) {
    if (&src != &dst)
        CopyWeakRef(&dst.handle, src.handle);
}

struct UnkCharacterMovement_l976140 {
    virtual const base::reflection::CClass* vf_0x0();
    u8 pad_0x8[0x230 - 0x8];
    UnkMovementSettings_l976140 settings;  // 0x230
    UnkWeakRef_l976140 target;  // 0x278
};
static_assert(__builtin_offsetof(UnkCharacterMovement_l976140, target) == 0x278, "layout");

struct UnkMovementEntity_l976140 {
    u8 pad_0x0[0x1f0];
    UnkCharacterMovement_l976140* movement;  // 0x1f0
};

void fn_7100976140(UnkCharacterMovement_l976140* self, UnkMovementEntity_l976140* entity) {
    if (!entity)
        return;
    UnkCharacterMovement_l976140* other = entity->movement;
    if (!other)
        return;
    const base::reflection::CClass* type = other->vf_0x0();
    fn_7100975b3c();
    if (!fn_710009d50c(type, &g_unk_717273a930))
        return;
    UnkCharacterMovement_l976140& dst = *self;
    const UnkCharacterMovement_l976140& src = *other;
    dst.settings = src.settings;
    AssignWeakRef(dst.target, src.target);
}
