// Unit 269 of tools/tu_map.py (0x71001ea42c-0x71001ea85c, no reflected class): the destructor of an
// object with an array of 256 entries of 0xa0 bytes at 0x858, each with a member destroyed by
// fn_7100216934 (in reverse order), and its deleting form. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "game/UnkTU269.h"
#include "types.h"

struct UnkEntry269 {
    char storage[0xa0];

    __attribute__((always_inline)) ~UnkEntry269() { fn_7100216934(storage); }
};

struct UnkObject269 {
    void* vtable;
    u8 pad_0x8[0x850];
    UnkEntry269 entries[256];  // 0x858

    __attribute__((always_inline)) ~UnkObject269() { vtable = g_unk_7101a1ee90 + 0x10; }
};

void fn_71001ea4d4(UnkObject269* self) { self->~UnkObject269(); }

void fn_71001ea518(UnkObject269* self) {
    self->~UnkObject269();
    fn_7100083c00(self);
}
