// CWaterNozzleComponent's unit (tools/tu_map.py unit 1352): fn_71008a23fc checks the nozzle's water
// level. The target is a lazily resolved handle (+0x2e8, resolved from +0x2b8 while +0x2e0 is 1, the
// shape of CCharClassWaterPoolComponent_Reset_b51.cpp). When the target's water object
// (fn_71008a1ed0) has a level (fn_7100902abc, a fraction of the target's height from fn_7100d7bc70)
// and the nozzle waits for it (+0x2f8), the owner (+0x30) gets g_unk_710164af50 (fn_7100d7c828) once
// its height plus the offset (+0x2f0, its +0x58) reaches that level. Names and layouts are
// placeholders.
#include "base/UnkRefHandle.h"
#include "base/math/CVector.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkCountedWater_b51 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkRef {
    UnkCountedWater_b51* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

struct UnkEntityBox {
    base::math::CVector3D min;
    base::math::CVector3D max;
};
struct UnkEntityBounds;
UnkEntityBox fn_7100d7bc70(const UnkEntityBounds* self);
f32 fn_7100902abc(void*);
void* fn_71008a1ed0(void* target);
extern u64 g_unk_710164af50;

struct UnkNozzleTarget_ovb0134 {
    u8 pad_0x0[0x54];
    f32 y;  // 0x54
};

struct UnkNozzleOffset_ovb0134 {
    u8 pad_0x0[0x58];
    f32 height;  // 0x58
};

struct UnkWaterNozzleU_ovb0134 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x280];
    u8 source[0x28];  // 0x2b8
    u32 state;        // 0x2e0
    u32 pad_0x2e4;
    base::UnkRefHandle handle;          // 0x2e8
    UnkNozzleOffset_ovb0134* offset;    // 0x2f0
    bool waiting;                       // 0x2f8
};
static_assert(__builtin_offsetof(UnkWaterNozzleU_ovb0134, waiting) == 0x2f8, "layout");

// Resolves the handle if it is still pending.
static inline void Resolve(UnkWaterNozzleU_ovb0134* self) {
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
}

// The target, resolving the handle first if it is still pending.
static inline UnkNozzleTarget_ovb0134* Target(UnkWaterNozzleU_ovb0134* self) {
    Resolve(self);
    auto* c = static_cast<UnkCountedWater_b51*>(self->handle.ptr);
    return static_cast<UnkNozzleTarget_ovb0134*>(c ? c->field_0x10 : nullptr);
}

void fn_71008a23fc(UnkWaterNozzleU_ovb0134* self) {
    if (!Target(self))
        return;
    void* water = fn_71008a1ed0(Target(self));
    if (!water)
        return;
    UnkEntityBox box = fn_7100d7bc70(reinterpret_cast<const UnkEntityBounds*>(Target(self)));
    f32 fraction = fn_7100902abc(water);
    f32 top = box.max.y;
    f32 bottom = box.min.y;
    Resolve(self);
    if (!self->waiting)
        return;
    auto* target = static_cast<UnkNozzleTarget_ovb0134*>(
        static_cast<UnkCountedWater_b51*>(self->handle.ptr)->field_0x10);
    f32 level = fraction * (top - bottom) + target->y;
    UnkActor* owner = self->owner;
    if (reinterpret_cast<UnkNozzleTarget_ovb0134*>(owner)->y + self->offset->height >= level) {
        fn_7100d7c828(owner, &g_unk_710164af50, false);
        self->waiting = false;
    }
}
