// Copy constructor of an unidentified class: a byte buffer (copied into an array of the other's
// capacity), a string id, a sub-object holding a refcounted handle, a word and an array of
// 16-byte values, and 11 bytes of plain fields.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkErasedVector.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A byte array with the engine array's layout, copied with memcpy.
struct UnkByteArray_71007abba4 {
    u8* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkByteArray_71007abba4(const UnkByteArray_71007abba4& o)
        : data(nullptr), count(0), capacity(0), step(o.step), field_0x14(o.field_0x14), flags(o.flags),
          allocator(o.allocator) {
        u32 n = o.capacity;
        if (n) {
            u8* p = static_cast<u8*>(allocator->vf_0x40(n, 8, flags));
            if (data) {
                __builtin_memcpy(p, data, count);
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        __builtin_memcpy(data, o.data, o.count);
        count = o.count;
    }
};

class UnkSubBase_71007abba4 {
public:
    virtual void vf_0x0();

    UnkSubBase_71007abba4() {
        values.data = nullptr;
        values.count = 0;
        values.capacity = 0;
        values.step = 10;
        values.field_0x14 = true;
        values.flags = 0xf;
        values.allocator = fn_7100083990();
        field_0x68 = 0;
    }

    UnkErasedVector values;  // built field by field (base::UnkVector's constructor doesn't match)
    u32 field_0x68;
};

class UnkSub_71007abba4 : public UnkSubBase_71007abba4 {
public:
    void vf_0x0() override;

    UnkSub_71007abba4(const UnkSub_71007abba4& o) {
        handle.ptr = nullptr;
        handle.Set(o.handle.ptr);
        field_0x68 = o.field_0x68;
        fn_710023b29c(&values, &o.values);
    }

    base::UnkRefHandle handle;
};

class UnkStruct_71007abba4 {
public:
    virtual void vf_0x0();

    // The implicit copy (members copied in order, the plain tail with one memcpy).
    __attribute__((always_inline)) UnkStruct_71007abba4(const UnkStruct_71007abba4&) = default;

    UnkByteArray_71007abba4 buffer;
    CStrId id;
    UnkSub_71007abba4 sub;
    u32 field_0x78;
    u32 field_0x7c;
    u8 field_0x80;
    u8 field_0x81;
    u8 field_0x82;
};

void fn_71007abba4(UnkStruct_71007abba4* self, const UnkStruct_71007abba4* other) {
    new (self) UnkStruct_71007abba4(*other);
}
