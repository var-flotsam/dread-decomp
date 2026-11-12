// Unit 2620 (tools/tu_map.py): the copy in this unit of the reflected-object reader's name step
// (as fn_710104b540, src/game/CBlackboard/CBlackboard_Sections_ovb0026.cpp): reads the object's name id, and, for a
// pointer, the id of the type it points to (the registered type replaces the object's own when there is one), then
// reads the object as that type. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "game/UnkTU2620.h"
#include "game/UnkTU83.h"
#include "types.h"

struct UnkVersionRegistry;
struct UnkVersionEntry;

// A typed pointer to a value the reader fills (a CName id here).
struct UnkReadArg_b0249 {
    const base::reflection::CType* type;
    u64* value;
};
void fn_7100fc29e8(void* reader, u32 flags, UnkReadArg_b0249* arg, const base::global::CStrId* name);
const base::reflection::CType* fn_710009c640(base::reflection::UnkReflObject* obj);
using base::reflection::CType;
using base::UnkStrIdMember;

struct UnkNameReader_b0249 {
    u8 pad_0x0[0x30];
    void* reader;
};

s32 fn_7100fc2600(UnkNameReader_b0249* self, base::reflection::UnkReflObject* obj) {
    UnkReadArg_b0249 arg;
    u64 name = (u64)-1;
    void* reader = self->reader;
    arg.type = fn_710000fd7c();
    arg.value = &name;
    fn_7100fc29e8(reader, 0, &arg, &g_unk_7101d470e0);
    const base::reflection::CType* type = fn_710009c640(obj);
    if (fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->vf_0x0()),
                      reinterpret_cast<const base::reflection::CType*>(fn_710009bedc()))) {
        u64 id = (u64)-1;
        reader = self->reader;
        arg.type = fn_710000fd7c();
        arg.value = &id;
        fn_7100fc29e8(reader, 1, &arg, &g_unk_7101d470e0);
        if (id != (u64)-1)
            type = reinterpret_cast<const base::reflection::CType*>(fn_71000a5118(g_unk_7101d4c8d0, &id));
    }
    return fn_7100fc2700(reinterpret_cast<UnkParseContext*>(self), reinterpret_cast<const base::reflection::CType*>(obj),
                         const_cast<base::reflection::CType*>(type));
}

// The script arguments being read: the Lua state, the stack index of the first one and how many there are.
struct UnkLuaArgs_b0249;
void fn_7100fc17d0(UnkLuaArgs_b0249* self, lua_State* L, u32 first);

struct UnkLuaArgs_b0249 {
    lua_State* L;
    s32 first;
    u32 pad_0xc;
    s32 count;
    // Built out of line (fn_7100fc17d0), so held in unions: no member constructor runs here.
    union {
        base::UnkStrIdMember field_0x18;
    };
    u64 pad_0x20;
    union {
        base::UnkVector<base::UnkStrIdMember> names;  // 0x28
    };

    UnkLuaArgs_b0249(lua_State* L_, u32 first_) { fn_7100fc17d0(this, L_, first_); }
    __attribute__((always_inline)) ~UnkLuaArgs_b0249() {
        names.Free();
        field_0x18.~UnkStrIdMember();
    }

    // True when the argument is missing: the last one given (or the first when none are).
    bool Missing() {
        return lua_type(L, count >= 1 ? lua_gettop(L) : first) == LUA_TNONE;
    }
};

// Parses the script argument with the target set up for it through the callbacks; whether it was given and parsed.
bool fn_7100fc24d0(UnkLuaArgs_b0249* self, UnkReadTarget* target, const UnkParseCallbacks* callbacks) {
    if (self->Missing())
        return false;
    if (self->Missing())
        return false;
    UnkParseContext ctx(*callbacks, reinterpret_cast<UnkFileReader*>(self), target);
    UnkReadEntry* end = target->entries + target->count;
    return fn_7100fc2600(reinterpret_cast<UnkNameReader_b0249*>(&ctx),
                         target->count ? reinterpret_cast<base::reflection::UnkReflObject*>(end[-1].field_0x0->field_0x10)
                                       : nullptr) != 0;
}

struct UnkTypedObject_b0249 {
    const CType* type;
    void* object;
};
struct UnkTypedObject;
using base::reflection::UnkReflValue;

// Reads the script argument at `first` as `object` through the callbacks; whether it was given and parsed.
bool fn_7100fc22a0(lua_State* L, u32 first, const UnkTypedObject_b0249* object, u32 unused, const UnkParseCallbacks* callbacks) {
    UnkLuaArgs_b0249 args(L, first);
    bool ok = false;
    if (!args.Missing()) {
        base::reflection::UnkReflValue value;
        fn_71000a1828(&value, reinterpret_cast<const UnkTypedObject*>(object), 1, 2);
        UnkReadTarget target;
        fn_71000f0568(&target, &value, 5);
        ok = fn_7100fc24d0(&args, &target, callbacks);
        for (u64 i = 0; i < target.count; ++i) {
            UnkReadEntry& e = target.entries[i];
            if (e.ops) {
                e.ops->destroy(e.storage);
                e.ops = nullptr;
            }
        }
        fn_71000a1c60(&value);
    }
    return ok;
}
