// Unit 83: the "insert by key" function (slot 0x120 of the reflected collection type) of a keyed
// array of reflected functions (0xb8 bytes each, UnkReflFunction) and their names: the inlined
// append of fn_7100097550 (src/base/reflection/fn_7100097550.cpp), with the function moved in from
// the argument. The types are local views of that file's; names are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

extern char g_unk_7101a1b460[];  // UnkReflFunction's virtual table (its address point is +0x10)

namespace refl_function_insert_b62a1 {

template <class T>
struct UnkKeyedArray {
    T* data;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 field_0x1c;
    base::UnkAllocator* allocator;

    UnkKeyedArray(const UnkKeyedArray& other)
        : data(other.data), keys(other.keys), count(other.count), capacity(other.capacity), step(other.step),
          field_0x1c(other.field_0x1c), allocator(other.allocator) {}
};

template <class T>
struct UnkVectorView {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkVectorView(UnkVectorView&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

struct UnkReflFunctionData {
    struct Name {
        UnkStringData* data;
        Name(Name&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    };
    struct Pair {
        void* field_0x0;
        u32 field_0x8;
        template <class A>
        Pair(const Pair& other, A& moved_array) {
            moved_array.count = 0;
            void* a = other.field_0x0;
            u32 b = other.field_0x8;
            moved_array.data = nullptr;
            moved_array.keys = nullptr;
            field_0x8 = b;
            field_0x0 = a;
        }
    };

    void* vtbl;
    UnkVectorView<const void*> params;
    Name name;
    const void* return_type;
    void* field_0x40;
    void* invoker;
    UnkKeyedArray<void> field_0x50;
    Pair field_0x78;
    UnkVectorView<void*> param_attrs;
    u32 field_0xb0_w;

    UnkReflFunctionData(UnkReflFunctionData&& other)
        : vtbl(g_unk_7101a1b460 + 0x10), params(static_cast<UnkVectorView<const void*>&&>(other.params)),
          name(static_cast<Name&&>(other.name)), return_type(other.return_type), field_0x40(other.field_0x40),
          invoker(other.invoker), field_0x50(other.field_0x50), field_0x78(other.field_0x78, other.field_0x50),
          param_attrs(static_cast<UnkVectorView<void*>&&>(other.param_attrs)), field_0xb0_w(other.field_0xb0_w) {}
};
static_assert(sizeof(UnkReflFunctionData) == 0xb8, "UnkReflFunction layout");

}  // namespace refl_function_insert_b62a1

using namespace refl_function_insert_b62a1;

// Grow the keyed array to `capacity` (signature as src/clones/clones_00.cpp declares it).
void fn_7100097f80(UnkKeyedArray<UnkReflFunctionData>*, u32);  // as fn_7100097550.cpp declares it

void fn_71000dd958(UnkKeyedArray<UnkReflFunctionData>* self, const CStrId* name, UnkReflFunctionData* value) {
    if (self->count == self->capacity)
        fn_7100097f80(self, self->step + self->count);
    CStrId* key = &self->keys[self->count];
    UnkStringData* data = name->data;
    __builtin_memcpy(key, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
    new (&self->data[self->count]) UnkReflFunctionData(static_cast<UnkReflFunctionData&&>(*value));
    ++self->count;
}
