// CAudioComponent unit (tools/tu_map.py): the destructor. Every sound is stopped, the slots' two
// handles are let go, then the pair list, the links and the slots are freed, then the base.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAudioComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU101.h"
#include "game/UnkTU32.h"
#include "game/UnkTU50.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;

struct UnkAudioSound_ovb0015;
void CAudioComponent_ReleaseSoundSlotIds(UnkAudioSound_ovb0015* self);

// As src/game/CAudioComponent/CAudioComponent_Stop_b65a4.cpp has them.
struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};

// The object a handle names while it is still the one the handle was made for.
static inline UnkSoundObject_b50* Resolve_l718dec(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return handle->ref->object;
    return nullptr;
}

// CStrId::Reset on the file-path pool.
struct UnkAudioPathId_l718dec {
    base::global::UnkStringData* data;

    void Reset() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// A sound slot (0xd0 bytes): the playing sound and its rumble.
struct UnkAudioSlot_l718dec {
    u8 pad_0x0[0x78];
    UnkSoundHandle sound;   // 0x78
    UnkSoundHandle rumble;  // 0x88
    u8 pad_0x98[0x38];
};
static_assert(sizeof(UnkAudioSlot_l718dec) == 0xd0, "layout");

// A sound link (0xb0 bytes).
struct UnkAudioLink_l718dec {
    u8 pad_0x0[0x38];
    CStrId field_0x38;
    UnkAudioPathId_l718dec field_0x40;
    u8 pad_0x48[0x48];
    u8 field_0x90[0x18];  // the id pair (UnkIdPair)
    u8 pad_0xa8[0x8];
};
static_assert(sizeof(UnkAudioLink_l718dec) == 0xb0, "layout");

template <class T>
struct UnkAudioVector_l718dec {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkAudioPairList_l718dec {
    void* data;
    void** ptrs;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkAudioComponent_l718dec {
    void** vtable;
    u8 pad_0x8[0x38];
    void** vtable_0x40;
    UnkAudioVector_l718dec<UnkAudioSlot_l718dec> sounds;  // 0x48
    UnkAudioVector_l718dec<UnkAudioLink_l718dec> links;   // 0x70
    UnkAudioPairList_l718dec pairs;                       // 0x98
};
static_assert(__builtin_offsetof(UnkAudioComponent_l718dec, pairs) == 0x98, "layout");

void fn_7100718dec(UnkAudioComponent_l718dec* self) {
    self->vtable = g_unk_7101a6cca0 + 2;
    self->vtable_0x40 = g_unk_7101a6cca0 + 0x34;
    fn_7100719064(reinterpret_cast<UnkEntitySoundComponent*>(self), true, true, true, true);

    UnkAudioSlot_l718dec* end = self->sounds.data + self->sounds.count;
    for (UnkAudioSlot_l718dec* s = self->sounds.data; s != end; ++s) {
        if (UnkSoundObject_b50* sound = Resolve_l718dec(&s->sound))
            fn_7100106024(sound, nullptr);
        if (UnkSoundObject_b50* rumble = Resolve_l718dec(&s->rumble))
            fn_7100072030(rumble, nullptr);
    }

    UnkAudioPairList_l718dec& pairs = self->pairs;
    pairs.count = 0;
    if (pairs.ptrs)
        pairs.allocator->vf_0x48(pairs.ptrs);
    if (pairs.data)
        pairs.allocator->vf_0x48(pairs.data);

    UnkAudioVector_l718dec<UnkAudioLink_l718dec>& links = self->links;
    if (links.data) {
        // Each link's ids: the id pair, then the file-path id and the string id.
        for (u32 i = 0; i < links.count; ++i) {
            UnkAudioLink_l718dec& link = links.data[i];
            fn_7100026f70(reinterpret_cast<UnkIdPair*>(link.field_0x90));
            link.field_0x40.Reset();
            link.field_0x38.Reset();
        }
        links.count = 0;
        links.allocator->vf_0x48(links.data);
        links.capacity = 0;
    }

    UnkAudioVector_l718dec<UnkAudioSlot_l718dec>& sounds = self->sounds;
    if (sounds.data) {
        for (u32 i = 0; i < sounds.count; ++i)
            CAudioComponent_ReleaseSoundSlotIds(reinterpret_cast<UnkAudioSound_ovb0015*>(&sounds.data[i]));
        sounds.count = 0;
        sounds.allocator->vf_0x48(sounds.data);
        sounds.capacity = 0;
    }

    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
