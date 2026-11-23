// Unit 83 (base::reflection): erases one reflected function (CFunction, 0xb8 bytes) from a keyed
// array of functions with their names alongside: the function is destroyed through its virtual
// destructor and the last function and name moved into its place. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "base/reflection/UnkReflFunctionVtable.h"
#include "types.h"

using base::global::CRntSmallDictionary;
using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStringData;

namespace refl_function_erase_l0dded4 {

struct UnkReflFunctionData;

// The first slot of CFunction's virtual table: its destructor.
struct UnkReflFunctionVtbl {
    void (*destruct)(UnkReflFunctionData* self);
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
    CRntVector<const void*> params;
    UnkMovedStrId name;
    const void* return_type;
    void* field_0x40;
    void* invoker;
    CRntSmallDictionary<CStrId, void*> field_0x50;
    Pair field_0x78;
    CRntVector<void*> param_attrs;
    u32 field_0xb0_w;

    UnkReflFunctionData(UnkReflFunctionData&& other)
        : vtbl(reinterpret_cast<UnkReflFunctionVtbl*>(g_unk_7101a1b460 + 0x10)),
          params(static_cast<CRntVector<const void*>&&>(other.params)), name(static_cast<UnkMovedStrId&&>(other.name)),
          return_type(other.return_type), field_0x40(other.field_0x40), invoker(other.invoker),
          field_0x50(static_cast<CRntSmallDictionary<CStrId, void*>&&>(other.field_0x50)), field_0x78(other.field_0x78),
          param_attrs(static_cast<CRntVector<void*>&&>(other.param_attrs)), field_0xb0_w(other.field_0xb0_w) {}

    void Destroy() { vtbl->destruct(this); }
};
static_assert(sizeof(UnkReflFunctionData) == 0xb8, "CFunction layout");

}  // namespace refl_function_erase_l0dded4

using namespace refl_function_erase_l0dded4;

// Erases the function `it` points at: the last function and name take its place.
UnkReflFunctionData* fn_71000dded4(char* array, void* it) {
    auto* self = reinterpret_cast<CRntSmallDictionary<CStrId, UnkReflFunctionData>*>(array);
    u32 index = static_cast<u32>(static_cast<UnkReflFunctionData*>(it) - self->values);
    self->values[index].Destroy();
    self->keys[index].Reset();
    if (self->count - 1 != index) {
        new (&self->values[index])
            UnkReflFunctionData(static_cast<UnkReflFunctionData&&>(self->values[self->count - 1]));
        self->values[self->count - 1].Destroy();
        CStrId::MoveConstructReloading(&self->keys[index], self->keys[self->count - 1]);
        self->keys[self->count - 1].Reset();
    }
    --self->count;
    return &self->values[index];
}
