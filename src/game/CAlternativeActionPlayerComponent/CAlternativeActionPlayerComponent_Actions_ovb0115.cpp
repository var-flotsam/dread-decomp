// CAlternativeActionPlayerComponent's unit (tools/tu_map.py unit 1138): the component keeps parallel
// arrays of animation ids (+0x50) and 16-byte entries (+0x48; count +0x58, capacity +0x5c, step +0x60).
// fn_7100712538 clears them on a 0xf4061ff25a2163d6 message of kind 2 and leaves the deferred set it joined
// (+0x90); fn_710071263c grows the arrays (as CAlternativeActionPlayerComponent_Add_A3_b74.cpp uses it).
// fn_7100711c88 plays the request the owner's CCharClassTweakAbortAlternativeActionTrack fills in;
// fn_71007121e0 builds a named request (descriptors as src/game_s2/UnkSpitSend.h) and fn_7100711fe8 plays an
// entry's animation once a random draw falls under its building chance. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

// As src/game/CAnimationComponent/fn_7101176ea8.cpp declares it (the key is a hash).
struct UnkStruct_7101176ea8;
struct UnkRequest_7101176ea8;

struct UnkClone_fn_710071294c;
using UnkAltActionRemoveSet_ovb0115 = base::UnkDeferredSet<UnkClone_fn_710071294c>;
extern template void UnkAltActionRemoveSet_ovb0115::Remove(UnkClone_fn_710071294c*);

struct UnkAltActionMsgType_ovb0115 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkAltActionMsg_ovb0115 {
    u32 kind;
    u8 pad_0x4[0xc];
    UnkAltActionMsgType_ovb0115* type;
};

struct UnkAltActionClear_ovb0115 {
    u8 pad_0x0[0x50];
    base::global::UnkStringData** ids;  // 0x50
    u32 count;                          // 0x58
    u8 pad_0x5c[0x34];
    UnkAltActionRemoveSet_ovb0115* set;  // 0x90
};

bool fn_7100712538(UnkAltActionClear_ovb0115* self, UnkAltActionMsg_ovb0115* msg) {
    if (msg->type->hash != 0xf4061ff25a2163d6ull || msg->kind != 2)
        return true;
    if (self->ids && self->count) {
        for (u64 i = 0; i < self->count; ++i) {
            base::global::UnkStringPool* pool = g_unk_7101d47108;
            if (!pool)
                continue;
            base::global::UnkStringData** ids = self->ids;
            base::global::UnkStringData* data = ids[i];
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (ids[i]->ref_count == 0) {
                    fn_71000803a4(pool, ids[i]);
                    pool = g_unk_7101d47108;
                }
            }
            ids[i] = &pool->empty;
        }
    }
    self->count = 0;
    if (self->set) {
        self->set->Remove(reinterpret_cast<UnkClone_fn_710071294c*>(&self->set));
        self->set = nullptr;
    }
    return false;
}

struct UnkAltActionRect_ovb0115 {
    f32 v[4];
};

// The parallel arrays of CAlternativeActionPlayerComponent_Add_A3_b74.cpp (UnkAltActionArrays_A3_b74).
struct UnkAltActionArrays_ovb0115 {
    UnkAltActionRect_ovb0115* rects;  // 0x0
    CStrId* ids;                      // 0x8
    u32 count;                        // 0x10
    u32 capacity;                     // 0x14
    u32 step;                         // 0x18
    u32 flags;                        // 0x1c
    base::UnkAllocator* allocator;    // 0x20
};

struct UnkAltActionArrays_A3_b74;

// Grows both arrays to `capacity`: the rectangles are copied, the ids moved.
void fn_710071263c(UnkAltActionArrays_A3_b74* arrays, u32 capacity) {
    UnkAltActionArrays_ovb0115* self = reinterpret_cast<UnkAltActionArrays_ovb0115*>(arrays);
    if (self->capacity >= capacity)
        return;
    UnkAltActionRect_ovb0115* rects = static_cast<UnkAltActionRect_ovb0115*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkAltActionRect_ovb0115), 8, self->flags));
    CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
    if (self->rects) {
        __builtin_memcpy(rects, self->rects, (u64)self->count * sizeof(UnkAltActionRect_ovb0115));
        self->allocator->vf_0x48(self->rects);
    }
    if (CStrId* src = self->ids) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++src) {
            UnkStringData* data = src->data;
            __builtin_memcpy(&ids[j], &data, sizeof(data));
            src->data = &g_unk_7101d47108->empty;
            // The moved-from id's release, decrement and test in two steps.
            if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
                if (src->data && src->data != &pool->empty) {
                    --src->data->ref_count;
                    if (src->data->ref_count == 0) {
                        fn_71000803a4(pool, src->data);
                        pool = g_unk_7101d47108;
                    }
                }
                src->data = &pool->empty;
            }
        }
        self->allocator->vf_0x48(self->ids);
    }
    self->rects = rects;
    self->ids = ids;
    self->capacity = capacity;
}

struct UnkAltDesc_ovb0115 {
    u64 words[2];
};

// As in src/game/CCharClassToStateTrack/CCharClassToStateTrack_Fill.cpp.
struct __attribute__((packed, aligned(4))) UnkAltBlend_ovb0115 {
    u64 kind;
    f32 value;
};

// An animation request: unset while the id is -1.
struct UnkAltRequest_ovb0115 {
    s64 id;
    u64 desc[2];       // 0x8
    u64 blend_kind;    // 0x18
    u32 blend_value;   // 0x20
    u32 flags;         // 0x24
    u32 mask;          // 0x28
};

// What the abort track fills in.
struct UnkAltAbortTrack_ovb0115 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkAltRequest_ovb0115* request);
};

struct UnkAltOwner_ovb0115 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkAltActionAbort_ovb0115 {
    u8 pad_0x0[0x30];
    UnkAltOwner_ovb0115* owner;  // 0x30
};

// Asks the owner's CCharClassTweakAbortAlternativeActionTrack for an animation request and plays it.
void fn_7100711c88(UnkAltActionAbort_ovb0115* self) {
    UnkAltRequest_ovb0115 request;
    const UnkAltDesc_ovb0115* desc = reinterpret_cast<const UnkAltDesc_ovb0115*>(&g_unk_710164b958);
    u64 desc0 = desc->words[0];
    u64 desc1 = desc->words[1];
    const UnkAltBlend_ovb0115* blend = reinterpret_cast<const UnkAltBlend_ovb0115*>(&g_unk_7172769740);
    u64 kind = blend->kind;
    u32 value = *reinterpret_cast<const u32*>(&blend->value);
    u32 mask = UnkAll_SpitSend().value;
    request.id = -1;
    request.desc[0] = desc0;
    request.desc[1] = desc1;
    request.blend_kind = kind;
    request.blend_value = value;
    request.flags = 0;
    request.mask = mask;
    UnkTrackQuery query = {fn_7100c5561c(), nullptr, -1.0f};
    if (!fn_710117b08c(self->owner->animation, &query))
        return;
    reinterpret_cast<UnkAltAbortTrack_ovb0115*>(query.result)->vf_0x18(&request);
    if (request.id == -1)
        return;
    fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(self->owner->animation),
                  reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6);
}

struct UnkAltDesc_ovb0115c {
    u64 words[2];
};

struct __attribute__((packed, aligned(4))) UnkAltBlend_ovb0115c {
    u64 kind;
    f32 value;
};

// A request's flags (an 8-byte slot when built as a temporary).
struct alignas(8) UnkAltFlags_ovb0115 {
    u32 value;
    UnkAltFlags_ovb0115() { value = 0; }
};

// An animation request with a name.
struct UnkAltNamedRequest_ovb0115;
void fn_71007121e0(UnkAltNamedRequest_ovb0115* self, const CStrId& name, const UnkAltFlags_ovb0115& flags, UnkAltDesc_ovb0115c desc,
                   UnkAltBlend_ovb0115c blend);
struct UnkAltNamedRequest_ovb0115 {
    u64 id;
    UnkAltDesc_ovb0115c desc;    // 0x8
    UnkAltBlend_ovb0115c blend;  // 0x18
    u32 flags;                   // 0x24
    u32 mask;                    // 0x28
    u8 pad_0x2c[4];
    union {
        CStrId name;  // 0x30
    };

    UnkAltNamedRequest_ovb0115(const CStrId& name, const UnkAltFlags_ovb0115& flags, UnkAltDesc_ovb0115c desc,
                               UnkAltBlend_ovb0115c blend) {
        fn_71007121e0(this, name, flags, desc, blend);
    }
    // The name's release, decrement and test in two steps.
    ~UnkAltNamedRequest_ovb0115() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (name.data && name.data != &pool->empty) {
                --name.data->ref_count;
                if (name.data->ref_count == 0) {
                    fn_71000803a4(pool, name.data);
                    pool = g_unk_7101d47108;
                }
            }
            name.data = &pool->empty;
        }
    }
};

// Builds a request for the animation `name` (its hash is the id) with an empty name.
// Weak so that its caller in this file cannot see that it leaves the name alone: the original reloads the
// caller's temporary id after the call.
__attribute__((weak)) void fn_71007121e0(UnkAltNamedRequest_ovb0115* self, const CStrId& name, const UnkAltFlags_ovb0115& flags, UnkAltDesc_ovb0115c desc,
                   UnkAltBlend_ovb0115c blend) {
    u64 id = *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(name.data) + 0x30);
    u32 f = flags.value;
    u32 mask = UnkAll_SpitSend().value;
    self->id = id;
    self->desc = desc;
    self->blend = blend;
    self->flags = f;
    self->mask = mask;
    self->name.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->name.data = &g_unk_7101d47108->empty;
}

extern "C" int rand();

struct UnkAltRect_ovb0115c {
    f32 v[4];
};

struct UnkAltOwner_ovb0115c {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;  // 0x220
};

struct UnkAltActionPlay_ovb0115 {
    u8 pad_0x0[0x30];
    UnkAltOwner_ovb0115c* owner;  // 0x30
    u8 pad_0x38[0x10];
    UnkAltRect_ovb0115c* rects;  // 0x48
    CStrId* ids;                 // 0x50
    u32 count;                   // 0x58

    UnkAltRect_ovb0115c* End() { return &rects[count]; }
    UnkAltRect_ovb0115c* Find(const CStrId& id) {
        for (u64 i = 0; i < count; ++i)
            if (ids[i].data == id.data)
                return &rects[i];
        return End();
    }
};

// While the entry for `id` is idle, its chance builds up by `step` (up to `max`) until a random draw
// falls below it; then its animation is requested.
void fn_7100711fe8(UnkAltActionPlay_ovb0115* self, const CStrId& id, f32 step, f32 max) {
    UnkAltRect_ovb0115c* rect = self->Find(id);
    if (rect == self->End() || rect->v[2] != 0.0f)
        return;
    f32 draw = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
    if (!(rect->v[3] >= draw)) {
        f32 chance = rect->v[3] + step;
        rect->v[3] = chance > max ? max : chance;
        return;
    }
    const UnkAltDesc_ovb0115c* desc = reinterpret_cast<const UnkAltDesc_ovb0115c*>(&g_unk_710164b958);
    const UnkAltBlend_ovb0115c* blend = reinterpret_cast<const UnkAltBlend_ovb0115c*>(&g_unk_7172769740);
    UnkAltNamedRequest_ovb0115 request(CStrId(id), UnkAltFlags_ovb0115(), *desc, *blend);
    fn_7101176ea8(self->owner->animation, reinterpret_cast<const UnkRequest_7101176ea8*>(&request),
                  0xf4061ff25a2163d6);
}
