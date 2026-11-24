// Static initialiser of CLogicCamera's unit (tools/tu_map.py unit 2655): the header ids, the unit's
// property names, the class and its pointer, link and reference types, CGameLink<CLogicCamera> and
// three conversions to it. Names and layouts are placeholders.
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"

using namespace unk_static_init;

struct UnkStatics_71010532e0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    u8 pad_0x58[0x8];
    u8 field_0x60[8];
};
extern UnkStatics_71010532e0 g_unk_71729fdfc0 __attribute__((visibility("hidden")));

extern char g_unk_71729fdd08[];
extern char g_unk_71729fdd09[];
extern char g_unk_71729fdd0a[];
extern char g_unk_71729fdd0b[];
extern char g_unk_71729fde48[];
const base::reflection::CType* fn_71009112ac();  // base::reflection::refl_GetType<CLogicCamera*>
UnkEnumType* fn_71010510b4();
const base::reflection::CType* fn_7100910720();  // base::reflection::refl_GetType<CGameObjectRef<CLogicCamera>>
extern UnkEnumType g_unk_71729fdd10;  // the CGameLink<CLogicCamera> class
void fn_7101051230(UnkEnumType* type, const CStrId& name, const base::reflection::CType* parent, void*,
                   void (*setup)(base::reflection::CClass*), void*);
void fn_7101051190(base::reflection::CClass* cls);
extern UnkConversion g_unk_71729fde50;
struct UnkCameraCallbackSet_ovb0070_ovb0070;
UnkCameraCallbackSet_ovb0070_ovb0070* CLogicCamera_CopyAssignGameLink(UnkCameraCallbackSet_ovb0070_ovb0070* self,
                                            const UnkCameraCallbackSet_ovb0070_ovb0070* other);
extern UnkConversion g_unk_71729fde70;
UnkEnumType* fn_7100231b40();
void fn_71010532c8();
extern UnkConversion g_unk_71729fde90;
UnkEnumType* fn_7100231c1c();
void fn_7101052448() __attribute__((visibility("hidden")));

void CLogicCamera_InitStatics() {
    InitHeaderIds(&g_unk_71729fdfc0.ids);
    InitId(&g_unk_71729fdfc0.field_0x18, "Controller");
    InitId(&g_unk_71729fdfc0.field_0x20, "Static");
    InitId(&g_unk_71729fdfc0.field_0x28, "Position");
    InitId(&g_unk_71729fdfc0.field_0x30, "Dir");
    InitId(&g_unk_71729fdfc0.field_0x38, "FovX");
    InitId(&g_unk_71729fdfc0.field_0x40, "MinExtraZDist");
    InitId(&g_unk_71729fdfc0.field_0x48, "MaxExtraZDist");
    InitId(&g_unk_71729fdfc0.field_0x50, "DefaultInterp");
    fn_7101050878();
    fn_710009ec00(g_unk_71729fdd08, g_unk_71729fdfc0.field_0x60);
    fn_710009ec00(g_unk_71729fdd09, const_cast<base::reflection::CType*>(fn_71009112ac()));
    fn_710009ec00(g_unk_71729fdd0a, fn_71010510b4());
    fn_710009ec00(g_unk_71729fdd0b, const_cast<base::reflection::CType*>(fn_7100910720()));
    {
        CStrId n("CGameLink<CLogicCamera>", true);
        fn_7101051230(&g_unk_71729fdd10, n, &g_unk_71727bbe68, nullptr, fn_7101051190, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729fdd10, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729fde48, &g_unk_71729fdd10);
    InitConversion(&g_unk_71729fde50, TypeOf<fn_7100009658>, &g_unk_71729fdd10,
                   reinterpret_cast<void (*)()>(&CLogicCamera_CopyAssignGameLink));
    InitConversion(&g_unk_71729fde70, fn_7100231b40, TypeOf<fn_71009112ac>, fn_71010532c8);
    InitConversion(&g_unk_71729fde90, fn_7100231c1c, &g_unk_71729fdd10, fn_7101052448);
}
