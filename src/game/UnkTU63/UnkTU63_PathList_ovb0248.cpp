// A unit of tools/tu_map.py (63, no reliable name; the registry of base/UnkRegistry.h): the paths a
// registry's handler lists below a path, each paired with the handler's id and appended to `out`.
// The handler is the one stored under the path's id in the registry's chained map. Names and layouts
// are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CRntVector.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "game/UnkRegistryHandler_b0248.h"
#include "game/UnkRegistryNode_b0248.h"
#include "game/UnkRegistryString_b0248.h"
#include "game/UnkRegistry_b0248.h"
#include "game/UnkTU63.h"
#include "types.h"
#include "base/UnkVtableList.h"

using base::UnkStrIdMemberOf;

struct UnkStrPair_0x10;

// Room for a pair that is destroyed through fn_710008d8e8.
union UnkPairSlot_b0248 {
    UnkStrIdPathPair pair;
    UnkPairSlot_b0248() {}
    ~UnkPairSlot_b0248() {}
};

// A path id and the link to the next one.
struct UnkListedPath_b0248 {
    UnkStrIdMemberOf<g_unk_7101d47200> id;
    UnkListedPath_b0248* next;
};

struct UnkFilePathList_b0248 : base::UnkVtableList<UnkListedPath_b0248> {
    UnkFilePathList_b0248() {
        vtable = &g_unk_7101a1b030[2];
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkFilePathList_b0248() {
        vtable = &g_unk_7101a1b030[2];
        for (UnkListedPath_b0248* n = head; n;) {
            UnkListedPath_b0248* next = n->next;
            n->id.~UnkStrIdMemberOf();
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

// A node of a list of id and path pairs.
struct UnkListedPair_b0248 {
    UnkStrIdPathPair pair;
    UnkListedPair_b0248* next;
};

struct UnkRegistryKey_b0248 {
    UnkRegistryString_b0248* id;
    UnkListedPath_b0248 path;
};

// Lists the paths below one path, recursing `depth` levels (not matched yet).
bool fn_7100087b0c(UnkRegistry_b0248* self, UnkRegistryHandler_b0248* handler, UnkListedPath_b0248* path, u32 depth,
                   UnkFilePathList_b0248* out);

bool fn_71000878f0(UnkRegistry_0x70* registry, const UnkStrPair_0x10* key_, u32 flags, void* out_) {
    auto* self = reinterpret_cast<UnkRegistry_b0248*>(registry);
    auto* key = reinterpret_cast<const UnkRegistryKey_b0248*>(key_);
    auto* out = static_cast<base::UnkVtableList<UnkListedPair_b0248>*>(out_);
    if (!self->count)
        return false;
    UnkRegistryString_b0248* id = key->id;
    u32 i = ((u32)(id->hash >> 32) ^ (u32)id->hash) % self->capacity;
    UnkRegistryNode_b0248** bucket = &self->buckets[i];
    if (!bucket)
        return false;
    UnkRegistryNode_b0248* node = *bucket;
    for (; node; node = node->next) {
        if (node->key == id)
            break;
    }
    if (!node)
        return false;
    UnkRegistryHandler_b0248* handler = node->value;
    if (!handler)
        return false;
    UnkFilePathList_b0248 found;
    bool ok = fn_7100087b0c(self, handler, const_cast<UnkListedPath_b0248*>(&key->path), flags, &found);
    if (ok) {
        for (UnkListedPath_b0248* n = found.head; n; n = n->next) {
            UnkPairSlot_b0248 temp;
            fn_710008d88c(&temp.pair, handler->vf_0x18(), &n->id);
            auto* item = static_cast<UnkListedPair_b0248*>(out->allocator->vf_0x40(sizeof(UnkListedPair_b0248), 8, out->flags));
            if (item) {
                fn_710008d830(&item->pair, &temp.pair);
                item->next = nullptr;
                if (out->count == 0) {
                    out->head = item;
                    out->tail = item;
                    out->count = 1;
                } else {
                    out->tail->next = item;
                    item->next = nullptr;
                    out->tail = item;
                    out->count = out->count + 1;
                }
            }
            fn_710008d8e8(&temp.pair);
        }
    }
    return ok;
}
