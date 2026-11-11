// behaviortree::CGoToPointAction's destructors: its vtable, its path object (0x50, destroyed by
// fn_71002ad0dc) and a string id (0x30) destroyed, then the base's destructor fn_7100393ea0 (the shape of
// fn_7100da619c and fn_7100da621c, src/game/sound/UnkMusicTrigger_Unit2538_Dtor_b44.cpp). Names and
// layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/behaviortree/CBaseAction.h"
#include "game/behaviortree/CGoToPointAction.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

// As src/game/navmesh/SNavMeshPath/fn_71002ad0dc.cpp defines it.
struct UnkObject_71002ad0dc;

struct UnkPathMember_b45a1 {
    ~UnkPathMember_b45a1() { fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(this)); }
};

struct CGoToPointActionDtorTag_b45a1;
template <>
struct base::UnkDtorTraits<CGoToPointActionDtorTag_b45a1> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a25e50); }
    static void base(char* self) { fn_7100393ea0(self); }
};

__attribute__((flatten)) void fn_7100380870(char* self) {
    base::refl_Dtor<CGoToPointActionDtorTag_b45a1, base::UnkMemberAt<UnkPathMember_b45a1, 0x50>,
                    base::UnkStrIdAt<0x30>>(self);
}

// The deleting form.
__attribute__((flatten)) void fn_71003808f0(char* self) {
    base::refl_DeletingDtor<CGoToPointActionDtorTag_b45a1, base::UnkMemberAt<UnkPathMember_b45a1, 0x50>,
                            base::UnkStrIdAt<0x30>>(self);
}
