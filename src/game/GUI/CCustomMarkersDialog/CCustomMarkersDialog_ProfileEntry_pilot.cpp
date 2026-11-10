// Looks up the entry for the current profile's id (+0x4a8 of the object at +0x10 of the slot at
// +0xd63f40 of the game manager's +0x2228): first by id (fn_7100ea6120), then, for a non-empty id, by
// id and the profile's index (fn_7100ea6164), and finally falls back to fn_7100ea61c4. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkProfile {
    u8 pad_0x0[0x4a8];
    CStrId field_0x4a8;
    u32 field_0x4b0;
};

struct UnkProfileSlot {
    u8 pad_0x0[0x10];
    UnkProfile* field_0x10;
};

struct UnkProfileStore {
    u8 pad_0x0[0xd63f40];
    UnkProfileSlot* field_0xd63f40;
};

void* fn_7100ea6120(UnkProfileStore* self, const CStrId* id);
void* fn_7100ea6164(UnkProfileStore* self, const CStrId* id, u32 index);
void* fn_7100ea61c4(UnkProfileStore* self);

static inline UnkProfileStore* GetProfileStore(UnkGameManager* manager) {
    return *reinterpret_cast<UnkProfileStore**>(reinterpret_cast<u8*>(manager) + 0x2228);
}

void* fn_7100afd628() {
    UnkProfileStore* store = GetProfileStore(g_unk_7101d47310);
    UnkProfileSlot* slot = store->field_0xd63f40;
    UnkProfile* profile = slot ? slot->field_0x10 : nullptr;
    CStrId id(profile->field_0x4a8);
    void* entry = fn_7100ea6120(store, &id);
    if (!entry && id.data != &g_unk_7101d47108->empty) {
        entry = fn_7100ea6164(store, &id, profile->field_0x4b0);
        if (!entry)
            entry = fn_7100ea61c4(store);
    }
    return entry;
}
