// Unit 1369 (CChozoCommanderXLifeComponent): the sub-id of `name` within a qualified name, kept in a
// chained hash map after the name's string (+0xd0) keyed by the string id; a missing one is built
// (fn_71003328ac) and inserted. Names and layouts are placeholders.
#include "base/UnkTunableLine.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

struct UnkStruct_71003327a4;
struct UnkStruct_71003327a4_Node;
using UnkMap_71003327a4 = base::global::CRntVector<UnkStruct_71003327a4_Node*>;
UnkStruct_71003327a4_Node** fn_7100332978(UnkMap_71003327a4* map, u32 index);

// A string id built from a string (fn_7100000850).
struct UnkSubIdName_8c0004 {
    UnkStringData* data;

    explicit UnkSubIdName_8c0004(const CRntString& str) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, true);
    }
    ~UnkSubIdName_8c0004() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
    const CStrId& Id() const { return *reinterpret_cast<const CStrId*>(this); }
};

// A new sub-id, built from the owner and the name's string.
struct UnkSubIdValue_8c0004 {
    CStrId id;
    u64 field_0x8;
    UnkSubIdValue_8c0004(UnkStruct_71003327a4* owner, void* src) asm("fn_71003328ac");
};

struct UnkSubIdNode_8c0004 {
    CStrId key;
    UnkSubId_7100fc4b34 value;
    UnkSubIdNode_8c0004* next;

    UnkSubIdNode_8c0004(const CStrId& key, const UnkSubIdValue_8c0004& value)
        : key(key), value{value.id, value.field_0x8}, next(nullptr) {}
};

struct UnkSubIdMap_8c0004 {
    UnkQualifiedName name;
    UnkMap_71003327a4 buckets;  // 0xd0
    u32 first_index;                  // 0xf8
    UnkSubIdNode_8c0004* first_node;  // 0x100
    base::UnkAllocator* allocator;    // 0x108
    u32 flags;                        // 0x110
    u32 size;                         // 0x114
    u32 allocations;                  // 0x118

    UnkSubIdNode_8c0004*& Bucket(u32 index) {
        return *reinterpret_cast<UnkSubIdNode_8c0004**>(fn_7100332978(&buckets, index));
    }

};
static_assert(__builtin_offsetof(UnkSubIdMap_8c0004, buckets) == 0xd0, "layout");
static_assert(__builtin_offsetof(UnkSubIdMap_8c0004, allocations) == 0x118, "layout");

static inline u32 HashOf(s64 key) { return (u32)key ^ (u32)((u64)key >> 32); }

// The map's key compare: the id's string against `str`, by key (the id's text is read twice).
static inline bool SameString(const CStrId& id, const CRntString& str) {
    id.Text();
    id.Text();
    return id.data->Key() == (u64)str.key;
}

const UnkSubId_7100fc4b34& fn_71008c0004(UnkQualifiedName* self, const char* name) {
    UnkSubIdMap_8c0004* map = reinterpret_cast<UnkSubIdMap_8c0004*>(self);
    UnkTunableLine text;
    fn_710007f564(text.Str(), name);

    UnkSubIdNode_8c0004* node = nullptr;
    if (map->buckets.count) {
        u32 index = HashOf(text.Str()->key) % map->buckets.capacity;
        if (index < map->buckets.count) {
            for (node = map->Bucket(index); node; node = node->next) {
                if (SameString(node->key, *text.Str()))
                    break;
            }
        }
    }
    if (!node) {
        UnkSubIdName_8c0004 id(*text.Str());
        UnkSubIdValue_8c0004 value(reinterpret_cast<UnkStruct_71003327a4*>(self), &text);
        ++map->allocations;
        node = static_cast<UnkSubIdNode_8c0004*>(map->allocator->vf_0x40(sizeof(UnkSubIdNode_8c0004), 8, map->flags));
        if (node)
            new (node) UnkSubIdNode_8c0004(id.Id(), value);
        u32 index = map->buckets.count ? id.Id().Hash() % map->buckets.capacity : (u32)-1;
        u32 first = map->first_index;
        map->first_index = index < first ? index : first;
        if (index <= first)
            map->first_node = node;
        UnkSubIdNode_8c0004*& bucket = map->Bucket(index);
        node->next = bucket;
        bucket = node;
        ++map->size;
    }
    return node->value;
}
