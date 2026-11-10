// Unit 222 (tools/tu_map.py, no reliable name): the world matrix of a bone found by id, composed with
// each parent's matrix up the chain (fn_71000127c8). Names and layouts are placeholders.
#include "base/UnkCloneCalls.h"
#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkBoneNode_b0261 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkBone_b0261 {
    UnkBone_b0261* parent;  // 0x0
    u8 pad_0x8[0x60];
    UnkBoneNode_b0261* node;  // 0x68
    u8 pad_0x70[0x28];
};
static_assert(sizeof(UnkBone_b0261) == 0x98, "bone");

struct UnkBoneSetTag_b0261;

struct UnkBoneSet_b0261 {
    u8 pad_0x0[0x8];
    base::UnkKeyedArray<UnkBoneSetTag_b0261, u64, UnkBone_b0261> by_id;  // 0x8
};

UnkMatrix44* fn_7100015508(void* matrices, u64 id);

UnkMatrix44 fn_710019bb08(UnkBoneSet_b0261* self, const u64* id, void* matrices) {
    u64 key = *id;
    UnkBone_b0261* bone = self->by_id.Find(key);
    if (bone == self->by_id.End())
        bone = nullptr;
    UnkMatrix44 result;
    UnkCopyMatrix(&result, *fn_7100015508(matrices, bone->node->id));
    for (UnkBone_b0261* p = bone->parent; p; p = p->parent) {
        UnkMatrix44 m = fn_71000127c8(*fn_7100015508(matrices, p->node->id), result);
        UnkCopyMatrix(&result, m);
    }
    return result;
}
