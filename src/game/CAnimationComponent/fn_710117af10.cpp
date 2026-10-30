// Returns whether any listed object's class name contains the given class's name (optionally skipping
// objects that report busy); the name id of a track by its name hash (fn_7101177ae8) and whether that
// track reports busy (fn_71011782ec).
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkClsStr_710117af10 {
    const char* str;
};

struct UnkCls_710117af10 {
    u8 unk_0x0[0x10];
    UnkClsStr_710117af10 name;
    u8 unk_0x18[0x2c];
    union {
        s32 field_0x44;
    };
};

struct UnkObj_710117af10;

struct UnkElem_710117af10 {
    u8 unk_0x0[0x70];
    UnkObj_710117af10* field_0x70;
};

struct UnkList_710117af10 {
    u8 unk_0x0[0xf0];
    UnkElem_710117af10** items;
    u8 unk_0xf8[0x8];
    u32 count;
};

struct UnkStruct_710117af10 {
    u8 unk_0x0[0x40];
    UnkList_710117af10* field_0x40;
};

UnkCls_710117af10** fn_710070d7f8(UnkObj_710117af10* obj);
bool fn_710070d830(UnkObj_710117af10* obj);

bool fn_710117af10(UnkStruct_710117af10* self, UnkCls_710117af10** name, bool skipBusy) {
    for (UnkElem_710117af10** it = self->field_0x40->items;
         it != self->field_0x40->items + self->field_0x40->count; ++it) {
        UnkObj_710117af10* obj = (*it)->field_0x70;
        UnkCls_710117af10** cls = fn_710070d7f8(obj);
        (*cls)->field_0x44++;
        UnkCls_710117af10* c = *cls;
        if (c == reinterpret_cast<UnkCls_710117af10*>(&g_unk_7101d47108->empty))
            continue;
        const UnkClsStr_710117af10* str = &c->name;
        (*name)->field_0x44++;
        const char* n = (*name)->name.str ? (*name)->name.str : "";
        if (!fn_710007fb9c(reinterpret_cast<const base::global::CRntString*>(str), n))
            continue;
        if (skipBusy && fn_710070d830(obj))
            continue;
        return true;
    }
    return false;
}

// The name id of the track whose name hash is `key` (what fn_710070d7f8 gives for it), or the
// global empty id when there is no such entry or track.
template <UnkCls_710117af10** (*Name)(UnkObj_710117af10*)>
static inline const base::global::CStrId* TrackName(UnkAnimationComponent* self, u64 key) {
    UnkTrackSet* set = self->field_0x40;
    UnkTrackEntry** it = set->field_0xc8.Find(key);
    if (it == set->field_0xc8.End())
        return &g_unk_7101d470e0;
    UnkTrackEntry* entry = *it;
    if (entry && entry->field_0x70)
        return reinterpret_cast<const base::global::CStrId*>(
            Name(reinterpret_cast<UnkObj_710117af10*>(entry->field_0x70)));
    return &g_unk_7101d470e0;
}

const base::global::CStrId* fn_7101177ae8(UnkAnimationComponent* self, u64 key) { return TrackName<fn_710070d7f8>(self, key); }
// The same through fn_710070d818, next door.
UnkCls_710117af10** fn_710070d818(UnkObj_710117af10* obj);
const base::global::CStrId* fn_7101177cac(UnkAnimationComponent* self, u64 key) { return TrackName<fn_710070d818>(self, key); }

// Whether the track whose name hash is `key` reports busy (fn_710070d830). The entry is not checked:
// a missing key reads the slot past the last one, as in the original.
template <bool (*Test)(UnkObj_710117af10*)>
static inline bool TrackTest(UnkAnimationComponent* self, u64 key) {
    UnkTrackLookup& lookup = self->field_0x40->field_0xc8;
    u32 count = lookup.count;
    u64 i = 0;
    for (; i != count; ++i)
        if (lookup.keys[i] == key)
            break;
    return Test(reinterpret_cast<UnkObj_710117af10*>(lookup.values[i]->field_0x70));
}

bool fn_71011782ec(UnkAnimationComponent* self, u64 key) { return TrackTest<fn_710070d830>(self, key); }
// The same test through fn_710070df1c, next door.
bool fn_710070df1c(UnkObj_710117af10* obj);
bool fn_7101178340(UnkAnimationComponent* self, u64 key) { return TrackTest<fn_710070df1c>(self, key); }
