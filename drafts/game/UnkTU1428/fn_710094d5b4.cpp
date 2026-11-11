// CMorphBallMovement's unit: remembers the current hit (+0x128) and the new one. Before the flag at
// +0x2518 is set, the current hit is first copied to the saved one (+0x24d0); when the new hit has the
// same collider as the current one, it replaces the saved one unless the collider (kind 0x10 by vf_0x18)
// gives a different entry or index than the cached ones (+0x2430, +0x2438); the flag is then set. Always
// answers true. Names and layouts are placeholders.
#include "types.h"

struct UnkPool_0x48;
struct UnkStruct_0x48;

struct UnkAssignedSlot {
    void* ptr;

    UnkAssignedSlot& operator=(const UnkAssignedSlot& other);
};
void fn_71002a5d1c(UnkAssignedSlot* self, const UnkAssignedSlot* other);
__attribute__((always_inline)) inline UnkAssignedSlot& UnkAssignedSlot::operator=(const UnkAssignedSlot& other) {
    fn_71002a5d1c(this, &other);
    return *this;
}

struct UnkMorphBallCollider_d5b4 {
    void** vtable;
    s32 vf_0x18() {
        return reinterpret_cast<s32 (*)(UnkMorphBallCollider_d5b4*)>(vtable[0x18 / 8])(this);
    }
};

struct UnkMorphBallNode_d5b4 {
    u8 pad_0x0[0x10];
    UnkMorphBallNode_d5b4* field_0x10;
    u8 pad_0x18[0x158 - 0x18];
    UnkMorphBallCollider_d5b4* field_0x158;
};

struct UnkMorphBallHit_d5b4 {
    union {
        UnkAssignedSlot slot;
        UnkMorphBallNode_d5b4* ref;
    };
    UnkAssignedSlot slot_0x8;
    union {
        u32 words[13];  // 0x10
        struct {
            u16 index;
        } v;
    };

    UnkMorphBallHit_d5b4& operator=(const UnkMorphBallHit_d5b4& other) {
        if (this != &other) {
            slot = other.slot;
            slot_0x8 = other.slot_0x8;
            for (u32 i = 0; i < 13; ++i)
                words[i] = other.words[i];
        }
        return *this;
    }
};

struct UnkMorphBall_d5b4 {
    u8 pad_0x0[0x128];
    UnkMorphBallHit_d5b4 current;  // 0x128
    u8 pad_0x170[0x2430 - 0x170];
    UnkStruct_0x48* field_0x2430;
    s32 field_0x2438;
    u8 pad_0x243c[0x24d0 - 0x243c];
    UnkMorphBallHit_d5b4 saved;  // 0x24d0
    bool saved_flag;             // 0x2518
};
static_assert(__builtin_offsetof(UnkMorphBall_d5b4, saved_flag) == 0x2518, "layout");

UnkStruct_0x48* fn_710012431c(UnkPool_0x48*, u32);

bool fn_710094d5b4(UnkMorphBall_d5b4* self, UnkMorphBallHit_d5b4* hit) {
    if (!self->saved_flag)
        self->saved = self->current;
    UnkMorphBallCollider_d5b4* collider = self->current.ref->field_0x10->field_0x158;
    if (collider == hit->ref->field_0x10->field_0x158) {
        s32 kind = collider->vf_0x18();
        bool copy = true;
        if ((collider != nullptr) & (kind == 0x10)) {
            UnkStruct_0x48* entry =
                fn_710012431c(reinterpret_cast<UnkPool_0x48*>(reinterpret_cast<u8*>(collider) + 0xa8), hit->words[0]);
            copy = entry == self->field_0x2430 && hit->v.index == self->field_0x2438;
        }
        if (copy)
            self->saved = *hit;
        self->saved_flag = true;
    }
    return true;
}
