// CCharClassVulkranAIComponent's unit (tools/tu_map.py unit 1083): the constructor of the class whose
// destructor is fn_71006da89c (src/game/CCharClassVulkranAIComponent/CCharClassVulkranAIComponent_Dtor_A4_b70.cpp):
// the base constructor fn_7100416db8, the vtable 0x7101a687e8, four records (a small array, a string
// id set to g_unk_7101d470e0, two floats of -1) at +0x570, an array of records at +0x690 and a string
// id at +0x6d8 set to the empty string. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassBehaviorTreeAIComponent.h"
#include "game/CCharClassVulkranAIComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkObject_7100416db8;

struct UnkVulkranRecord_ovb0142 {
    base::UnkVector<u32> values{15};
    CStrId id = g_unk_7101d470e0;
    f32 field_0x30 = -1.0f;
    f32 field_0x34 = -1.0f;
    u64 field_0x38 = 0;
    u64 field_0x40 = 0;
};

struct UnkCharClassVulkran_ovb0142 {
    void** vtable;
    u8 pad_0x8[0x568];
    UnkVulkranRecord_ovb0142 records[4];  // 0x570
    base::UnkVector<UnkVulkranRecord_ovb0142> list{15};  // 0x690
    u8 pad_0x6b8[0x20];
    base::global::UnkStringData* id;  // 0x6d8
};
static_assert(__builtin_offsetof(UnkCharClassVulkran_ovb0142, id) == 0x6d8, "layout");

void fn_71006da644(UnkCharClassVulkran_ovb0142* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a687e8 + 2;
    new (&self->records[0]) UnkVulkranRecord_ovb0142;
    new (&self->records[1]) UnkVulkranRecord_ovb0142;
    new (&self->records[2]) UnkVulkranRecord_ovb0142;
    new (&self->records[3]) UnkVulkranRecord_ovb0142;
    new (&self->list) base::UnkVector<UnkVulkranRecord_ovb0142>(15);
    self->id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id = &g_unk_7101d47108->empty;
}
