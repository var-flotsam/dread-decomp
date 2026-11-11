// Unit 2506 (no reliable name): tells whether the group kept for one object (looked up through its weak
// handle) holds a record whose first reference points at a second object.
// Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// A counted reference to a weak handle; two of them are equal when they reach the same object.
struct UnkWeakRef_ld48698 {
    UnkWeakHandle* handle;

    explicit UnkWeakRef_ld48698(UnkWeakHandle* h) : handle(h) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkWeakRef_ld48698() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
    UnkWeakTarget* Get() const { return handle ? handle->target : nullptr; }
};

#pragma pack(push, 4)
struct UnkSlotRecord_ld48698 {
    UnkWeakRef_ld48698 refs[5];
    u8 pad_0x28[0x48];
};
struct UnkSlotGroup_ld48698 {
    u32 count;
    UnkSlotRecord_ld48698 items[128];

    UnkSlotRecord_ld48698* Begin() { return items; }
    UnkSlotRecord_ld48698* End() { return &items[count]; }
};
#pragma pack(pop)
static_assert(sizeof(UnkSlotRecord_ld48698) == 0x70);
static_assert(sizeof(UnkSlotGroup_ld48698) == 0x3804);

// Groups keyed by a weak reference, in two parallel arrays.
struct UnkGroupMap_ld48698 {
    UnkSlotGroup_ld48698* groups;
    UnkWeakRef_ld48698* keys;
    u32 count;

    u32 IndexOf(const UnkWeakRef_ld48698& key) const {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i].Get() == key.Get())
                return i;
        }
        return count;
    }
    UnkSlotGroup_ld48698* End() { return &groups[count]; }
    UnkSlotGroup_ld48698* Find(const UnkWeakRef_ld48698& key) {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i].Get() == key.Get())
                return &groups[static_cast<u32>(i)];
        }
        return End();
    }
    UnkSlotGroup_ld48698& At(const UnkWeakRef_ld48698& key) { return groups[IndexOf(key)]; }
};

struct UnkOwner_ld48698 {
    u8 pad_0x0[0xf0];
    UnkGroupMap_ld48698 map;  // 0xf0
};

bool fn_7100d48698(UnkOwner_ld48698* self, UnkWeakTarget* owner, UnkWeakTarget* wanted) {
    UnkWeakRef_ld48698 key(GetWeakHandle(owner));
    if (self->map.Find(key) == self->map.End())
        return false;
    UnkSlotGroup_ld48698& group = self->map.At(key);
    UnkSlotRecord_ld48698* it = group.Begin();
    UnkSlotRecord_ld48698* end = group.End();
    for (; it != end; ++it) {
        if (it->refs[0].Get() == wanted)
            break;
    }
    return it != end;
}
