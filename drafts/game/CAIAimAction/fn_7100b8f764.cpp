// CAIAimAction unit (tools/tu_map.py): picks the entry the action plays from the owner's table by
// a fixed id, or by a second id when the owner's properties say "Recoil" is set. Names and layouts are
// placeholders.
#include <algorithm>

#include "../../../src/game_s7/UnkIter.h"
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

// A value in the property bag: where it is in the bag's stream, the hash of its name and its type.
struct UnkPropValue_lb8f764 {
    u64 offset;
    u64 key;
    s32 type;  // 'b' for a bool
};

static inline UnkPropValue_lb8f764* Values_lb8f764(UnkPropBag& props) {
    return reinterpret_cast<UnkPropValue_lb8f764*>(props.values.data);
}

// The value named `id`, or the end of the values when there is none.
static inline UnkPropValue_lb8f764* FindValue_lb8f764(UnkPropBag& props, const CStrId& id) {
    UnkPropValue_lb8f764* values = Values_lb8f764(props);
    u64 key = id.data->Key();
    UnkIter<UnkPropValue_lb8f764> first{values};
    UnkIter<UnkPropValue_lb8f764> last{&values[props.values.count]};
    return std::find_if(first, last, [key](const UnkPropValue_lb8f764& v) { return v.key == key; }).p;
}

struct UnkFrameEntry_lb8f764 {
    u8 pad_0x0[0x120];
};

// Entries by the hash of their id: the entries and their keys in two arrays.
struct UnkFrameTable_lb8f764 {
    u8 pad_0x0[0x80];
    UnkFrameEntry_lb8f764* entries;
    u64* keys;
    u32 count;

    UnkFrameEntry_lb8f764* Find(const CStrId& id) {
        for (u32 i = 0; i < count; ++i) {
            if (keys[i] == id.data->Key())
                return &entries[i];
        }
        return &entries[count];
    }
    // The entry named `id`, or null.
    UnkFrameEntry_lb8f764* Get(const CStrId& id) {
        UnkFrameEntry_lb8f764* entry = Find(id);
        return entry == &entries[count] ? nullptr : entry;
    }
};
static_assert(__builtin_offsetof(UnkFrameTable_lb8f764, count) == 0x90, "layout");

struct UnkPropOwner_lb8f764 {
    u8 pad_0x0[0x80];
    UnkPropBag props;
};

struct UnkActionOwner_lb8f764 {
    u8 pad_0x0[0x220];
    UnkPropOwner_lb8f764* properties;
};

struct UnkAimAction_lb8f764 {
    u8 pad_0x0[0x10];
    UnkFrameTable_lb8f764* frames;
    UnkActionOwner_lb8f764* owner;
};

struct UnkAimFrames_lb8f764 {
    u8 pad_0x0[0x10];
    UnkFrameEntry_lb8f764* entry;
};

void fn_7100b8f764(UnkAimAction_lb8f764* self, UnkAimFrames_lb8f764* out) {
    static CStrId s_recoil("Recoil", true);
    bool recoil = false;
    UnkPropBag& props = self->owner->properties->props;
    UnkPropValue_lb8f764* value = FindValue_lb8f764(props, s_recoil);
    if (value != &Values_lb8f764(props)[props.values.count] && value->type == 'b') {
        fn_710007c00c(&props.lock, value->offset, 0);
        fn_710007c70c(&props.lock, &recoil, 1);
    }
    out->entry = self->frames->Get(unk_prop_bag::Key(0xd88));
    if (recoil && self->frames->count != 0) {
        if (UnkFrameEntry_lb8f764* entry = self->frames->Get(unk_prop_bag::Key(0x1d48)))
            out->entry = entry;
    }
}
