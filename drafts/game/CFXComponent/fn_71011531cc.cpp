// CFXComponent's unit: with a positive time, records it and the direction for later; otherwise attaches
// the component to the owner's scene on one of the two last overlay models (by the direction's sign),
// optionally restarting both models and switching on the named effect for that side.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CActorComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU132.h"
#include "game/UnkTU134.h"
#include "game/UnkTU247.h"
#include "game/UnkTU285.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkFXTriggerSceneHolder {
    u8 pad_0x0[0x50];
    void* scene;  // 0x50
};

struct UnkFXTriggerRider {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkFXTriggerActor {
    u8 pad_0x0[0x1b5];
    bool disabled;  // 0x1b5
    u8 pad_0x1b6[2];
    UnkFXTriggerRider* rider;  // 0x1b8
    u8 pad_0x1c0[0x68];
    UnkFXTriggerSceneHolder* scene_holder;  // 0x228
};
static_assert(__builtin_offsetof(UnkFXTriggerActor, scene_holder) == 0x228, "UnkFXTriggerActor layout");

struct UnkFXTriggerSub {
    u8 pad_0x0[0x119];
    bool disabled;  // 0x119
};

struct UnkFXTriggerEffect {
    void** vtable;
    u8 pad_0x8[3];
    u8 kind;  // 0xb
    u8 pad_0xc[0x1c];
    u8 field_0x28[0xe0];
    f32 field_0x108;
    u8 pad_0x10c[0x4e];
    bool field_0x15a;
};
static_assert(__builtin_offsetof(UnkFXTriggerEffect, field_0x15a) == 0x15a, "UnkFXTriggerEffect layout");

struct UnkFXTriggerEntry {
    UnkFXTriggerEffect* effect;
    u8 pad_0x8[0x18];
};

struct UnkFXTriggerName {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkFXTriggerEffects {
    UnkFXTriggerEntry* values;
    UnkFXTriggerName** names;
    u32 count;

    UnkFXTriggerEntry* End() { return &values[count]; }
    UnkFXTriggerEntry* Find(u64 hash) {
        for (u64 i = 0; i < count; ++i)
            if (names[i]->hash == hash)
                return &values[i];
        return End();
    }
    UnkFXTriggerEntry* Get(u64 hash) {
        UnkFXTriggerEntry* p = Find(hash);
        return p != End() ? p : nullptr;
    }
};

struct UnkFXTriggerModel {
    u8 pad_0x0[0x130];
    f32 field_0x130;
    u8 pad_0x134[0x18];
    bool field_0x14c;
    u8 pad_0x14d[3];
    UnkResetOwner* field_0x150;
};
static_assert(__builtin_offsetof(UnkFXTriggerModel, field_0x150) == 0x150, "UnkFXTriggerModel layout");

struct UnkFXTriggerComponent {
    u8 pad_0x0[0x30];
    UnkFXTriggerActor* actor;  // 0x30
    u8 pad_0x38[0x18];
    UnkFXTriggerEffects effects;  // 0x50
    u8 pad_0x68[0x78];
    UnkFXTriggerSub* field_0xe0;
    u8 pad_0xe8[0x3c];
    f32 delay;           // 0x124
    UnkVec3f direction;  // 0x128
    u8 pad_0x134[0x3c];
    void* scene;  // 0x170
};
static_assert(__builtin_offsetof(UnkFXTriggerComponent, scene) == 0x170, "UnkFXTriggerComponent layout");

struct UnkFXComponent_b0052;

// The overlay-model manager, created on first use.
static inline UnkModelManager* Manager() {
    UnkModelManager* m = g_unk_71726b0cd8;
    if (!m) {
        m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return m;
}

static inline UnkFXTriggerModel* Model(u32 index) {
    return reinterpret_cast<UnkFXTriggerModel*>(Manager()->models[index]);
}

static inline void Restart(u32 index) {
    fn_71001bc068(Model(index)->field_0x150);
    UnkFXTriggerModel* model = Model(index);
    model->field_0x130 = 1.0f;
    model->field_0x14c = true;
}

// Switches on the named effect and restarts it.
static inline void SwitchOn(UnkFXTriggerComponent* self, u64 hash) {
    UnkFXTriggerEntry* entry = self->effects.Get(hash);
    UnkFXTriggerEffect* effect = entry ? entry->effect : nullptr;
    if (effect && effect->kind == 6) {
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(effect), true);
        fn_7100208a40(effect->field_0x28);
        effect->field_0x15a = false;
        effect->field_0x108 = 1.0f;
    }
}

void fn_71011531cc(UnkFXComponent_b0052* component, u32 flag, void* arg, f32 value) {
    UnkFXTriggerComponent* self = reinterpret_cast<UnkFXTriggerComponent*>(component);
    const UnkVec3f* direction = static_cast<const UnkVec3f*>(arg);
    if (value > 0.0f) {
        self->delay = value;
        self->direction = *direction;
        return;
    }
    self->delay = 0.0f;
    UnkFXTriggerActor* actor = self->actor;
    if (actor->disabled)
        return;
    if (self->field_0xe0 && self->field_0xe0->disabled)
        return;
    if (UnkFXTriggerRider* rider = actor->rider) {
        if (!fn_710024fbc8(rider) || rider->state == 2)
            return;
    }
    UnkFXTriggerSceneHolder* holder = self->actor->scene_holder;
    if (!holder || !holder->scene)
        return;
    if (self->scene) {
        fn_710014580c(Manager()->models[5], self->scene);
        fn_710014580c(Manager()->models[6], self->scene);
        self->scene = holder->scene;
        if (!self->scene)
            return;
    } else {
        self->scene = holder->scene;
    }

    if (flag & 1) {
        Restart(5);
        Restart(6);
        if (direction->x < 0.0f)
            SwitchOn(self, 0x4ee55b237f094cc8ULL);
        else
            SwitchOn(self, 0x4b8b88c186eb688fULL);
    }
    f32 side = direction->x;
    UnkModelManager* manager = Manager();
    if (side < 0.0f)
        fn_71001457fc(manager->models[5], self->scene, 0);
    else
        fn_71001457fc(manager->models[6], self->scene, 0);
}
