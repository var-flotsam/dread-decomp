// CFXComponent's unit: switches the effects of a named group that are bound to the owner's current
// material on (attaching scene-bound ones to the owner's scene) or off (also those of the material it
// left), restarting them when asked.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CCollisionMaterialCacheComponent.h"
#include "game/UnkTU132.h"
#include "game/UnkTU1428.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkEventOwner_m_ovb0137;

struct UnkFXGroupName {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkFXGroupEffect {
    void** vtable;
    u8 pad_0x8[3];
    u8 kind;  // 0xb
    u8 pad_0xc[0x10d];
    u8 on;  // 0x119
};
static_assert(__builtin_offsetof(UnkFXGroupEffect, on) == 0x119, "UnkFXGroupEffect layout");

struct UnkFXGroupEntry {
    UnkFXGroupEffect* effect;
    u8 pad_0x8[0x18];
};

struct UnkFXGroupEffects {
    UnkFXGroupEntry* values;
    UnkFXGroupName** names;
    u32 count;

    UnkFXGroupEntry* End() { return &values[count]; }
    UnkFXGroupEntry* Find(u64 hash) {
        for (u64 i = 0; i < count; ++i)
            if (names[i]->hash == hash)
                return &values[i];
        return End();
    }
    UnkFXGroupEntry* Get(u64 hash) {
        UnkFXGroupEntry* p = Find(hash);
        return p == End() ? nullptr : p;
    }
};

// An effect name bound to a material id.
struct UnkFXGroupBinding {
    u32 material;
    UnkFXGroupName* name;  // 0x8
};

struct UnkFXGroup {
    UnkFXGroupBinding* bindings;
    u32 count;  // 0x8
    u8 pad_0xc[0x1c];

    UnkFXGroupBinding* begin() { return bindings; }
    UnkFXGroupBinding* end() { return &bindings[count]; }
};

struct UnkFXGroups {
    UnkFXGroup* values;
    UnkStringData** keys;
    u32 count;

    UnkFXGroup* End() { return &values[count]; }
    UnkFXGroup* Find(UnkStringData* const& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
    UnkFXGroup* Get(UnkStringData* const& key) {
        UnkFXGroup* p = Find(key);
        return p == End() ? nullptr : p;
    }
};

struct UnkFXGroupScene {
    u8 pad_0x0[0x50];
    void* scene;  // 0x50
};

struct UnkFXGroupOwner {
    u8 pad_0x0[0x228];
    UnkFXGroupScene* field_0x228;
};

struct UnkFXGroupComponent {
    u8 pad_0x0[0x30];
    UnkFXGroupOwner* owner;  // 0x30
    u8 pad_0x38[0x18];
    UnkFXGroupEffects effects;  // 0x50
    u8 pad_0x68[0x10];
    UnkFXGroups groups;  // 0x78
};

struct UnkFXGroupMessage {
    u8 pad_0x0[0x10];
    UnkStringData* group;  // 0x10
    u8 on;  // 0x18
    u8 restart;  // 0x19
};

static inline void SetOn(UnkFXGroupEffect* effect, bool on) {
    reinterpret_cast<void (*)(UnkFXGroupEffect*, bool)>(effect->vtable[0x40 / 8])(effect, on);
}

static inline void Restart(UnkFXGroupEffect* effect) {
    reinterpret_cast<void (*)(UnkFXGroupEffect*)>(effect->vtable[0x118 / 8])(effect);
}

static inline UnkFXGroupEffect* EffectNamed(UnkFXGroupComponent* self, UnkFXGroupName* name) {
    UnkFXGroupEntry* entry = self->effects.Get(name->hash);
    return entry ? entry->effect : nullptr;
}

struct UnkFXComponent_b0052;

void fn_71011553d0(UnkFXComponent_b0052* component, UnkFXGroupMessage* msg) {
    UnkFXGroupComponent* self = reinterpret_cast<UnkFXGroupComponent*>(component);
    UnkFXGroup* group = self->groups.Get(msg->group);
    void* source = fn_710094c9ac(reinterpret_cast<UnkEventOwner_m_ovb0137*>(self->owner));
    if (!group || !source)
        return;
    u8 on = msg->on;
    u8 restart = msg->restart;
    u32 entered = fn_7100798e88();
    if (on) {
        for (UnkFXGroupBinding& b : *group) {
            if (b.material != entered)
                continue;
            UnkFXGroupEffect* effect = EffectNamed(self, b.name);
            if (!effect)
                continue;
            if (effect->kind == 4 && !effect->on) {
                if (void* scene = self->owner->field_0x228->scene)
                    fn_71001457fc(effect, scene, 1);
            }
            SetOn(effect, true);
            if (restart)
                Restart(effect);
        }
    } else {
        u32 left = fn_7100798e90(source);
        for (UnkFXGroupBinding& b : *group) {
            if (b.material != entered && b.material != left)
                continue;
            UnkFXGroupEffect* effect = EffectNamed(self, b.name);
            if (!effect)
                continue;
            if (effect->kind == 4 && effect->on) {
                if (void* scene = self->owner->field_0x228->scene)
                    fn_710014580c(effect, scene);
            }
            SetOn(effect, false);
            if (restart)
                Restart(effect);
        }
    }
}
