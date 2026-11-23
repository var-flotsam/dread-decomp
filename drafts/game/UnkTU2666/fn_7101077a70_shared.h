#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "types.h"
#include "base/UnkVtableList.h"

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

struct lua_State;
s32 fn_71010c0270(lua_State* L, const char* fmt, ...);
int fn_71010c0b40(lua_State* L, const char* fmt, ...);
extern u32 g_unk_71727bebb8;

// The object the game manager knows under a name (the name's CRC64 and the kind g_unk_71727bebb8).
static inline void* FindByName(UnkGameManager* manager, const char* name) {
    u32 kind_value = g_unk_71727bebb8;
    u64 hash;
    alignas(8) u32 kind;
    hash = StringCrc64(name, __builtin_strlen(name));
    kind = kind_value;
    return fn_7100deba8c(manager, &hash, &kind);
}

// A string id's pool hash (the u64 at +0x30 of its entry).
struct UnkStrIdHash_b0164 {
    u8 pad_0x0[0x30];
    u64 hash;
};
struct UnkWeakTarget;
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);

// A handle made from a string id (fn_7100ae986c).
struct UnkStrIdHandle_b0164 {
    CStrId id;
    void* field_0x8;
    UnkStrIdHandle_b0164(const CStrId& id);
};

// A temporary list of objects (vtable g_unk_7101a32680, nodes from the default allocator with flags 0x13).
extern void* g_unk_7101a32680[];  // vtable (reached through the GOT)

struct UnkList_b0164Node {
    void* value;
    UnkList_b0164Node* next;
};

struct UnkList_b0164 : base::UnkVtableList<UnkList_b0164Node> {
    using Node = UnkList_b0164Node;

    UnkList_b0164() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = GetDefaultAllocator();
        flags = 0x13;
    }
    ~UnkList_b0164() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        Node* n = head;
        if (!n)
            return;
        while (n) {
            Node* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
    }
};
