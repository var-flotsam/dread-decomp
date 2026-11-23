// CFXComponent's unit: for every active named effect, reads its two flags from the effect's property bag
// (when it has one), restarts it and switches it on only when both flags are set.
// Names and layouts are placeholders.
#include <algorithm>

#include "../../game_s7/UnkIter.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

__attribute__((visibility("hidden"))) extern CStrId g_unk_7172a098b0;
__attribute__((visibility("hidden"))) extern CStrId g_unk_7172a098b8;

// A value in the bag: where it is in the bag's stream, the hash of its name and its type.
struct UnkBagValue_l14e6b8 {
    u64 offset;
    u64 key;
    s32 type;  // 'b' for a bool
};

static inline UnkBagValue_l14e6b8* Values_l14e6b8(UnkPropBag& bag) {
    return reinterpret_cast<UnkBagValue_l14e6b8*>(bag.values.data);
}

// The value named `id`, or the end of the values when there is none.
static inline UnkBagValue_l14e6b8* FindValue_l14e6b8(UnkPropBag& bag, const CStrId& id) {
    UnkBagValue_l14e6b8* values = Values_l14e6b8(bag);
    u64 key = id.data->Key();
    UnkIter<UnkBagValue_l14e6b8> first{values};
    UnkIter<UnkBagValue_l14e6b8> last{&values[bag.values.count]};
    return std::find_if(first, last, [key](const UnkBagValue_l14e6b8& v) { return v.key == key; }).p;
}

// Reads the bool named `id` from the bag into `out`, when the bag has one.
static inline void ReadBool_l14e6b8(UnkPropBag& bag, const CStrId& id, bool* out) {
    UnkBagValue_l14e6b8* it = FindValue_l14e6b8(bag, id);
    if (it != &Values_l14e6b8(bag)[bag.values.count] && it->type == 'b') {
        CRntFile_Seek(&bag.lock, it->offset, 0);
        fn_710007c70c(&bag.lock, out, 1);
    }
}

struct UnkFXPropEffect {
    void** vtable;
};

struct UnkFXPropSource {
    u8 pad_0x0[0x8];
    UnkPropBag* bag;  // 0x8
};

struct UnkFXPropEntry {
    UnkFXPropEffect* effect;
    u8 pad_0x8;
    bool active;  // 0x9
    u8 pad_0xa[0x6];
    UnkFXPropSource* props;  // 0x10
    u8 pad_0x18[0x8];
};

struct UnkFXPropComponent {
    u8 pad_0x0[0x50];
    UnkFXPropEntry* values;  // 0x50
    void* names;  // 0x58
    u32 count;  // 0x60
};

struct UnkFXComponent_b0052;

static inline void Restart(UnkFXPropEffect* effect) {
    reinterpret_cast<void (*)(UnkFXPropEffect*)>(effect->vtable[0x118 / 8])(effect);
}

static inline void SetOn(UnkFXPropEffect* effect, bool on) {
    reinterpret_cast<void (*)(UnkFXPropEffect*, bool)>(effect->vtable[0x40 / 8])(effect, on);
}

void fn_710114e6b8(UnkFXComponent_b0052* component) {
    UnkFXPropComponent* self = reinterpret_cast<UnkFXPropComponent*>(component);
    for (UnkFXPropEntry* entry = self->values; entry != &self->values[self->count]; ++entry) {
        if (!entry->active)
            continue;
        bool visible = false;
        bool enabled = true;
        if (entry->props) {
            UnkPropBag& bag = *entry->props->bag;
            ReadBool_l14e6b8(bag, g_unk_7172a098b0, &visible);
            ReadBool_l14e6b8(bag, g_unk_7172a098b8, &enabled);
        }
        Restart(entry->effect);
        SetOn(entry->effect, visible && enabled);
    }
}
