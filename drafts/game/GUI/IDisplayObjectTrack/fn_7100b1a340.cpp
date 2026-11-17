// GUI::IDisplayObjectTrack unit (tools/tu_map.py, unit 1671): writes a string value under a key into a
// track output: finds the key's slot in the file (or appends one at the end, typed 's'), seeks there and
// writes the text in a 256-byte field, zero-padded. Names and layouts are placeholders.
#include <algorithm>
#include <cstring>

#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/UnkHashStream.h"
#include "../src/game_s7/UnkIter.h"
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
    using It = UnkIter<UnkTrackSlot_lb18a14>;
    It end{slots.end()};
    return std::find_if(It{slots.begin()}, end, [&key](const UnkTrackSlot_lb18a14& slot) { return slot.key == key; }).p;
}

void fn_7100b1a340(UnkTrackOut_A5_b73* self, const u64* key, const char* text) {
    UnkTrackSlot_lb18a14* slot = FindSlot(self->slots, *key);
    u64 offset = fn_710007c1a4(&self->file);
    if (slot != self->slots.end())
        offset = slot->offset;
    else
        self->slots.EmplaceBack(UnkTrackSlot_lb18a14(offset, *key, 's'));
    fn_710007c00c(&self->file, offset, 0);
    u64 size = strlen(text) + 1;
    u64 written = size < 0x100 ? size : 0x100;
    fn_710007c3f0(&self->file, text, written);
    if (size < 0x100)
        fn_710007c4b8(&self->file, 0, 0x100 - written);
}
