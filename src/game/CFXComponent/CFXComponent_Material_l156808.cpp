// CFXComponent's unit: switches the effects tied to material ids, turning off those of the material the
// owner leaves and on those of the material it enters.
// Names and layouts are placeholders.
#include "game/UnkSwitchObject.h"
#include "types.h"

struct UnkEventOwner_m_ovb0137;
struct UnkStruct_710024fa0c;

void* fn_710094c9ac(UnkEventOwner_m_ovb0137* owner);  // the owner's material source, if any
u32 fn_7100798e88();
u32 fn_7100798e90(void* source);

struct UnkFXMaterialName {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkFXMaterialEffect {
    void** vtable;
    u8 pad_0x8[3];
    u8 kind;  // 0xb
};

struct UnkFXMaterialEntry {
    UnkFXMaterialEffect* effect;
    u8 pad_0x8[0x18];
};

struct UnkFXMaterialEffects {
    UnkFXMaterialEntry* values;
    UnkFXMaterialName** names;
    u32 count;

    UnkFXMaterialEntry* End() { return &values[count]; }
    UnkFXMaterialEntry* Find(u64 hash) {
        for (u64 i = 0; i < count; ++i)
            if (names[i]->hash == hash)
                return &values[i];
        return End();
    }
    UnkFXMaterialEntry* Get(u64 hash) {
        UnkFXMaterialEntry* p = Find(hash);
        return p == End() ? nullptr : p;
    }
};

// An effect name bound to a material id.
struct UnkFXMaterialBinding {
    u32 material;
    UnkFXMaterialName* name;  // 0x8
};

struct UnkFXMaterialGroup {
    UnkFXMaterialBinding* bindings;
    u32 count;  // 0x8
    u8 pad_0xc[0x1c];
};

struct UnkFXMaterialComponent {
    u8 pad_0x0[0x30];
    UnkEventOwner_m_ovb0137* owner;  // 0x30
    u8 pad_0x38[0x18];
    UnkFXMaterialEffects effects;  // 0x50
    u8 pad_0x68[0x10];
    UnkFXMaterialGroup* groups;  // 0x78
    void* group_keys;  // 0x80
    u32 group_count;  // 0x88
};

static inline void SetOn(UnkFXMaterialEffect* effect, bool on) {
    reinterpret_cast<void (*)(UnkFXMaterialEffect*, bool)>(effect->vtable[0x40 / 8])(effect, on);
}

static inline UnkFXMaterialEffect* EffectNamed(UnkFXMaterialComponent* self, UnkFXMaterialName* name) {
    UnkFXMaterialEntry* entry = self->effects.Get(name->hash);
    return entry ? entry->effect : nullptr;
}

void fn_7101156808(UnkStruct_710024fa0c* component) {
    UnkFXMaterialComponent* self = reinterpret_cast<UnkFXMaterialComponent*>(component);
    void* source = fn_710094c9ac(self->owner);
    u32 entered = fn_7100798e88();
    u32 left = fn_7100798e90(source);
    UnkFXMaterialGroup* groupsEnd = &self->groups[self->group_count];
    for (UnkFXMaterialGroup* group = self->groups; group != groupsEnd; ++group) {
        UnkFXMaterialBinding* end = &group->bindings[group->count];
        for (UnkFXMaterialBinding* binding = group->bindings; binding != end; ++binding) {
            if (binding->material == left) {
                UnkFXMaterialEffect* effect = EffectNamed(self, binding->name);
                if (effect) {
                    if (effect->kind == 6)
                        fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(effect), false);
                    else
                        SetOn(effect, false);
                }
            } else if (binding->material == entered) {
                UnkFXMaterialEffect* effect = EffectNamed(self, binding->name);
                if (effect)
                    SetOn(effect, true);
            }
        }
    }
}
