// CAudioComponent's unit (tools/tu_map.py): the component's two sound arrays. The component holds a
// growable array of 0xd0-byte sound slots at +0x48 (each with string ids at +0x0, +0x20, a file-path
// id at +0x28, a handle to the playing sound at +0x78 and the sound's id pair at +0x98) and one of
// 0xb0-byte sound links at +0x70 (a handle at +0x0, string and file-path ids at +0x38/+0x40, an id
// pair at +0x90). This file has the constructor, the slot and link clean-ups, the link array's
// reserve, a link's copy and its constructor, the stop of every slot, the lookup of a link by handle,
// the per-group level setter and the save of the group settings. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAudioComponent.h"
#include "game/CComponent.h"
#include "game/UnkTU101.h"
#include "game/UnkTU50.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;

// ---- Shared declarations ----

struct UnkEntitySoundComponent;

struct UnkIdPair;
void fn_7100026f70(UnkIdPair* pair);

struct UnkStateObject;
extern void* g_unk_7101a6cca0[];  // the component's vtables

struct UnkStruct_710071faf4;
void fn_710071faf4(UnkStruct_710071faf4* self, const UnkStruct_710071faf4* other);

struct UnkInterp_7100105f94;

// As src/game/CAudioComponent/CAudioComponent_Stop_b65a4.cpp has them.
struct UnkSoundObject_b50;
struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};

// The object a handle names while it is still the one the handle was made for.
static inline UnkSoundObject_b50* Resolve_ovb0015(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return handle->ref->object;
    return nullptr;
}

// As src/game/CRumbleComponent.cpp has it.
struct UnkHandleTarget;
struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};

// The eight sound groups (their bit numbers in a group mask).
extern const s32 g_unk_710164a8ec[8];

// CStrId::Reset on the file-path pool.
struct UnkAudioPathId_ovb0015 {
    base::global::UnkStringData* data;

    void Reset() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// Ids and the counted resource as the copies see them (as src/game/CAudioComponent/fn_710071faf4.cpp):
// the count is a plain member, and the pool's empty entry stays in a register across the increments.
struct UnkIdData_ovb0015 {
    u8 unk_0x0[0x8];
    s32 ref_count;
};

template <base::global::UnkStringPool** Pool>
struct UnkId_ovb0015 {
    UnkIdData_ovb0015* data;
    UnkId_ovb0015(const UnkId_ovb0015& other) : data(other.data) {
        if (&(*Pool)->empty != reinterpret_cast<base::global::UnkStringData*>(data))
            ++data->ref_count;
    }
};
using UnkStrId_ovb0015 = UnkId_ovb0015<&g_unk_7101d47108>;
using UnkPathIdCopy_ovb0015 = UnkId_ovb0015<&g_unk_7101d47200>;

struct UnkShared_ovb0015 {
    u8 unk_0x0[0x54];
    s32 ref_count;
};
typedef s32 __attribute__((may_alias)) UnkAliasS32_ovb0015;

struct UnkRef_ovb0015 {
    UnkShared_ovb0015* ptr;
    UnkRef_ovb0015(const UnkRef_ovb0015& other) : ptr(nullptr) {
        if (UnkShared_ovb0015* p = other.ptr) {
            ++*reinterpret_cast<UnkAliasS32_ovb0015*>(&p->ref_count);
            if (ptr && --ptr->ref_count == 0)
                fn_710001737c(reinterpret_cast<UnkCountedResource*>(ptr));
            ptr = p;
        }
    }
};

// A string id, a file-path id and the resource they name (the slots' +0x98, the links' +0x90).
struct UnkAudioIdRes_ovb0015 {
    UnkStrId_ovb0015 id;
    UnkPathIdCopy_ovb0015 path;
    UnkRef_ovb0015 resource;
};

// ---- The sound slots (0xd0 bytes) ----

// What a slot's handle names: its current values, saved into the slot when it stops.
struct UnkAudioSoundState_ovb0015 {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0x50];
    u32 field_0x98[3];
    u8 pad_0xa4[0x40];
    u32 field_0xe4;
    u32 field_0xe8;
    u32 field_0xec;
    u8 pad_0xf0[0xc];
    u32 field_0xfc;
};

struct UnkAudioSound_ovb0015 {
    CStrId field_0x0;
    s32 group;  // 0x8
    u8 pad_0xc[0x14];
    CStrId field_0x20;
    UnkAudioPathId_ovb0015 field_0x28;
    u32 field_0x30[3];
    u8 pad_0x3c[0x4];
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u8 pad_0x50[0x4];
    u32 field_0x54;
    u8 pad_0x58[0x20];
    UnkSoundHandle handle;  // 0x78
    u8 pad_0x88[0x10];
    u8 field_0x98[0x18];  // the id pair (UnkIdPair)
    u8 pad_0xb0[0x10];
    bool stopped;  // 0xc0
    u8 pad_0xc1[0xf];
};
static_assert(sizeof(UnkAudioSound_ovb0015) == 0xd0, "UnkAudioSound_ovb0015 layout");

// Releases the slot's ids: the id pair, then the file-path id and the string ids, left empty.
__attribute__((noinline)) void fn_710071f878(UnkAudioSound_ovb0015* self) {
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(self->field_0x98));
    self->field_0x28.Reset();
    self->field_0x20.Reset();
    self->field_0x0.Reset();
}

// ---- The sound links (0xb0 bytes) ----

struct UnkWords5_ovb0015 {
    u32 field_0x0[5];
};
struct UnkPair64_ovb0015 {
    u64 field_0x0[2];
};
struct UnkBytes_ovb0015 {
    u8 field_0x0[0x3b];
};

// A link as its copy constructor sees it.
struct UnkAudioLinkCopy_ovb0015 {
    UnkPair64_ovb0015 field_0x0;
    UnkPair64_ovb0015 field_0x10;
    UnkWords5_ovb0015 field_0x20;
    UnkStrId_ovb0015 field_0x38;
    UnkPathIdCopy_ovb0015 field_0x40;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    UnkBytes_ovb0015 field_0x54;
    UnkStrId_ovb0015 field_0x90;
    UnkPathIdCopy_ovb0015 field_0x98;
    UnkRef_ovb0015 field_0xa0;
    bool field_0xa8;
    bool field_0xa9;

    UnkAudioLinkCopy_ovb0015(const UnkAudioLinkCopy_ovb0015& o)
        : field_0x0(o.field_0x0), field_0x10(o.field_0x10), field_0x20(o.field_0x20),
          field_0x38(o.field_0x38), field_0x40(o.field_0x40), field_0x48(o.field_0x48),
          field_0x4c(o.field_0x4c), field_0x50(o.field_0x50), field_0x54(o.field_0x54),
          field_0x90(o.field_0x90), field_0x98(o.field_0x98), field_0xa0(o.field_0xa0),
          field_0xa8(o.field_0xa8), field_0xa9(o.field_0xa9) {}
};
static_assert(sizeof(UnkAudioLinkCopy_ovb0015) == 0xb0, "UnkAudioLinkCopy_ovb0015 layout");

// The constructor is inlined into the original; flatten keeps it from being emitted out of line.
__attribute__((flatten, noinline)) void fn_710071f97c(UnkAudioLinkCopy_ovb0015* self,
                                                      const UnkAudioLinkCopy_ovb0015* other) {
    new (self) UnkAudioLinkCopy_ovb0015(*other);
}

// A link as a new one is built: the handle, the null handle, defaults (the empty ids, distances
// and factors) and the sound's ids and resource.
struct UnkAudioLinkNew_ovb0015 {
    UnkSoundHandle handle;
    UnkHandle field_0x10;
    u64 field_0x20;
    u64 field_0x28;
    u32 field_0x30;
    UnkStrId_ovb0015 field_0x38;
    UnkPathIdCopy_ovb0015 field_0x40;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    f32 field_0x70;
    f32 field_0x74;
    f32 field_0x78;
    f32 field_0x7c;
    f32 field_0x80;
    f32 field_0x84;
    f32 field_0x88;
    bool field_0x8c;
    bool field_0x8d;
    bool field_0x8e;
    UnkAudioIdRes_ovb0015 sound;  // 0x90
    bool field_0xa8;
    bool field_0xa9;

    UnkAudioLinkNew_ovb0015(UnkSoundHandle h, const UnkAudioIdRes_ovb0015& s)
        : handle(h), field_0x10(g_unk_7101d586c0), field_0x20(0), field_0x28(0), field_0x30(0),
          field_0x38(reinterpret_cast<const UnkStrId_ovb0015&>(g_unk_7101d470e0)),
          field_0x40(reinterpret_cast<const UnkPathIdCopy_ovb0015&>(g_unk_7101d470e8)),
          field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f), field_0x58(1.0f),
          field_0x5c(500.0f), field_0x60(2000.0f), field_0x64(1.0f), field_0x68(0.0f), field_0x6c(0.0f),
          field_0x70(0.0f), field_0x74(-1.0f), field_0x78(-1.0f), field_0x7c(-1.0f), field_0x80(0.5f),
          field_0x84(5.0f), field_0x88(10.0f), field_0x8c(false), field_0x8d(false), field_0x8e(false),
          sound(s), field_0xa8(false), field_0xa9(false) {}
};
static_assert(sizeof(UnkAudioLinkNew_ovb0015) == 0xb0, "UnkAudioLinkNew_ovb0015 layout");

void fn_7100718994(UnkAudioLinkNew_ovb0015* self, UnkSoundHandle handle);

// Builds a link for `handle` playing `sound`, then sets it up (fn_7100718994).
__attribute__((flatten)) void fn_710071cca8(UnkAudioLinkNew_ovb0015* self, UnkSoundHandle handle,
                                            const UnkAudioIdRes_ovb0015* sound) {
    new (self) UnkAudioLinkNew_ovb0015(handle, *sound);
    fn_7100718994(self, handle);
}

// A link as the rest of the unit sees it.
struct UnkAudioLink_ovb0015 {
    UnkSoundHandle handle;  // 0x0
    u8 pad_0x10[0x28];
    CStrId field_0x38;
    UnkAudioPathId_ovb0015 field_0x40;
    u8 pad_0x48[0x48];
    u8 field_0x90[0x18];  // the id pair (UnkIdPair)
    u8 pad_0xa8[0x1];
    bool stopped;  // 0xa9
    u8 pad_0xaa[0x6];

    __attribute__((always_inline)) void Release() {
        fn_7100026f70(reinterpret_cast<UnkIdPair*>(field_0x90));
        field_0x40.Reset();
        field_0x38.Reset();
    }
};
static_assert(sizeof(UnkAudioLink_ovb0015) == 0xb0, "UnkAudioLink_ovb0015 layout");

// The engine's growable array (base/UnkVector.h) over the slots and the links.
template <class T>
struct UnkAudioVector_ovb0015 {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkAudioVector_ovb0015(u32 step_)
        : data(nullptr), count(0), capacity(0), step(step_), field_0x14(true), flags(0xb),
          allocator(fn_7100083990()) {}
};

// Makes room for `n` links: the links are copied into a new array and released in the old one.
__attribute__((noinline)) void fn_7100719444(UnkAudioVector_ovb0015<UnkAudioLink_ovb0015>* self, u32 n) {
    if (self->capacity >= n)
        return;
    UnkAudioLink_ovb0015* p = static_cast<UnkAudioLink_ovb0015*>(
        self->allocator->vf_0x40((u64)n * sizeof(UnkAudioLink_ovb0015), 8, self->flags));
    if (UnkAudioLink_ovb0015* src = self->data) {
        UnkAudioLink_ovb0015* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            fn_710071f97c(reinterpret_cast<UnkAudioLinkCopy_ovb0015*>(dst),
                          reinterpret_cast<UnkAudioLinkCopy_ovb0015*>(src));
            src->Release();
        }
        self->allocator->vf_0x48(self->data);
    }
    self->data = p;
    self->capacity = n;
}

// ---- The component ----

// Two parallel arrays (records and their pointers) sharing one count.
struct UnkAudioPairList_ovb0015 {
    void* data;
    void** ptrs;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkAudioPairList_ovb0015()
        : data(nullptr), ptrs(nullptr), count(0), capacity(0), step(10), flags(0xe),
          allocator(fn_7100083990()) {}
};

struct UnkAudioComponent_ovb0015 {
    void** vtable;
    u8 pad_0x8[0x38];
    void** vtable_0x40;
    UnkAudioVector_ovb0015<UnkAudioSound_ovb0015> sounds;  // 0x48
    UnkAudioVector_ovb0015<UnkAudioLink_ovb0015> links;    // 0x70
    UnkAudioPairList_ovb0015 pairs;                        // 0x98
    u16 field_0xc0;
    u8 pad_0xc2[0x2];
    u8 field_0xc4[0x24];
    u8 pad_0xe8[0x30];
    s32 group_levels[8];  // 0x118
    u8 pad_0x138[0x20];
    u32 field_0x158;
};
static_assert(__builtin_offsetof(UnkAudioComponent_ovb0015, group_levels) == 0x118, "layout");
static_assert(__builtin_offsetof(UnkAudioComponent_ovb0015, field_0x158) == 0x158, "layout");

// The constructor: the base component, the vtables, room for 128 slots and 128 links.
void fn_7100718c80(UnkAudioComponent_ovb0015* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a6cca0 + 2;
    self->vtable_0x40 = g_unk_7101a6cca0 + 0x34;
    new (&self->sounds) UnkAudioVector_ovb0015<UnkAudioSound_ovb0015>(0x20);
    UnkAudioVector_ovb0015<UnkAudioSound_ovb0015>& v = self->sounds;
    if (v.capacity < 0x80) {
        UnkAudioSound_ovb0015* p = static_cast<UnkAudioSound_ovb0015*>(
            v.allocator->vf_0x40(0x80 * sizeof(UnkAudioSound_ovb0015), 8, v.flags));
        if (UnkAudioSound_ovb0015* src = v.data) {
            UnkAudioSound_ovb0015* dst = p;
            for (u32 i = 0; i < v.count; ++i, ++dst, ++src) {
                fn_710071faf4(reinterpret_cast<UnkStruct_710071faf4*>(dst),
                              reinterpret_cast<UnkStruct_710071faf4*>(src));
                fn_710071f878(src);
            }
            v.allocator->vf_0x48(v.data);
        }
        v.data = p;
        v.capacity = 0x80;
    }
    new (&self->links) UnkAudioVector_ovb0015<UnkAudioLink_ovb0015>(0x80);
    fn_7100719444(&self->links, 0x80);
    new (&self->pairs) UnkAudioPairList_ovb0015();
    self->field_0xc0 = 0;
    self->field_0x158 = 0;
    __builtin_memset(self->field_0xc4, 0, sizeof(self->field_0xc4));
}

// Stops every slot: a sound still playing has its values saved into the slot and is stopped
// (fn_7100105034), the slot gets the empty handle; the live count drops for each slot. Then every
// slot's ids are released and the array emptied.
void fn_710071e534(UnkAudioComponent_ovb0015* self) {
    UnkAudioSound_ovb0015* end = self->sounds.data + self->sounds.count;
    for (UnkAudioSound_ovb0015* s = self->sounds.data; s != end; ++s) {
        if (UnkAudioSoundState_ovb0015* state =
                reinterpret_cast<UnkAudioSoundState_ovb0015*>(Resolve_ovb0015(&s->handle))) {
            s->field_0x4c = state->field_0x44;
            s->field_0x40 = state->field_0xec;
            s->field_0x54 = state->field_0xfc;
            s->field_0x30[0] = state->field_0x98[0];
            s->field_0x30[1] = state->field_0x98[1];
            s->field_0x30[2] = state->field_0x98[2];
            // Read again without type-based alias information (the original reloads it).
            UnkAudioSoundState_ovb0015* current;
            __builtin_memcpy(&current, &s->handle.ref->object, sizeof(current));
            s->field_0x44 = current->field_0xe8;
            s->field_0x48 = current->field_0xe4;
            fn_7100105034(state);
            s->handle = g_unk_7108b6c460;
            s->stopped = false;
        }
        --g_unk_717270f61c;
    }
    for (u32 i = 0; i < self->sounds.count; ++i)
        fn_710071f878(&self->sounds.data[i]);
    self->sounds.count = 0;
}

// The first link not stopped whose handle names the same sound as `handle` (both may name none).
UnkAudioLink_ovb0015* fn_7100719d00(UnkAudioComponent_ovb0015* self, const UnkSoundHandle* handle) {
    UnkAudioLink_ovb0015* link = self->links.data;
    UnkAudioLink_ovb0015* end = link + self->links.count;
    UnkSoundHandle target = *handle;
    for (; link != end; ++link) {
        if (!link->stopped && Resolve_ovb0015(&link->handle) == Resolve_ovb0015(&target))
            return link;
    }
    return nullptr;
}

// Sets the level of every group in the mask that has another level, and of the playing sounds of
// those groups (fn_7100105f94, over `time`).
void fn_710071f634(UnkAudioComponent_ovb0015* self, const u32* groups, s32 level, f32 time) {
    for (const s32* g = g_unk_710164a8ec; g != g_unk_710164a8ec + 8; ++g) {
        s32 group = *g;
        if (!(*groups & (1 << group)) || self->group_levels[group] == level)
            continue;
        self->group_levels[group] = level;
        for (UnkAudioSound_ovb0015* s = self->sounds.data; s != self->sounds.data + self->sounds.count; ++s) {
            u32 bit = 1 << s->group;
            if (!(*groups & bit))
                continue;
            if (UnkInterp_7100105f94* sound = reinterpret_cast<UnkInterp_7100105f94*>(Resolve_ovb0015(&s->handle)))
                fn_7100105f94(sound, level, time);
        }
    }
}

// ---- The group settings ----

struct UnkAudioSettings_ovb0015 {
    u32 field_0x0;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;

    UnkAudioSettings_ovb0015& operator=(const UnkAudioSettings_ovb0015& o) {
        field_0x0 = o.field_0x0;
        field_0x4 = o.field_0x4;
        field_0x8 = o.field_0x8;
        field_0x10 = o.field_0x10;
        field_0x14 = o.field_0x14;
        field_0x18 = o.field_0x18;
        field_0x1c = o.field_0x1c;
        return *this;
    }
};

struct UnkAudioSettingsOwner_ovb0015 {
    u8 pad_0x0[0x118];
    UnkAudioSettings_ovb0015 current;  // 0x118
    UnkAudioSettings_ovb0015 saved;    // 0x138
};

// Saves the group settings at +0x118 to +0x138, then stops the sounds (fn_7100719064 with true,
// false, true, true).
void fn_710071f5e8(UnkAudioSettingsOwner_ovb0015* self) {
    self->saved = self->current;
    fn_7100719064(reinterpret_cast<UnkEntitySoundComponent*>(self), true, false, true, true);
}
