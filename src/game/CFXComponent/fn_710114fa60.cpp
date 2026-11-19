// CFXComponent's unit: for each effect entry (list +0x50, count +0x60) calls vf_0x160 on the effect; an
// effect of kind 4 is attached to or detached from the owner's scene according to vf_0x58, and entries with
// the flag at +0xa get vf_0x40(false). Then the two scene nodes (+0xe0, +0xe8) go to layer +0x40.
// Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "game/UnkTU132.h"
#include "types.h"

template <u32 Off, class R = void, class... A>
static inline R VCall_l14fa60(void* obj, A... a) {
    return reinterpret_cast<R (*)(void*, A...)>((*reinterpret_cast<void***>(obj))[Off / 8])(obj, a...);
}

static inline void Detach_l14fa60(void* effect, void* scene) {
    reinterpret_cast<void (*)(void*, void*)>(fn_710014580c)(effect, scene);
}

struct UnkFXEffect_l14fa60 {
    void** vtable;
    u8 pad_0x8[3];
    u8 kind;  // 0xb
};

static inline bool IsKind4(const UnkFXEffect_l14fa60& e, const UnkFXEffect_l14fa60* p) { return e.kind == 4 && p != nullptr; }

struct UnkFXEntry_l14fa60 {
    UnkFXEffect_l14fa60* effect;
    u8 pad_0x8[2];
    bool refresh;  // 0xa
    u8 pad_0xb[0x15];
};

struct UnkFXSceneHolder_l14fa60 {
    u8 pad_0x0[0x50];
    void* scene;  // 0x50
};

struct UnkFXOwner_l14fa60 {
    u8 pad_0x0[0x228];
    UnkFXSceneHolder_l14fa60* holder;  // 0x228
};

struct UnkFXNodeObject_l14fa60 {
    u8 pad_0x0[8];
    UnkSceneNode node;  // 0x8
};

struct UnkFXComponent_l14fa60 {
    u8 pad_0x0[0x30];
    UnkFXOwner_l14fa60* owner;  // 0x30
    u8 pad_0x38[8];
    u32 layer;  // 0x40
    u8 pad_0x44[0xc];
    UnkFXEntry_l14fa60* entries;  // 0x50
    u8 pad_0x58[8];
    u32 count;  // 0x60
    u8 pad_0x64[0x7c];
    UnkFXNodeObject_l14fa60* node_a;  // 0xe0
    UnkFXNodeObject_l14fa60* node_b;  // 0xe8
};

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_710114fa60(UnkFXComponent_l14fa60* self) {
    for (UnkFXEntry_l14fa60* it = self->entries; it != self->entries + self->count; ++it) {
        VCall_l14fa60<0x160>(it->effect);
        UnkFXEffect_l14fa60* effect = it->effect;
        if (IsKind4(*effect, effect)) {
            void* scene = self->owner->holder->scene;
            bool on = VCall_l14fa60<0x58, bool>(effect);
            if (scene && on)
                fn_71001457fc(effect, scene, 1);
            else if (scene)
                Detach_l14fa60(effect, scene);
        }
        if (it->refresh)
            VCall_l14fa60<0x40>(it->effect, false);
    }
    if (self->node_a) {
        fn_7100219ae0(g_unk_7101d47300, &self->node_a->node, self->layer);
        if (self->node_b)
            fn_7100219ae0(g_unk_7101d47300, &self->node_b->node, self->layer);
    }
}
