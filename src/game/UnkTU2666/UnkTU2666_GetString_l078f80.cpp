// Unit 2666 Lua binding: reads a string argument, looks it up through the game manager (fn_7100dfb1b0)
// and pushes the resulting string (or an empty one). Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkNamedRegistry.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2677.h"
#include "types.h"

using base::global::CRntString;

struct lua_State;

CRntString fn_7100dfb1b0(UnkGameManager* manager, const base::UnkNamedEntry* key);

s32 fn_7101078f80(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    union Slot {
        Slot() {}
        ~Slot() {}
        CRntString string;
    } result;
    base::UnkNamedEntry key;
    fn_710007e704(&key, name);
    new (&result.string) CRntString(fn_7100dfb1b0(manager, &key));
    fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    const char* text = result.string.data ? result.string.data : g_unk_710162e2ed;
    s32 ret = fn_71010c0b40(L, "s", text);
    fn_710007ecd4(&result.string);
    return ret;
}
