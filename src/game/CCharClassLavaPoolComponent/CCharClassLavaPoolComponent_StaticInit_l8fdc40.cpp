// CCharClassLavaPoolComponent's unit (tools/tu_map.py unit 1393): the unit's static initialiser. The header
// data and a pair of -1 limits with a flag, the "LAVA_POOL" id, the "Used" and "LAVA_VOLUMES" properties, the
// component's class, CLavaPoolConfig's class, CLavaPoolComponent's class and its registry id.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/UnkStrIdRegistry.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/CCharClassLavaPoolComponent.h"
#include "game/CCharClassLiquidPoolComponent.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTUSymbols.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;

// The unit's statics (the compiler merges them into one block).
struct UnkLavaPoolTU {
    UnkTUHeader header;
    u8 pad_0x18[0x10];
    CStrId component_id;  // 0x28
    f32 field_0x30;
    f32 field_0x34;
    bool field_0x38;
    u8 pad_0x39[7];
    char charclass_type[0x178 - 0x40];  // 0x40
    char component_type[8];             // 0x178
};
static_assert(__builtin_offsetof(UnkLavaPoolTU, component_type) == 0x178, "layout");
extern UnkLavaPoolTU g_unk_7172733538 __attribute__((visibility("hidden")));

extern CStrId g_unk_7172733330;
extern base::reflection::CType g_unk_71727333a8;  // CLavaPoolConfig
extern char g_unk_71727333a0[];
extern char g_unk_71727334e0[];
extern char g_unk_71727334e1[];
extern void* g_unk_7101a809f0[];  // the "Used" property's vtable
void fn_71008fd2ac(UnkProperty* p);
void fn_71008fab30(void* p);
void fn_71008fb614(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_71008fb2b4(CClass* cls);

void CCharClassLavaPoolComponent_InitStatics() {
    UnkLavaPoolTU& tu = g_unk_7172733538;
    InitHeader(tu.header);
    tu.field_0x30 = -1.0f;
    tu.field_0x34 = -1.0f;
    tu.field_0x38 = true;
    new (&g_unk_7172733330) CStrId("LAVA_POOL", true);
    fn_7100000250(CStrId_Destroy, &g_unk_7172733330, &g_unk_7101a19000);
    InitProperty(&g_unk_7172733338, g_unk_7101a809f0, g_unk_71726db788, "Used", refl_TypeOf<fn_710000d228>(),
                 fn_71008fd2ac, fn_71008fab30);
    InitPropertyEx(&g_unk_7172733360, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101a45f90, fn_71002ef5bc,
                   "LAVA_VOLUMES", &g_unk_7172733a70, fn_71004fd2cc, fn_71004fa3b8);
    fn_71008fabac();
    fn_710009ec00(g_unk_71727333a0, tu.charclass_type);

    {
        CStrId name("CLavaPoolConfig", true);
        fn_71008fb614(&g_unk_71727333a8, name, nullptr, nullptr, fn_71008fb2b4, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71727333a8, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71727334e0, &g_unk_71727333a8);

    fn_71008fb79c();
    fn_710009ec00(g_unk_71727334e1, tu.component_type);
    {
        CStrId name("CLavaPoolComponent", true);
        new (&tu.component_id) CStrId(fn_7100de8f78(name));
    }
    fn_7100000250(CStrId_Destroy, &tu.component_id, &g_unk_7101a19000);
}
