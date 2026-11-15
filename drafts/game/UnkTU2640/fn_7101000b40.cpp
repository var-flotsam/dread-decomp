// Unit 2640: the move helper of the iterator value over a reflected array of records of a word, two arrays and a
// 29-byte block: both arrays give up their buffers. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkTU2640Array {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkTU2640Array(UnkTU2640Array&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

// Copied as one block of 29 bytes (packed: the copy skips the tail padding).
struct __attribute__((packed)) UnkTU2640Block {
    u8 field_0x0[0x1d];
};

struct UnkTU2640Record {
    u32 field_0x0;
    UnkTU2640Array first;  // 0x8
    UnkTU2640Block block;  // 0x30
    UnkTU2640Array second;  // 0x50

    UnkTU2640Record(UnkTU2640Record&& other) = default;
};

// The iterator value's move constructor (base/UnkCollectionIter.h) written out in the function: the record is too
// large for it to be inlined otherwise.
void fn_7101000b40(void* dst, const void* src) {
    using Value = base::UnkIterValue<UnkTU2640Record>;
    Value* out = static_cast<Value*>(dst);
    Value* in = static_cast<Value*>(const_cast<void*>(src));
    out->state = 0;
    if (in->state == 1) {
        out->state = 1;
        new (&out->value) Value::Value(static_cast<Value::Value&&>(in->value));
    } else if (in->state == 2) {
        out->state = 1;
        Value::Ref r = in->ref;
        new (&out->value) Value::Value{static_cast<u32&&>(*const_cast<u32*>(r.index)), static_cast<UnkTU2640Record&&>(*r.element)};
    }
}
