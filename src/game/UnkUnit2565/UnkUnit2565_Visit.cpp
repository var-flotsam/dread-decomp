// Unit 2565 (tools/tu_map.py; no reflected class): wraps a callback argument in a small visitor object (vtable
// 0x7101aa9dd8) on the stack and hands it to fn_7100e791dc, whose item count it returns. Names and layouts are
// placeholders.
#include "game/UnkTU2565.h"
#include "types.h"

extern char g_unk_7101aa9dd8[];  // the visitor's vtable

struct UnkUnit2565Visitor {
    char* vtable;
    void* arg;
};

u32 fn_7100e791dc(void* self, UnkUnit2565Visitor* visitor, u32 kind, bool flag, void* extra);

// `extra` (x4) reaches fn_7100e791dc untouched in the original; CMinimapManager passes a fifth
// argument (tools/forwarded_args.py).
u32 fn_7100e791a4(void* self, void* arg, u32 kind, bool flag, void* extra) {
    UnkUnit2565Visitor visitor;
    visitor.arg = arg;
    asm("" ::: "memory");  // the base part is stored before the vtable, as by a constructor
    visitor.vtable = g_unk_7101aa9dd8 + 0x10;
    return fn_7100e791dc(self, &visitor, kind, flag, extra);
}
