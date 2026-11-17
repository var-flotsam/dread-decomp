// CCharClassShootActivatorComponent unit: turns the elapsed time into a progress, tints the indicator by it,
// passes it to both lists of parts and, at either end, finishes or resets the activator (storing true under
// g_unk_717272c420 in the scenario's blackboard section once complete). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CCharClassShootActivatorComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

// The "ACTIVATABLE" component of an object, or null.
struct UnkRefl_fn_71008922f0;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_71008922f0>(void*);

struct UnkSteamJetPart_b49;
struct UnkSteamJetParts_b49;
// The element at `index` (the vector's grow-on-index access).
UnkSteamJetPart_b49** fn_71008921a0(UnkSteamJetParts_b49* self, u32 index);

extern const u64 g_unk_710164aee0;
extern const u64 g_unk_710164aee8;

struct UnkShootPart_l891ddc {
    void** vtable;
    u8 pad_0x8[0x10];
    void* entity;  // 0x18

    void vf_0x168(f32 value) { reinterpret_cast<void (*)(UnkShootPart_l891ddc*, f32)>(vtable[0x168 / 8])(this, value); }
    void vf_0x170(f32 value) { reinterpret_cast<void (*)(UnkShootPart_l891ddc*, f32)>(vtable[0x170 / 8])(this, value); }
};

struct UnkShootParts_l891ddc {
    UnkShootPart_l891ddc** data;
    u32 count;
    u8 pad_0xc[0x1c];
};
static_assert(sizeof(UnkShootParts_l891ddc) == 0x28, "UnkShootParts layout");

static inline UnkShootPart_l891ddc* PartAt(UnkShootParts_l891ddc* parts, u32 index) {
    return reinterpret_cast<UnkShootPart_l891ddc*>(
        *fn_71008921a0(reinterpret_cast<UnkSteamJetParts_b49*>(parts), index));
}

struct UnkShootTint_l891ddc {
    u8 pad_0x0[0x30];
    base::math::CVector4D* color;  // 0x30
};

struct UnkShootIndicator_l891ddc {
    u8 pad_0x0[0x14c];
    base::math::CVector4D color;  // 0x14c
    u8 pad_0x15c[0x1f0 - 0x15c];
    UnkShootTint_l891ddc* tint;  // 0x1f0
};

struct UnkShootValue_l891ddc {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkShootActivator_l891ddc {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x3d8 - 0x38];
    UnkShootParts_l891ddc parts_a;  // 0x3d8
    UnkShootParts_l891ddc parts_b;  // 0x400
    u8 pad_0x428[0x448 - 0x428];
    UnkShootIndicator_l891ddc* indicator;  // 0x448
    u8 pad_0x450[0x488 - 0x450];
    UnkShootValue_l891ddc* values;  // 0x488
    u64* keys;  // 0x490
    u32 count;  // 0x498
    u8 pad_0x49c[0x4b0 - 0x49c];
    f32 duration;  // 0x4b0
    u8 pad_0x4b4[8];
    f32 time;  // 0x4bc

    void vf_0x1f8(bool done) { reinterpret_cast<void (*)(UnkShootActivator_l891ddc*, bool)>(vtable[0x1f8 / 8])(this, done); }
    void vf_0x208(f32 progress) {
        reinterpret_cast<void (*)(UnkShootActivator_l891ddc*, f32)>(vtable[0x208 / 8])(this, progress);
    }
    void vf_0x210() { reinterpret_cast<void (*)(UnkShootActivator_l891ddc*)>(vtable[0x210 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkShootActivator_l891ddc, time) == 0x4bc, "UnkShootActivator layout");

static inline bool IsActivatable(UnkShootPart_l891ddc* part) {
    return base::reflection::refl_FindTyped<UnkRefl_fn_71008922f0>(part->entity) != nullptr;
}

// Green to yellow over the first half of the progress, yellow to red over the second.
static inline base::math::CVector4D TintFor(f32 progress) {
    if (progress > 0.5f)
        return {1.0f, 1.0f - (progress - 0.5f) * 2.0f, 0.0f, 1.0f};
    return {progress * 2.0f, 1.0f, 0.0f, 1.0f};
}

void fn_7100891ddc(UnkShootActivator_l891ddc* self) {
    f32 progress = self->time / self->duration;
    if (progress <= 0.0f)
        progress = 0.0f;
    else if (progress >= 1.0f)
        progress = 1.0f;

    base::math::CVector4D color = TintFor(progress);
    UnkShootIndicator_l891ddc* indicator = self->indicator;
    fn_710007b518(&indicator->color, &color);
    if (indicator->tint)
        *indicator->tint->color = color;
    self->vf_0x208(progress);

    for (u32 i = 0; i < self->parts_a.count; ++i) {
        if (!IsActivatable(PartAt(&self->parts_a, i)) || progress >= 1.0f)
            PartAt(&self->parts_a, i)->vf_0x168(progress);
    }
    for (u32 i = 0; i < self->parts_b.count; ++i) {
        if (!IsActivatable(PartAt(&self->parts_b, i)) || progress >= 1.0f)
            PartAt(&self->parts_b, i)->vf_0x170(1.0f - progress);
    }

    if (progress >= 1.0f) {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        u64 index = 0;
        u32 n = self->count;
        if (n) {
            u64* k = self->keys;
            u64 key = (u64)g_unk_717272c420.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        UnkShootValue_l891ddc* v = self->values;
        bool value = true;
        SetSectionBool(blackboard, section_id, &v[index].name, &value);
        fn_7100d7c828(self->owner, &g_unk_710164aee0, false);
        self->vf_0x1f8(true);
    } else if (progress <= 0.0f) {
        self->vf_0x210();
        fn_7100d7c828(self->owner, &g_unk_710164aee8, false);
        self->vf_0x1f8(false);
    }
}
