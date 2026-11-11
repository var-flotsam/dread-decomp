// CNailongAIComponent's unit (tools/tu_map.py, the name from reflection): a reset. After the
// behaviour-tree step fn_71003695b8 it drops the reference at +0x14830, then reads the owner's
// velocity (+0x30 -> +0x1f0, its vf_0x210) and, when its x is clearly negative or positive, sends id 4
// or 8 to the owner's +0x220 controller (fn_7101178564, as CShakernautAIComponent_Flip_b53.cpp).
// Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53 {
    u32 value;
    u32 unk_0x4;
};

struct UnkNailongVec_b67a4 {
    f32 x, y, z;
    UnkNailongVec_b67a4(const UnkNailongVec_b67a4& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkNailongMover_b67a4 {
    void** vtable;
    UnkNailongVec_b67a4 Velocity() {
        return reinterpret_cast<UnkNailongVec_b67a4 (*)(UnkNailongMover_b67a4*)>(vtable[0x210 / 8])(this);
    }
};

struct UnkNailongOwner_b67a4 {
    u8 pad_0x0[0x1f0];
    UnkNailongMover_b67a4* mover;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

struct UnkNailongRef_b67a4 {
    void** vtable;
    s32 refCount;
};

struct UnkNailongAI_b67a4 : UnkBehaviorTreeAI {
    u8 pad_0x38[0x14830 - 0x38];
    UnkNailongRef_b67a4* ref;  // 0x14830
    UnkNailongOwner_b67a4* Owner() { return static_cast<UnkNailongOwner_b67a4*>(field_0x30); }
};

void fn_710061ff98(UnkNailongAI_b67a4* self) {
    fn_71003695b8(self);
    if (UnkNailongRef_b67a4* ref = self->ref) {
        if (--ref->refCount == 0)
            reinterpret_cast<void (*)(UnkNailongRef_b67a4*)>(ref->vtable[0x10 / 8])(ref);
        self->ref = nullptr;
    }
    UnkNailongVec_b67a4 v = self->Owner()->mover->Velocity();
    if (v.x < -0.001f) {
        UnkShakernautId_b53 id;
        UnkShakernautCtrl_b53* ctrl = self->Owner()->ctrl;
        id.value = 4;
        fn_7101178564(ctrl, &id, true, false);
    } else if (v.x > 0.001f) {
        UnkShakernautId_b53 id;
        UnkShakernautCtrl_b53* ctrl = self->Owner()->ctrl;
        id.value = 8;
        fn_7101178564(ctrl, &id, true, false);
    }
}
