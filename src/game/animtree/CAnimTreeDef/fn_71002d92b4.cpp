// Counts an animated node: when the node picked from the argument's holder (inline, or behind
// a pointer for 9 or more entries or kind 1) is a CAnimatedNode, the counter at 0xa8 of the
// object at 8 is incremented. Returns an empty result of kind 2.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkNode {
    virtual const CClass* vf_0x0();
};
struct UnkNodeHolder {
    UnkNode* node;
};
struct UnkNodeSet {
    u8 unk_0x0[0x18];
    u64 count;
};
struct UnkNodeSource {
    UnkNodeHolder* holder;
    UnkNodeSet* set;
    u8 unk_0x10[4];
    s32 kind;
};
struct UnkCounted {
    u8 unk_0x0[0xa8];
    u32 count;
};
struct UnkCounter {
    u8 unk_0x0[8];
    UnkCounted* counted;
};
struct UnkResult_0x18 {
    u64 empty[2];
    u64 kind_and_flag;
};

UnkResult_0x18 fn_71002d92b4(void* unused, UnkCounter* counter, UnkNodeSource** source) {
    UnkNodeSource* src = *source;
    UnkCounted* counted = counter->counted;
    UnkNode** slot;
    if (src->kind == 1 || src->set->count >= 9)
        slot = reinterpret_cast<UnkNode**>(src->holder);
    else
        slot = reinterpret_cast<UnkNode**>(src);
    UnkNode* node = *slot;
    if (fn_710009d50c(node->vf_0x0(), fn_71002d2250()))
        counted->count++;
    UnkResult_0x18 result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
