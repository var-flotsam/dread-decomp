// Copy of fn_7100eae08c (src/game_aca57c/fn_7100d3d424.cpp) for another array type, naming its own callees and data
// (the comments are adapted from the original's): fn_7100138ee0. Move-assignment of an array whose elements follow
// another pointer (destroying the elements through fn_710007ecd4, keeping the storage).
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

struct UnkRecord30 {
    u64 unk_0x0[0x30 / 8];

    ~UnkRecord30();
};
inline UnkRecord30::~UnkRecord30() { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this)); }

template <class T>
struct UnkFirstPtrArray {
    void* field_0x0;
    T* data;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 field_0x1c;
    base::UnkAllocator* allocator;
};

template <class T, class Index>
static inline UnkFirstPtrArray<T>* MoveAssign(UnkFirstPtrArray<T>* self, UnkFirstPtrArray<T>* other) {
    if (self != other) {
        if (self->data)
            for (Index i = 0; i < self->count; ++i)
                self->data[i].~T();
        self->count = 0;
        self->allocator = other->allocator;
        self->data = other->data;
        self->field_0x0 = other->field_0x0;
        self->capacity = other->capacity;
        self->count = other->count;
        self->field_0x1c = other->field_0x1c;
        self->field_0x18 = other->field_0x18;
        other->count = 0;
        other->field_0x0 = nullptr;
        other->data = nullptr;
    }
    return self;
}

UnkFirstPtrArray<UnkRecord30>* fn_7100138ee0(UnkFirstPtrArray<UnkRecord30>* self, UnkFirstPtrArray<UnkRecord30>* other) {
    return MoveAssign<UnkRecord30, u32>(self, other);
}
