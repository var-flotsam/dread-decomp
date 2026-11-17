// UnkTU2611 (tools/tu_map.py unit 2611): the copy constructor of a sub-area's 0x188-byte record: every member
// is default-constructed (empty ids, empty lists with flags 0xf, cleared counters and flags), then, unless
// the source is the record itself, each is assigned from the source's. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkObject_lfa45a4;
struct UnkRecord_lfa579c;
struct UnkStrIdRecord_lfa4b64;
struct UnkSubAreaRecord_0x188;
struct UnkSubArea;
// As src/base/reflection/reflection_Construct_lfa45a4.cpp defines it.
void fn_7100fa45a4(UnkObject_lfa45a4* self);
// As src/base/reflection/reflection_Assign_lfa579c.cpp defines it.
UnkRecord_lfa579c* fn_7100fa579c(UnkRecord_lfa579c* self, const UnkRecord_lfa579c* other);
// As src/base/reflection/reflection_Assign_lfa4b64.cpp defines it.

// Nine ids, assigned together (fn_7100fa4b64).
struct UnkSubAreaIds_lfaa9ec {
    CStrId field_0x0;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    CStrId field_0x20;
    CStrId field_0x28;
    CStrId field_0x30;
    CStrId field_0x38;
    CStrId field_0x40;
};

struct UnkSubAreaData_lfaa9ec {
    CStrId name;
    u64 field_0x8;
    CRntVector<void*> field_0x10;
    u8 field_0x38[0xd0];  // constructed by fn_7100fa45a4, assigned by fn_7100fa579c
    UnkSubAreaIds_lfaa9ec field_0x108;
    CRntVector<void*> field_0x150;  // assigned by fn_71000ce19c
    u32 field_0x178;
    u32 field_0x17c;
    bool field_0x180;
    bool field_0x181;
};
static_assert(__builtin_offsetof(UnkSubAreaData_lfaa9ec, field_0x108) == 0x108, "UnkSubAreaData_lfaa9ec layout");
static_assert(__builtin_offsetof(UnkSubAreaData_lfaa9ec, field_0x178) == 0x178, "UnkSubAreaData_lfaa9ec layout");

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100faa9ec(UnkSubAreaRecord_0x188* self_, UnkSubArea* other_) {
    UnkSubAreaData_lfaa9ec* self = reinterpret_cast<UnkSubAreaData_lfaa9ec*>(self_);
    const UnkSubAreaData_lfaa9ec* other = reinterpret_cast<const UnkSubAreaData_lfaa9ec*>(other_);

    MakeEmpty(&self->name);
    new (&self->field_0x10) CRntVector<void*>(0xf);
    fn_7100fa45a4(reinterpret_cast<UnkObject_lfa45a4*>(self->field_0x38));
    MakeEmpty(&self->field_0x108.field_0x0);
    MakeEmpty(&self->field_0x108.field_0x8);
    MakeEmpty(&self->field_0x108.field_0x10);
    MakeEmpty(&self->field_0x108.field_0x18);
    MakeEmpty(&self->field_0x108.field_0x20);
    MakeEmpty(&self->field_0x108.field_0x28);
    MakeEmpty(&self->field_0x108.field_0x30);
    MakeEmpty(&self->field_0x108.field_0x38);
    MakeEmpty(&self->field_0x108.field_0x40);
    new (&self->field_0x150) CRntVector<void*>(0xf);
    self->field_0x178 = 0;
    self->field_0x17c = 0;
    self->field_0x180 = false;
    self->field_0x181 = false;

    // Nothing to assign when the record is copied from itself.
    if (reinterpret_cast<UnkSubArea*>(self_) != other_) {
        self->name = other->name;
        self->field_0x8 = other->field_0x8;
        self->field_0x10.Assign(other->field_0x10);
        fn_7100fa579c(reinterpret_cast<UnkRecord_lfa579c*>(self->field_0x38),
                      reinterpret_cast<const UnkRecord_lfa579c*>(other->field_0x38));
        fn_7100fa4b64(reinterpret_cast<UnkStrIdRecord_lfa4b64*>(&self->field_0x108),
                      reinterpret_cast<const UnkStrIdRecord_lfa4b64*>(&other->field_0x108));
        fn_71000ce19c(reinterpret_cast<UnkStruct_0x28*>(&self->field_0x150),
                      reinterpret_cast<const UnkStruct_0x28*>(&other->field_0x150));
        self->field_0x178 = other->field_0x178;
        self->field_0x17c = other->field_0x17c;
    }
}
