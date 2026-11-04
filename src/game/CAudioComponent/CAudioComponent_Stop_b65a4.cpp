// CAudioComponent's unit (tools/tu_map.py): two functions with the same body over members 0x40 apart
// (fn_71007195bc and fn_71007196d4): forget a sound object. Each 0xd0-byte entry of the first array
// whose handle (+0x78) still names `object` gets the empty handle and its flag at +0xc0 set, and the
// live count g_unk_717270f61c drops by one; each 0xb0-byte entry of the second array whose handle
// (+0x0) names it gets its flag at +0xa9 set. Names and layouts are placeholders.
#include "types.h"

// As src/game/CAudioComponent/CAudioComponent_b50.cpp defines them.
struct UnkSoundObject_b50;
struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};
extern const UnkSoundHandle g_unk_7108b6c460;  // the empty handle

extern s32 g_unk_717270f61c;  // the live sound count (reached through the GOT)

static inline UnkSoundObject_b50* Resolve_b65a4(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return handle->ref->object;
    return nullptr;
}

struct UnkSoundSlot_b65a4 {
    u8 pad_0x0[0x78];
    UnkSoundHandle handle;  // 0x78
    u8 pad_0x88[0x38];
    bool stopped;  // 0xc0
    u8 pad_0xc1[0xf];
};
static_assert(sizeof(UnkSoundSlot_b65a4) == 0xd0, "UnkSoundSlot_b65a4");

struct UnkSoundLink_b65a4 {
    UnkSoundHandle handle;  // 0x0
    u8 pad_0x10[0x99];
    bool stopped;  // 0xa9
    u8 pad_0xaa[0x6];
};
static_assert(sizeof(UnkSoundLink_b65a4) == 0xb0, "UnkSoundLink_b65a4");

template <class T>
struct UnkSoundArray_b65a4 {
    T* data;
    u32 count;
    T* begin() { return data; }
    T* end() { return data + count; }
};

struct UnkSoundLists_b65a4 {
    UnkSoundArray_b65a4<UnkSoundSlot_b65a4> slots;  // +0x0
    u8 pad_0x10[0x18];
    UnkSoundArray_b65a4<UnkSoundLink_b65a4> links;  // +0x28
};

template <u32 Offset>
struct UnkAudioStop_b65a4 {
    u8 pad_0x0[Offset];
    UnkSoundLists_b65a4 lists;
};

template <u32 Offset>
static inline void Forget_b65a4(UnkAudioStop_b65a4<Offset>* self, UnkSoundObject_b50* object) {
    if (!object)
        return;
    for (UnkSoundSlot_b65a4* slot = self->lists.slots.data; slot != self->lists.slots.data + self->lists.slots.count;
         ++slot) {
        if (Resolve_b65a4(&slot->handle) == object) {
            slot->handle = g_unk_7108b6c460;
            slot->stopped = true;
            --g_unk_717270f61c;
        }
    }
    for (UnkSoundLink_b65a4& link : self->lists.links) {
        if (Resolve_b65a4(&link.handle) == object)
            link.stopped = true;
    }
}

void fn_71007195bc(UnkAudioStop_b65a4<0x48>* self, UnkSoundObject_b50* object) {
    Forget_b65a4(self, object);
}

void fn_71007196d4(UnkAudioStop_b65a4<0x8>* self, UnkSoundObject_b50* object) {
    Forget_b65a4(self, object);
}
