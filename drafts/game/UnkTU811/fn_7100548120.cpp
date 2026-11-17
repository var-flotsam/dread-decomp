// CEmmyAIComponent's unit (tools/tu_map.py unit 811): the E.M.M.I. AI's reset. After the base reset it
// leaves state 1, sends the owner's character class a fixed request, registers itself with the managers,
// clears its targeting state, re-registers its two sets and creates its scene object. Names and layouts
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkDeferredSet.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CEmmyOverrideDeathPositionDef.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU140.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU362.h"
#include "game/UnkTU363.h"
#include "game/UnkTU39.h"
#include "game/UnkTU794.h"
#include "game_s2/UnkChargeComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::UnkDeferredSet;

struct UnkAIModeOwner;
struct UnkHeatableShieldMap_A5_b66;
struct UnkSceneNode;
struct UnkSceneObjectSub;
struct UnkNavTarget_l548120;
struct UnkNavPoint_l548120;

void fn_710086c7a8(UnkHeatableShieldMap_A5_b66* self, const u64* key, bool value);
void fn_7100dac964(UnkEmmyRegistry_b55* registry, void* ai);
void fn_71010ff50c(UnkAIModeOwner* self, bool flag, u32 value);
void fn_7100287904(UnkNavTarget_l548120* self, const UnkNavPoint_l548120* point);
void fn_7100148130(UnkSceneObject* self, u32 index, u32 a, u32 b, u32 c);
void fn_71001bfd44(UnkSceneObjectSub* self, u32 index, u32 value);
extern char g_unk_71726e1f90[] __attribute__((visibility("hidden")));
extern u64 g_unk_71726e1f60 __attribute__((visibility("hidden")));

// A navigation point (fn_7100287598 builds an empty one).
struct UnkNavPoint_l548120 {
    u32 raw[3];
    UnkNavPoint_l548120() { fn_7100287598(reinterpret_cast<UnkNavMeshHit*>(this)); }
    ~UnkNavPoint_l548120() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }
};

// A navigation target: a cell and side and a position, built from a point.
struct UnkNavTarget_l548120 {
    u16 cell;
    u8 side;
    u32 x;
    u32 y;
    u32 z;
    explicit UnkNavTarget_l548120(const UnkNavPoint_l548120& point) { fn_7100287904(this, &point); }
    ~UnkNavTarget_l548120() { fn_7100287954(reinterpret_cast<UnkMsg_b152*>(this)); }
    UnkNavTarget_l548120& operator=(const UnkNavTarget_l548120& other) {
        cell = other.cell;
        side = other.side;
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};
static_assert(sizeof(UnkNavTarget_l548120) == 0x10, "UnkNavTarget_l548120 layout");

struct UnkEmmyFlagged_l548120 {
    u8 pad_0x0[0x8];
    bool flag;
};

struct UnkEmmyState_l548120 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual UnkEmmyFlagged_l548120* vf_0xd8();
};

// The charge tunable's word at +0x78.
struct UnkEmmyTunable_l548120 {
    u8 pad_0x0[0x78];
    u32 field_0x78;
};

// A set and an entry registered in a set (its own one, after a reset).
struct UnkEmmySet_l548120 {
    UnkDeferredSet<void> set;
    UnkDeferredSet<void>* entry;  // 0x80: the set the entry is in

    // Moves the entry from the set it is in to this one.
    void Reregister() {
        if (entry) {
            fn_71004f3ea4(entry, &entry);
            entry = nullptr;
        }
        entry = &set;
        fn_71004f3cf0(&set, &entry);
    }
};

struct UnkEmmyBarrier_l548120 {
    bool up;
};

struct UnkEmmyAI_l548120 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14779 - 0x38];
    bool field_0x14779;
    u8 pad_0x1477a[0x14b80 - 0x1477a];
    bool field_0x14b80;
    f32 field_0x14b84[3];
    f32 field_0x14b90[3];
    u8 pad_0x14b9c[0x14ba8 - 0x14b9c];
    u64 field_0x14ba8;
    u32 field_0x14bb0;
    u8 pad_0x14bb4[0x4];
    u64 field_0x14bb8;
    UnkNavTarget_l548120 nav_target;  // 0x14bc0
    u8 pad_0x14bd0[0x14bf0 - 0x14bd0];
    UnkSceneObject* scene;  // 0x14bf0
    f32 field_0x14bf8;
    u8 pad_0x14bfc[0x14c24 - 0x14bfc];
    f32 field_0x14c24;
    u8 pad_0x14c28[0x14c38 - 0x14c28];
    u64 field_0x14c38;
    u8 pad_0x14c40[0x14c48 - 0x14c40];
    u32 field_0x14c48;
    u32 field_0x14c4c;
    u32 field_0x14c50;
    u8 pad_0x14c54[0x14c58 - 0x14c54];
    u16 field_0x14c58;
    u8 pad_0x14c5a[0x14c60 - 0x14c5a];
    UnkWeakHandle* handle;  // 0x14c60
    u8 pad_0x14c68[0x27720 - 0x14c68];
    UnkEmmyBarrier_l548120 barrier;  // 0x27720
    u8 pad_0x27721[0x29380 - 0x27721];
    UnkEmmySet_l548120 set_a;  // 0x29380
    u8 pad_0x29408[0x29448 - 0x29408];
    UnkEmmySet_l548120 set_b;  // 0x29448
    u8 pad_0x294d0[0x29ee0 - 0x294d0];
    UnkEmmyState_l548120* state;  // 0x29ee0
    u8 pad_0x29ee8[0x29eec - 0x29ee8];
    u32 field_0x29eec;
    s32 field_0x29ef0;
    u8 pad_0x29ef4[0x2a250 - 0x29ef4];
    u32 field_0x2a250;
    u8 pad_0x2a254[0x2a258 - 0x2a254];
    bool field_0x2a258;
    u8 pad_0x2a259[0x2a368 - 0x2a259];
    u16 field_0x2a368;
    u8 pad_0x2a36a[0x2a36c - 0x2a36a];
    u32 field_0x2a36c;
    u8 pad_0x2a370[0x2a378 - 0x2a370];
    u8 field_0x2a378[0x2a6b4 - 0x2a378];
    u32 field_0x2a6b4;

    void vf_0x850() { reinterpret_cast<void (*)(UnkEmmyAI_l548120*)>(vtable[0x850 / 8])(this); }
    void vf_0x858() { reinterpret_cast<void (*)(UnkEmmyAI_l548120*)>(vtable[0x858 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEmmyAI_l548120, nav_target) == 0x14bc0, "UnkEmmyAI_l548120 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l548120, handle) == 0x14c60, "UnkEmmyAI_l548120 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l548120, set_b) == 0x29448, "UnkEmmyAI_l548120 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l548120, field_0x2a6b4) == 0x2a6b4, "UnkEmmyAI_l548120 layout");


static inline UnkSceneNode* Node_l548120(UnkSceneObject* object) {
    return object ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(object) + 8) : nullptr;
}

void fn_7100548120(UnkEmmyAI_l548120* self) {
    fn_71003695b8(reinterpret_cast<UnkBehaviorTreeAI*>(self));
    fn_710002fa90(self->field_0x2a378);
    self->field_0x29eec = 0;
    if (self->field_0x29ef0) {
        self->field_0x29ef0 = 0;
        self->vf_0x850();
    }
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    if (void* track = self->owner->field_0x220) {
        mode.id = 0x80;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(track, 0xe27a4a9050c8ea64, 0x7c5476647f60cdaa, &mode, &g_unk_710164b958, &g_unk_717276974c,
                      &mask);
    }
    if (UnkDoorLifePart_b56* part = fn_71005012bc(reinterpret_cast<UnkDoorLifeOwner_b56*>(self->owner)))
        fn_710086c7a8(reinterpret_cast<UnkHeatableShieldMap_A5_b66*>(part),
                      reinterpret_cast<const u64*>(g_unk_71726e1f90), true);
    fn_7100dac964(g_unk_71726af0a8, self);
    fn_7100de9540(g_unk_7101d47310, &g_unk_71726e1f60, reinterpret_cast<UnkWeakTarget*>(self));

    self->field_0x14bf8 = 3.40282347e+38f;
    self->field_0x14b80 = false;
    self->field_0x14b84[0] = 0.0f;
    self->field_0x14b84[1] = 0.0f;
    self->field_0x14b84[2] = 0.0f;
    self->field_0x14b90[0] = -3.40282347e+38f;
    self->field_0x14b90[1] = -3.40282347e+38f;
    self->field_0x14b90[2] = -3.40282347e+38f;
    self->field_0x14ba8 = 0;
    self->field_0x14bb0 = 0;
    self->field_0x14bb8 = 0;
    {
        UnkNavPoint_l548120 point;
        self->nav_target = UnkNavTarget_l548120(point);
    }
    self->field_0x14c24 = -3.40282347e+38f;
    self->field_0x14c48 = g_unk_71726e1ee8;
    self->field_0x14c4c = 0;
    self->field_0x14c50 = 0;
    self->field_0x14c58 = 0;
    AssignRef(&self->handle, GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(self)));
    self->barrier.up = false;
    self->vf_0x858();
    self->field_0x2a250 = reinterpret_cast<const UnkEmmyTunable_l548120*>(&g_unk_71726e0aa8)->field_0x78;
    self->set_a.Reregister();
    self->set_b.Reregister();
    self->state->vf_0xd8()->flag = true;
    self->field_0x14779 = true;
    fn_71010ff50c(reinterpret_cast<UnkAIModeOwner*>(self), false, 0);
    self->field_0x14c38 = 0;
    self->field_0x2a258 = false;
    self->field_0x2a368 = 0;
    self->field_0x2a36c = 0;

    UnkRenderManager* render = g_unk_7101d47300;
    UnkSceneObject* scene = nullptr;
    fn_7100149930(render->field_0x218, nullptr, &scene, &g_unk_7101d470e0);
    fn_7100219ae0(render, Node_l548120(scene), 0);
    self->scene = scene;
    fn_7100148130(self->scene, 0, 4, 6, 0x1c);
    fn_7100148130(self->scene, 1, 4, 6, 0x1c);
    fn_71001bfd44(self->scene->field_0x130, 0, 3);
    fn_71001bfd44(self->scene->field_0x130, 1, 3);
    fn_7100149490(self->scene, false);
    self->field_0x2a6b4 = 0;
}
