// Creates an object through the global allocator from a type description: the description is
// chosen by whether the source's first word equals the global's, checked with the cast helper
// (fn_710009d50c), and, when it has a constructor hook, its storage is allocated (size at +0x18,
// alignment 8) and constructed through the hook (at +0x28). Returns the object or null.
#include "types.h"

struct UnkAllocator {
    void** vtable;
    void* alloc(u64 size, u32 align, u32 flags) {
        return reinterpret_cast<void* (*)(UnkAllocator*, u64, u32, u32)>(vtable[0x40 / 8])(this, size, align, flags);
    }
};

struct UnkTypeDesc {
    u8 pad_0x0[0x18];
    u64 field_0x18;
    u8 pad_0x20[8];
    void* (*field_0x28)(void*);
};

struct UnkSource {
    u64 field_0x0;
};

extern u64 g_unk_71727b9db0;
UnkTypeDesc* fn_71002e970c();
UnkTypeDesc* fn_71002effc4();
void* fn_7100016894();
bool fn_710009d50c(UnkTypeDesc* desc, void* cls);
UnkAllocator* fn_7100083990();

// NON_MATCHING(minor): the two description calls are laid out the other way round (the branch on the
// comparison is b.ne where the original has b.eq); behaviour is identical
void* fn_71002f6bc8(void* self, UnkSource* source, u32 flags) {
    UnkTypeDesc* desc = source->field_0x0 == g_unk_71727b9db0 ? fn_71002effc4() : fn_71002e970c();
    if (fn_710009d50c(desc, fn_7100016894()) && desc->field_0x28) {
        void* obj = fn_7100083990()->alloc(desc->field_0x18, 8, flags);
        if (obj)
            desc->field_0x28(obj);
        return obj;
    }
    return nullptr;
}
