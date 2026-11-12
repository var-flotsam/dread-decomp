// CCharClassSteamJetComponent unit (tools/tu_map.py): fn_710089a600 passes a progress to the jet's two
// lists of parts (+0x90, count +0x98: vtable slot 0x168; +0xb8, count +0xc0: slot 0x170 with 1 - progress)
// and, once it reaches 1, stores true under the property g_unk_717272c420 in the current scenario's
// blackboard section (keys at +0x1d0, count +0x1d8, values at +0x1c8; lookup and setter as in
// src/game/CActionSwitcherComponent/CActionSwitcherComponent_b48.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CCharClassShootActivatorComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

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

struct UnkSteamJetPart_b49;
struct UnkSteamJetPartVtable_b49 {
    u8 pad_0x0[0x168];
    void (*vf_0x168)(UnkSteamJetPart_b49* self, f32 value);
    void (*vf_0x170)(UnkSteamJetPart_b49* self, f32 value);
};
struct UnkSteamJetPart_b49 {
    UnkSteamJetPartVtable_b49* vtable;
};

struct UnkSteamJetParts_b49 {
    UnkSteamJetPart_b49** data;
    u32 count;
};
// The element at `index` (the vector's grow-on-index access).
UnkSteamJetPart_b49** fn_71008921a0(UnkSteamJetParts_b49* self, u32 index);

struct UnkSteamJetValue_b49 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkSteamJet_b49 {
    u8 pad_0x0[0x90];
    UnkSteamJetParts_b49 parts_a;
    u8 pad_0xa0[0x18];
    UnkSteamJetParts_b49 parts_b;
    u8 pad_0xc8[0x100];
    UnkSteamJetValue_b49* values;
    u64* keys;
    u32 count;
};

void fn_710089a600(UnkSteamJet_b49* self, f32 progress) {
    for (u32 i = 0; i < self->parts_a.count; ++i) {
        UnkSteamJetPart_b49* part = *fn_71008921a0(&self->parts_a, i);
        part->vtable->vf_0x168(part, progress);
    }
    for (u32 i = 0; i < self->parts_b.count; ++i) {
        UnkSteamJetPart_b49* part = *fn_71008921a0(&self->parts_b, i);
        part->vtable->vf_0x170(part, 1.0f - progress);
    }
    if (progress == 1.0f) {
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
        UnkSteamJetValue_b49* v = self->values;
        bool value = true;
        SetSectionBool(blackboard, section_id, &v[index].name, &value);
    }
}
