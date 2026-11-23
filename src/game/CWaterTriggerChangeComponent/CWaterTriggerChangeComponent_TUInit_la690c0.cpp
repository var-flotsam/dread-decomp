// CWaterTriggerChangeComponent's static initialiser (tools/tu_map.py unit 1579): the unit header, the
// unset origin and target heights, the component's id, its eight properties (the origin's and the
// target's initial height and offset and their two corner indices) and the class's registration.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

// The unit's data (its statics, merged into one block).
struct UnkWaterTriggerChangeTU {
    UnkTUHeader header;
    u8 field_0x18[0x18];
    f32 origin_height;  // 0x30
    f32 target_height;  // 0x34
    bool field_0x38;
    UnkReflType type;  // 0x40
};
static_assert(__builtin_offsetof(UnkWaterTriggerChangeTU, type) == 0x40, "layout");

extern UnkWaterTriggerChangeTU g_unk_717274e900 __attribute__((visibility("hidden")));
extern CStrId g_unk_717274e770;
extern UnkProperty g_unk_717274e778;
extern UnkProperty g_unk_717274e7a0;
extern UnkProperty g_unk_717274e7c8;
extern UnkProperty g_unk_717274e7f0;
extern UnkProperty g_unk_717274e818;
extern UnkProperty g_unk_717274e840;
extern UnkProperty g_unk_717274e868;
extern UnkProperty g_unk_717274e890;
extern char g_unk_717274e8b8;
extern char g_unk_7101a8f9d8[];
void fn_7100a68d04(UnkProperty* p);
void fn_7100a653b8(void* p);

void fn_7100a690c0() {
    UnkWaterTriggerChangeTU& tu = g_unk_717274e900;
    void** vtable = reinterpret_cast<void**>(g_unk_7101a8f9d8);
    InitHeader(tu.header);
    tu.origin_height = -1.0f;
    tu.target_height = -1.0f;
    tu.field_0x38 = true;
    new (&g_unk_717274e770) CStrId("WATERCHANGE", true);
    fn_7100000250(CStrId_Destroy, &g_unk_717274e770, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_717274e778, vtable, g_unk_71726db788, "OriginInitH",
                            refl_TypeOf<fn_710000abbc>(), fn_7100a68d04, fn_7100a653b8);
    InitProperty(&g_unk_717274e7a0, vtable, g_unk_71726db788, "OriginInitY", refl_TypeOf<fn_710000abbc>(),
                 fn_7100a68d04, fn_7100a653b8);
    InitPropertyVtableFirst(&g_unk_717274e7c8, vtable, g_unk_71726db788, "TROriginIdx",
                            refl_TypeOf<fn_7100005178>(), fn_7100a68d04, fn_7100a653b8);
    InitProperty(&g_unk_717274e7f0, vtable, g_unk_71726db788, "TLOriginIdx", refl_TypeOf<fn_7100005178>(),
                 fn_7100a68d04, fn_7100a653b8);
    InitPropertyVtableFirst(&g_unk_717274e818, vtable, g_unk_71726db788, "TargetInitH",
                            refl_TypeOf<fn_710000abbc>(), fn_7100a68d04, fn_7100a653b8);
    InitProperty(&g_unk_717274e840, vtable, g_unk_71726db788, "TargetInitY", refl_TypeOf<fn_710000abbc>(),
                 fn_7100a68d04, fn_7100a653b8);
    InitPropertyVtableFirst(&g_unk_717274e868, vtable, g_unk_71726db788, "TRTargetIdx",
                            refl_TypeOf<fn_7100005178>(), fn_7100a68d04, fn_7100a653b8);
    InitProperty(&g_unk_717274e890, vtable, g_unk_71726db788, "TLTargetIdx", refl_TypeOf<fn_7100005178>(),
                 fn_7100a68d04, fn_7100a653b8);
    fn_7100a65434();
    fn_710009ec00(&g_unk_717274e8b8, &tu.type);
}
