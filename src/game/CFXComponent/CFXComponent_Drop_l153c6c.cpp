// CFXComponent's unit: drops an effect the object holds: takes its entry out of the effect list at +0x50
// (fn_7101157dbc), stops it, restores its saved visibility and has the model manager drop it (fn_71011a91c4).
// Names and layouts are placeholders.
#include "game/CFXComponent.h"
#include "base/global/CRntVector.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU2739.h"
#include "types.h"

struct UnkFXEffect_l153c6c;
struct UnkFXEffectVtable_l153c6c {
    void* unk_0x0[9];
    void (*set_visible)(UnkFXEffect_l153c6c* self, bool on);  // 0x48
    void* unk_0x50[34];
    void (*stop)(UnkFXEffect_l153c6c* self);  // 0x160
};
struct UnkFXEffect_l153c6c {
    UnkFXEffectVtable_l153c6c* vtable;
};

struct UnkFXEffectEntry_l153c6c {
    UnkFXEffect_l153c6c* effect;
    bool visible;  // 0x8
    u8 pad_0x9[0x17];
};

struct UnkFXEffectList_l153c6c {
    UnkFXEffectEntry_l153c6c* values;
    void** keys;
    u32 count;

    UnkFXEffectEntry_l153c6c* begin() { return values; }
    UnkFXEffectEntry_l153c6c* end() { return &values[count]; }
    // The entry holding `effect`, null when there is none.
    UnkFXEffectEntry_l153c6c* Find(UnkFXEffect_l153c6c* effect) {
        UnkFXEffectEntry_l153c6c* it = begin();
        if (it != end()) {
            if (it->effect == effect)
                return it;
            for (++it; it != end(); ++it)
                if (it->effect == effect)
                    return it;
        }
        return nullptr;
    }
    // The index of `key` in keys, count when it is not there.
    u64 IndexOf(void* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return static_cast<u32>(i);
        return count;
    }
};

struct UnkFXComponent_l153c6c {
    u8 pad_0x0[0x40];
    u32 field_0x40;
    u8 pad_0x44[0xc];
    UnkFXEffectList_l153c6c effects;  // 0x50
};

void fn_7101157dbc(UnkFXEffectList_l153c6c* self, UnkFXEffectEntry_l153c6c* entry);

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

void fn_7101153c6c(void* object, void* fx) {
    UnkFXComponent_l153c6c* self = static_cast<UnkFXComponent_l153c6c*>(object);
    UnkFXEffect_l153c6c* effect = static_cast<UnkFXEffect_l153c6c*>(fx);
    UnkFXEffectList_l153c6c& effects = self->effects;
    if (UnkFXEffectEntry_l153c6c* entry = effects.Find(effect)) {
        void** keys = effects.keys;
        u32 slot = entry - effects.values;
        u64 index = effects.IndexOf(keys[slot]);
        if (index != effects.count)
            fn_7101157dbc(&effects, &effects.values[index]);
        effect->vtable->stop(effect);
        effect->vtable->set_visible(effect, entry->visible);
    }
    fn_71011a91c4(ModelManager(), fx, self->field_0x40);
}
