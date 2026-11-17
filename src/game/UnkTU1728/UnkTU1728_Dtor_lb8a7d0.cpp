// Unit 1728 (timeline motion): destroys a timeline set. Its keyed parts are cleared first, then its
// members are released last to first: the tracks, the owned objects, the shared part, the keyed parts'
// arrays, the key list, the resource and the name. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU1728.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CRntSmallDictionary;
using base::global::CRntVector;

struct UnkRecord120;

static inline void ReleaseResource(UnkCountedResource* resource) {
    if (resource && --resource->ref_count == 0)
        fn_710001737c(resource);
}

// A keyed part (0x120 bytes), destroyed out of line.
struct UnkTimelinePart_lb8a7d0 {
    u8 pad_0x0[0x120];
    ~UnkTimelinePart_lb8a7d0() { fn_7100b8b7e8(reinterpret_cast<UnkRecord120*>(this)); }
};

// A track: two lists and a resource.
struct UnkTimelineTrack_lb8a7d0 {
    u64 field_0x0;
    CRntVector<u32> field_0x8;
    CRntVector<u32> field_0x30;
    UnkCountedResource* resource;  // +0x58

    ~UnkTimelineTrack_lb8a7d0() {
        ReleaseResource(resource);
        field_0x30.Free();
        field_0x8.Free();
    }
};
static_assert(sizeof(UnkTimelineTrack_lb8a7d0) == 0x60, "UnkTimelineTrack_lb8a7d0");

struct UnkTimelineParts_lb8a7d0 : CRntSmallDictionary<u64, UnkTimelinePart_lb8a7d0> {
    void Free() {
        Clear();
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

struct UnkTimelineSet_lb8a7d0 {
    u8 pad_0x0[0x38];
    base::UnkStrIdMember name;               // +0x38
    u8 pad_0x40[0x10];
    UnkCountedResource* resource;            // +0x50
    CRntVector<u32> keys;                    // +0x58
    UnkTimelineParts_lb8a7d0 parts;          // +0x80
    u8 shared_part[0xc8];                    // +0xa8
    UnkOwnedList owned;                      // +0x170
    CRntVector<UnkTimelineTrack_lb8a7d0> tracks;  // +0x198
};
static_assert(__builtin_offsetof(UnkTimelineSet_lb8a7d0, owned) == 0x170, "layout");
static_assert(__builtin_offsetof(UnkTimelineSet_lb8a7d0, tracks) == 0x198, "layout");

void fn_7100b8a7d0(void* value) {
    auto* self = static_cast<UnkTimelineSet_lb8a7d0*>(value);
    self->parts.Clear();
    self->tracks.Free();
    self->owned.~UnkOwnedList();
    fn_7100b8aa94(self->shared_part);
    self->parts.Free();
    self->keys.Free();
    ReleaseResource(self->resource);
    self->name.~UnkStrIdMemberOf();
}
