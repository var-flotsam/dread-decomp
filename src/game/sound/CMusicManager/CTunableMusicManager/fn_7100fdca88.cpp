// sound::CMusicManager::CTunableMusicManager's unit: the music manager's entry for a name in its map at
// +0x1c0, created (default-built, then named) when the map has none. Names and layouts are placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/sound/UnkMusicMapValue.h"
#include "types.h"

using base::global::CStrId;

// Names the value after a copy of the id.
static void SetName(UnkMusicMapValue& value, CStrId id) {
    value.name = reinterpret_cast<const base::UnkStrIdMember&>(id);
}

struct UnkMusicNode_dca88 {
    CStrId key;
    UnkMusicMapValue value;  // 0x8
    UnkMusicNode_dca88* next;      // 0x70
    UnkMusicNode_dca88* prev;      // 0x78
    UnkMusicNode_dca88* chain;     // 0x80

    UnkMusicNode_dca88(const CStrId& k, UnkMusicMapValue&& v)
        : key(k), value(static_cast<UnkMusicMapValue&&>(v)), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkMusicNode_dca88*& Next() { return next; }
    UnkMusicNode_dca88*& Prev() { return prev; }
};

// A hash map by name whose nodes are also kept in a list.
struct UnkMusicMap_dca88 {
    base::IAllocator* allocator;
    u32 flags;
    u32 count;  // 0xc
    UnkMusicNode_dca88** buckets;
    u32 bucket_count;
    u32 divisor;
    u8 pad_0x20[0x18];
    UnkMusicNode_dca88* tail;  // 0x38
    UnkMusicNode_dca88* head;  // 0x40

    UnkMusicNode_dca88* Find(const CStrId& key) {
        UnkMusicNode_dca88** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = key.Hash() % divisor;
            bucket = buckets + i;
        }
        if (!bucket)
            return nullptr;
        for (UnkMusicNode_dca88* n = *bucket; n; n = n->chain)
            if (n->key.data == key.data)
                return n;
        return nullptr;
    }
    UnkMusicNode_dca88* Insert(const CStrId& key, UnkMusicMapValue&& value) {
        UnkMusicNode_dca88** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = key.Hash() % divisor;
            bucket = buckets + i;
        }
        UnkMusicNode_dca88* old = *bucket;
        auto* node = static_cast<UnkMusicNode_dca88*>(allocator->vf_0x40(sizeof(UnkMusicNode_dca88), 8, flags));
        new (node) UnkMusicNode_dca88(key, static_cast<UnkMusicMapValue&&>(value));
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            node->Prev() = tail;
            tail->Next() = node;
        }
        tail = node;
        ++count;
        return node;
    }
};

struct UnkMusicManagerMap_dca88 {
    u8 pad_0x0[0x1c0];
    UnkMusicMap_dca88 values;  // 0x1c0
};

UnkMusicMapValue* fn_7100fdca88(UnkMusicManagerMap_dca88* self, const CStrId& name) {
    if (self->values.Find(name))
        return &self->values.Find(name)->value;
    UnkMusicMapValue& value = self->values.Insert(name, UnkMusicMapValue())->value;
    SetName(value, name);
    fn_7100fed650(reinterpret_cast<char*>(&value.layer_set.layers), reinterpret_cast<char*>(const_cast<CStrId*>(&name)));
    return &value;
}
