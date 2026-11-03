// CMultiLockOnBlockComponent's static initialiser: the unit header, the tunable's and the
// component's registrations with the tunable itself, the component's id, one bool property, two
// name prefixes and a global block of three empty vectors.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

// The unit's data (its statics, merged into one block).
struct UnkMultiLockOnBlockTU {
    UnkTUHeader header;
    u64 field_0x18;
    u64 field_0x20;
    CStrId try_to_activate_point;  // 0x28
    CStrId relax_point;            // 0x30
    UnkReflType tunable_type;      // 0x38
    u8 field_0x38_rest[0x170 - 0x38 - sizeof(UnkReflType)];
    UnkReflType type;              // 0x170
};

// Three vectors and a flag; element types are unknown.
struct UnkMultiLockOnLists {
    base::UnkVector<void*> lists[3];
    bool field_0x78;

    UnkMultiLockOnLists()
        : lists{base::UnkVector<void*>(0xf), base::UnkVector<void*>(0xf), base::UnkVector<void*>(0xf)} {
        field_0x78 = false;
    }
};

extern UnkMultiLockOnBlockTU g_unk_717272b730 __attribute__((visibility("hidden")));
extern char g_unk_717272b5f8;
extern UnkTunable g_unk_717272b600;
extern char g_unk_7101a7bb48[];
void fn_7100075104(void* self);
extern char g_unk_717272b629;
extern CStrId g_unk_717272b630;
extern UnkProperty g_unk_717272b638;
extern char g_unk_7101a7bb78[];
void fn_7100883c28(UnkProperty* p);
void fn_71008823e8(void* p);
extern UnkMultiLockOnLists g_unk_717272b660;
void fn_7100882460(void* p);

void fn_7100883e20() {
    UnkMultiLockOnBlockTU& tu = g_unk_717272b730;
    InitHeader(tu.header);
    fn_7100881a68();
    fn_710009ec00(&g_unk_717272b5f8, &tu.tunable_type);
    InitTunable(&g_unk_717272b600, fn_7100136ecc, reinterpret_cast<void**>(g_unk_7101a7bb48), fn_7100075104);
    fn_7100882100();
    fn_710009ec00(&g_unk_717272b629, &tu.type);
    new (&g_unk_717272b630) CStrId("MULTI_LOCK_ON_BLOCK", true);
    fn_7100000250(fn_710000051c, &g_unk_717272b630, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_717272b638, reinterpret_cast<void**>(g_unk_7101a7bb78), g_unk_71726db788,
                            "Activated", refl_TypeOf<fn_710000d228>(), fn_7100883c28, fn_71008823e8);
    new (&tu.try_to_activate_point) CStrId("try_to_activate_point_", true);
    fn_7100000250(fn_710000051c, &tu.try_to_activate_point, &g_unk_7101a19000);
    new (&tu.relax_point) CStrId("relax_point_", true);
    fn_7100000250(fn_710000051c, &tu.relax_point, &g_unk_7101a19000);
    new (&g_unk_717272b660) UnkMultiLockOnLists();
    fn_7100000250(fn_7100882460, &g_unk_717272b660, &g_unk_7101a19000);
}
