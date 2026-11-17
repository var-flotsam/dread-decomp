// CHecathonAIComponent's unit (tools/tu_map.py unit 865): after the behaviour-tree base update the owner
// is added to the AI manager's set at +0x2b8, the two scene links are resolved and the weak handles of
// what fn_7100382150 finds for their targets are kept; then the state is reset from the tunable
// g_unk_71726e8d00 and the attack is reset (fn_71005a9eac). Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CGameLink.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkManager_71726af468.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkTunable;
// As src/game/CHecathonAIComponent/CHecathonAIComponent_Timeline_b60.cpp declares it.
extern UnkTunable g_unk_71726e8d00;

struct UnkNodeSet_b43a1;
void fn_71005ac8f4(UnkNodeSet_b43a1* self, u64 v);

struct UnkMagnetPathHolder_b52;
UnkMagnetPathHolder_b52* fn_7100382150(UnkActor* actor);

struct UnkHecathonAI_l5a9a04;
void fn_71005a9eac(UnkHecathonAI_l5a9a04* self);

// CCharClassHecathonAIComponent's tunable, as this function reads it.
struct UnkHecathonTunable_l5a9a04 {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
};

static inline const UnkHecathonTunable_l5a9a04& HecathonTunable() {
    return reinterpret_cast<const UnkHecathonTunable_l5a9a04&>(g_unk_71726e8d00);
}

// A scene link as this unit uses it: resolved on first use.
struct UnkLink_l5a9a04 : CGameLink<UnkActor> {
    void Resolve() {
        if (state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(key);
                fn_7100234f78(&handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
    }

    UnkActor* Get() {
        Resolve();
        return Target();
    }
};

struct UnkHecathonAI_l5a9a04 {
    u8 pad_0x0[0x30];
    u64 owner;  // 0x30
    u8 pad_0x38[0x146d8 - 0x38];
    UnkLink_l5a9a04 link_0x146d8;
    u8 pad_0x14718[0x8];
    UnkLink_l5a9a04 link_0x14720;
    u8 pad_0x14760[0x1477c - 0x14760];
    f32 field_0x1477c;
    f32 field_0x14780;
    f32 field_0x14784;
    u8 pad_0x14788[0x14794 - 0x14788];
    bool field_0x14794;
    u8 pad_0x14795[0x14798 - 0x14795];
    f32 field_0x14798;
    f32 field_0x1479c;
    f32 field_0x147a0;
    f32 field_0x147a4;
    f32 field_0x147a8;
    f32 field_0x147ac;
    f32 field_0x147b0;
    f32 field_0x147b4;
    f32 field_0x147b8;
    f32 field_0x147bc;
    f32 field_0x147c0;
    f32 field_0x147c4;
    bool field_0x147c8;
    bool field_0x147c9;
    bool field_0x147ca;
    bool field_0x147cb;
    bool field_0x147cc;
    bool field_0x147cd;
    bool field_0x147ce;
    bool field_0x147cf;
    bool field_0x147d0;
    bool field_0x147d1;
    bool field_0x147d2;
    bool field_0x147d3;
    u8 pad_0x147d4[0x4];
    UnkWeakHandle* field_0x147d8;
    UnkWeakHandle* field_0x147e0;
};
static_assert(__builtin_offsetof(UnkHecathonAI_l5a9a04, link_0x14720) == 0x14720, "layout");
static_assert(__builtin_offsetof(UnkHecathonAI_l5a9a04, field_0x147c8) == 0x147c8, "layout");
static_assert(__builtin_offsetof(UnkHecathonAI_l5a9a04, field_0x147e0) == 0x147e0, "layout");

void fn_71005a9a04(UnkHecathonAI_l5a9a04* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    fn_71005ac8f4(reinterpret_cast<UnkNodeSet_b43a1*>(reinterpret_cast<char*>(m) + 0x2b8), self->owner);
    // Keep the weak handles of what fn_7100382150 finds for the two links' targets.
    if (self->link_0x146d8.Get() && fn_7100382150(self->link_0x146d8.Get())) {
        UnkWeakTarget* found = reinterpret_cast<UnkWeakTarget*>(fn_7100382150(self->link_0x146d8.Get()));
        AssignRef(&self->field_0x147d8, GetWeakHandle(found));
    }
    if (self->link_0x14720.Get() && fn_7100382150(self->link_0x14720.Get())) {
        UnkWeakTarget* found = reinterpret_cast<UnkWeakTarget*>(fn_7100382150(self->link_0x14720.Get()));
        AssignRef(&self->field_0x147e0, GetWeakHandle(found));
    }

    self->field_0x147c8 = true;
    self->field_0x147c9 = false;
    self->field_0x147ca = true;
    self->field_0x147cb = false;
    self->field_0x14794 = false;
    self->field_0x147cc = false;
    self->field_0x147d3 = false;
    self->field_0x147d1 = false;
    self->field_0x147d2 = true;
    self->field_0x147cd = false;
    self->field_0x147ce = false;
    self->field_0x147cf = false;
    self->field_0x147d0 = false;
    self->field_0x147a4 = 0.0f;
    self->field_0x147a8 = 0.0f;
    self->field_0x147b0 = 0.0f;
    self->field_0x14784 = 0.0f;
    self->field_0x147c0 = 0.0f;
    const UnkHecathonTunable_l5a9a04& tunable = HecathonTunable();
    self->field_0x1477c = tunable.field_0x44;
    self->field_0x14798 = tunable.field_0x48;
    self->field_0x1479c = tunable.field_0x4c;
    self->field_0x147ac = tunable.field_0x50;
    self->field_0x14780 = tunable.field_0x54;
    self->field_0x147a0 = tunable.field_0x58;
    self->field_0x147b4 = tunable.field_0x5c;
    self->field_0x147bc = tunable.field_0x60;
    self->field_0x147c4 = tunable.field_0x64;
    fn_71005a9eac(self);
    self->field_0x147b8 = self->field_0x147b4;
}
