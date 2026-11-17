// CBatalloonAIComponent's unit (tools/tu_map.py unit 583): clears the component's sync state, then copies it from
// the first other Batalloon of the spawn group that is synced or plays animation kind 11.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::reflection::CType;

struct UnkNavComponent;
struct UnkNavGroupInfo;
UnkNavGroupInfo* fn_710110c3d8(UnkNavComponent* component);

struct UnkSwifterMember_ovb0114;
struct UnkSwifterActor_ovb0114;
UnkSwifterActor_ovb0114** fn_71003df2d0(UnkSwifterMember_ovb0114* member);

struct UnkBatalloonOwner_l3dd8b0 {
    u8 pad_0x0[0x220];
    UnkTakumakuComponent* animation;  // +0x220
};

struct UnkBatalloonAI_l3dd8b0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkBatalloonOwner_l3dd8b0* owner;  // +0x30
    u8 pad_0x38[0x15c8];
    u16 field_0x1600;
    u8 field_0x1602;
    u8 pad_0x1603;
    u32 field_0x1604;
    u32 field_0x1608;
    u8 pad_0x160c[0x130cd];
    u8 synced;  // +0x146d9
    u8 pad_0x146da[2];
    u32 field_0x146dc;
    u8 field_0x146e0;

    void vf_0x720(bool flag, f32 value) {
        reinterpret_cast<void (*)(UnkBatalloonAI_l3dd8b0*, bool, f32)>(vtable[0x720 / 8])(this, flag, value);
    }
};
static_assert(__builtin_offsetof(UnkBatalloonAI_l3dd8b0, synced) == 0x146d9, "");

struct UnkBatalloonComponentHolder_l3dd8b0 {
    u8 pad_0x0[0x240];
    UnkBatalloonAI_l3dd8b0* ai;  // +0x240
};

struct UnkBatalloonActorData_l3dd8b0 {
    u8 pad_0x0[0x10];
    UnkBatalloonComponentHolder_l3dd8b0* holder;  // +0x10
};

struct UnkBatalloonActor_l3dd8b0 {
    u8 pad_0x0[0x40];
    UnkBatalloonActorData_l3dd8b0* data;  // +0x40
};

struct UnkBatalloonHandle_l3dd8b0 {
    u8 pad_0x0[0x10];
    UnkBatalloonActor_l3dd8b0* actor;  // +0x10
};

// A spawned member (0x40 bytes).
struct UnkBatalloonMember_l3dd8b0 {
    u8 pad_0x0[0x40];
};

struct UnkBatalloonGroup_l3dd8b0 {
    u8 pad_0x0[0x68];
    UnkBatalloonMember_l3dd8b0* members;  // +0x68
    u32 memberCount;                    // +0x70
};

struct UnkBatalloonGroupSource_l3dd8b0 {
    u8 pad_0x0[0x60];
    u8 part[1];  // +0x60
};

struct UnkBatalloonGroupHandle_l3dd8b0 {
    u8 pad_0x0[0x10];
    void* target;  // +0x10
};

UnkBatalloonGroupSource_l3dd8b0* fn_71003ddc44(UnkNavGroupInfo* info);
UnkBatalloonGroup_l3dd8b0* fn_71003ddd20(void* target);

__attribute__((visibility("hidden"))) extern char g_unk_71726c0118[];

static inline bool IsBatalloonAI(UnkBatalloonAI_l3dd8b0* ai) {
    const CType* type = reinterpret_cast<base::reflection::UnkReflObject*>(ai)->vf_0x0();
    fn_71003dd3e4();
    return fn_710009d50c(type, reinterpret_cast<const CType*>(g_unk_71726c0118));
}

// Whether the Batalloon is synced already or plays animation kind `kind`.
static inline bool IsSyncSource(UnkBatalloonAI_l3dd8b0* ai, u32 kind) {
    UnkTakumakuComponent* animation = ai->owner->animation;
    if (ai->synced != 1 && animation)
        return fn_7101178e6c(animation, kind);
    return ai->synced == 1;
}

void fn_71003dd8b0(UnkBatalloonAI_l3dd8b0* self) {
    self->synced = 0;
    self->field_0x146dc = 0;
    self->field_0x146e0 = 0;

    UnkNavGroupInfo* info = fn_710110c3d8(reinterpret_cast<UnkNavComponent*>(self));
    if (!info)
        return;
    UnkBatalloonGroupSource_l3dd8b0* source = fn_71003ddc44(info);
    if (!source)
        return;
    UnkBatalloonGroupHandle_l3dd8b0* handle = reinterpret_cast<UnkBatalloonGroupHandle_l3dd8b0*>(
        *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(source->part)));
    if (!handle || !handle->target)
        return;
    UnkBatalloonGroup_l3dd8b0* group = fn_71003ddd20(handle->target);
    if (!group)
        return;

    for (UnkBatalloonMember_l3dd8b0 *it = group->members, *end = it + group->memberCount; it != end; ++it) {
        UnkBatalloonHandle_l3dd8b0* member = reinterpret_cast<UnkBatalloonHandle_l3dd8b0*>(*fn_71003df2d0(reinterpret_cast<UnkSwifterMember_ovb0114*>(it)));
        if (!member || !member->actor || !member->actor->data || !member->actor->data->holder)
            continue;
        UnkBatalloonAI_l3dd8b0* component = member->actor->data->holder->ai;
        if (!component)
            continue;
        UnkBatalloonAI_l3dd8b0* other = IsBatalloonAI(component) ? component : nullptr;
        if (other == self || !other)
            continue;
        if (IsSyncSource(other, 11)) {
            self->field_0x146dc = 0;
            self->vf_0x720(true, 0.0f);
            self->field_0x1604 = other->field_0x1604;
            self->field_0x1608 = other->field_0x1608;
            self->field_0x1602 = other->field_0x1602;
            self->field_0x1600 = other->field_0x1600;
            return;
        }
    }
}
