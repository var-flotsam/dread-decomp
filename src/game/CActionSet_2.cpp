// CActionSet unit: the "Version" attribute of a class, 0x30000 plus the 16-bit number in
// g_unk_710164b950. Names are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"

using base::reflection::CClass;

extern u16 g_unk_710164b950;

void fn_7100b8bb94(CClass* cls) {
    fn_710009dd6c(cls)->Add<u32>(fn_7100005070(), g_unk_710164b950 | 0x30000u);
}
