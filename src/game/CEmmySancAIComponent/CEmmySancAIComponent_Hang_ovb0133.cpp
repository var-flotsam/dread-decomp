// CEmmySancAIComponent's unit (tools/tu_map.py): whether the actor the E.M.M.I. acts against
// (fn_7100d7c990 of its owner, +0x30) hangs on one of the component's links: its movement (+0x208)
// must be in the state of the kind fn_71009a60fc names, and one of the links of the array at
// +0x2a700 (64-byte entries; resolved first in state 1 through fn_710023b580, as
// src/game/CBossAIComponent/CBossAIComponent_CanAct_b49.cpp) must hold a target whose +0x30 is that
// state's (fn_71009ad440). Otherwise fn_7100552314 answers. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/UnkActor.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkHangMoveState_A6_b70;
void* fn_71009ad440(UnkHangMoveState_A6_b70* state);
u32 fn_71009a60fc();

struct UnkSancMoveState_ovb0133 {
    void* vtable;
    u8 pad_0x8[0x8];
    s32 kind;
};

#pragma pack(push, 4)
struct UnkSancMove_ovb0133 {
    u8 pad_0x0[0x2d08];
    UnkSancMoveState_ovb0133* current;
    u8 pad_0x2d10[0x2d18 - 0x2d10];
    u32 state_count;
    UnkSancMoveState_ovb0133* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

struct UnkSancTarget_ovb0133 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

struct UnkSancHolder_ovb0133 : base::UnkRefCounted {
    UnkSancTarget_ovb0133* target;  // 0x10
};

struct UnkSancLink_ovb0133 {
    u8 pad_0x0[0x8];
    u8 field_0x8[0x28];
    u32 state;  // 0x30
    u8 pad_0x34[4];
    base::UnkRefHandle handle;  // 0x38
};

struct UnkSancLinks_ovb0133 {
    UnkSancLink_ovb0133* data;
    u32 count;
};

struct UnkEmmySancAI_ovb0133 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x2a700 - 0x38];
    UnkSancLinks_ovb0133 links;  // 0x2a700
};

bool fn_7100552314(UnkEmmySancAI_ovb0133* self);

static inline UnkSancMoveState_ovb0133* FindState(UnkSancMove_ovb0133* self, s32 kind) {
    u64 size = (u64)self->state_count * sizeof(UnkSancMoveState_ovb0133*);
    asm("" : "+r"(size));
    if (size == 0)
        return nullptr;
    UnkSancMoveState_ovb0133** it = self->states;
    asm("" : "+r"(it));
    for (; size != 0; ++it, size -= sizeof(*it)) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

static inline void* StateTarget(UnkSancMoveState_ovb0133* state) {
    return fn_71009ad440(reinterpret_cast<UnkHangMoveState_A6_b70*>(state));
}

static inline bool HangsOnLink(UnkEmmySancAI_ovb0133* self) {
    UnkActor* other = fn_7100d7c990(self->owner);
    if (other) {
        if (UnkSancMove_ovb0133* move = static_cast<UnkSancMove_ovb0133*>(other->field_0x208)) {
            u32 kind = fn_71009a60fc();
            UnkSancMoveState_ovb0133* state = kind <= 14 ? FindState(move, kind) : nullptr;
            if (__builtin_expect(state && move->current == state && StateTarget(state), 1)) {
                UnkSancLinks_ovb0133* links = &self->links;
                u32 count = links->count;
                if (count == 0)
                    return false;
                UnkSancLink_ovb0133* end = links->data + count;
                for (UnkSancLink_ovb0133* link = links->data; link != end; ++link) {
                    if (link->state == 1) {
                        {
                            UnkRefPtr reference = fn_710023b580(link->field_0x8);
                            fn_7100234f78(&link->handle, reinterpret_cast<const base::UnkRefHandle*>(&reference));
                        }
                        link->state = 2;
                    }
                    auto* holder = static_cast<UnkSancHolder_ovb0133*>(link->handle.ptr);
                    if (!holder)
                        continue;
                    if (UnkSancTarget_ovb0133* target = holder->target) {
                        if (static_cast<UnkSancTarget_ovb0133*>(StateTarget(state))->field_0x30 == target->field_0x30)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

bool fn_71004ffedc(UnkEmmySancAI_ovb0133* self) {
    if (HangsOnLink(self))
        return true;
    return fn_7100552314(self);
}
