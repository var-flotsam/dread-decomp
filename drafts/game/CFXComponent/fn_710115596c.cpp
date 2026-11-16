// CFXComponent's unit: releases an effect the component holds: takes its entry out of the effect list at
// +0x50 (fn_7101157dbc), stops it, restores its saved visibility and hands it back to the model manager.
// Names and layouts are placeholders.
#include "game/CFXComponent.h"
#include "base/global/CRntVector.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU2739.h"
#include "types.h"

struct UnkFXEffect_l15596c;
struct UnkFXEffectVtable_l15596c {
    void* unk_0x0[9];
    void (*set_visible)(UnkFXEffect_l15596c* self, bool on);  // 0x48
    void* unk_0x50[34];
    void (*stop)(UnkFXEffect_l15596c* self);  // 0x160
};
struct UnkFXEffect_l15596c {
    UnkFXEffectVtable_l15596c* vtable;
};

struct UnkFXEffectEntry_l15596c {
    UnkFXEffect_l15596c* effect;
    bool visible;  // 0x8
    u8 pad_0x9[0x17];
};

struct UnkFXEffectList_l15596c {
    UnkFXEffectEntry_l15596c* values;
    void** keys;
    u32 count;

    UnkFXEffectEntry_l15596c* begin() { return values; }
    UnkFXEffectEntry_l15596c* end() { return &values[count]; }
    // The entry holding `effect`, null when there is none.
    UnkFXEffectEntry_l15596c* Find(UnkFXEffect_l15596c* effect) {
        UnkFXEffectEntry_l15596c* it = begin();
        if (it != end()) {
            if (it->effect == effect)
                return it;
            for (++it; it != end(); ++it)
                if (it->effect == effect)
                    return it;
        }
        return nullptr;
    }
    u32 IndexOf(void* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return i;
        return count;
    }
};

struct UnkFXComponent_l15596c {
    u8 pad_0x0[0x50];
    UnkFXEffectList_l15596c effects;  // 0x50
};

void fn_7101157dbc(UnkFXEffectList_l15596c* self, UnkFXEffectEntry_l15596c* entry);

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

void fn_710115596c(UnkRinkaControl_ruc* control, void* object) {
    UnkFXComponent_l15596c* self = reinterpret_cast<UnkFXComponent_l15596c*>(control);
    UnkFXEffect_l15596c* effect = static_cast<UnkFXEffect_l15596c*>(object);
    UnkFXEffectList_l15596c& effects = self->effects;
    if (UnkFXEffectEntry_l15596c* entry = effects.Find(effect)) {
        u32 slot = entry - effects.values;
        void* key = effects.keys[slot];
        u32 index = effects.IndexOf(key);
        if (index != effects.count)
            fn_7101157dbc(&effects, &effects.values[index]);
        effect->vtable->stop(effect);
        effect->vtable->set_visible(effect, entry->visible);
    }
    fn_71011a9bb4(ModelManager(), static_cast<UnkFxObject_ovb0041*>(object));
}
