// The deleting destructor of a class whose vtable is g_unk_7101aa50e0 and which owns a std::shared_ptr
// at 0xc0: it stores the vtable, releases the shared pointer and frees the object (fn_7100083c00).
// The same body is emitted in several units (CAIAction's, CActionInstance's and their neighbours);
// each copy is instantiated here with its own tag. Names and layouts are placeholders.
#include <memory>

#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101aa50e0[];

template <int Copy>
struct UnkSharedPtrHolder_b58 {
    char* vtable;
    u8 pad_0x8[0xb8];
    std::shared_ptr<void> field_0xc0;
};

template <int Copy>
static inline void DeleteHolder_b58(UnkSharedPtrHolder_b58<Copy>* self) {
    self->vtable = g_unk_7101aa50e0 + 0x10;
    self->field_0xc0.~shared_ptr();
    fn_7100083c00(self);
}

void fn_7100b8f348(UnkSharedPtrHolder_b58<0>* self) { DeleteHolder_b58(self); }
void fn_7100b8fcec(UnkSharedPtrHolder_b58<1>* self) { DeleteHolder_b58(self); }
void fn_7100b90a58(UnkSharedPtrHolder_b58<2>* self) { DeleteHolder_b58(self); }
void fn_7100c97a1c(UnkSharedPtrHolder_b58<3>* self) { DeleteHolder_b58(self); }
void fn_7100c986f4(UnkSharedPtrHolder_b58<4>* self) { DeleteHolder_b58(self); }
void fn_7100c99f60(UnkSharedPtrHolder_b58<5>* self) { DeleteHolder_b58(self); }
void fn_7100c9aec0(UnkSharedPtrHolder_b58<6>* self) { DeleteHolder_b58(self); }
void fn_7100c9b92c(UnkSharedPtrHolder_b58<7>* self) { DeleteHolder_b58(self); }
