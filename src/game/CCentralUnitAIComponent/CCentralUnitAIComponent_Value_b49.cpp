// CCentralUnitAIComponent unit (tools/tu_map.py): reads the object behind the lazily resolved handle
// at +0x38 of the holder (source +0x8, state +0x30; the shape of CTutoExitLogicAction_b47.cpp) as a
// reflected value of the type fn_7100407170 describes, holding a counted reference to it. Names and
// layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkReflValue.h"
#include "base/UnkDestructors.h"
#include "game/UnkTU437.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkEnumType;
UnkEnumType* fn_7100407170();

struct UnkCountedCentral_b49 : base::UnkRefCounted {
    void* field_0x10;
};

using UnkRef = base::UnkRefPtrMember;

struct UnkCentralHolder_b49 {
    u8 pad_0x0[0x8];
    u8 source[0x28];
    u32 state;
    u32 pad_0x34;
    base::UnkRefHandle handle;
};

UnkReflValue fn_7100407018(UnkCentralHolder_b49* holder) {
    if (holder->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(holder->source);
            fn_710031d208(&holder->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        holder->state = 2;
    }
    UnkRef ref;
    ref.ptr = nullptr;
    if (auto* p = static_cast<UnkCountedCentral_b49*>(holder->handle.ptr)) {
        ++p->ref_count;
        ref.ptr = p;
    }
    auto* type = reinterpret_cast<const base::reflection::CType*>(fn_7100407170());
    UnkReflValue value;
    value.kind = 2;
    value.field_0x0 = nullptr;
    value.type = nullptr;
    fn_71000a18d4(&value, type, &ref, 0);
    return value;
}
