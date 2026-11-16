// Unit 83 (base::reflection): erases one reflected function (UnkReflFunction, 0xb8 bytes) from a keyed
// array of functions with their names alongside: the function is destroyed through its virtual
// destructor and the last function and name moved into its place. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

extern char g_unk_7101a1b460[];  // UnkReflFunction's virtual table (its address point is +0x10)

namespace refl_function_erase_l0dded4 {

struct UnkReflFunctionData;

// The first slot of UnkReflFunction's virtual table: its destructor.
struct UnkReflFunctionVtbl {
    void (*destruct)(UnkReflFunctionData* self);
};

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

    UnkReflFunctionVtbl* vtbl;
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
        : vtbl(reinterpret_cast<UnkReflFunctionVtbl*>(g_unk_7101a1b460 + 0x10)),
          params(static_cast<UnkVectorView<const void*>&&>(other.params)), name(static_cast<Name&&>(other.name)),
          return_type(other.return_type), field_0x40(other.field_0x40), invoker(other.invoker),
          field_0x50(static_cast<UnkKeyedArray&&>(other.field_0x50)), field_0x78(other.field_0x78),
          param_attrs(static_cast<UnkVectorView<void*>&&>(other.param_attrs)), field_0xb0_w(other.field_0xb0_w) {}

    void Destroy() { vtbl->destruct(this); }
};
static_assert(sizeof(UnkReflFunctionData) == 0xb8, "UnkReflFunction layout");

// The functions with their names alongside in a second array.
struct UnkFunctionArray {
    UnkReflFunctionData* functions;
    CStrId* names;
    u32 count;
};

}  // namespace refl_function_erase_l0dded4

using namespace refl_function_erase_l0dded4;

// Erases the function `it` points at: the last function and name take its place.
UnkReflFunctionData* fn_71000dded4(char* array, void* it) {
    UnkFunctionArray* self = reinterpret_cast<UnkFunctionArray*>(array);
    u32 index = static_cast<u32>(static_cast<UnkReflFunctionData*>(it) - self->functions);
    self->functions[index].Destroy();
    self->names[index].Reset();
    if (self->count - 1 != index) {
        new (&self->functions[index])
            UnkReflFunctionData(static_cast<UnkReflFunctionData&&>(self->functions[self->count - 1]));
        self->functions[self->count - 1].Destroy();
        CStrId::MoveConstructReloading(&self->names[index], self->names[self->count - 1]);
        self->names[self->count - 1].Reset();
    }
    --self->count;
    return &self->functions[index];
}
