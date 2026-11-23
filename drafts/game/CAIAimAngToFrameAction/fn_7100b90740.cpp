// CAIAimAngToFrameAction unit (tools/tu_map.py): picks the entry the action plays from the owner's table by
// a fixed id, or by a second id when the owner's properties say "Recoil" is set. Names and layouts are
// placeholders.
#include <algorithm>

#include "../../../src/game_s7/UnkIter.h"
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

// A value in the property bag: where it is in the bag's stream, the hash of its name and its type.
struct UnkPropValue_lb90740 {
    u64 offset;
    u64 key;
    s32 type;  // 'b' for a bool
};

static inline UnkPropValue_lb90740* Values_lb90740(UnkPropBag& props) {
    return reinterpret_cast<UnkPropValue_lb90740*>(props.values.data);
}

// The value named `id`, or the end of the values when there is none.
static inline UnkPropValue_lb90740* FindValue_lb90740(UnkPropBag& props, const CStrId& id) {
    UnkPropValue_lb90740* values = Values_lb90740(props);
    u64 key = id.data->Key();
    UnkIter<UnkPropValue_lb90740> first{values};
    UnkIter<UnkPropValue_lb90740> last{&values[props.values.count]};
    return std::find_if(first, last, [key](const UnkPropValue_lb90740& v) { return v.key == key; }).p;
}

struct UnkFrameEntry_lb90740 {
    u8 pad_0x0[0x120];
};

// Entries by the hash of their id: the entries and their keys in two arrays.
struct UnkFrameTable_lb90740 {
    u8 pad_0x0[0x80];
    UnkFrameEntry_lb90740* entries;
    u64* keys;
    u32 count;

    UnkFrameEntry_lb90740* Find(const CStrId& id) {
        for (u32 i = 0; i < count; ++i) {
            if (keys[i] == id.data->Key())
                return &entries[i];
        }
        return &entries[count];
    }
    // The entry named `id`, or null.
    UnkFrameEntry_lb90740* Get(const CStrId& id) {
        UnkFrameEntry_lb90740* entry = Find(id);
        return entry == &entries[count] ? nullptr : entry;
    }
};
static_assert(__builtin_offsetof(UnkFrameTable_lb90740, count) == 0x90, "layout");

struct UnkPropOwner_lb90740 {
    u8 pad_0x0[0x80];
    UnkPropBag props;
};

struct UnkActionOwner_lb90740 {
    u8 pad_0x0[0x220];
    UnkPropOwner_lb90740* properties;
};

struct UnkAimAngToFrameAction_lb90740 {
    u8 pad_0x0[0x10];
    UnkFrameTable_lb90740* frames;
    UnkActionOwner_lb90740* owner;
};

struct UnkAimFrames_lb90740 {
    u8 pad_0x0[0x10];
    UnkFrameEntry_lb90740* entry;
};

void fn_7100b90740(UnkAimAngToFrameAction_lb90740* self, UnkAimFrames_lb90740* out) {
    static CStrId s_recoil("Recoil", true);
    bool recoil = false;
    UnkPropBag& props = self->owner->properties->props;
    UnkPropValue_lb90740* value = FindValue_lb90740(props, s_recoil);
    if (value != &Values_lb90740(props)[props.values.count] && value->type == 'b') {
        CRntFile_Seek(&props.lock, value->offset, 0);
        fn_710007c70c(&props.lock, &recoil, 1);
    }
    out->entry = self->frames->Get(unk_prop_bag::Key(0xd88));
    if (recoil && self->frames->count != 0) {
        if (UnkFrameEntry_lb90740* entry = self->frames->Get(unk_prop_bag::Key(0x1d48)))
            out->entry = entry;
    }
}
