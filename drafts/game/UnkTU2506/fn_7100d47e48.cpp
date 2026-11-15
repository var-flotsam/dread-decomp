// Unit 2506: whether the handle group kept for `owner` (in the map at +0xc8, keyed by weak handle)
// holds a weak handle of `target`. False when the map has no group for `owner`.
// Names and layouts are placeholders.
#include <algorithm>

#include "game_s2/UnkWeakHandle.h"
#include "types.h"

static inline UnkWeakTarget* TargetOf(UnkWeakHandle* handle) { return handle ? handle->target : nullptr; }

// A counted reference to a weak handle, held for the duration of a lookup.
struct UnkWeakRef_ld47e48 {
    UnkWeakHandle* handle;

    explicit UnkWeakRef_ld47e48(UnkWeakHandle* h) : handle(h) {
        if (h)
            ++h->ref_count;
    }
    ~UnkWeakRef_ld47e48() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
    UnkWeakTarget* Target() const { return TargetOf(handle); }
};

#pragma pack(push, 4)
struct UnkWeakGroup_ld47e48 {
    u32 count;
    UnkWeakHandle* items[128];
};
#pragma pack(pop)

struct UnkWeakGroupMap_ld47e48 {
    UnkWeakGroup_ld47e48* groups;
    UnkWeakHandle** keys;
    u32 count;

    UnkWeakGroup_ld47e48* End() const { return &groups[count]; }
    UnkWeakGroup_ld47e48* Find(const UnkWeakRef_ld47e48& key) const {
        for (u64 i = 0; i < count; ++i) {
            if (TargetOf(keys[i]) == key.Target())
                return &groups[(u32)i];
        }
        return End();
    }
};

struct UnkGroupOwner_ld47e48 {
    u8 pad_0x0[0xc8];
    UnkWeakGroupMap_ld47e48 map;  // 0xc8
};

bool fn_7100d47e48(UnkGroupOwner_ld47e48* self, UnkWeakTarget* owner, UnkWeakTarget* target) {
    UnkWeakRef_ld47e48 key(GetWeakHandle(owner));
    if (self->map.Find(key) == self->map.End())
        return false;
    UnkWeakGroup_ld47e48* group = self->map.Find(key);
    UnkWeakRef_ld47e48 wanted(GetWeakHandle(target));
    UnkWeakHandle** begin = group->items;
    UnkWeakHandle** end = group->items + group->count;
    return std::find_if(begin, end, [&wanted](UnkWeakHandle* h) { return TargetOf(h) == wanted.Target(); }) != end;
}
