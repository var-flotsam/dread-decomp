// behaviortree::CStoreStringInBlcAction's destructors: its vtable, its string (0x38) and string id
// (0x30) destroyed, then the base's destructor fn_7100393ea0 (the shape of fn_7100da619c and
// fn_7100da621c, src/game/sound/UnkMusicTrigger_Unit2538_Dtor_b44.cpp). Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "game/behaviortree/CBaseAction.h"
#include "game/behaviortree/CStoreStringInBlcAction.h"
#include "types.h"

struct CStoreStringInBlcActionDtorTag_b45a1;
template <>
struct base::UnkDtorTraits<CStoreStringInBlcActionDtorTag_b45a1> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a25640); }
    static void base(char* self) { fn_7100393ea0(self); }
};

__attribute__((flatten)) void fn_7100377074(char* self) {
    base::refl_Dtor<CStoreStringInBlcActionDtorTag_b45a1, base::UnkMemberAt<base::global::CRntString, 0x38>,
                    base::UnkStrIdAt<0x30>>(self);
}

// The deleting form.
__attribute__((flatten)) void fn_71003770f4(char* self) {
    base::refl_DeletingDtor<CStoreStringInBlcActionDtorTag_b45a1,
                            base::UnkMemberAt<base::global::CRntString, 0x38>, base::UnkStrIdAt<0x30>>(self);
}
