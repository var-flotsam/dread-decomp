// Unit 2648: erases the entry of a string-id map whose key is the given id's text in lower case; returns
// whether there was one (fn_710103ffdc). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkHashedString;
using base::global::UnkStringData;

struct UnkIdMap_l03f804;
struct UnkIdNode_l03f804;
UnkIdNode_l03f804* fn_710103f804(UnkIdMap_l03f804* self, const CStrId* key);

// A node of the map: its key first, the bucket chain at +0x5e8.
struct UnkIdNode_l03ffdc {
    UnkStringData* key;
    u8 unk_0x8[0x5e0];
    UnkIdNode_l03ffdc* chain;  // 0x5e8
};
static_assert(__builtin_offsetof(UnkIdNode_l03ffdc, chain) == 0x5e8, "chain");

struct UnkIdMap_l03ffdc {
    u8 unk_0x0[0x10];
    UnkIdNode_l03ffdc** buckets;  // 0x10
    u32 count;                    // 0x18
    u32 capacity;                 // 0x1c: the hash modulus
};

// The bucket a key hashes to; null while there are none.
inline UnkIdNode_l03ffdc** Bucket_l03ffdc(UnkIdMap_l03ffdc* map, UnkStringData* const& key) {
    if (!map->count)
        return nullptr;
    const UnkHashedString* hashed = static_cast<const UnkHashedString*>(key);
    u32 h = hashed->hash[0] ^ hashed->hash[1];
    u32 i = h % map->capacity;
    return map->buckets + i;
}

// The node of a key, or null.
inline UnkIdNode_l03ffdc* Find_l03ffdc(UnkIdMap_l03ffdc* map, UnkStringData* const& key) {
    if (UnkIdNode_l03ffdc** b = Bucket_l03ffdc(map, key))
        for (UnkIdNode_l03ffdc* n = *b; n; n = n->chain)
            if (n->key == key)
                return n;
    return nullptr;
}

// A string with 0x100 bytes of inline storage (0x148 bytes).
struct UnkInlineString_l03ffdc {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    explicit UnkInlineString_l03ffdc(const char* text) {
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
        CRntString_AssignCString(Str(), text);
    }
    ~UnkInlineString_l03ffdc() {
        CRntString_Clear(Str());
        CRntString_Destroy(Str());
    }
};

// `text` with A-Z turned into a-z.
inline UnkInlineString_l03ffdc LowerCase_l03ffdc(const char* text) {
    char lower[0x100];
    lower[0] = 0;
    u32 length = __builtin_strlen(text);
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = text[i];
            lower[i] = (u8)(c - 'A') < 26 ? c + 0x20 : c;
        }
        lower[length] = 0;
    }
    return UnkInlineString_l03ffdc(lower);
}

// A string id built from a string (fn_7100000850), released as CStrId is.
struct UnkKeyId_l03ffdc {
    UnkStringData* data;

    explicit UnkKeyId_l03ffdc(const CRntString& str) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, true);
    }
    ~UnkKeyId_l03ffdc() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
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

bool fn_710103ffdc(UnkIdMap_l03ffdc* self, const CStrId& key) {
    UnkKeyId_l03ffdc id(*LowerCase_l03ffdc(key.Text()).Str());
    bool erased = false;
    if (Find_l03ffdc(self, id.data)) {
        fn_710103f804(reinterpret_cast<UnkIdMap_l03f804*>(self), reinterpret_cast<const CStrId*>(&id));
        erased = true;
    }
    return erased;
}
