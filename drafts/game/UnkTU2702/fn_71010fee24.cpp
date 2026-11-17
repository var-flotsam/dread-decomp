// UnkTU2702 (CAIComponent's unit): the length of the walk animation's forward motion in a direction,
// cached per direction in the component's map at +0x144b8; -1 for any animation other than "walk".
// Names and layouts are placeholders.
#include <math.h>
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_7101176ea8;

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

u64 fn_71002a6bdc(u32 value);

// One cached length: in its bucket's chain (+0x18) and in the map's insertion list (+0x8, +0x10).
struct UnkLengthNode_l0fee24 {
    u32 key;
    f32 value;
    UnkLengthNode_l0fee24* next;
    UnkLengthNode_l0fee24* prev;
    UnkLengthNode_l0fee24* chain;

    UnkLengthNode_l0fee24(u32 key, f32 value) : key(key), value(value), next(nullptr), prev(nullptr), chain(nullptr) {}
};

struct UnkLengthMap_l0fee24 {
    base::UnkAllocator* allocator;
    u32 alloc_flags;  // 0x8
    u32 count;        // 0xc
    UnkLengthNode_l0fee24** buckets;  // 0x10
    u32 bucket_count;                 // 0x18
    u32 modulus;                      // 0x1c
    u8 pad_0x20[0x18];
    UnkLengthNode_l0fee24* tail;  // 0x38
    UnkLengthNode_l0fee24* head;  // 0x40

    UnkLengthNode_l0fee24** Bucket(u32 key) {
        if (bucket_count == 0)
            return nullptr;
        return &buckets[key % modulus];
    }

    bool Contains(u32 key) {
        UnkLengthNode_l0fee24** bucket = Bucket(key);
        if (!bucket)
            return false;
        for (UnkLengthNode_l0fee24* node = *bucket; node; node = node->chain) {
            if (node->key == key)
                return true;
        }
        return false;
    }

    f32 At(u32 key) {
        UnkLengthNode_l0fee24* node = *Bucket(key);
        while (node->key != key)
            node = node->chain;
        return node->value;
    }

    void Insert(u32 key, f32 value) {
        UnkLengthNode_l0fee24** bucket = Bucket(key);
        UnkLengthNode_l0fee24* first = *bucket;
        void* mem = allocator->vf_0x40(sizeof(UnkLengthNode_l0fee24), 8, alloc_flags);
        UnkLengthNode_l0fee24* node = new (mem) UnkLengthNode_l0fee24(key, value);
        node->chain = first;
        *bucket = node;
        if (!head) {
            head = node;
        } else if (tail) {
            node->prev = tail;
            tail->next = node;
        }
        tail = node;
        ++count;
    }
};

struct UnkTU2702AI_l0fee24 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x144b8 - 0x38];
    UnkLengthMap_l0fee24 walk_lengths;
};

static inline const CStrId& SingletonStrId_l0fee24(u64 offset) {
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) +
                                            offset);
}

f32 fn_71010fee24(UnkTU2702AI_l0fee24* self, const CStrId& anim, u32 dir) {
    CStrId walk = SingletonStrId_l0fee24(0x24f0);
    f32 length = -1.0f;
    if (anim == walk) {
        UnkLengthMap_l0fee24& lengths = self->walk_lengths;
        if (lengths.Contains(dir)) {
            length = lengths.At(dir);
        } else if (UnkStruct_7101176ea8* actions = static_cast<UnkStruct_7101176ea8*>(self->owner->field_0x220)) {
            UnkTimeline* timeline =
                static_cast<UnkTimeline*>(fn_7101176e0c(actions, walk.data->Key(), fn_71002a6bdc(dir)));
            if (timeline) {
                UnkStruct_71007142e8 motion;
                fn_71007142e8(&motion);
                UnkMotionTracks* tracks = timeline->tracks;
                UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
                fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
                length = fabsf(motion.m[11]);
                lengths.Insert(dir, length);
            }
        }
    }
    return length;
}
