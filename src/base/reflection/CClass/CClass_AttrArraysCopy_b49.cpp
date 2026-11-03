// Unit 72 (base::reflection::CClass): the copy constructor of a reflected element's attribute
// arrays (CBlackboard.h's UnkPropertyArrays holding 0x18-byte reflected values and their keys'
// string ids): fn_7100095d68. The storage is reserved (fn_7100093dd0), each value copied
// through fn_71000a18d4 (inline when its type fits in 8 bytes), then the keys copied, counting a
// use of each. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::UnkStringData;
using base::reflection::CType;
using base::reflection::UnkReflValue;

namespace {

// The size of a value of the type (CType+0x18).
inline u64 AttrTypeSize(const char* type) {
    return *reinterpret_cast<const u64*>(type + 0x18);
}

}  // namespace

void fn_7100095d68(UnkPropertyArrays* self, const UnkPropertyArrays* other) {
    self->ids = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->values = nullptr;
    self->step = other->step;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100093dd0(self, other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        UnkReflValue* dst = &static_cast<UnkReflValue*>(self->values)[i];
        const UnkReflValue* src = &static_cast<const UnkReflValue*>(other->values)[i];
        dst->field_0x0 = nullptr;
        dst->type = nullptr;
        dst->kind = src->kind;
        const char* type = src->type;
        const void* value = nullptr;
        if (type)
            value = (src->field_0x14 == 1 || AttrTypeSize(type) > 8) ? src->field_0x0 : src;
        fn_71000a18d4(dst, reinterpret_cast<const CType*>(type), value, 0);
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        UnkStringData* data = other->ids[i].data;
        __builtin_memcpy(&self->ids[i], &data, sizeof(data));
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
}
