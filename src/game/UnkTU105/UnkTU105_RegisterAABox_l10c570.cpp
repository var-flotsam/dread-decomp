// Unit 105: registers the reflected class base::spatial::CAABox and its two named values.
// Names and layouts are placeholders.
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

extern base::reflection::CType g_unk_7108b6d3d0;
extern char g_unk_7108b6d508[];
extern UnkNamedValue g_unk_7108b6d510;
extern UnkNamedValue g_unk_7108b6d520;
void fn_710010c2a0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710010c194();

void fn_710010c570() {
    InitClass(reinterpret_cast<UnkReflType*>(&g_unk_7108b6d3d0), "base::spatial::CAABox", fn_710010c2a0, fn_710010c194);
    fn_710009ec00(g_unk_7108b6d508, &g_unk_7108b6d3d0);
    InitNamedValue(&g_unk_7108b6d510, "Min", TypeOf<fn_7100009658>, fn_710010c424);
    InitNamedValue(&g_unk_7108b6d520, "Max", TypeOf<fn_7100009658>, fn_710010c424);
}
