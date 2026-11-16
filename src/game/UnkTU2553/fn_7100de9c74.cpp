// Unit 2553 (CGameManager::CTunableGameManager): makes a name no registered object uses yet, by replacing
// a trailing "_<n>" with a counter until neither id map nor the object list knows it. Names and layouts are
// placeholders.
#include <stdio.h>
#include <string.h>

#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// A string with 0x100 bytes of inline storage (0x148 bytes), as in src/base/UnkLuaDoString_b50.cpp.
struct UnkInlineString_lde9c74 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_lde9c74() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        field_0x138 = 0;
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x140 = 0;
    }
    ~UnkInlineString_lde9c74() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

struct UnkNamedObject_lde9c74 {
    u8 pad_0x0[0x30];
    CStrId name;  // its key is the CRC-64 of the name
};

// Whether `map` holds an object under `id` (the id's data is the key; its +0x30 the hash).
static inline bool MapHas(const UnkGameManagerHashMap& map, const CStrId& id) {
    if (map.count) {
        const UnkGameManagerKey* key = reinterpret_cast<const UnkGameManagerKey*>(id.data);
        u32 index = id.Hash() % map.capacity;
        UnkGameManagerNode** bucket = map.buckets + index;
        if (bucket) {
            for (UnkGameManagerNode* n = *bucket; n; n = n->next) {
                if (n->key == key)
                    return true;
            }
        }
    }
    return false;
}

static inline bool IsNameTaken(UnkGameManager* self, const CStrId& id, const char* name) {
    if (MapHas(self->field_0x2608, id) || MapHas(self->field_0x2650, id))
        return true;
    for (UnkGameManagerListNode* n = self->field_0x2698; n; n = n->next) {
        UnkNamedObject_lde9c74* object = static_cast<UnkNamedObject_lde9c74*>(n->object);
        CRntString key(name);
        ++object->name.data->reads;
        if (object->name.data->Key() == key.key)
            return true;
    }
    return false;
}

void fn_7100de9c74(UnkGameManager* self, const char* name, CRntString* out) {
    char buffer[0x80];
    memcpy(buffer, name, strlen(name) + 1);
    CStrId id(buffer, true);
    s32 suffix = 0;
    while (IsNameTaken(self, id, buffer)) {
        UnkInlineString_lde9c74 stem;
        fn_710007e77c(stem.Str(), buffer);
        s32 underscore = fn_710007fb10(stem.Str(), '_', -1);
        u32 length = underscore == -1 ? stem.Str()->length : underscore;
        const char* text = stem.Str()->data;
        {
            UnkInlineString_lde9c74 prefix;
            fn_710007e7f8(prefix.Str(), text, length);
            fn_710007eaa0(stem.Str(), prefix.Str());
        }
        sprintf(buffer, "%s_%i", stem.Str()->data ? stem.Str()->data : g_unk_710162e2ed, ++suffix);
        base::global::UnkStrIdBase next(buffer, true);
        // The old string is released, but the id is not reset to the empty one before it takes the new.
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (id.data && id.data != &pool->empty && --id.data->ref_count == 0)
                fn_71000803a4(pool, id.data);
        }
        id.data = next.data;
    }
    fn_710007e77c(out, buffer);
}
