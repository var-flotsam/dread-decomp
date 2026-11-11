// CCharClassInfesterAIComponent (tools/tu_map.py unit 891, the name from reflection): the
// destructor and the deleting destructor. The members at +0x610 and +0x520 are destroyed by
// out-of-line functions (fn_71005c4c5c, fn_71005c5048), then the string id at +0x510 is released
// and the base destructor (fn_7100416e80) runs. Member types are placeholders.
#include "base/UnkDestructors.h"
#include "game/CCharClassBehaviorTreeAIComponent.h"

struct UnkTag_71005ca278;
extern char g_unk_7101a51a08[];  // the component's vtable

struct UnkStruct_7100416e80;
void fn_71005c4c5c(char*);
void fn_71005c5048(char*);

template <>
struct base::UnkDtorTraits<UnkTag_71005ca278> {
    static char* vtable() { return g_unk_7101a51a08; }
    static void base(char* p) { fn_7100416e80(reinterpret_cast<UnkStruct_7100416e80*>(p)); }
};

template <void (*Destroy)(char*), u32 Offset>
struct UnkCalledAt_71005ca278 {
    static void Release(char* self) { Destroy(self + Offset); }
};

using UnkInfesterMembers_b44a2 =
    base::UnkMembers<UnkCalledAt_71005ca278<fn_71005c4c5c, 0x610>, UnkCalledAt_71005ca278<fn_71005c5048, 0x520>,
                     base::UnkStrIdAt<0x510>>;

void fn_71005ca278(char* self) { base::refl_Dtor<UnkTag_71005ca278, UnkInfesterMembers_b44a2>(self); }
void fn_71005ca304(char* self) { base::refl_DeletingDtor<UnkTag_71005ca278, UnkInfesterMembers_b44a2>(self); }
