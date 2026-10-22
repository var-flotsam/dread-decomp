// Copy assignment of a container: copies its flag at +0x8, assigns the record at +0x10
// (fn_71002ad4f4), copies the array of 16-bit words at +0x11984 (its length at +0x11980) and
// re-applies the four entries of the block at +0x12a60 through the setters fn_71002ad688, fn_71002ad978
// and fn_71002adbb0 (the first two take the field at +0x10 of their entry, or null) and a plain copy.
#include "types.h"

struct UnkEntry {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkBlock {
    UnkEntry* field_0x0;
    UnkEntry* field_0x8;
    void* field_0x10;
    void* field_0x18;
};

struct UnkContainer {
    u8 pad_0x0[8];
    u8 field_0x8;
};

template <class T>
static inline T& At(const void* base, u64 offset) {
    return *reinterpret_cast<T*>(const_cast<char*>(reinterpret_cast<const char*>(base)) + offset);
}

void fn_71002ad4f4(void* self, const void* other);
void fn_71002ad688(UnkContainer* self, void* value);
void fn_71002ad978(UnkContainer* self, void* value);
void fn_71002adbb0(UnkContainer* self, void* value);

UnkContainer* fn_71002ae6e8(UnkContainer* self, const UnkContainer* other) {
    if (self != other) {
        u64* self_block_tail = &At<u64>(self, 0x12a78);
        u32* self_count = &At<u32>(self, 0x11980);
        const u32* other_count = &At<u32>(other, 0x11980);
        self->field_0x8 = other->field_0x8;
        const UnkBlock* block = &At<UnkBlock>(other, 0x12a60);
        const void* other_record = &At<u8>(other, 0x10);
        fn_71002ad4f4(&At<u8>(self, 0x10), other_record);
        __builtin_memcpy(&At<u8>(self, 0x11984), &At<u8>(other, 0x11984), (u64)*other_count * 2);
        *self_count = *other_count;
        fn_71002ad688(self, block->field_0x0 ? block->field_0x0->field_0x10 : nullptr);
        fn_71002ad978(self, block->field_0x8 ? block->field_0x8->field_0x10 : nullptr);
        fn_71002adbb0(self, block->field_0x10);
        *self_block_tail = reinterpret_cast<u64>(block->field_0x18);
    }
    return self;
}
