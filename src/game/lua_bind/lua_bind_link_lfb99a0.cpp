// lua_bind's unit (tools/tu_map.py): a script binding that finds three entities by name and links the
// second and third to the first one's door-like logic object (fn_710055b104, fn_7100acca50). Names and
// layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::global::CStrId;

struct UnkLuaDoorTarget;
void fn_7100acca50(UnkLuaDoorTarget* target, void* first, void* second);

struct UnkLuaStrData {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

static inline void* FindObject(UnkGameManager* manager, const CStrId& id, u32 kind_value) {
    u64 hash;
    alignas(8) u32 kind;
    u32 k = kind_value;
    u64 h = reinterpret_cast<UnkLuaStrData*>(id.data)->hash;
    hash = h;
    kind = k;
    return fn_7100deba8c(manager, &hash, &kind);
}

int fn_7100fb99a0(lua_State* L) {
    const char* name;
    const char* first_name;
    const char* second_name;
    if (fn_71010c0270(L, "sss", &name, &first_name, &second_name) && *name && *first_name && *second_name) {
        void* object;
        {
            UnkGameManager* manager = g_unk_7101d47310;
            CStrId id(name, true);
            object = FindObject(manager, id, g_unk_71727bebb8);
        }
        void* first;
        {
            UnkGameManager* manager = g_unk_7101d47310;
            CStrId id(first_name, true);
            first = FindObject(manager, id, g_unk_71727bebb8);
        }
        void* second;
        {
            UnkGameManager* manager = g_unk_7101d47310;
            CStrId id(second_name, true);
            second = FindObject(manager, id, g_unk_71727bebb8);
        }
        if (object && first && second) {
            if (UnkLuaDoorTarget* target = fn_710055b104(object))
                fn_7100acca50(target, first, second);
        }
    }
    return 0;
}
