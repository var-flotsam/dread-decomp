// Unit 72 (base::reflection::CClass): shrinking a keyed array of reflected functions (0xb8 bytes
// each, UnkReflFunction) and their names to its count: the functions are moved and the old ones
// destroyed, the names copied and released. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

extern char g_unk_7101a1b460[];  // UnkReflFunction's virtual table (its address point is +0x10)

namespace refl_function_shrink_l096c80 {

template <class T>
struct UnkKeyedArray {
    T* data;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkKeyedArray(const UnkKeyedArray& other)
        : data(other.data), keys(other.keys), count(other.count), capacity(other.capacity), step(other.step),
          flags(other.flags), allocator(other.allocator) {}
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

struct UnkReflFunctionData;

struct UnkReflFunctionVtbl {
    void (*dtor)(UnkReflFunctionData* self);
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

    const UnkReflFunctionVtbl* vtbl;
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
        : vtbl(reinterpret_cast<const UnkReflFunctionVtbl*>(g_unk_7101a1b460 + 0x10)),
          params(static_cast<UnkVectorView<const void*>&&>(other.params)),
          name(static_cast<Name&&>(other.name)), return_type(other.return_type), field_0x40(other.field_0x40),
          invoker(other.invoker), field_0x50(other.field_0x50), field_0x78(other.field_0x78, other.field_0x50),
          param_attrs(static_cast<UnkVectorView<void*>&&>(other.param_attrs)), field_0xb0_w(other.field_0xb0_w) {}

    void Destroy() { vtbl->dtor(this); }
};
static_assert(sizeof(UnkReflFunctionData) == 0xb8, "UnkReflFunction layout");

}  // namespace refl_function_shrink_l096c80

using namespace refl_function_shrink_l096c80;

void fn_7100096c80(UnkKeyedArray<UnkReflFunctionData>* self) {
    if (self->count == self->capacity)
        return;
    UnkReflFunctionData* values = static_cast<UnkReflFunctionData*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkReflFunctionData), 8, self->flags));
    CStrId* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
    if (UnkReflFunctionData* src = self->data) {
        UnkReflFunctionData* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkReflFunctionData(static_cast<UnkReflFunctionData&&>(*src));
            src->Destroy();
        }
        self->allocator->vf_0x48(self->data);
    }
    if (CStrId* src = self->keys) {
        for (s32 i = 0; (u32)i < self->count; ++i, ++src) {
            new (&keys[i]) CStrId(*src);
            src->Reset();  // released in two steps, like the destructor
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->data = values;
    self->keys = keys;
    self->capacity = self->count;
}
