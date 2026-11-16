// sound::SGUISoundEventsSelector's unit (tools/tu_map.py unit 2625): drops from the GUI's playing-sound
// map (entries of two handles keyed by a 64-bit value) every entry whose sound is the empty handle's,
// collecting their keys first in a function-local static array. Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU101.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

// The sound block a handle points at, as this unit reads it.
struct UnkSoundBlock_fca0f0 {
    void* sound;
    s64 serial;
};

// The sound a handle names while its block still has the handle's serial; null otherwise.
static inline void* HandleSound_fca0f0(const UnkSoundHandle& handle) {
    const UnkSoundBlock_fca0f0* block = reinterpret_cast<const UnkSoundBlock_fca0f0*>(handle.ref);
    if (block && handle.serial != -1 && block->serial == handle.serial)
        return block->sound;
    return nullptr;
}

struct UnkSoundMapEntry_fca0f0 {
    UnkSoundHandle handle;
    UnkSoundHandle field_0x10;
};

// A map of entries with a parallel key array.
struct UnkSoundMap_fca0f0 {
    void** vtable;
    void** vtable_8;
    UnkSoundMapEntry_fca0f0* entries;  // 0x10
    u64* keys;                         // 0x18
    u32 count;                         // 0x20

    UnkSoundMapEntry_fca0f0* begin() const { return entries; }
    UnkSoundMapEntry_fca0f0* end() const { return &entries[count]; }

    // The key of the entry at `it`.
    u64& KeyOf(const UnkSoundMapEntry_fca0f0* it) {
        u32 index = it - entries;
        return keys[index];
    }

    // The entry with `key`, or the end.
    UnkSoundMapEntry_fca0f0* Find(u64 key) {
        u32 i = 0;
        for (; i < count; ++i)
            if (keys[i] == key)
                break;
        return &entries[i];
    }

    // Removes the entry at `it`, moving the last one into its place.
    void Erase(UnkSoundMapEntry_fca0f0* it) {
        u32 index = it - entries;
        if (index != count - 1) {
            entries[index] = entries[count - 1];
            keys[index] = keys[count - 1];
        }
        count = count - 1;
    }

    void Remove(u64 key) {
        UnkSoundMapEntry_fca0f0* it = Find(key);
        if (it != &entries[count])
            Erase(it);
    }
};

// The keys to remove: a function-local static array (allocation flags 0xf), its guard and destructor.
__attribute__((visibility("hidden"))) extern u64 g_unk_71729f0308;
__attribute__((visibility("hidden"))) extern base::global::CRntVector<u64> g_unk_71729f0340;
void fn_7100fca440(void* array);

void fn_7100fca0f0(UnkStateSink* sink) {
    UnkSoundMap_fca0f0* self = reinterpret_cast<UnkSoundMap_fca0f0*>(sink);
    base::global::CRntVector<u64>& removed = g_unk_71729f0340;
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71729f0308), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71729f0308)) {
            new (&removed) base::global::CRntVector<u64>(0xf);
            fn_7100000250(fn_7100fca440, &removed, g_unk_7101a19000);
            __cxa_guard_release(&g_unk_71729f0308);
        }
    }
    removed.Clear();
    if (self->count == 0)
        return;
    UnkSoundMapEntry_fca0f0* end = self->end();
    for (UnkSoundMapEntry_fca0f0* it = self->begin(); it != end; ++it) {
        if (HandleSound_fca0f0(it->handle) == HandleSound_fca0f0(g_unk_7108b6c460))
            removed.PushBack(self->KeyOf(it));
    }
    for (u64* key = removed.data; key != removed.data + removed.count; ++key)
        self->Remove(*key);
}
