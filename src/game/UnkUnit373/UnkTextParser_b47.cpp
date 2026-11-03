// Unit 373 (tools/tu_map.py): a parser over [begin, end) with three cursor stacks of 512 entries each
// (+0x20, +0x1024, +0x2028): the constructor, and the entry point that resets it and steps it
// (fn_71002b49f0) until it stops, succeeding when the bottom cursor of the first stack reached the end.
// Names are placeholders.
#include "types.h"

struct UnkCursor_b47 {
    const char* pos;
};

// The entries are only 4-aligned (the original reads them with ldur).
#pragma pack(push, 4)
struct UnkCursorStack_b47 {
    u32 count;
    UnkCursor_b47* items[512];
};
#pragma pack(pop)

struct UnkTextParser_b47 {
    void** vtable;
    const char* begin;  // 0x8
    const char* end;    // 0x10
    u32 field_0x18;
    u32 field_0x1c;
    UnkCursorStack_b47 stack0;  // 0x20
    UnkCursorStack_b47 stack1;  // 0x1024
    UnkCursorStack_b47 stack2;  // 0x2028
};
static_assert(__builtin_offsetof(UnkTextParser_b47, stack1) == 0x1024, "");
static_assert(__builtin_offsetof(UnkTextParser_b47, stack2) == 0x2028, "");

bool fn_71002b49f0(UnkTextParser_b47* self);

bool fn_71002b497c(UnkTextParser_b47* self, const char* begin, const char* end, u32 a, u32 b) {
    self->stack1.count = 0;
    self->stack2.count = 0;
    self->stack0.count = 0;
    self->field_0x1c = b;
    self->field_0x18 = a;
    self->begin = begin;
    self->end = end;
    if (begin != end) {
        while (fn_71002b49f0(self)) {
        }
        if (self->stack0.count == 0 || self->stack0.items[0]->pos != end)
            return false;
    }
    return true;
}

extern void* g_unk_7101a21210[];  // the parser's vtable

void fn_71002b4958(UnkTextParser_b47* self) {
    self->vtable = &g_unk_7101a21210[2];
    self->stack2.count = 0;
    self->stack1.count = 0;
    self->stack0.count = 0;
}
