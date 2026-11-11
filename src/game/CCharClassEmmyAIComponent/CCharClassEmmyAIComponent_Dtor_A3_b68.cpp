// CCharClassEmmyAIComponent's destructor (tools/tu_map.py; the shape of base::refl_Dtor in
// include/base/UnkDestructors.h, as the clone fn_7100910fcc in src/clones/clones_03.cpp): the vtable
// (0x7101a480a8), the string ids at +0x618 and +0x608, the member at +0x510 (fn_7100504bcc), then the base destructor fn_7100416e80. Names are
// placeholders.
#include "base/UnkDestructors.h"
#include "game/CCharClassBehaviorTreeAIComponent.h"
#include "game/CCharClassEmmyAIComponent.h"
#include "types.h"

// Defined in src/lifted/ with this signature.
char* fn_7100504bcc(char* p0);
// As src/clones/clones_01.cpp declares it.
struct UnkStruct_7100416e80;

using base::UnkStrIdMember;

void fn_710051ebcc(char* self) {
    *reinterpret_cast<char**>(self) = reinterpret_cast<char*>(g_unk_7101a480a8) + 0x10;
    reinterpret_cast<UnkStrIdMember*>(self + 0x618)->~UnkStrIdMember();
    reinterpret_cast<UnkStrIdMember*>(self + 0x608)->~UnkStrIdMember();
    fn_7100504bcc(self + 0x510);
    fn_7100416e80(reinterpret_cast<UnkStruct_7100416e80*>(self));
}
