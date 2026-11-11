// In the CTriggerComponent unit (name from reflection): fn_7100a5e240 runs the base step
// (fn_71010c4228), registers the object with the game manager under the id g_unk_717274e4d0
// (fn_7100de9540) and hands the registry map at +0x280 a counted copy of the owner's (+0x18) name id
// at +0x30 (refl_RegistryMapInit, fn_7100a5e2f4). Same shape as fn_71009d0184. Names and layouts are
// placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "game/CComponent.h"
#include "game/CTriggerComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkParent;
extern u64 g_unk_717274e4d0 __attribute__((visibility("hidden")));
struct UnkRefl_fn_7100a5e2f4;
extern template bool base::reflection::refl_RegistryMapInit<UnkRefl_fn_7100a5e2f4>(
    base::reflection::UnkReflRegistryMap*, const base::global::CStrId&);

void fn_7100a5e240(char* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    fn_7100de9540(g_unk_7101d47310, &g_unk_717274e4d0, reinterpret_cast<UnkWeakTarget*>(self));
    char* m = self + 0x280;
    base::global::UnkStringData* d =
        *reinterpret_cast<base::global::UnkStringData* const*>(*reinterpret_cast<char**>(self + 0x18) + 0x30);
    {
        union Id {
            base::global::CStrId id;
            Id() {}
            ~Id() { id.~CStrId(); }
        } u;
        u.id.data = d;
        if (&g_unk_7101d47108->empty != d)
            ++d->ref_count;
        base::reflection::refl_RegistryMapInit<UnkRefl_fn_7100a5e2f4>(
            reinterpret_cast<base::reflection::UnkReflRegistryMap*>(m), u.id);
    }
}

// fn_7100a5eb64: removes the entries of the inline array at +0x64 (0x18 bytes each, count at +0x60)
// from the last one on (fn_7100a5fdcc, given a pointer to the entry), then unregisters the object
// from the game manager under g_unk_717274e4d8 and, when the flag at +0x205 is set, under
// g_unk_717274e4e0 (fn_7100de9730).
struct UnkSpawnGroupKey;
void fn_7100de9730(UnkGameManager*, UnkSpawnGroupKey*, void*);
extern UnkSpawnGroupKey g_unk_717274e4d8 __attribute__((visibility("hidden")));
extern UnkSpawnGroupKey g_unk_717274e4e0 __attribute__((visibility("hidden")));

struct UnkTriggerEntry_b50a5 {
    u8 unk_0x0[0x18];
};

struct UnkTrigger_b50a5 {
    u8 unk_0x0[0x60];
    u32 count;                           // 0x60
    UnkTriggerEntry_b50a5 entries[16];   // 0x64
    u8 unk_0x1e4[0x21];
    bool field_0x205;
};

// The entry is passed by address. The original's unit defines fn_7100a5fdcc, so the compiler knows
// it doesn't keep the address (noescape here), which lets the last call be a tail call.

static inline void RemoveLast_b50a5(UnkTrigger_b50a5* self) {
    UnkTriggerEntry_b50a5* e = (self->entries - 1) + self->count;
    fn_7100a5fdcc(self, &e);
}

void fn_7100a5eb64(UnkTrigger_b50a5* self) {
    while (self->count != 0)
        RemoveLast_b50a5(self);
    fn_7100de9730(g_unk_7101d47310, &g_unk_717274e4d8, self);
    if (self->field_0x205)
        fn_7100de9730(g_unk_7101d47310, &g_unk_717274e4e0, self);
}
