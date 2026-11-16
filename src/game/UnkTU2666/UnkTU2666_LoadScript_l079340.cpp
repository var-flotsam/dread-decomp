// Unit 2666 Lua binding: hands one name ("s") to the game manager (fn_7100dfeacc) as a one-entry
// string list, with fn_7101097034 as its callback. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2677.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;

struct lua_State;

void fn_7101097034(void* context);

// A load's callback: a context and a function.
struct UnkReloadCallback {
    void* context;
    void* fn;
};

void fn_7100dfeacc(UnkGameManager* manager, const UnkReloadCallback* callback, CRntVector<CRntString>* files,
                   bool flag);

s32 fn_7101079340(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        CRntVector<CRntString> files(0xf, 1);
        files.Reserve(1);
        files.EmplaceBack(CRntString(name));
        UnkGameManager* manager = g_unk_7101d47310;
        UnkReloadCallback callback;
        callback.context = nullptr;
        callback.fn = reinterpret_cast<void*>(&fn_7101097034);
        fn_7100dfeacc(manager, &callback, &files, false);
        files.Free();
    }
    return 0;
}
