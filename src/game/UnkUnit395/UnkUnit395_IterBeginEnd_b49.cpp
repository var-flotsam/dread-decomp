// Unit 395 of tools/tu_map.py (0x71002c717c-0x71002ceb78): the begin and end functions of a
// reflected collection of 0x48-byte entries (an array, a string id and plain data). The
// iterator's constructor and assignment re-point its cached value through fn_71002cafac, kept out
// of line (as src/base/UnkUnit25/UnkIdPathIterBeginEnd_b49.cpp). Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

// An array freed by its destructor: the count is cleared before the free, the capacity after.
struct UnkUnit395Array_b49a1 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkUnit395Array_b49a1() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkUnit395Entry_b49a1 {
    UnkUnit395Array_b49a1 field_0x0;
    base::UnkStrIdMember field_0x28;
    u64 field_0x30[3];
};
static_assert(sizeof(UnkUnit395Entry_b49a1) == 0x48, "UnkUnit395Entry_b49a1 layout");

struct UnkUnit395Entries_b49a1 {
    UnkUnit395Entry_b49a1* data;
    u32 count;
};

struct UnkUnit395Iter_b49a1;
void fn_71002cafac(UnkUnit395Iter_b49a1* it);  // re-points `current` at (index, element)

struct UnkUnit395Iter_b49a1 {
    u32 index;
    UnkUnit395Entry_b49a1* element;
    base::UnkIterValue<UnkUnit395Entry_b49a1, u32> current;

    UnkUnit395Iter_b49a1(u32 index, UnkUnit395Entry_b49a1* element) : index(index), element(element) {
        fn_71002cafac(this);
    }
    UnkUnit395Iter_b49a1& operator=(const UnkUnit395Iter_b49a1& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            fn_71002cafac(this);
        }
        return *this;
    }
};

void fn_71002ce34c(UnkUnit395Entries_b49a1* self, UnkUnit395Iter_b49a1* out) {
    *out = UnkUnit395Iter_b49a1(0, self->data);
}

void fn_71002ce430(UnkUnit395Entries_b49a1* self, UnkUnit395Iter_b49a1* out) {
    *out = UnkUnit395Iter_b49a1(self->count, self->data + self->count);
}
