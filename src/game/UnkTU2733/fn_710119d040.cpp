// The language singleton's unit (tools/tu_map.py, unit 2733): the localized text for a key, looked up
// as given and then in upper case; a missing one reads "[<key>]", and an empty text while the language
// is not loaded (fn_710119d040). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "base/global/timeline/CEvent.h"
#include "game/UnkTU2733.h"
#include "game/nn/time.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkHashedString;
using base::global::UnkStringData;


// As src/game/UnkAdamDialog.cpp declares it.
struct UnkDialogText {
    u8 unk_0x0[0x20];
    ~UnkDialogText() { fn_7100066e44(this); }
};

// As src/game/GUI/CLabel.cpp declares it: here it appends UTF-32 text.
struct UnkLayoutText;
struct UnkTextCommand;

// Appends UTF-32 text to a text.
static inline void Append_l19d040(UnkDialogText* text, const void* chars) {
    fn_71000673e8(reinterpret_cast<UnkLayoutText*>(text), static_cast<const UnkTextCommand*>(chars));
}

// A node of the language's map (0x40 bytes): the key, the text, the insertion links and the bucket chain.
struct UnkTextNode_l19d040 {
    UnkStringData* key;
    u8 text[0x20];  // 0x8
    UnkTextNode_l19d040* next;
    UnkTextNode_l19d040* prev;
    UnkTextNode_l19d040* chain;  // 0x38
};
static_assert(sizeof(UnkTextNode_l19d040) == 0x40, "node");

// The parts of the language singleton read here.
struct UnkTextMap_l19d040 {
    u8 unk_0x0[0x10];
    UnkTextNode_l19d040** buckets;  // 0x10
    u32 count;                      // 0x18
    u32 capacity;                   // 0x1c: the hash modulus
    u8 unk_0x20[0x3d];
    bool loaded;  // 0x5d
};
static_assert(__builtin_offsetof(UnkTextMap_l19d040, loaded) == 0x5d, "loaded");

// The bucket a key hashes to; null while there are none.
inline UnkTextNode_l19d040** Bucket_l19d040(UnkTextMap_l19d040* map, UnkStringData* const& key) {
    if (!map->count)
        return nullptr;
    const UnkHashedString* hashed = static_cast<const UnkHashedString*>(key);
    u32 h = hashed->hash[0] ^ hashed->hash[1];
    u32 i = h % map->capacity;
    return map->buckets + i;
}

// The node of a key, or null.
inline UnkTextNode_l19d040* Find_l19d040(UnkTextMap_l19d040* map, UnkStringData* const& key) {
    if (UnkTextNode_l19d040** b = Bucket_l19d040(map, key))
        for (UnkTextNode_l19d040* n = *b; n; n = n->chain)
            if (n->key == key)
                return n;
    return nullptr;
}

// A string with 0x100 bytes of inline storage (0x148 bytes).
struct UnkInlineString_l19d040 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    explicit UnkInlineString_l19d040(const char* text) {
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
        CRntString_AssignCString(Str(), text);
    }
    ~UnkInlineString_l19d040() {
        CRntString_Clear(Str());
        CRntString_Destroy(Str());
    }
};

// `text` with a-z turned into A-Z.
inline UnkInlineString_l19d040 UpperCase_l19d040(const char* text) {
    char upper[0x100];
    upper[0] = 0;
    u32 length = __builtin_strlen(text);
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = text[i];
            upper[i] = (u8)(c - 'a') < 26 ? c - 0x20 : c;
        }
        upper[length] = 0;
    }
    return UnkInlineString_l19d040(upper);
}

// A string id built from a string (fn_7100000850), released as CStrId is.
struct UnkTextId_l19d040 {
    UnkStringData* data;

    explicit UnkTextId_l19d040(const CRntString& str) {
        CStrId_ConstructFromRntString(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, true);
    }
    ~UnkTextId_l19d040() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

UnkDialogText fn_710119d040(UnkLanguage* language, const CStrId& key) {
    UnkTextMap_l19d040* map = reinterpret_cast<UnkTextMap_l19d040*>(language);
    UnkDialogText text;
    if (!map->loaded) {
        fn_7100066a50(&text, U"");
        return text;
    }
    UnkTextNode_l19d040* node = Find_l19d040(map, key.data);
    if (!node) {
        UnkInlineString_l19d040 upper = UpperCase_l19d040(key.Text());
        UnkTextId_l19d040 id(*upper.Str());
        node = Find_l19d040(map, id.data);
    }
    if (node) {
        fn_7100066cac(&text, node->text);
        return text;
    }
    wchar_t chars[0x100];
    ++key.data->reads;
    fn_710006c000(&key.data->String(), chars, 0x100);
    fn_7100066a50(&text, U"[");
    Append_l19d040(&text, chars);
    Append_l19d040(&text, U"]");
    return text;
}
