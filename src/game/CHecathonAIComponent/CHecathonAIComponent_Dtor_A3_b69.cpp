// CHecathonAIComponent's unit (tools/tu_map.py unit 865): the component's destructor (vtable 0x7101a4f7f0):
// two refcounted pointers at +0x147e0 and +0x147d8, two callback lists (base::UnkCallbackListShort) at
// +0x14720 and +0x146d8, then the base's destructor fn_7100369380. Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CHecathonAIComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkHecathonDtor_A3_b69;

template <>
struct base::UnkDtorTraits<UnkHecathonDtor_A3_b69> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a4f7f0); }
    static void base(char* self) { fn_7100369380(self); }
};

template <>
struct base::UnkCallbackTraits<UnkHecathonDtor_A3_b69> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

template <u32 Offset>
struct UnkHecathonList_A3_b69 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkCallbackListShort<UnkHecathonDtor_A3_b69>*>(self + Offset)->~UnkCallbackListShort();
    }
};

__attribute__((flatten)) void fn_71005a96c4(char* self) {
    base::refl_Dtor<UnkHecathonDtor_A3_b69, base::UnkRefPtrAt<0x147e0>, base::UnkRefPtrAt<0x147d8>,
                    UnkHecathonList_A3_b69<0x14720>, UnkHecathonList_A3_b69<0x146d8>>(self);
}
