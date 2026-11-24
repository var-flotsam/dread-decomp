// base::reflection::CClass's unit (72): grows a class's keyed array of reflected functions (0xb8 bytes
// each) and their names to `capacity`: each function is moved over and destroyed, each name moved and
// released. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflFunctionVtable.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

namespace refl_reserve_functions_l097f80 {

struct UnkReflFunctionData;

// The slots of the function's virtual table this unit calls.
struct UnkFunctionVtbl {
    void (*destroy)(UnkReflFunctionData* self);  // the complete destructor
};

// base::global::CRntVector's layout, with the move constructor the function's uses.
template <class T>
struct UnkVectorView {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    UnkVectorView(UnkVectorView&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

// A keyed array (UntypedSmallDictionary in CClass.h): values and their names, side by side.
template <class T>
struct UnkKeyedArray {
    T* data;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 field_0x1c;
    base::IAllocator* allocator;

    UnkKeyedArray(const UnkKeyedArray& other)
        : data(other.data), keys(other.keys), count(other.count), capacity(other.capacity), step(other.step),
          field_0x1c(other.field_0x1c), allocator(other.allocator) {}
};

// A reflected function (CFunction) with its whole layout.
struct UnkReflFunctionData {
    // The name: moved, the source gets the pool's empty id.
    struct Name {
        UnkStringData* data;
        Name(Name&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    };
    // The two fields after the keyed array. Their copy is read before the array's source is
    // cleared and written after it (the order the original's move constructor has).
    struct Pair {
        void* field_0x0;
        u32 field_0x8;
        Pair(const Pair& other, UnkKeyedArray<void>& moved_array) {
            moved_array.count = 0;
            void* a = other.field_0x0;
            u32 b = other.field_0x8;
            moved_array.data = nullptr;
            moved_array.keys = nullptr;
            field_0x8 = b;
            field_0x0 = a;
        }
    };

    const UnkFunctionVtbl* vtbl;
    UnkVectorView<const void*> params;
    Name name;
    const void* return_type;
    void* field_0x40;
    void* invoker;
    UnkKeyedArray<void> field_0x50;
    Pair field_0x78;
    UnkVectorView<void*> param_attrs;
    u32 field_0xb0;

    UnkReflFunctionData(UnkReflFunctionData&& other)
        : vtbl(reinterpret_cast<const UnkFunctionVtbl*>(g_unk_7101a1b460 + 0x10)),
          params(static_cast<UnkVectorView<const void*>&&>(other.params)), name(static_cast<Name&&>(other.name)),
          return_type(other.return_type), field_0x40(other.field_0x40), invoker(other.invoker),
          field_0x50(other.field_0x50), field_0x78(other.field_0x78, other.field_0x50),
          param_attrs(static_cast<UnkVectorView<void*>&&>(other.param_attrs)), field_0xb0(other.field_0xb0) {}

    void Destroy() { vtbl->destroy(this); }
};
static_assert(sizeof(UnkReflFunctionData) == 0xb8, "CFunction layout");


}  // namespace refl_reserve_functions_l097f80

using namespace refl_reserve_functions_l097f80;

void fn_7100097f80(UnkKeyedArray<UnkReflFunctionData>* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkReflFunctionData* values =
        static_cast<UnkReflFunctionData*>(self->allocator->vf_0x40((u64)capacity * sizeof(UnkReflFunctionData), 8, self->field_0x1c));
    CStrId* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->field_0x1c));
    if (UnkReflFunctionData* old = self->data) {
        UnkReflFunctionData* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
            new (dst) UnkReflFunctionData(static_cast<UnkReflFunctionData&&>(*old));
            old->Destroy();
        }
        self->allocator->vf_0x48(self->data);
    }
    if (CStrId* old = self->keys) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            CStrId::MoveConstructReloading(&keys[j], *old);
            old->Reset();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->data = values;
    self->keys = keys;
    self->capacity = capacity;
}
