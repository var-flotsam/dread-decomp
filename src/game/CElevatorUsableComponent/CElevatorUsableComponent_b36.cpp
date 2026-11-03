// In the CElevatorUsableComponent unit: two getters like those of CTrainUsableComponent_b36.cpp
// that pick one of two string ids of the CCharClassElevatorUsableComponent (the object at +0x10,
// cast to that class) by the side at +0x54c. The class is not checked for null, as in the original.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkElevatorUsableClass {
    u8 pad_0x0[0x1b8];
    CStrId ids[4];  // 0x1b8
};

struct UnkElevatorUsable {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
    u8 pad_0x18[0x534];
    s32 side;  // 0x54c
};

static inline UnkElevatorUsableClass* GetElevatorClass(UnkElevatorUsable* self) {
    UnkReflObject* object = self->object;
    return object && fn_710009d50c(object->vf_0x0(), fn_7100a7f920())
               ? reinterpret_cast<UnkElevatorUsableClass*>(object)
               : nullptr;
}

// The id at index `First` (side 0) or `Second` (any other side).
#define ELEVATOR_SIDE_ID(name, First, Second)                \
    const CStrId& name(UnkElevatorUsable* self) {            \
        s32 side = self->side;                               \
        UnkElevatorUsableClass* cls = GetElevatorClass(self); \
        if (side == 0)                                       \
            return cls->ids[First];                          \
        return cls->ids[Second];                             \
    }

ELEVATOR_SIDE_ID(fn_7100a81d98, 0, 1)
ELEVATOR_SIDE_ID(fn_7100a81e08, 3, 2)
