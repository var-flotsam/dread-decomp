// animtree::CAnimTreeDef's unit (tools/tu_map.py unit 401): steps an iteration cursor (+0xa0) over
// the 16-byte entries of the array at +0x78 (count +0x80): hands out the current entry and advances,
// or gives null and false at the end. Names are placeholders.
#include "types.h"

struct UnkEntry401 {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkAnimTreeDef401 {
    u8 pad_0x0[0x78];
    UnkEntry401* entries;  // 0x78
    u32 count;             // 0x80
    u8 pad_0x84[0x1c];
    UnkEntry401* cursor;  // 0xa0
};

bool fn_71002d82f4(UnkAnimTreeDef401* self, UnkEntry401** out) {
    UnkEntry401* end = self->entries + self->count;
    if (self->cursor != end) {
        *out = self->cursor;
        ++self->cursor;
        return true;
    }
    *out = nullptr;
    return false;
}

// The same cursor walk over two arrays of pointers (+0x18 count +0x20, cursor +0x40; +0x48 count
// +0x50, cursor +0x70), handing out the pointed-to element.
struct UnkPtrWalk401 {
    void** items;
    u32 count;
};

static inline bool NextPtr401(const UnkPtrWalk401* walk, void*** cursor, void** out) {
    void** end = walk->items + walk->count;
    if (*cursor != end) {
        *out = **cursor;
        ++*cursor;
        return true;
    }
    *out = nullptr;
    return false;
}

struct UnkAnimTreeDefPtrs401 {
    u8 pad_0x0[0x18];
    UnkPtrWalk401 first;  // 0x18
    u8 pad_0x28[0x18];
    void** first_cursor;   // 0x40
    UnkPtrWalk401 second;  // 0x48
    u8 pad_0x58[0x18];
    void** second_cursor;  // 0x70
};

bool fn_71002d8274(UnkAnimTreeDefPtrs401* self, void** out) {
    return NextPtr401(&self->first, &self->first_cursor, out);
}

bool fn_71002d82b4(UnkAnimTreeDefPtrs401* self, void** out) {
    return NextPtr401(&self->second, &self->second_cursor, out);
}
