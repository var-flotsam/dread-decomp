// The complete-object destructor of the class whose deleting destructor is fn_7100b8f348
// (UnkSharedPtrHolderDelete_b58.cpp): it stores the vtable g_unk_7101aa50e0, releases the
// std::shared_ptr at 0xc0, then stores its base's vtable g_unk_7101a20a08. Names and layouts are
// placeholders.
#include <memory>

#include "types.h"

extern char g_unk_7101aa50e0[];
extern char g_unk_7101a20a08[];

struct UnkSharedPtrHolder_A5_b69 {
    char* vtable;
    u8 pad_0x8[0xb8];
    std::shared_ptr<void> field_0xc0;
};

void fn_7100b8f2d4(UnkSharedPtrHolder_A5_b69* self) {
    self->vtable = g_unk_7101aa50e0 + 0x10;
    self->field_0xc0.~shared_ptr();
    self->vtable = g_unk_7101a20a08 + 0x10;
}
