// CHeatableShieldComponent unit (unit 1322): heats the shield entries a hit reaches. Each enabled entry whose
// condition tunable holds and whose targets include the hit's source starts heating and gains the hit's heat
// (scaled by a tunable factor and the owner's modifier), capped at the hit's maximum; the first entry left
// under 1 is switched off and 12 returned. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkTunableRegistry.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkTunableRegistry;

void fn_710013c7e0(UnkTunableRegistry* registry, const char* group, const char* name, f32* value);
void fn_710013c78c(UnkTunableRegistry* registry, const char* group, const char* name, bool* value);

struct UnkHeatDefaults_l86bf60 {
    u8 pad_0x0[0x60];
    f32 factor;  // +0x60
};
extern UnkHeatDefaults_l86bf60 g_unk_7172729300;

struct UnkHeatTarget_l86bf60 {
    u8 pad_0x0[0x30];
    void* id;  // +0x30
};

struct UnkHeatSource_l86bf60 {
    u8 pad_0x0[0x18];
    void* id;  // +0x18
};

struct UnkHeatableShieldEntry_l86bf60 {
    u8 pad_0x0[0x20];
    UnkHeatTarget_l86bf60** targets;  // +0x20
    u32 target_count;                 // +0x28
    u8 pad_0x2c[0x50 - 0x2c];
    f32 heat;          // +0x50
    u32 field_0x54;    // +0x54
    f32 start_heat;    // +0x58
    f32 max_heat;      // +0x5c
    u32 field_0x60;    // +0x60
    u8 pad_0x64[0x78 - 0x64];
    CStrId group;      // +0x78
    CStrId name;       // +0x80
    u8 pad_0x88[0x98 - 0x88];
    CStrId condition;  // +0x98
    bool heating;      // +0xa0
    bool cooled;       // +0xa1
    bool enabled;      // +0xa2
    u8 pad_0xa3[0xd0 - 0xa3];

    UnkHeatTarget_l86bf60** begin() const { return targets; }
    UnkHeatTarget_l86bf60** end() const { return targets + target_count; }
};
static_assert(sizeof(UnkHeatableShieldEntry_l86bf60) == 0xd0, "UnkHeatableShieldEntry_l86bf60");


struct UnkHeatHit_l86bf60 {
    u8 pad_0x0[0x60];
    UnkHeatSource_l86bf60* source;  // +0x60
    u8 pad_0x68[0xc0 - 0x68];
    f32 max_heat;  // +0xc0
    f32 rate;      // +0xc4
};

struct UnkHeatModifier_l86bf60 {
    void** vtable;

    void Modify(UnkHeatableShieldEntry_l86bf60* entry, f32* factor) {
        reinterpret_cast<void (*)(UnkHeatModifier_l86bf60*, UnkHeatableShieldEntry_l86bf60*, f32*)>(
            vtable[0x6d8 / 8])(this, entry, factor);
    }
};

struct UnkHeatOwner_l86bf60 {
    u8 pad_0x0[0x240];
    UnkHeatModifier_l86bf60* modifier;  // +0x240
};

struct UnkHeatableShield_l86bf60 {
    u8 pad_0x0[0x20];
    bool active;  // +0x20
    u8 pad_0x21[0x30 - 0x21];
    UnkHeatOwner_l86bf60* owner;  // +0x30
    u8 pad_0x38[0x40 - 0x38];
    UnkHeatableShieldEntry_l86bf60* entries;  // +0x40
    u8 pad_0x48[0x50 - 0x48];
    u32 entry_count;  // +0x50
};

inline bool IsSet(const CStrId& id) {
    base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
    return id.data != empty;
}

static inline UnkTunableRegistry* Registry() {
    UnkTunableRegistry* registry = g_unk_7101d47350;
    if (!registry) {
        registry = static_cast<UnkTunableRegistry*>(fn_71000839a0(0x58, 0x3f));
        fn_710013ac14(registry);
        g_unk_7101d47350 = registry;
    }
    return registry;
}

u32 fn_710086bf60(UnkHeatableShield_l86bf60* self, UnkHeatHit_l86bf60* hit) {
    if (!self->active || self->entry_count == 0)
        return 0;
    UnkHeatableShieldEntry_l86bf60* end = self->entries + self->entry_count;
    for (UnkHeatableShieldEntry_l86bf60* e = self->entries; e != end; ++e) {
        if (e->cooled || !e->enabled)
            continue;
        if (IsSet(e->condition)) {
            bool holds = true;
            UnkTunableRegistry* registry = Registry();
            const char* group = e->group.Text();
            const char* name = e->condition.Text();
            fn_710013c78c(registry, group, name, &holds);
            if (!holds)
                continue;
        }
        if (!hit->source)
            continue;
        bool targeted = false;
        for (UnkHeatTarget_l86bf60* target : *e) {
            if (target->id == hit->source->id) {
                targeted = true;
                break;
            }
        }
        if (!targeted)
            continue;
        e->field_0x60 = 0;
        e->heating = true;
        e->field_0x54 = 0;
        e->start_heat = e->heat;
        e->max_heat = hit->max_heat;
        f32 heat = e->heat;
        f32 max_heat = hit->max_heat;
        if (max_heat > heat) {
            f32 rate = hit->rate;
            f32 factor = g_unk_7172729300.factor;
            if (IsSet(e->group) && IsSet(e->name)) {
                UnkTunableRegistry* registry = Registry();
                const char* group = e->group.Text();
                const char* name = e->name.Text();
                fn_710013c7e0(registry, group, name, &factor);
            }
            if (self->owner && self->owner->modifier)
                self->owner->modifier->Modify(e, &factor);
            heat = e->heat + max_heat * rate * factor;
            e->heat = heat;
            if (heat > hit->max_heat) {
                heat = hit->max_heat;
                e->heat = heat;
            }
        }
        if (heat < 1.0f) {
            e->cooled = false;
            return 12;
        }
    }
    return 0;
}
