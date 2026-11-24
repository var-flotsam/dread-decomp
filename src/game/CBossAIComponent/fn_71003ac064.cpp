// CBossAIComponent unit: the constructor. The behaviour-tree base constructor (fn_710036932c), the
// vtable, then the boss's members from +0x146c8: an empty array, two landmark names, the landmark
// positions, five callback sets, timers and flags, the life bar (fn_710035d4e0) and an empty string id.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CBossAIComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkTU441.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkBossCallbackSetBase_l3ac064 {
    void** vtable;
    UnkOwnedErasedVector callables;
    u32 field_0x30;

    UnkBossCallbackSetBase_l3ac064() : vtable(g_unk_7101a20998 + 2), field_0x30(0) {}
};

struct UnkBossCallbackSet_l3ac064 : UnkBossCallbackSetBase_l3ac064 {
    void* owner;

    UnkBossCallbackSet_l3ac064() {
        vtable = g_unk_7101a20950 + 2;
        owner = nullptr;
    }
};
static_assert(sizeof(UnkBossCallbackSet_l3ac064) == 0x40, "layout");

struct UnkBossAI_l3ac064 {
    void** vtable;
    u8 pad_0x8[0x146c8 - 0x8];
    base::global::CRntVector<void*> field_0x146c8;
    CStrId landmark_id_0;                         // 0x146f0
    CStrId landmark_id_1;
    f32 height_tolerance;                         // 0x14700
    bool landmark_found_0;                        // 0x14704
    base::math::CVector3D landmark_pos_0;
    bool landmark_found_1;
    base::math::CVector3D landmark_pos_1;
    f32 near_height_time;                         // 0x14724
    UnkBossCallbackSet_l3ac064 callbacks_a[3];    // 0x14728
    bool field_0x147e8;
    bool field_0x147e9;
    f32 field_0x147ec;
    f32 field_0x147f0;
    bool field_0x147f4;
    u64 field_0x147f8;
    u32 field_0x14800;
    s32 field_0x14804;
    bool field_0x14808;
    f32 field_0x1480c;
    u32 field_0x14810;
    u32 field_0x14814;
    u8 pad_0x14818[0x148d8 - 0x14818];
    bool field_0x148d8;
    f32 field_0x148dc;
    bool field_0x148e0;
    u32 field_0x148e4;
    u32 field_0x148e8;
    u8 pad_0x148ec[0x4];
    u8 life_bar[0x18];                         // 0x148f0
    void* field_0x14908;
    UnkBossCallbackSet_l3ac064 callbacks_b;    // 0x14910
    UnkEmptyStrId id;                 // 0x14950
    UnkBossCallbackSet_l3ac064 callbacks_c;    // 0x14958
    bool field_0x14998;
};
static_assert(__builtin_offsetof(UnkBossAI_l3ac064, near_height_time) == 0x14724, "layout");
static_assert(__builtin_offsetof(UnkBossAI_l3ac064, field_0x14808) == 0x14808, "layout");
static_assert(__builtin_offsetof(UnkBossAI_l3ac064, life_bar) == 0x148f0, "layout");
static_assert(__builtin_offsetof(UnkBossAI_l3ac064, field_0x14998) == 0x14998, "layout");

void fn_71003ac064(UnkBossAI_l3ac064* self) {
    fn_710036932c(reinterpret_cast<UnkComponent_71004076e4*>(self));
    self->vtable = g_unk_7101a29438 + 2;
    new (&self->field_0x146c8) base::global::CRntVector<void*>(0xf);
    new (&self->landmark_id_0) CStrId(g_unk_710162e2ed, true);
    new (&self->landmark_id_1) CStrId(g_unk_710162e2ed, true);
    self->height_tolerance = 0.0f;
    self->landmark_found_0 = false;
    self->landmark_pos_0 = {};
    self->landmark_found_1 = false;
    self->landmark_pos_1 = {};
    self->near_height_time = 0.0f;
    new (&self->callbacks_a[0]) UnkBossCallbackSet_l3ac064;
    new (&self->callbacks_a[1]) UnkBossCallbackSet_l3ac064;
    new (&self->callbacks_a[2]) UnkBossCallbackSet_l3ac064;
    self->field_0x147e8 = true;
    self->field_0x147e9 = false;
    self->field_0x147ec = 0.0f;
    self->field_0x147f0 = 0.0f;
    self->field_0x147f4 = false;
    self->field_0x147f8 = 0;
    self->field_0x14800 = 0;
    self->field_0x14804 = -1;
    self->field_0x1480c = -1.0f;
    self->field_0x14810 = 0;
    self->field_0x14814 = 0;
    self->field_0x148d8 = false;
    self->field_0x148dc = 0.0f;
    self->field_0x148e0 = false;
    self->field_0x148e4 = 0;
    self->field_0x148e8 = 0;
    fn_710035d4e0(self->life_bar);
    self->field_0x14908 = nullptr;
    new (&self->callbacks_b) UnkBossCallbackSet_l3ac064;
    new (&self->id) UnkEmptyStrId;
    new (&self->callbacks_c) UnkBossCallbackSet_l3ac064;
    self->field_0x14998 = true;
}
