// sound::SGUISoundEventsSelector's unit (tools/tu_map.py unit 2625): stops the GUI's playing sounds.
// With `all`, every entry's sound is stopped, its second object released and the map emptied; otherwise
// only the entries whose sound is gone or done are dropped, their keys collected first in a
// function-local static array. Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

void fn_7100071e9c(void* object);

struct UnkSoundObject_fca550 {
    u8 pad_0x0[0x119];
    bool done;  // 0x119
};

// The block a handle points at, as this unit reads it.
struct UnkSoundBlock_fca550 {
    void* object;
    s64 serial;
};

// What a handle names while its block still has the handle's serial; null otherwise.
static inline void* HandleObject_fca550(const UnkSoundHandle& handle) {
    const UnkSoundBlock_fca550* block = reinterpret_cast<const UnkSoundBlock_fca550*>(handle.ref);
    if (block && handle.serial != -1 && block->serial == handle.serial)
        return block->object;
    return nullptr;
}

struct UnkSoundMapEntry_fca550 {
    UnkSoundHandle sound;
    UnkSoundHandle field_0x10;
};

// A map of entries with a parallel key array.
struct UnkSoundMap_fca550 {
    void** vtable;
    void** vtable_8;
    UnkSoundMapEntry_fca550* entries;  // 0x10
    u64* keys;                         // 0x18
    u32 count;                         // 0x20

    UnkSoundMapEntry_fca550* begin() const { return entries; }
    UnkSoundMapEntry_fca550* end() const { return &entries[count]; }

    // The key of the entry at `it`.
    u64& KeyOf(const UnkSoundMapEntry_fca550* it) {
        u32 index = it - entries;
        return keys[index];
    }

    // The entry with `key`, or the end.
    UnkSoundMapEntry_fca550* Find(u64 key) {
        u32 i = 0;
        for (; i < count; ++i)
            if (keys[i] == key)
                break;
        return &entries[i];
    }

    // Removes the entry at `it`, moving the last one into its place.
    void Erase(UnkSoundMapEntry_fca550* it) {
        u32 index = it - entries;
        if (index != count - 1) {
            entries[index] = entries[count - 1];
            keys[index] = keys[count - 1];
        }
        count = count - 1;
    }

    void Remove(u64 key) {
        UnkSoundMapEntry_fca550* it = Find(key);
        if (it != &entries[count])
            Erase(it);
    }
};

// The keys to remove: a function-local static array (allocation flags 0xf), its guard and destructor.
__attribute__((visibility("hidden"))) extern u64 g_unk_71729f0310;
__attribute__((visibility("hidden"))) extern base::global::CRntVector<u64> g_unk_71729f0368;
void fn_7100fca440(void* array);
// The array's element accessor, out of line (CRntVector::GrowAt).
u64* fn_71000d3b38(base::global::CRntVector<u64>* array, u32 index);

void fn_7100fca550(UnkSoundMap_fca550* self, bool all) {
    if (all) {
        for (UnkSoundMapEntry_fca550& entry : *self) {
            UnkSoundHandle second = entry.field_0x10;
            if (void* sound = HandleObject_fca550(entry.sound))
                fn_7100105034(sound);
            if (void* object = HandleObject_fca550(second))
                fn_7100071e9c(object);
        }
        self->count = 0;
        return;
    }
    base::global::CRntVector<u64>& removed = g_unk_71729f0368;
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71729f0310), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71729f0310)) {
            new (&removed) base::global::CRntVector<u64>(0xf);
            fn_7100000250(fn_7100fca440, &removed, g_unk_7101a19000);
            __cxa_guard_release(&g_unk_71729f0310);
        }
    }
    removed.Clear();
    if (self->count == 0)
        return;
    UnkSoundMapEntry_fca550* end = self->end();
    for (UnkSoundMapEntry_fca550* it = self->begin(); it != end; ++it) {
        UnkSoundObject_fca550* sound = static_cast<UnkSoundObject_fca550*>(HandleObject_fca550(it->sound));
        if (!sound || sound->done)
            removed.PushBack(self->KeyOf(it));
    }
    for (u32 i = 0; i < removed.count; ++i)
        self->Remove(*fn_71000d3b38(&removed, i));
}
