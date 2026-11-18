// CQuetzoaEnergyWaveAttack (tools/tu_map.py unit 968): whether the energy wave may start. The owner's AI
// component must be a CQuetzoaAIComponent, with a target and a character class; past the base check
// (fn_7100360b84), the wave must find an aim from its spawn point (fn_7100645568's offset) and be able to
// travel along it (fn_71006457c4); the AI's object at +0x14a20 must be idle and the owner's navigation, if
// any, must be at rest (its first record in state 3) with an entry at +0x18. Names and layouts are
// placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkVec3_ovb0102 {
    f32 x, y, z;
};

struct UnkEnergyWaveAttack_ovb0102 {
    void** vtable;
    UnkActor* owner;  // +0x8
    u8 pad_0x10[0x20];
    UnkVec3_ovb0102 dir;  // +0x30
};

static inline f32 Sign(f32 f) {
    return (f32)((s32)(f > 0.0f) - (s32)(f < 0.0f));
}

// The direction from the wave's origin (115 units ahead along the facing sign, 20 up from `pos`)
// to the owner's target, mirrored by the facing; false when there is no target or the angle is more
// than 70 degrees off the horizontal.
static inline bool Aim(UnkEnergyWaveAttack_ovb0102* self, const UnkVec3_ovb0102* pos, const f32* facing,
                       UnkVec3_ovb0102* dir) {
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return false;
    f32 sign = Sign(*facing);
    f32 x = pos->x + sign * 115.0f;
    f32 y = pos->y + 20.0f;
    f32 tx = target->field_0x44;
    f32 ty = target->field_0x54;
    f32 dx = tx - x;
    f32 dy = ty - y;
    f32 angle = atan2f(dy, dx * sign);
    if (!(angle >= -1.2217305f && angle <= 1.2217305f))
        return false;
    f32 c = cosf(angle) * sign;
    f32 s = sinf(angle);
    dir->x = c;
    dir->y = s;
    dir->z = 0.0f;
    return true;
}

static inline void Offset(UnkVec3_ovb0102* pos, const f32* facing) {
    f32 sign = Sign(*facing);
    UnkVec3_ovb0102 offset = {sign * -211.0f, 78.0f, 0.0f};
    pos->x += offset.x;
    pos->y += offset.y;
    pos->z += offset.z;
}

// Whether the wave can travel from `pos` along `dir`.
bool fn_71006457c4(UnkEnergyWaveAttack_ovb0102* self, const UnkVec3_ovb0102* pos, const UnkVec3_ovb0102* dir);

// What the AI is busy with (+0x14a20 of the component).
struct UnkQuetzoaTask_l645a08 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Refresh();  // vf_0x10
    u32 ready;               // 0x8
    void* current;           // 0x10
};

struct UnkQuetzoaAI_l645a08 {
    u8 pad_0x0[0x14a20];
    UnkQuetzoaTask_l645a08* task;  // 0x14a20
};

struct UnkQuetzoaNavState_l645a08 {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};

struct UnkQuetzoaNavRecord_l645a08 {
    u8 pad_0x0[0x10];
    UnkQuetzoaNavState_l645a08* state;  // 0x10
};

struct UnkQuetzoaNavList_l645a08 {
    UnkQuetzoaNavRecord_l645a08* first;  // 0x0
    u8 pad_0x8[0x10];
    void* field_0x18;
};

struct UnkQuetzoaNavigation_l645a08 {
    u8 pad_0x0[0xb0];
    UnkQuetzoaNavList_l645a08* records;  // 0xb0
};

bool fn_7100645a08(UnkEnergyWaveAttack_ovb0102* self, void* arg, u32 mode) {
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_7100649630()))
        ai = nullptr;
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!ai || !target || !self->owner->field_0x220)
        return false;
    if (!fn_7100360b84(reinterpret_cast<CAttack*>(self), arg, mode))
        return false;

    UnkActor* owner = self->owner;
    f32 x = owner->field_0x44;
    f32 y = owner->field_0x54;
    f32 z = owner->field_0x64;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    UnkVec3_ovb0102 dir = {m.m[2], m.m[6], m.m[10]};
    UnkVec3_ovb0102 pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    Offset(&pos, &dir.x);
    if (!Aim(self, &pos, &dir.x, &dir) || !fn_71006457c4(self, &pos, &dir))
        return false;

    if (UnkQuetzoaTask_l645a08* task = reinterpret_cast<UnkQuetzoaAI_l645a08*>(ai)->task) {
        u32 ready = task->ready;
        void* current = task->current;
        if (!ready)
            task->Refresh();
        if (current)
            return false;
    }
    UnkQuetzoaNavigation_l645a08* navigation = static_cast<UnkQuetzoaNavigation_l645a08*>(self->owner->field_0x298);
    if (!navigation)
        return true;
    UnkQuetzoaNavList_l645a08* records = navigation->records;
    if (!records->first || !records->first->state || records->first->state->kind != 3)
        return false;
    return records->field_0x18 != nullptr;
}
