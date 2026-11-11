// CFXComponent's unit (tools/tu_map.py unit 2712): releases the component's effects. The effect
// objects of the entries at +0x50 are collected and dropped (fn_7101153c6c), the ids at +0x58
// reset, the count cleared and the three models handed back to the model manager.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkModelManager.h"
#include "types.h"

void fn_7101153c6c(void* object, void* effect);
void fn_71011a91c4(UnkModelManager* self, void* handle, u32 id);

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

struct UnkFXEntry_0x20 {
    void* effect;
    u8 pad_0x8[0x18];
};

struct UnkFXComponent_l14e130 {
    u8 pad_0x0[0x40];
    u32 field_0x40;
    u8 pad_0x44[0xc];
    UnkFXEntry_0x20* entries;  // 0x50
    base::global::CStrId* ids;               // 0x58
    u32 count;                 // 0x60
    u8 pad_0x64[0x7c];
    void* field_0xe0;
    void* field_0xe8;
    u8 pad_0xf0[0x10];
    void* field_0x100;
};
static_assert(__builtin_offsetof(UnkFXComponent_l14e130, field_0x100) == 0x100,
              "UnkFXComponent_l14e130 layout");

static inline void ReleaseModel(UnkFXComponent_l14e130* self, void*& model) {
    if (model) {
        fn_71011a91c4(ModelManager(), model, self->field_0x40);
        model = nullptr;
    }
}

void fn_710114e130(UnkFXComponent_l14e130* self) {
    base::UnkVector<void*> effects(0x1a);
    if (self->count) {
        for (UnkFXEntry_0x20* e = self->entries; e != self->entries + self->count; ++e)
            effects.PushBack(e->effect);
    }
    for (u32 i = 0; i < effects.count; ++i)
        fn_7101153c6c(self, effects.data[i]);
    if (self->ids) {
        for (u32 i = 0; i < self->count; ++i)
            self->ids[i].Reset();
    }
    self->count = 0;
    ReleaseModel(self, self->field_0xe0);
    ReleaseModel(self, self->field_0xe8);
    ReleaseModel(self, self->field_0x100);
    effects.Free();
}
