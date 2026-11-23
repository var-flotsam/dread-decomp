// GUI::IDisplayObjectTrack unit (tools/tu_map.py, unit 1671): writes a bool value under a key into a
// track output: finds the key's slot in the file (or appends one at the end, typed 'b'), seeks there and
// writes the byte. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/UnkHashStream.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntVector;

// Where a key's value sits in the file, and its type character.
struct UnkTrackSlot_lb18a14 {
    u64 offset;
    u64 key;
    u32 type;

    UnkTrackSlot_lb18a14(u64 offset, u64 key, u32 type) : offset(offset), key(key), type(type) {}
    ~UnkTrackSlot_lb18a14() {}
};

struct UnkTrackOut_A5_b73 {
    CRntVector<UnkTrackSlot_lb18a14> slots;
    CRntFile file;  // 0x28
};
static_assert(__builtin_offsetof(UnkTrackOut_A5_b73, file) == 0x28, "layout");

// The slot that holds `key`, or the end of the slots.
static inline UnkTrackSlot_lb18a14* FindSlot(CRntVector<UnkTrackSlot_lb18a14>& slots, u64 key) {
    UnkTrackSlot_lb18a14* slot = slots.begin();
    UnkTrackSlot_lb18a14* end = slots.end();
    while (slot != end && slot->key != key)
        ++slot;
    return slot;
}

void fn_7100b18a14(UnkTrackOut_A5_b73* self, const u64* key, bool value) {
    UnkTrackSlot_lb18a14* slot = FindSlot(self->slots, *key);
    u64 offset = fn_710007c1a4(&self->file);
    if (slot != self->slots.end())
        offset = slot->offset;
    else
        self->slots.EmplaceBack(UnkTrackSlot_lb18a14(offset, *key, 'b'));
    CRntFile_Seek(&self->file, offset, 0);
    CRntFile_WriteElements(&self->file, &value, 1, 1);
}
