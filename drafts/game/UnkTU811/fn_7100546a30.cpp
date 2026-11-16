// UnkTU811 (CEmmyAIComponent's unit): builds a target record for `state` (fn_7100546d18) and keeps it as
// the AI's current target (+0x14b40) unless it is not a hand-over (state 2), the owner's animation has a
// CCharClassEmmyStopAndSearchTrack and the new position is where the last one was; then runs the virtual
// for the current state. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU55.h"
#include "game/UnkTU811.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern const UnkVec3 g_unk_7101649ed8 __attribute__((visibility("hidden")));  // a zero vector

// A counted reference to a weak handle.
struct UnkWeakRef_l546a30 {
    UnkWeakHandle* handle = nullptr;

    ~UnkWeakRef_l546a30() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

struct UnkPos2_l546a30 {
    f32 x = 0.0f;
    f32 y = 0.0f;

    UnkPos2_l546a30& operator=(const UnkPos2_l546a30& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
};

struct UnkCell_l546a30 {
    u16 cell;
    bool field_0x1a;
    bool field_0x1b;
    u32 field_0x1c;
};

// What the AI aims at: the target, the kind of aim (1, 2 or other), a score and where it is.
struct UnkEmmyTarget_l546a30 {
    UnkWeakRef_l546a30 target;  // 0x0
    u32 state;                  // 0x8
    f32 score;                  // 0xc
    UnkPos2_l546a30 position;   // 0x10
    UnkCell_l546a30 cell;       // 0x18

    UnkEmmyTarget_l546a30() {
        cell.field_0x1b = true;
        cell.field_0x1c = 0;
        state = 0;
        score = -3.4028235e38f;
        fn_710007b444(&position, &g_unk_7101649ed8);
        cell = UnkCell_l546a30{0xffff, false, false, 0};
    }
};
static_assert(sizeof(UnkEmmyTarget_l546a30) == 0x20, "UnkEmmyTarget_l546a30 size");

struct UnkEmmyTargetSlot_l546a30 {
    UnkEmmyTarget_l546a30 current;  // 0x0
    u8 pad_0x20[0x10];
    base::math::CVector2D lastPosition;  // 0x30
};

struct UnkEmmyOwner_l546a30 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkEmmyAI_l546a30 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkEmmyOwner_l546a30* owner;  // 0x30
    u8 pad_0x38[0x14b40 - 0x38];
    UnkEmmyTargetSlot_l546a30 target;  // 0x14b40

    void vf_0x7c0() { reinterpret_cast<void (*)(UnkEmmyAI_l546a30*)>(vtable[0x7c0 / 8])(this); }
    void vf_0x7c8(bool flag) { reinterpret_cast<void (*)(UnkEmmyAI_l546a30*, bool)>(vtable[0x7c8 / 8])(this, flag); }
    void vf_0x7d0() { reinterpret_cast<void (*)(UnkEmmyAI_l546a30*)>(vtable[0x7d0 / 8])(this); }
};

void fn_7100546d18(UnkEmmyRelaxAI_b59* self, UnkEmmyTarget_l546a30* out, u32 state, UnkActor* other, void* arg);

// Takes `from` as the current target: the weak handle of what it refers to, and its values.
static inline void Assign(UnkEmmyTarget_l546a30& to, const UnkEmmyTarget_l546a30& from) {
    if (&to.target != &from.target)
        CopyWeakRef(&to.target.handle, from.target.handle);
    to.state = from.state;
    to.score = from.score;
    to.position = from.position;
    to.cell = from.cell;
}

// Whether `p` lies within 0.001 of `c` on both axes.
static inline bool IsNear(const UnkPos2_l546a30& p, const base::math::CVector2D& c) {
    return p.x <= c.x + 0.001f && c.x - 0.001f <= p.x && c.y - 0.001f <= p.y && p.y <= c.y + 0.001f;
}

void fn_7100546a30(UnkEmmyRelaxAI_b59* ai, u32 state, UnkActor* other, void* arg) {
    UnkEmmyTarget_l546a30 target;
    UnkEmmyAI_l546a30* self = reinterpret_cast<UnkEmmyAI_l546a30*>(ai);
    UnkEmmyTargetSlot_l546a30& slot = self->target;
    fn_7100546d18(ai, &target, state, other, arg);
    bool searching = fn_710117b348(self->owner->animation, fn_7100c18138());
    if (target.state == 2 || !searching || !IsNear(target.position, slot.lastPosition))
        Assign(slot.current, target);
    if (slot.current.state == 1)
        self->vf_0x7c8(false);
    else if (slot.current.state == 2)
        self->vf_0x7c0();
    else
        self->vf_0x7d0();
}
