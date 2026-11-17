// CSluggerAIComponent unit: fn_71006c6274 gives the motion of the animation action `key` names over its whole
// timeline, from the unit's cache (g_unk_7172708078) or, the first time, sampled (fn_7100b8b3bc) from the
// action the owner's (+0x30) animation (+0x220) finds and cached. False when there is no such action.
// Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "types.h"

struct UnkFlags_l6c6274 {
    u32 bits;
    UnkFlags_l6c6274() : bits(0) {}
    UnkFlags_l6c6274(u32 b) : bits(b) {}
    UnkFlags_l6c6274(const UnkFlags_l6c6274& o) : bits(o.bits) {}
};

inline const UnkFlags_l6c6274& UnkFlagsNone_l6c6274() {
    static UnkFlags_l6c6274 s_none asm("g_unk_71726a4340");
    return s_none;
}

inline const UnkFlags_l6c6274& UnkFlagsAll_l6c6274() {
    static UnkFlags_l6c6274 s_all asm("g_unk_71726a4330")(~UnkFlagsNone_l6c6274().bits & 7);
    return s_all;
}

struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

// The sampled part of a motion; assigning it to itself does nothing.
struct UnkSluggerMotionPart_l6c6274 {
    u32 words[23];

    UnkSluggerMotionPart_l6c6274& operator=(const UnkSluggerMotionPart_l6c6274& other) {
        if (this != &other) {
            for (u32 i = 0; i < 23; i++)
                words[i] = other.words[i];
        }
        return *this;
    }
};

// What fn_71007142e8 builds and fn_7100b8b3bc fills (0x70 bytes).
struct UnkSluggerMotion_l6c6274 {
    UnkSluggerMotionPart_l6c6274 part;
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
};

struct UnkSluggerMotionNode_l6c6274 {
    u64 key;
    UnkSluggerMotion_l6c6274 motion;  // 0x8
    UnkSluggerMotionNode_l6c6274* next;  // 0x78
};

struct UnkSluggerInsertResult_l6c6274 {
    UnkSluggerMotionNode_l6c6274* node;
    void* bucket;
    bool inserted;
};

struct UnkSluggerMotionCache_l6c6274 {
    UnkSluggerMotionNode_l6c6274** buckets;
    u32 count;
    u32 capacity;

    u32 IndexOf(const u64& key) const { return ((u32)(key >> 32) ^ (u32)key) % capacity; }

    UnkSluggerMotionNode_l6c6274* Find(const u64& key) const {
        if (count == 0)
            return nullptr;
        u32 index = IndexOf(key);
        if (index >= count)
            return nullptr;
        for (UnkSluggerMotionNode_l6c6274* node = buckets[index]; node; node = node->next) {
            if (node->key == key)
                return node;
        }
        return nullptr;
    }
};

__attribute__((visibility("hidden"))) extern UnkSluggerMotionCache_l6c6274 g_unk_7172708078;

UnkSluggerMotionNode_l6c6274** fn_71006c6c98(UnkSluggerMotionCache_l6c6274* self, u32 index);
UnkSluggerInsertResult_l6c6274 fn_71006c6e74(UnkSluggerMotionCache_l6c6274* self, const u64* key,
                                             const UnkSluggerMotion_l6c6274* motion);

struct UnkSluggerOwner_l6c6274 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkSluggerAI_l6c6274 {
    u8 pad_0x0[0x30];
    UnkSluggerOwner_l6c6274* owner;
};

bool fn_71006c6274(UnkSluggerAI_l6c6274* self, const u64* key, UnkSluggerMotion_l6c6274* out) {
    if (g_unk_7172708078.Find(*key)) {
        UnkSluggerMotionNode_l6c6274* node = *fn_71006c6c98(&g_unk_7172708078, g_unk_7172708078.IndexOf(*key));
        while (node && node->key != *key)
            node = node->next;
        *out = node->motion;
        return true;
    }
    void* animation = self->owner->field_0x220;
    if (!animation)
        return false;
    auto* timeline = static_cast<UnkTimeline*>(
        fn_7101176d34(static_cast<UnkStruct_7101176ea8*>(animation), reinterpret_cast<void*>(*key)));
    if (!timeline)
        return false;
    UnkSluggerMotion_l6c6274 motion;
    fn_71007142e8(reinterpret_cast<UnkStruct_71007142e8*>(&motion));
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_l6c6274 flags = UnkFlagsAll_l6c6274();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    *out = motion;
    fn_71006c6e74(&g_unk_7172708078, key, &motion);
    return true;
}
