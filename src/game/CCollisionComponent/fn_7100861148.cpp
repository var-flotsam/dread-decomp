// When a pending flag is set: hands each registered collider (array at +0x98) to its owner,
// as a CColliderGroup or a CCollider depending on its class, then calls one virtual with 4 on
// each entry of the array at +0x48, and clears the flag.
#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkStruct_7100861148_Collider : UnkReflObject {
    u8 pad_0x8[0x28];
    void* field_0x30;
};

void fn_7100d5abfc(void* owner, UnkStruct_7100861148_Collider* collider);
void fn_7100d5ac50(void* owner, UnkStruct_7100861148_Collider* group);

using UnkVec_7100861148_A = base::UnkVector<UnkStruct_7100861148_Collider*>;

UnkStruct_7100861148_Collider** fn_7100862088(UnkVec_7100861148_A* vec, u32 index);

struct UnkStruct_7100861148 {
    u8 pad_0x0[0x48];
    base::UnkVector<UnkCollisionRecord> field_0x48;
    u8 pad_0x70[0x28];
    UnkVec_7100861148_A field_0x98;
    u8 pad_0xc0[0x8];
    bool field_0xc8;
};

void fn_7100861148(UnkStruct_7100861148* self) {
    if (!self->field_0xc8)
        return;
    for (u32 i = 0; i < self->field_0x98.count; ++i) {
        UnkStruct_7100861148_Collider* c = *fn_7100862088(&self->field_0x98, i);
        void* owner = c->field_0x30;
        if (!owner)
            continue;
        if (fn_710009d52c(c->vf_0x0(), fn_7100d454fc()))
            fn_7100d5ac50(owner, fn_710009d50c(c->vf_0x0(), fn_7100d454fc()) ? c : nullptr);
        else
            fn_7100d5abfc(owner, fn_710009d50c(c->vf_0x0(), fn_7100d361c0()) ? c : nullptr);
    }
    for (u32 i = 0; i < self->field_0x48.count; ++i)
        fn_71007253b8(&self->field_0x48, i).field_0x28->vf_0x40(4);
    self->field_0xc8 = false;
}
