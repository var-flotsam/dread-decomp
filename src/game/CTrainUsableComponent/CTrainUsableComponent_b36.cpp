// In the CTrainUsableComponent unit: six getters (the first is fn_7100a99878) that pick one of two
// string ids of the CCharClassTrainUsableComponent (the object at +0x10, cast to that class) by the
// side at +0x548. The class is not checked for null, as in the original. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkTrainUsableClass {
    u8 pad_0x0[0x1b8];
    CStrId ids[12];  // 0x1b8
};

struct UnkTrainUsable {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
    u8 pad_0x18[0x530];
    s32 side;  // 0x548
};

static inline UnkTrainUsableClass* GetTrainClass(UnkTrainUsable* self) {
    UnkReflObject* object = self->object;
    return object && fn_710009d50c(object->vf_0x0(), fn_7100a97794())
               ? reinterpret_cast<UnkTrainUsableClass*>(object)
               : nullptr;
}

// The id at index `First` (side 0) or `Second` (any other side).
#define TRAIN_SIDE_ID(name, First, Second)                \
    const CStrId& name(UnkTrainUsable* self) {            \
        s32 side = self->side;                            \
        UnkTrainUsableClass* cls = GetTrainClass(self);   \
        if (side == 0)                                    \
            return cls->ids[First];                       \
        return cls->ids[Second];                          \
    }

TRAIN_SIDE_ID(fn_7100a99878, 0, 1)
TRAIN_SIDE_ID(fn_7100a998e8, 2, 3)
TRAIN_SIDE_ID(fn_7100a99958, 5, 4)
TRAIN_SIDE_ID(fn_7100a999c8, 6, 7)
TRAIN_SIDE_ID(fn_7100a99a38, 9, 8)
TRAIN_SIDE_ID(fn_7100a99aa8, 11, 10)
