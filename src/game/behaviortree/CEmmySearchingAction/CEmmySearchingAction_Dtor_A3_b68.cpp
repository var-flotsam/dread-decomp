// behaviortree::CEmmySearchingAction's destructor (tools/tu_map.py): its vtable (0x7101a47e90, as the
// constructor fn_710051d360 stores it), the embedded list owner at +0x12af8 (vtable 0x7101a47f48, its list
// base at +8 emptied as in src/game/CEmmyAIComponent/UnkListOwnerDeletingDtor_b47.cpp), the path object at
// +0x68 (fn_71002ad0dc), the linked secondary base at +0x40 (refl_ReleaseLink), then the base's destructor
// fn_7100393ea0. Names are placeholders.
#include "base/UnkDestructors.h"
#include "game/behaviortree/CBaseAction.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a47e90[];
extern void* g_unk_7101a22d48[];
extern void* g_unk_7101a47f48[];
extern void* g_unk_7101a47f88[];

// As src/game/navmesh/SNavMeshPath/fn_71002ad0dc.cpp defines it.
struct UnkObject_71002ad0dc;

struct UnkEmmySearchingDtor_A3_b68;
struct UnkEmmySearchingList_A3_b68;

template <>
struct base::UnkDtorTraits<UnkEmmySearchingDtor_A3_b68> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a47e90); }
    static void base(char* self) { fn_7100393ea0(self); }
};

template <>
struct base::UnkDtorTraits<UnkEmmySearchingList_A3_b68> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a47f88); }
};

template <>
struct base::UnkLinkTraits<UnkEmmySearchingDtor_A3_b68> {
    static char* link_vtable() { return reinterpret_cast<char*>(g_unk_7101a22d48); }
    static void unlink(char* list, char* link) { fn_710117b3f0(reinterpret_cast<UnkWeakTarget*>(list), link); }
};

struct UnkEmmySearchingListOwner_A3_b68 {
    static void Release(char* self) {
        char* owner = self + 0x12af8;
        *reinterpret_cast<char**>(owner) = reinterpret_cast<char*>(g_unk_7101a47f48) + 0x10;
        base::refl_ListDtor<UnkEmmySearchingList_A3_b68, 0x20, 0x20>(owner + 8);
    }
};

struct UnkEmmySearchingPath_A3_b68 {
    static void Release(char* self) { fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(self + 0x68)); }
};

struct UnkEmmySearchingLink_A3_b68 {
    static void Release(char* self) { base::refl_ReleaseLink<UnkEmmySearchingDtor_A3_b68, 0x40>(self); }
};

__attribute__((flatten)) void fn_7100516e20(char* self) {
    base::refl_Dtor<UnkEmmySearchingDtor_A3_b68, UnkEmmySearchingListOwner_A3_b68, UnkEmmySearchingPath_A3_b68,
                    UnkEmmySearchingLink_A3_b68>(self);
}
