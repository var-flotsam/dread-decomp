// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): sets or clears flags on the
// owner's flag target, either the one named or every listed flag whose name contains it.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CModelUpdaterComponent.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

u32 fn_71011735a0(UnkFlagTarget* target);
const CStrId* fn_7101173584(UnkFlagTarget* target, u32 index);

struct UnkFlagOwner_l8d0a08 {
    u8 unk_0x0[0x228];
    UnkFlagTarget* field_0x228;
};

struct UnkFlagComponent_l8d0a08 {
    u8 unk_0x0[0x30];
    UnkFlagOwner_l8d0a08* owner;
};

// Whether `id` names a string containing `part`'s text (an empty id contains nothing).
static inline bool NameContains(const CStrId* id, const CStrId* part) {
    ++id->data->reads;
    UnkStringData* data = id->data;
    if (data == &g_unk_7101d47108->empty)
        return false;
    return fn_710007fb9c(&data->String(), part->Text());
}

// The flag calls take the name's key by address.
static inline void SetFlag(UnkFlagTarget* target, const CStrId* id) {
    const u64 key = id->data->Key();
    fn_7101172764(target, reinterpret_cast<const char*>(&key));
}

static inline void ClearFlag(UnkFlagTarget* target, const CStrId* id) {
    u64 key = id->data->Key();
    fn_7101172780(target, reinterpret_cast<const char*>(&key));
}

// Sets or clears every listed flag whose name contains `name`; whether any was set.
static inline bool SetContaining(UnkFlagTarget* target, const CStrId* name, bool set) {
    bool any = false;
    for (u32 i = 0; i < fn_71011735a0(target); ++i) {
        const CStrId* id = fn_7101173584(target, i);
        fn_7101173584(target, i);
        if (!NameContains(id, name))
            continue;
        if (set) {
            SetFlag(target, id);
            any = true;
        } else {
            ClearFlag(target, id);
        }
    }
    return any;
}

bool fn_71008d0a08(UnkFlagComponent_l8d0a08* self, const CStrId* name, bool set, bool exact) {
    UnkFlagTarget* target = self->owner->field_0x228;
    if (!target)
        return false;
    if (!exact)
        return SetContaining(target, name, set);
    if (set)
        SetFlag(target, name);
    else
        ClearFlag(target, name);
    return set;
}
