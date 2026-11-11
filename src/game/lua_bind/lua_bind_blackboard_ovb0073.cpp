// lua_bind's unit (tools/tu_map.py): script bindings over the game manager's blackboards, with the
// unit's argument reader (fn_71010c0270) and result pusher (fn_71010c0b40): whether a section or a
// section's name exists (fn_7101074840, fn_7101074910), creating and removing a group of names
// (fn_71010742f0, fn_7101074410) and selecting a slot of the blackboard named "savedata",
// "checkpoint" or "current" (fn_71010750e0). Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2677.h"
#include "types.h"

using base::global::CStrId;

struct UnkBlackboardGroups_b0026;
struct UnkNameList_b0026;
struct UnkSectionGroup_b0026;
UnkSectionGroup_b0026* fn_7101048820(UnkBlackboardGroups_b0026* self, const CStrId& key, UnkNameList_b0026* list);
class UnkKeyedSlots_b48a3;
void fn_7101048280(UnkKeyedSlots_b48a3* self, const u64* key);
struct UnkStrIdHandle;
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);
bool fn_71010474f4(CBlackboard* self, const CStrId& section, const CStrId& name);
void fn_7101048940(void* groups, const CStrId& key);
extern void* g_unk_7101a1a070[];

// The game manager's group table, after its blackboard.
static inline void* Groups_ovb0073(UnkGameManager* manager) {
    return reinterpret_cast<u8*>(manager) + 0xbf8;
}

// Removes the group named by the argument.
int fn_7101074410(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    const char* name;
    if (fn_71010c0270(L, "s", &name))
        fn_7101048940(Groups_ovb0073(manager), CStrId(name, true));
    return 0;
}

// Whether the game blackboard has the section named by the argument.
int fn_7101074840(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    const char* name;
    bool has = false;
    if (fn_71010c0270(L, "s", &name))
        has = fn_7101047208(&manager->field_0x968, CStrId(name, true));
    return fn_71010c0b40(L, "b", has);
}

// A string id handle (fn_7100ae986c) on the stack, its id released when it goes.
struct UnkLuaIdHandle_ovb0073 {
    union {
        CStrId id;
    };
    void* field_0x8;

    explicit UnkLuaIdHandle_ovb0073(const CStrId& from) { fn_7100ae986c(reinterpret_cast<UnkStrIdHandle*>(this), from); }
    ~UnkLuaIdHandle_ovb0073() { id.~CStrId(); }
};

// Whether the game blackboard's section (first argument) has the name (second argument).
int fn_7101074910(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    const char* section;
    const char* name;
    bool has = false;
    if (fn_71010c0270(L, "ss", &section, &name))
        has = fn_71010474f4(&manager->field_0x968, CStrId(section, true),
                            UnkLuaIdHandle_ovb0073(CStrId(name, true)).id);
    return fn_71010c0b40(L, "b", has);
}

// The blackboard named by the first argument ("savedata", "checkpoint" or "current"): selects the
// slot named by the second.
int fn_71010750e0(lua_State* L) {
    const char* which;
    const char* name;
    if (fn_71010c0270(L, "ss", &which, &name)) {
        UnkKeyedSlots_b48a3* slots;
        if (__builtin_strcmp(which, "savedata") == 0)
            slots = reinterpret_cast<UnkKeyedSlots_b48a3*>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0xbf8);
        else if (__builtin_strcmp(which, "checkpoint") == 0)
            slots = reinterpret_cast<UnkKeyedSlots_b48a3*>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0xab0);
        else if (__builtin_strcmp(which, "current") == 0)
            slots = reinterpret_cast<UnkKeyedSlots_b48a3*>(&g_unk_7101d47310->field_0x968);
        else
            slots = nullptr;
        CStrId id(name, true);
        fn_7101048280(slots, reinterpret_cast<const u64*>(&id));
    }
    return 0;
}

// A list of strings (nodes linked through +0x30) filled by the argument reader's "S".
struct UnkLuaNameList_ovb0073 {
    void** vtable;
    u32 count;  // 0x8
    u8* head;  // 0x10
    u8* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 field_0x28;

    UnkLuaNameList_ovb0073() : vtable(g_unk_7101a1a070 + 2), count(0), head(nullptr), tail(nullptr) {
        allocator = fn_7100083990();
        field_0x28 = 0x13;
    }
    ~UnkLuaNameList_ovb0073() {
        vtable = g_unk_7101a1a070 + 2;
        for (u8* node = head; node;) {
            u8* next = *reinterpret_cast<u8**>(node + 0x30);
            fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(node));
            allocator->vf_0x48(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

// Creates the group named by the first argument with the names that follow.
int fn_71010742f0(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    const char* name;
    UnkLuaNameList_ovb0073 names;
    if (fn_71010c0270(L, "sS", &name, &names))
        fn_7101048820(reinterpret_cast<UnkBlackboardGroups_b0026*>(Groups_ovb0073(manager)), CStrId(name, true),
                      reinterpret_cast<UnkNameList_b0026*>(&names));
    return 0;
}
