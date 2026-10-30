// In CFXComponent's unit (unit 2712 of tools/tu_map.py): the effect an owner keeps under a name's hash,
// from its parallel arrays at +0x50 (0x20-byte entries whose first word is the effect) and +0x58 (the
// names, whose hash is at +0x30 of the string), or null; and switching that effect on or off
// (fn_710114fe8c). Names and layouts are placeholders.
#include "types.h"

struct UnkComponent;

struct UnkFXName {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkFXEntry {
    UnkComponent* effect;
    u8 pad_0x8[0x18];
};

struct UnkObject {
    u8 pad_0x0[0x50];
    UnkFXEntry* values;
    UnkFXName** names;
    u32 count;
};

// NON_MATCHING(minor): the original compares the slot with the end before testing it for null (in its
// conditional compare); same behaviour.
UnkComponent* fn_7101152b90(UnkObject* self, u64 hash) {
    UnkFXEntry* slot = [&] {
        for (u32 i = 0; i < self->count; ++i)
            if (self->names[i]->hash == hash)
                return &self->values[i];
        return &self->values[self->count];
    }();
    if (slot == &self->values[self->count])
        return nullptr;
    return slot ? slot->effect : nullptr;
}

// ---- fn_710114fe8c: switching an effect on or off ----

void fn_71001457fc(void* effect, void* scene, u32 flag);  // attaches it to the scene
void fn_710014580c(void* effect);                         // detaches it

// An effect as the switch reads it: its kind (+0xb; 4 is attached to the scene) and whether it is
// on (+0x119), with its virtual setter (vf_0x40) and its restart (vf_0x118).
struct UnkFXEffect {
    void** vtable;
    u8 pad_0x8[0x3];
    u8 kind;  // 0xb
    u8 pad_0xc[0x10d];
    u8 on;  // 0x119, tested as a byte
};

struct UnkFXOwnerScene {
    u8 pad_0x0[0x50];
    void* scene;
};
struct UnkFXOwner {
    u8 pad_0x0[0x228];
    UnkFXOwnerScene* field_0x228;
};

// Switches `effect` on or off (attaching a scene-bound effect to the owner's scene when turning it on,
// detaching it when turning it off), and restarts it when `restart` is set.
static inline void Switch(UnkObject* self, UnkFXEffect* effect, u32 on, u32 restart) {
    if (effect && effect->kind == 4 && (((effect->on != 0) ^ on) & 1)) {
        void* scene = reinterpret_cast<UnkFXOwner*>(reinterpret_cast<char**>(self)[6])->field_0x228->scene;
        if (!scene || !(on & 1)) {
            if (scene)
                fn_710014580c(effect);
        } else {
            fn_71001457fc(effect, scene, 1);
        }
    }
    reinterpret_cast<void (*)(UnkFXEffect*, bool)>(effect->vtable[0x40 / 8])(effect, on & 1);
    if (restart & 1)
        reinterpret_cast<void (*)(UnkFXEffect*)>(effect->vtable[0x118 / 8])(effect);
}

// NON_MATCHING(minor): the original compares the kind shifted into the top byte and keeps the flag
// test as an exclusive or; the order of the scene tests differs too. Same behaviour.
void fn_7101152c0c(UnkObject* self, void* effect, u32 on, u32 restart) {
    Switch(self, static_cast<UnkFXEffect*>(effect), on, restart);
}

// Switches the effect `hash` names on or off (attaching a scene-bound effect to the owner's scene
// when turning it on, detaching it when turning it off), and restarts it when `restart` is set.
// NON_MATCHING(minor): as fn_7101152b90, the slot is tested for null before the end, and the effect's
// flag is compared as a bool where the original tests it as a byte; same behaviour.
void fn_710114fe8c(UnkObject* self, u64 hash, u32 on, u32 restart) {
    UnkFXEntry* slot = [&] {
        for (u32 i = 0; i < self->count; ++i)
            if (self->names[i]->hash == hash)
                return &self->values[i];
        return &self->values[self->count];
    }();
    if (slot == &self->values[self->count] || !slot)
        return;
    Switch(self, reinterpret_cast<UnkFXEffect*>(slot->effect), on, restart);
}
