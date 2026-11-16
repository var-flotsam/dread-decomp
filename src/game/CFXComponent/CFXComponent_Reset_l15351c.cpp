// CFXComponent's unit: detaches the component's scene from the two last overlay models and switches
// off two named effects.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU132.h"
#include "game/UnkTU134.h"
#include "game/UnkTU285.h"
#include "types.h"

struct UnkFXResetEffect {
    void** vtable;
    u8 pad_0x8[3];
    u8 kind;  // 0xb
};

struct UnkFXResetEntry {
    UnkFXResetEffect* effect;
    u8 pad_0x8[0x18];
};

struct UnkFXResetName {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkFXResetEffects {
    UnkFXResetEntry* values;
    UnkFXResetName** names;
    u32 count;

    UnkFXResetEntry* End() { return &values[count]; }
    UnkFXResetEntry* Find(u64 hash) {
        for (u64 i = 0; i < count; ++i)
            if (names[i]->hash == hash)
                return &values[i];
        return End();
    }
    UnkFXResetEntry* Get(u64 hash) {
        UnkFXResetEntry* p = Find(hash);
        return p == End() ? nullptr : p;
    }
};

struct UnkFXResetComponent {
    u8 pad_0x0[0x50];
    UnkFXResetEffects effects;  // 0x50
    u8 pad_0x68[0xbc];
    u32 field_0x124;
    u8 pad_0x128[0x48];
    void* scene;  // 0x170
};
static_assert(__builtin_offsetof(UnkFXResetComponent, scene) == 0x170, "UnkFXResetComponent layout");

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

static inline void Detach(void* model, void* scene) {
    reinterpret_cast<void (*)(void*, void*)>(fn_710014580c)(model, scene);
}

static inline void SwitchOff(UnkFXResetComponent* self, u64 hash) {
    UnkFXResetEntry* entry = self->effects.Get(hash);
    UnkFXResetEffect* effect = entry ? entry->effect : nullptr;
    if (effect && effect->kind == 6) {
        UnkSwitchObject* object = reinterpret_cast<UnkSwitchObject*>(effect);
        fn_7100146dac(object, false);
        fn_7100208a40(object->field_0x28);
    }
}

struct UnkFXComponent_b0052;

void fn_710115351c(UnkFXComponent_b0052* component) {
    UnkFXResetComponent* self = reinterpret_cast<UnkFXResetComponent*>(component);
    self->field_0x124 = 0;
    if (self->scene) {
        Detach(Manager()->models[5], self->scene);
        Detach(Manager()->models[6], self->scene);
        self->scene = nullptr;
    }
    SwitchOff(self, 0x4b8b88c186eb688fULL);
    SwitchOff(self, 0x4ee55b237f094cc8ULL);
}
