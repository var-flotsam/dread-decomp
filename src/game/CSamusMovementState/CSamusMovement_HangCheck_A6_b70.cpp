// A CSamusMovement check (tools/tu_map.py, the CSamusMovement unit): whether the given object's +0x30
// is what fn_71009ad440 gives for the movement's state of the kind fn_71009a60fc names, taken only
// while that state is the current one (+0x2d08, of kind 13). An owner whose character class has no
// CCharClassHangTrack passes. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CSamusMagnetGloveMovementState/CParams.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkHangMoveState_A6_b70 {
    void* vtable;
    u8 pad_0x8[0x8];
    s32 kind;
};

#pragma pack(push, 4)
struct UnkHangMove_A6_b70 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x2d08 - 0x38];
    UnkHangMoveState_A6_b70* current;
    u8 pad_0x2d10[0x2d18 - 0x2d10];
    u32 state_count;
    UnkHangMoveState_A6_b70* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

struct UnkHangTarget_A6_b70 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

bool fn_710097a564(UnkHangMove_A6_b70* self);
u32 fn_71009a60fc();

static inline UnkHangMoveState_A6_b70* FindState(UnkHangMove_A6_b70* self, s32 kind) {
    u64 size = (u64)self->state_count * sizeof(UnkHangMoveState_A6_b70*);
    asm("" : "+r"(size));
    if (size == 0)
        return nullptr;
    UnkHangMoveState_A6_b70** it = self->states;
    for (; size != 0; ++it, size -= sizeof(*it)) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

bool fn_71009e86e8(UnkHangMove_A6_b70* self, UnkHangTarget_A6_b70* target) {
    if (!fn_710097a564(self))
        return false;
    auto* cls = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (!cls || !fn_710117b348(cls, fn_7100d029c8()))
        return true;
    if (!self->current || self->current->kind != 13)
        return false;
    u32 kind = fn_71009a60fc();
    UnkHangMoveState_A6_b70* state = kind <= 14 ? FindState(self, kind) : nullptr;
    return fn_71009ad440(state == self->current ? state : nullptr) == target->field_0x30;
}
