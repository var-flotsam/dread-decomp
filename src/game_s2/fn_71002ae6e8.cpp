// Copy assignment of a container: copies its flag at +0x8, assigns the record at +0x10
// (fn_71002ad4f4), copies the array of 16-bit words at +0x11984 (its length at +0x11980) and
// re-applies the four entries of the block at +0x12a60 through the setters fn_71002ad688, fn_71002ad978
// and fn_71002adbb0 (the first two take the field at +0x10 of their entry, or null) and a plain copy.
#include "game/UnkNavMeshPath.h"
#include "types.h"

struct UnkBlock {
    UnkWeakHandle* field_0x0;
    UnkWeakHandle* field_0x8;
    void* field_0x10;
    void* field_0x18;
};

template <class T>
static inline T& At(const void* base, u64 offset) {
    return *reinterpret_cast<T*>(const_cast<char*>(reinterpret_cast<const char*>(base)) + offset);
}

void fn_71002ad4f4(void* self, const void* other);
void fn_71002ad978(UnkNavMeshPath* self, void* value);
void fn_71002adbb0(UnkNavMeshPath* self, void* value);

UnkNavMeshPath* fn_71002ae6e8(UnkNavMeshPath* self, const UnkNavMeshPath* other) {
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
        fn_71002ad688(self, block->field_0x0 ? block->field_0x0->target : nullptr);
        fn_71002ad978(self, block->field_0x8 ? block->field_0x8->target : nullptr);
        fn_71002adbb0(self, block->field_0x10);
        *self_block_tail = reinterpret_cast<u64>(block->field_0x18);
    }
    return self;
}
