// Two identical copy assignments of a reflected 0x38-byte value type in unit 232 (the class
// described by fn_71001aad0c): three words at +0x0 copied as a block, two counted resource
// references at +0x18 and +0x20, and two words at +0x28. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "types.h"

namespace {
// A counted reference whose assignment reads the source pointer first.
struct UnkUnit232Ref {
    UnkCountedResource* ptr;

    __attribute__((always_inline)) UnkUnit232Ref& operator=(const UnkUnit232Ref& other) {
        UnkCountedResource* p = other.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
};

struct UnkUnit232Value {
    struct {
        u64 words[3];
    } field_0x0;
    UnkUnit232Ref field_0x18;
    UnkUnit232Ref field_0x20;
    struct {
        u64 words[2];
    } field_0x28;

    __attribute__((always_inline)) UnkUnit232Value& operator=(const UnkUnit232Value& other) {
        if (this != &other) {
            field_0x0 = other.field_0x0;
            field_0x18 = other.field_0x18;
            field_0x20 = other.field_0x20;
            field_0x28 = other.field_0x28;
        }
        return *this;
    }
};
}  // namespace

void* fn_71001ab48c(void* dst, const void* src) {
    return &(*static_cast<UnkUnit232Value*>(dst) = *static_cast<const UnkUnit232Value*>(src));
}
void* fn_71001ab558(void* dst, const void* src) {
    return &(*static_cast<UnkUnit232Value*>(dst) = *static_cast<const UnkUnit232Value*>(src));
}
