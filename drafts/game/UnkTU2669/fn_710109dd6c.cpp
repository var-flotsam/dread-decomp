// Unit 2669 of tools/tu_map.py: loads a script once. The map (the script manager's chained map)
// is keyed by the lower-cased name: a script already there gets one more reference (1); else it
// is added and run (fn_710109d390): 2 when that worked, else it is dropped again (fn_710109d890) and
// the result is 0. Names and layouts are placeholders.
#include <new>

#include "../../../src/game_aca57c/UnkBufferString.h"
#include "base/UnkChainedMap.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2669.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::UnkNamedEntry;
using base::global::CRntString;

extern void* g_unk_7101a1b750[];

// The key: a string with 0x80 bytes of its own storage.
using UnkScriptKey_l09dd6c = UnkBufferString<0x80, g_unk_7101a1b750>;

bool fn_710109d390(void* name, void* arg, bool a, bool b);

struct UnkPooledChainedMap {
    UnkChainedMap map;
    base::UnkBlockPool pool;  // 0x48
};
static_assert(__builtin_offsetof(UnkPooledChainedMap, pool) == 0x48, "UnkPooledChainedMap layout");

// Sizes the buckets and the node pool (base::refl_HashMapInit, a generated clone in src/clones/).
void fn_710109e290(UnkPooledChainedMap* self, u32 buckets, u32 nodes);

// A loaded script: where it came from, its name and how many hold it.
struct UnkScript_l09dd6c {
    UnkNamedEntry source;
    UnkNamedEntry name;  // 0x30
    u32 refs;            // 0x60
    u32 field_0x64;
};
static_assert(sizeof(UnkScript_l09dd6c) == 0x68, "layout");

struct UnkScriptNode_l09dd6c {
    UnkNamedEntry key;
    UnkScript_l09dd6c* script;    // 0x30
    UnkScriptNode_l09dd6c* next;  // 0x38
    UnkScriptNode_l09dd6c* prev;  // 0x40
    UnkScriptNode_l09dd6c* chain;  // 0x48: the next node in the bucket

    // Nodes come from the map's allocator, which may give none.
    static void* operator new(decltype(sizeof(0)) size, base::UnkAllocator* allocator, u32 flags) noexcept {
        return allocator->vf_0x40(size, 8, flags);
    }

    UnkScriptNode_l09dd6c(const CRntString* name, UnkScript_l09dd6c* value) {
        CRntString_CopyConstruct(&key, reinterpret_cast<const UnkNamedEntry*>(name));
        script = value;
        next = nullptr;
        prev = nullptr;
        chain = nullptr;
    }
};
static_assert(sizeof(UnkScriptNode_l09dd6c) == 0x50, "layout");

// The script manager, made on first use (fn_710109d810, inlined).
static inline UnkScriptManager_ovb0085* ScriptManager() {
    UnkScriptManager_ovb0085* manager = g_unk_71727bed90;
    if (!manager) {
        UnkPooledChainedMap* self = static_cast<UnkPooledChainedMap*>(fn_71000839a0(0x88, 0x3f));
        new (&self->map) UnkChainedMap();
        self->pool.Init();
        self->pool.field_0x38 = 0;
        self->pool.field_0x3c = false;
        fn_710109e290(self, 0xb, 0x100);
        manager = reinterpret_cast<UnkScriptManager_ovb0085*>(self);
        g_unk_71727bed90 = manager;
    }
    return manager;
}

// Copies `src` lower-cased (ASCII) into `dst`.
static inline void ToLower(char* dst, const char* src) {
    dst[0] = '\0';
    u32 length = __builtin_strlen(src);
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            dst[i] = (u8)(c - 'A') < 26 ? c + ('a' - 'A') : c;
        }
        dst[length] = '\0';
    }
}

// The key's bucket, or null before the map has buckets.
inline UnkScriptNode_l09dd6c** Bucket(UnkChainedMap* map, const CRntString* key) {
    if (!map->buckets.count)
        return nullptr;
    u32 hash = (u32)(key->key >> 32) ^ (u32)key->key;
    return reinterpret_cast<UnkScriptNode_l09dd6c**>(&map->buckets.data[hash % map->buckets.capacity]);
}

inline UnkScriptNode_l09dd6c* Find(UnkChainedMap* map, const CRntString* key) {
    UnkScriptNode_l09dd6c** bucket = Bucket(map, key);
    if (!bucket)
        return nullptr;
    for (UnkScriptNode_l09dd6c* node = *bucket; node; node = node->chain) {
        if (fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(node),
                          reinterpret_cast<const UnkCompared_0x30*>(key)))
            return node;
    }
    return nullptr;
}

// Adds a node at the front of the key's bucket and at the end of the map's list.
inline void Insert(UnkChainedMap* map, const CRntString* key, UnkScript_l09dd6c* script) {
    UnkScriptNode_l09dd6c** bucket = Bucket(map, key);
    UnkScriptNode_l09dd6c* chain = *bucket;
    UnkScriptNode_l09dd6c* node = new (map->allocator, map->flags) UnkScriptNode_l09dd6c(key, script);
    node->chain = chain;
    *bucket = node;
    if (!map->tail) {
        map->tail = reinterpret_cast<UnkMapNode*>(node);
    } else if (map->head) {
        node->prev = reinterpret_cast<UnkScriptNode_l09dd6c*>(map->head);
        reinterpret_cast<UnkScriptNode_l09dd6c*>(map->head)->next = node;
    }
    map->head = reinterpret_cast<UnkMapNode*>(node);
    ++map->count;
}

u32 fn_710109dd6c(void* self, void* source, UnkUnit2669Str* name, bool a, bool b) {
    UnkChainedMap* map = static_cast<UnkChainedMap*>(self);
    char buffer[0x80];
    ToLower(buffer, reinterpret_cast<const CRntString*>(name)->data);
    UnkScriptKey_l09dd6c key(buffer);

    u32 result;
    if (UnkScriptNode_l09dd6c* node = Find(map, key.Str())) {
        ++node->script->refs;
        result = 1;
    } else {
        UnkScript_l09dd6c* script = static_cast<UnkScript_l09dd6c*>(fn_71000839a0(sizeof(UnkScript_l09dd6c), 6));
        CRntString_CopyConstruct(&script->source, static_cast<const UnkNamedEntry*>(source));
        CRntString_CopyConstruct(&script->name, reinterpret_cast<const UnkNamedEntry*>(name));
        script->refs = 0;
        Insert(map, key.Str(), script);
        ++script->refs;
        if (fn_710109d390(source, name, true, b)) {
            result = 2;
        } else {
            fn_710109d890(ScriptManager(), key.Str());
            result = 0;
        }
    }
    return result;
}
