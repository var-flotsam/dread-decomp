// CLogicDroppedItemManager::CTunableDroppableBillboard unit (tools/tu_map.py, the name from reflection):
// reflected setters of the tunable's two distances at +0x24c and +0x250. Each stores the value picked
// from the argument's holder (inline, or behind a pointer for 9 or more entries or kind 1) and, when the
// dropped-item manager g_unk_71726eb3e8 exists, hands the squares of the tunable g_unk_71727c10c8's two
// distances to its eight billboards (+0x48c of each; the first six get the first, the last two the
// second). Each returns an empty result of kind 2. Names and layouts are placeholders.
#include "types.h"

struct UnkValueSet_ddb {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_ddb {
    void* holder;
    UnkValueSet_ddb* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_ddb {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkDroppableTunable_ddb {
    u8 unk_0x0[0x24c];
    f32 field_0x24c;
    f32 field_0x250;
};

struct UnkDroppableOwner_ddb {
    u8 unk_0x0[8];
    UnkDroppableTunable_ddb* target;
};

struct UnkBillboard_ddb {
    u8 unk_0x0[0x48c];
    f32 field_0x48c;  // a squared distance
};

struct UnkDroppedItemManager_ddb {
    u8 unk_0x0[0x7848];
    UnkBillboard_ddb* billboards[8];
};

extern void* g_unk_71726eb3e8;
struct UnkDroppableTunable;
extern UnkDroppableTunable g_unk_71727c10c8;

static inline __attribute__((always_inline)) const u32* PickArg_ddb(UnkValueSource_ddb* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<const u32*>(src->holder);
    return reinterpret_cast<const u32*>(src);
}

template <u32 Offset>
static inline __attribute__((always_inline)) UnkResult_0x18_ddb SetDistance_ddb(UnkDroppableOwner_ddb* owner,
                                                                                UnkValueSource_ddb** source) {
    UnkDroppableTunable_ddb* target = owner->target;
    UnkValueSource_ddb* src = *source;
    f32 v = *reinterpret_cast<const f32*>(PickArg_ddb(src));
    if (Offset == 0x24c)
        target->field_0x24c = v;
    else
        target->field_0x250 = v;
    if (UnkDroppedItemManager_ddb* manager = static_cast<UnkDroppedItemManager_ddb*>(g_unk_71726eb3e8)) {
        const UnkDroppableTunable_ddb& tunable = reinterpret_cast<const UnkDroppableTunable_ddb&>(g_unk_71727c10c8);
        f32 near = tunable.field_0x24c * tunable.field_0x24c;
        manager->billboards[0]->field_0x48c = near;
        manager->billboards[1]->field_0x48c = near;
        manager->billboards[2]->field_0x48c = near;
        manager->billboards[3]->field_0x48c = near;
        manager->billboards[4]->field_0x48c = near;
        manager->billboards[5]->field_0x48c = near;
        f32 far = tunable.field_0x250 * tunable.field_0x250;
        manager->billboards[6]->field_0x48c = far;
        manager->billboards[7]->field_0x48c = far;
    }
    UnkResult_0x18_ddb result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

UnkResult_0x18_ddb fn_7100e2f7e8(void*, UnkDroppableOwner_ddb* owner, UnkValueSource_ddb** source) {
    return SetDistance_ddb<0x24c>(owner, source);
}
UnkResult_0x18_ddb fn_7100e2f8e0(void*, UnkDroppableOwner_ddb* owner, UnkValueSource_ddb** source) {
    return SetDistance_ddb<0x250>(owner, source);
}
