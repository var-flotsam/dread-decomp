// Unit 2730 (tools/tu_map.py): the sub-id a qualified name keeps for a path, from its chained map at +0xd0 keyed by
// string id (compared by the text's CRC-64); a missing one is built by fn_71003328ac and inserted. Names and
// layouts are placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

struct UnkStruct_71003327a4;

// The map's value: an id and one more word, built from the owner and the key.
struct UnkStruct_71003327a4_Value {
    CStrId field_0x0;
    u64 field_0x8;
    UnkStruct_71003327a4_Value(UnkStruct_71003327a4* owner, void* src) asm("fn_71003328ac");  // fn_71003328ac
};

struct UnkStruct_71003327a4_Node;
using UnkMap_71003327a4 = base::global::CRntVector<UnkStruct_71003327a4_Node*>;
UnkStruct_71003327a4_Node** fn_7100332978(UnkMap_71003327a4* map, u32 index);

// A string with 0x100 bytes of inline storage (0x148 bytes).
struct UnkInlineString_l19a610 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_l19a610() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    ~UnkInlineString_l19a610() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

// A string id built from a string's text.
struct UnkNameId_l19a610 {
    UnkStringData* data;

    explicit UnkNameId_l19a610(const CRntString& str) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, true);
    }
    ~UnkNameId_l19a610() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
};

// The pooled data behind a string id: its text at +0x10 and a count of the reads at +0x44 (a union
// member: the original reloads the id after the increment).
struct UnkIdText_l19a610 {
    u8 pad_0x0[0x10];
    CRntString text;
    u8 pad_0x40[4];
    union {
        u32 reads;
    };
};

static inline const CRntString* UseText(UnkStringData* data) {
    UnkIdText_l19a610* text = reinterpret_cast<UnkIdText_l19a610*>(data);
    ++text->reads;
    return &text->text;
}

struct UnkSubIdNode_l19a610 {
    CStrId key;
    UnkSubId_7100fc4b34 value;   // 0x8
    UnkSubIdNode_l19a610* next;  // 0x18
};

// The chained map: buckets of node lists, the first used bucket and its node, the nodes' allocator.
struct UnkSubIdMap_l19a610 {
    base::global::CRntVector<UnkSubIdNode_l19a610*> buckets;
    u32 first_bucket;              // 0x28
    UnkSubIdNode_l19a610* first;   // 0x30
    base::UnkAllocator* allocator;  // 0x38
    u32 flags;                     // 0x40
    u32 size;                      // 0x44
    u32 version;                   // 0x48

    UnkSubIdNode_l19a610** Bucket(u32 index) {
        return reinterpret_cast<UnkSubIdNode_l19a610**>(
            fn_7100332978(reinterpret_cast<UnkMap_71003327a4*>(&buckets), index));
    }
    // The bucket of a CRC-64 (its two halves folded), or ~0 while there are no buckets.
    u32 BucketOf(const s64& key) const {
        if (!buckets.count)
            return 0xffffffff;
        const u32* half = reinterpret_cast<const u32*>(&key);
        return (half[0] ^ half[1]) % buckets.capacity;
    }

    // The node whose name has the text's CRC-64, or null.
    UnkSubIdNode_l19a610* Find(const CRntString& text) {
        if (buckets.count) {
            u32 index = BucketOf(text.key);
            if (index < buckets.count) {
                for (UnkSubIdNode_l19a610* node = *Bucket(index); node; node = node->next) {
                    // The name is read twice (two counted reads) before its CRC-64 is compared.
                    UseText(node->key.data);
                    UseText(node->key.data);
                    if (reinterpret_cast<UnkIdText_l19a610*>(node->key.data)->text.key == text.key)
                        return node;
                }
            }
        }
        return nullptr;
    }
};

struct UnkNamedSubIds_l19a610 {
    UnkQualifiedName name;
    UnkSubIdMap_l19a610 sub_ids;  // 0xd0
};
static_assert(__builtin_offsetof(UnkNamedSubIds_l19a610, sub_ids) == 0xd0, "layout");

const UnkSubId_7100fc4b34& fn_710119a610(UnkQualifiedName* self, const void* path) {
    UnkNamedSubIds_l19a610* owner = reinterpret_cast<UnkNamedSubIds_l19a610*>(self);
    UnkInlineString_l19a610 str;
    fn_710007f3b4(str.Str(), static_cast<const CRntString*>(path));
    UnkSubIdMap_l19a610& map = owner->sub_ids;
    UnkSubIdNode_l19a610* node = map.Find(*str.Str());
    if (!node) {
        UnkNameId_l19a610 id(*str.Str());
        UnkStruct_71003327a4_Value value(reinterpret_cast<UnkStruct_71003327a4*>(owner), str.Str());
        ++map.version;
        node = static_cast<UnkSubIdNode_l19a610*>(map.allocator->vf_0x40(sizeof(UnkSubIdNode_l19a610), 8, map.flags));
        if (node) {
            new (&node->key) CStrId(reinterpret_cast<const CStrId&>(id));
            new (&node->value.id) CStrId(value.field_0x0);
            node->value.field_0x8 = value.field_0x8;
            node->next = nullptr;
        }
        u32 index = map.BucketOf(reinterpret_cast<UnkIdText_l19a610*>(id.data)->text.key);
        u32 first = map.first_bucket;
        map.first_bucket = index < first ? index : first;
        if (index <= first)
            map.first = node;
        UnkSubIdNode_l19a610** slot = map.Bucket(index);
        node->next = *slot;
        *slot = node;
        ++map.size;
    }
    return node->value;
}
