// Unit 2668 (as src/game/UnkTU2668/UnkTU2668_Lua_ovb0211.cpp): runs a script file. The file is read and
// loaded as a chunk named after its path, with a scratch array pushed for the run; optionally the named
// global table is created first, and its "main" function called after. Names and layouts are placeholders.
#include <string.h>

#include "../../../lib/lua/lua.h"
#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkStrPair.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntString;

// An array of 8-byte elements on the scratch stack (as UnkTU2668_Lua_ovb0211.cpp defines it).
struct __attribute__((packed, aligned(4))) UnkLuaScratch {
    u64* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u8 pad_0x15[3];
    u32 flags;
    u32 pad_0x1c;
    base::UnkAllocator* allocator;

    // Frees the array (the elements need no destruction).
    void Free() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }

    void PushBack(u64 value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        u64* p;
        if (old_capacity < new_capacity) {
            p = static_cast<u64*>(allocator->vf_0x40((u64)new_capacity * sizeof(u64), 8, flags));
            if (data) {
                memcpy(p, data, (u64)count * sizeof(u64));
                allocator->vf_0x48(data);
            }
            n = count;
            data = p;
            capacity = new_capacity;
        } else {
            p = data;
        }
        p[n] = value;
        ++count;
    }
};
static_assert(sizeof(UnkLuaScratch) == 0x28, "UnkLuaScratch layout");

struct UnkLuaScratchStack {
    u32 count;
    UnkLuaScratch entries[1];  // the real number is unknown
};

struct UnkFileReader;
int fn_71010a58e0(lua_State* L, const char* buff, size_t size, const char* name);

// Pushes a fresh scratch array of 0x190 elements for a script run.
static inline void PushScratch() {
    base::UnkAllocator* allocator = GetDefaultAllocator();
    u32 flags = 6;
    u64* data = static_cast<u64*>(allocator->vf_0x40(0x190 * sizeof(u64), 8, flags));
    u32 top = g_unk_7172a025a0.count;
    UnkLuaScratch& scratch = g_unk_7172a025a0.entries[top];
    scratch.data = data;
    scratch.count = 0;
    scratch.capacity = 0x190;
    scratch.step = 10;
    scratch.field_0x14 = true;
    scratch.flags = flags;
    scratch.allocator = allocator;
    g_unk_7172a025a0.count = top + 1;
}

// A string with inline storage, its CRntString first (src/base/UnkLuaDoString_b50.cpp).
struct UnkInlineString_710109d390;

// Runs the script file named by `str`; true when it loaded. With `create`, the global table `arg` names
// is created first when missing, and with `run_main` too its "main" function is called after.
bool fn_710109d0e0(UnkInlineString_710109d390* str, void* arg, bool create, bool run_main) {
    const CRntString* path = reinterpret_cast<const CRntString*>(str);
    const CRntString* table = static_cast<const CRntString*>(arg);
    lua_State* L = g_unk_7172a01d18;
    UnkHashStream_0x38 file;
    fn_710007be8c(&file, 0x16, nullptr);
    bool opened;
    {
        UnkStrPair_0x10 s;
        fn_710008d4c0(&s, reinterpret_cast<const UnkEntry_0x30*>(path));
        {
            CRntString mode("rb");
            opened = fn_710007c1ac(reinterpret_cast<UnkFileReader*>(&file), &s, &mode);
        }
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    bool loaded = false;
    if (opened) {
        if (create) {
            lua_getfield(L, LUA_GLOBALSINDEX, table->data);
            int type = lua_type(L, -1);
            lua_remove(L, -1);
            if (type == LUA_TNIL) {
                lua_createtable(L, 0, 0);
                lua_setfield(L, LUA_GLOBALSINDEX, table->data);
            }
        }

        CRntString name(*path);
        PushScratch();
        int status = fn_71010a58e0(L, reinterpret_cast<const char*>(file.data), fn_710007c1a4(&file), name.data);
        fn_710109c9e0();
        fn_710007c144(&file);
        loaded = status == 0;
        if (status == 0 && create && run_main) {
            lua_getfield(L, LUA_GLOBALSINDEX, table->data);
            lua_pushstring(L, "main");
            lua_gettable(L, -2);
            lua_remove(L, -2);
            if (lua_type(L, -1) == LUA_TFUNCTION) {
                PushScratch();
                int error = lua_pcall(L, 0, 0, 0);
                fn_710109c9e0();
                if (error)
                    lua_pop(g_unk_7172a01d18, 1);
            } else {
                lua_pop(L, 1);
            }
        }
    }
    fn_710007c0e4(&file);
    return loaded;
}
