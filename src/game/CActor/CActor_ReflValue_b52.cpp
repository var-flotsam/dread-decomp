// Copy of fn_7100407018 (src/game/CCentralUnitAIComponent/CCentralUnitAIComponent_Value_b49.cpp) in
// CActor's unit: reads the object behind the lazily resolved handle at +0x38 of the holder (source
// +0x8, state +0x30) as a reflected value of the type fn_7100229688 describes, holding a counted
// reference to it. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkReflValue.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkEnumType;
UnkEnumType* fn_7100229688();

struct UnkCountedActor_b52 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkActorRef_b52 {
    UnkCountedActor_b52* ptr;
    ~UnkActorRef_b52() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

struct UnkActorHolder_b52 {
    u8 pad_0x0[0x8];
    u8 source[0x28];
    u32 state;
    u32 pad_0x34;
    base::UnkRefHandle handle;
};

UnkReflValue fn_710023b068(UnkActorHolder_b52* holder) {
    if (holder->state == 1) {
        {
            UnkRefPtr fresh = fn_710023b580(holder->source);
            fn_7100234f78(&holder->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        holder->state = 2;
    }
    UnkActorRef_b52 ref;
    ref.ptr = nullptr;
    if (auto* p = static_cast<UnkCountedActor_b52*>(holder->handle.ptr)) {
        ++p->ref_count;
        ref.ptr = p;
    }
    auto* type = reinterpret_cast<const base::reflection::CType*>(fn_7100229688());
    UnkReflValue value;
    value.kind = 2;
    value.field_0x0 = nullptr;
    value.type = nullptr;
    fn_71000a18d4(&value, type, &ref, 0);
    return value;
}
