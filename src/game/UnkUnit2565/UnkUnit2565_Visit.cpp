// Unit 2565 (tools/tu_map.py; no reflected class): wraps a callback argument in a small visitor object (vtable
// 0x7101aa9dd8) on the stack and hands it to fn_7100e791dc. Names and layouts are placeholders.
#include "types.h"

extern char g_unk_7101aa9dd8[];  // the visitor's vtable

struct UnkUnit2565Visitor {
    char* vtable;
    void* arg;
};

void fn_7100e791dc(void* self, UnkUnit2565Visitor* visitor, void* context, bool flag);

void fn_7100e791a4(void* self, void* arg, void* context, bool flag) {
    UnkUnit2565Visitor visitor;
    visitor.arg = arg;
    asm("" ::: "memory");  // the base part is stored before the vtable, as by a constructor
    visitor.vtable = g_unk_7101aa9dd8 + 0x10;
    fn_7100e791dc(self, &visitor, context, flag);
}
