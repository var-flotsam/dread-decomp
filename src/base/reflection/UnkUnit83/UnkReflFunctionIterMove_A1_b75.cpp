// Unit 83 (base::reflection): the move helper (CType+0x38) of the iterator value over a keyed
// array of reflected functions (UnkReflFunction, 0xb8 bytes) and their names: base::UnkIterValue
// keyed by a string id, moved with refl_IterValueMove. The function's move constructor is the one
// of src/base/reflection/UnkUnit83/UnkReflFunctionArrayInsert_b62a1.cpp. Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

extern char g_unk_7101a1b460[];  // UnkReflFunction's virtual table (its address point is +0x10)

namespace refl_function_iter_a1_b75 {

struct UnkKeyedArray {
    void* data;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 field_0x1c;
    base::UnkAllocator* allocator;

    UnkKeyedArray(UnkKeyedArray&& other)
        : data(other.data), keys(other.keys), count(other.count), capacity(other.capacity), step(other.step),
          field_0x1c(other.field_0x1c), allocator(other.allocator) {
        other.data = nullptr;
        other.keys = nullptr;
        other.count = 0;
    }
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

struct Name {
    UnkStringData* data;
    Name(Name&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
};

struct UnkReflFunctionData {
    struct Pair {
        void* field_0x0;
        u32 field_0x8;
        Pair(const Pair& other) {
            void* a = other.field_0x0;
            u32 b = other.field_0x8;
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
    UnkKeyedArray field_0x50;
    Pair field_0x78;
    UnkVectorView<void*> param_attrs;
    u32 field_0xb0_w;

    UnkReflFunctionData(UnkReflFunctionData&& other)
        : vtbl(g_unk_7101a1b460 + 0x10), params(static_cast<UnkVectorView<const void*>&&>(other.params)),
          name(static_cast<Name&&>(other.name)), return_type(other.return_type), field_0x40(other.field_0x40),
          invoker(other.invoker), field_0x50(static_cast<UnkKeyedArray&&>(other.field_0x50)), field_0x78(other.field_0x78),
          param_attrs(static_cast<UnkVectorView<void*>&&>(other.param_attrs)), field_0xb0_w(other.field_0xb0_w) {}
};
static_assert(sizeof(UnkReflFunctionData) == 0xb8, "UnkReflFunction layout");

}  // namespace refl_function_iter_a1_b75

void fn_71000dd2b4(void* dst, const void* src) {
    using namespace refl_function_iter_a1_b75;
    base::refl_IterValueMove<UnkReflFunctionData, Name>(dst, src);
}
