// CFXComponent's unit: stops every named effect (detaching the scene-bound ones from the owner's
// scene), switches off the effects of the active effect set, detaches the owner's scene from the
// highlight overlay and hands the two render nodes back to their layer.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU132.h"
#include "types.h"

struct UnkFXComponent_ovb51;
void fn_7101150180(UnkFXComponent_ovb51* self);
bool fn_7100219a34(UnkRenderManager* self, UnkSceneNode* entry);

struct UnkFXStopName {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkFXStopEffect {
    void** vtable;
    u32 flags;  // 0x8; the kind in the top byte
    u8 pad_0xc[0x10d];
    u8 on;  // 0x119

    u32 Kind() const { return flags >> 24; }
    bool HasKind(u32 kind) const { return (flags & 0xff000000) == kind << 24; }
};
static_assert(__builtin_offsetof(UnkFXStopEffect, on) == 0x119, "UnkFXStopEffect layout");

struct UnkFXStopEntry {
    UnkFXStopEffect* effect;
    u8 pad_0x8[0x18];
};

struct UnkFXStopEffects {
    UnkFXStopEntry* values;
    UnkFXStopName** names;
    u32 count;

    UnkFXStopEntry* End() { return &values[count]; }
    UnkFXStopEntry* Find(u64 hash) {
        for (u64 i = 0; i < count; ++i)
            if (names[i]->hash == hash)
                return &values[i];
        return End();
    }
    UnkFXStopEntry* Get(u64 hash) {
        UnkFXStopEntry* p = Find(hash);
        return p == End() ? nullptr : p;
    }
};

struct UnkFXStopNames {
    UnkFXStopName** data;
    u32 count;

    UnkFXStopName** End() { return &data[count]; }
};

// The effect set the component switched on: the effects it switches (+0x20) and those it switches
// with their scene (+0x50).
struct UnkFXStopSet {
    u8 pad_0x0[0x20];
    UnkFXStopNames switched;  // 0x20
    u8 pad_0x30[0x20];
    UnkFXStopNames bound;  // 0x50
};

struct UnkFXStopScene {
    u8 pad_0x0[0x50];
    void* scene;  // 0x50
};

struct UnkFXStopOwner {
    u8 pad_0x0[0x228];
    UnkFXStopScene* field_0x228;
};

struct UnkFXStopNode {
    u8 pad_0x0[0x8];
    UnkSceneNode node;  // 0x8
};

struct UnkFXStopComponent {
    u8 pad_0x0[0x30];
    UnkFXStopOwner* owner;  // 0x30
    u8 pad_0x38[0x8];
    u32 field_0x40;
    u8 pad_0x44[0xc];
    UnkFXStopEffects effects;  // 0x50
    u8 pad_0x68[0x78];
    UnkFXStopNode* field_0xe0;
    UnkFXStopNode* field_0xe8;
    u8 pad_0xf0[0x1c];
    s32 field_0x10c;
    u8 pad_0x110[0x28];
    UnkFXStopSet* set;  // 0x138
};
static_assert(__builtin_offsetof(UnkFXStopComponent, set) == 0x138, "UnkFXStopComponent layout");

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

// The effect when it is bound to a scene (kind 4), else null.
static inline UnkFXStopEffect* SceneBound(UnkFXStopEffect* effect) {
    return effect->HasKind(4) ? effect : nullptr;
}

static inline void Stop(UnkFXStopEffect* effect) {
    reinterpret_cast<void (*)(UnkFXStopEffect*)>(effect->vtable[0x158 / 8])(effect);
}

static inline void SetOn(UnkFXStopEffect* effect, bool on) {
    reinterpret_cast<void (*)(UnkFXStopEffect*, bool)>(effect->vtable[0x40 / 8])(effect, on);
}

struct UnkFXComponent_b0052;

void fn_710114fb84(UnkFXComponent_b0052* component) {
    UnkFXStopComponent* self = reinterpret_cast<UnkFXStopComponent*>(component);
    for (UnkFXStopEntry* e = self->effects.values; e != self->effects.End(); ++e) {
        Stop(e->effect);
        UnkFXStopEffect* effect = e->effect;
        // The original reads the kind before it tests the effect for null.
        if (effect->Kind() == 4 && effect) {
            if (void* scene = self->owner->field_0x228->scene)
                fn_710014580c(effect, scene);
        }
    }
    if (self->set) {
        for (UnkFXStopName** name = self->set->bound.data; name != self->set->bound.End(); ++name) {
            if (UnkFXStopEntry* entry = self->effects.Get((*name)->hash)) {
                UnkFXStopEffect* effect = entry->effect;
                UnkFXStopEffect* bound = SceneBound(effect);
                if (bound && bound->on) {
                    if (void* scene = self->owner->field_0x228->scene)
                        fn_710014580c(bound, scene);
                }
                SetOn(effect, false);
            }
        }
        for (UnkFXStopName** name = self->set->switched.data; name != self->set->switched.End(); ++name) {
            UnkFXStopEntry* entry = self->effects.Get((*name)->hash);
            UnkFXStopEffect* effect = entry ? entry->effect : nullptr;
            if (effect && effect->Kind() == 6)
                fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(effect), false);
        }
        self->set = nullptr;
    }
    if (self->field_0x10c != -1) {
        self->field_0x10c = -1;
        UnkFXStopScene* owner_scene = self->owner->field_0x228;
        if (owner_scene && owner_scene->scene)
            fn_710014580c(Manager()->models[2], owner_scene->scene);
    }
    fn_7101150180(reinterpret_cast<UnkFXComponent_ovb51*>(self));
    if (self->field_0xe0) {
        fn_7100219a34(g_unk_7101d47300, &self->field_0xe0->node);
        if (self->field_0xe8)
            fn_7100219a34(g_unk_7101d47300, &self->field_0xe8->node);
    }
}
