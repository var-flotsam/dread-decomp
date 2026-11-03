// Unit 222 (0x710019b5ec-0x710019bd2c): a one-shot shutdown over two arrays (0x98-byte entries,
// then the owner's child pointers). Names and layouts are placeholders.
#include "types.h"

struct UnkEntry_0x98 {
    u8 unk_0x0[0x98];
};

struct UnkChild_710019b304;

struct UnkOwner_710019b9c8 {
    u8 unk_0x0[0x28];
    u32 child_count;  // 0x28
    u8 unk_0x2c[0x14];
    UnkChild_710019b304** children;  // 0x40
};

struct Unk_710019b9c8 {
    UnkOwner_710019b9c8* owner;  // 0x0
    UnkEntry_0x98* entries;      // 0x8
    u8 unk_0x10[0x8];
    u32 entry_count;  // 0x18
    u8 unk_0x1c[0x14];
    bool done;  // 0x30
};

void fn_7100195114(UnkEntry_0x98* entry);
void fn_710019b304(UnkChild_710019b304* child);

void fn_710019b9c8(Unk_710019b9c8* self) {
    if (self->done)
        return;
    for (UnkEntry_0x98 *it = self->entries, *end = it + self->entry_count; it != end; ++it)
        fn_7100195114(it);
    self->done = true;
    UnkOwner_710019b9c8* owner = self->owner;
    for (u64 i = 0; i < owner->child_count; ++i)
        fn_710019b304(owner->children[i]);
}
