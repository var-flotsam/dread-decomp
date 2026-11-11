// Unit 2666 Lua bindings: fn_7101095cf0 reads a string id from argument 1, makes an instance of the
// type registered under it, reads the optional argument 2 into it and pushes it; fn_7101079e20 reads
// a playthrough path, requests that resource, keeps it when it is a CPlaythrough and pushes it as a
// typed value. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/UnkTU25.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU426.h"
#include "game/UnkTU83.h"
#include "game/lua_lib.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkTypedRef;

struct lua_State;
struct UnkFormatArgs;

struct UnkVersionRegistry;
struct UnkVersionEntry;
extern UnkVersionRegistry* g_unk_7101d4c8d0;
base::reflection::UnkReflTypedPtr fn_710009d6d0(const base::reflection::CType* type, u32 flags);
int lua_type(lua_State* L, int idx);

struct UnkLuaReadScratch_b49a6 {
    u64 words[6];
};
bool fn_7100fc22a0(lua_State* L, s32 index, UnkTypedRef* value, u32 mode, UnkLuaReadScratch_b49a6* scratch);
s32 fn_7100fbde20(void* state, UnkFormatArgs* value, s32 arg0, s32 arg1);

// A string with 0x80 bytes of inline storage (as in fn_7100ae9a10).
struct UnkInlineString {
    u8 str[0x30];
    void** vtable;
    u8 buffer[0x80];
    u64 field_0xb8;
    u32 field_0xc0;
};
extern void* g_unk_7101a1b750[];

struct UnkPathIds;

struct UnkPathIds {
    union {
        CStrId id;
    };
    union {
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };

    explicit UnkPathIds(const UnkInlineString& source) { fn_710001f074(this, &source); }
    ~UnkPathIds() {
        path.~UnkStrIdMemberOf();
        id.~CStrId();
    }
};

struct UnkResourceHandle {
    UnkCountedResource* ptr;

    ~UnkResourceHandle() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};
struct UnkResourceManager;
extern UnkResourceManager* g_unk_71726ae8a8;

struct UnkEnumType;
UnkEnumType* fn_71002ea9dc();

struct UnkRcBinding_b0163 {
    UnkEnumType* type;
    UnkCountedResource** ref;
};

s32 fn_7101095cf0(lua_State* L) {
    union Slot {
        Slot() {}
        ~Slot() { id.~CStrId(); }
        CStrId id;
    } slot;
    CStrId& id = slot.id;
    id.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id.data = &g_unk_7101d47108->empty;
    bool ok = false;
    {
        UnkTypedRef idRef = {fn_7100009658(), &id};
        UnkLuaReadScratch_b49a6 scratch = {};
        fn_7100fc22a0(L, 1, &idRef, 2, &scratch);
    }
    if (id.data != &g_unk_7101d47108->empty) {
        const CType* type;
        {
                UnkVersionRegistry* registry = g_unk_7101d4c8d0;
                u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<u8*>(id.data) + 0x30);
                type = reinterpret_cast<const CType*>(fn_71000a5118(registry, &hash));
        }
        if (type) {
            UnkReflTypedPtr made = fn_710009d6d0(type, 5);
            if (made.type && made.ptr) {
                void* object = made.ptr;
                UnkTypedRef ref = {reinterpret_cast<const CType*>(fn_710009dcd8(made.type, object)), object};
                if (lua_type(L, 2) != 0) {
                    UnkLuaReadScratch_b49a6 scratch = {};
                    fn_7100fc22a0(L, 2, &ref, 2, &scratch);
                }
                ok = fn_7100fbde20(L, reinterpret_cast<UnkFormatArgs*>(&ref), 0, 0) != 0;
            }
        }
    }
    return ok;
}

s32 fn_7101079e20(lua_State* L) {
    UnkCountedResource* held = nullptr;
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        const char* text = name;
        UnkInlineString path;
        auto* str = reinterpret_cast<base::global::CRntString*>(&path);
        fn_710007e6e4(str, reinterpret_cast<base::UnkAllocator*>(&path.vtable));
        path.vtable = &g_unk_7101a1b750[2];
        path.field_0xb8 = 0;
        path.field_0xc0 = 0;
        fn_710007e77c(str, text);
        {
        union Slot {
            Slot() {}
            ~Slot() {}
            UnkResourceHandle handle;
        } slot;
        UnkResourceManager* manager = g_unk_71726ae8a8;
        UnkPathIds ids(path);
        new (&slot.handle) UnkResourceHandle(fn_71002f7048(manager, ids));
        UnkCountedResource* picked = slot.handle.ptr;
        if (picked) {
            if (!fn_710009d50c(reinterpret_cast<UnkReflObject*>(picked)->vf_0x0(), fn_71002e970c()))
                picked = nullptr;
        }
        if (held != picked) {
            if (picked)
                ++picked->ref_count;
            if (held && --held->ref_count == 0)
                fn_710001737c(held);
            held = picked;
        }
        slot.handle.~UnkResourceHandle();
        }
        fn_710007ed40(str);
        fn_710007ecd4(str);
    }
    UnkRcBinding_b0163 binding = {fn_71002ea9dc(), &held};
    s32 result = fn_7100fbd320(L, &binding, 0, 2);
    if (held && --held->ref_count == 0)
        fn_710001737c(held);
    return result;
}
