// lua_bind's unit (tools/tu_map.py): a script binding that finds an entity's door-like logic object
// (fn_710055b104), turns a list of names into pairs of string ids and hands them to the object the
// second argument names (fn_71006fa84c). Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/file/nx/CDeviceNX.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkLuaDoorObj {
    virtual void vf_0x0();
    u8 pad_0x8[0xc8];
};
static_assert(sizeof(UnkLuaDoorObj) == 0xd0, "layout");

struct UnkLuaDoorTarget {
    u8 pad_0x0[0x30];
    u8 (*items)[0x80];  // 0x30
    u32 item_count;     // 0x38
    u8 pad_0x3c[0x16a8 - 0x3c];
    UnkLuaDoorObj* objects;  // 0x16a8
    CStrId* ids;             // 0x16b0
    u32 count;               // 0x16b8
};
static_assert(__builtin_offsetof(UnkLuaDoorTarget, count) == 0x16b8, "layout");

UnkLuaDoorTarget* fn_710055b104(void* object);

struct UnkUnit1115List;
void fn_71006fa84c(UnkUnit1115List* self, void* arg);

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

// A list of strings (nodes linked through +0x30) filled by the argument reader's "S".
struct UnkLuaNameList_lfbaeb0 {
    void** vtable;
    u32 count;  // 0x8
    u8* head;  // 0x10
    u8* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 field_0x28;

    UnkLuaNameList_lfbaeb0() : vtable(g_unk_7101a1a070 + 2), count(0), head(nullptr), tail(nullptr) {
        allocator = fn_7100083990();
        field_0x28 = 0x13;
    }
    ~UnkLuaNameList_lfbaeb0() {
        vtable = g_unk_7101a1a070 + 2;
        for (u8* node = head; node;) {
            u8* next = *reinterpret_cast<u8**>(node + 0x30);
            fn_710007ecd4(reinterpret_cast<CRntString*>(node));
            allocator->vf_0x48(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

// A string id as the pairs hold it: built from a list node's string (fn_7100000850), copied with a
// reference, moved by taking the string and released as CStrId is.
struct UnkPairId_lfbaeb0 {
    UnkStringData* data;

    explicit UnkPairId_lfbaeb0(const CRntString& str) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, true);
    }
    UnkPairId_lfbaeb0(const UnkPairId_lfbaeb0& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkPairId_lfbaeb0(UnkPairId_lfbaeb0&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkPairId_lfbaeb0() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkIdPair_lfbaeb0 {
    UnkPairId_lfbaeb0 first;
    UnkPairId_lfbaeb0 second;

    UnkIdPair_lfbaeb0(const UnkPairId_lfbaeb0& first, const UnkPairId_lfbaeb0& second)
        : first(first), second(second) {}
    UnkIdPair_lfbaeb0(const UnkIdPair_lfbaeb0& other) = default;
    UnkIdPair_lfbaeb0(UnkIdPair_lfbaeb0&& other) = default;
};

int fn_7100fbaeb0(lua_State* L) {
    const char* name;
    const char* object_name;
    UnkLuaNameList_lfbaeb0 names;
    if (!fn_71010c0270(L, "ssS", &name, &object_name, &names) || !*name || !*object_name || names.count % 2 != 0)
        return 0;
    void* object;
    {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId id(name, true);
        object = FindObject(manager, id, g_unk_71727bebb8);
    }
    if (!object)
        return 0;
    UnkLuaDoorTarget* target = fn_710055b104(object);
    if (!target)
        return 0;
    CRntVector<UnkIdPair_lfbaeb0> pairs(0xf);
    for (u8* node = names.head; node;) {
        UnkPairId_lfbaeb0 first(*reinterpret_cast<CRntString*>(node));
        node = *reinterpret_cast<u8**>(node + 0x30);
        UnkPairId_lfbaeb0 second(*reinterpret_cast<CRntString*>(node));
        node = *reinterpret_cast<u8**>(node + 0x30);
        pairs.PushBackCopy(UnkIdPair_lfbaeb0(first, second));
    }
    {
        CStrId id(object_name, true);
        u64 count = target->count;
        u64 index = count;
        for (u64 i = 0; i != count; ++i) {
            if (target->ids[i] == id) {
                index = i;
                break;
            }
        }
        fn_71006fa84c(reinterpret_cast<UnkUnit1115List*>(&target->objects[index]), &pairs);
    }
    pairs.Free();
    return 0;
}
