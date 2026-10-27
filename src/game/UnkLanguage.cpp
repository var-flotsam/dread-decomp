// The language singleton (unit 2733; g_unk_7101d47358, 0x2068 bytes, created on first use by
// fn_710119c020): the localized texts of the current language in a hash map keyed by string id,
// and a cache of 1024 texts built from them. fn_710119cb44 loads a language's text file into the
// map; fn_710119c208 empties the cache and, unless asked to keep them, the map. Names and layouts
// are placeholders.
#include <string.h>

#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringEntry;
using base::global::UnkStringPool;

// A localized text (0x20 bytes: the UTF-32 characters at +0x8, null when empty, and their count at
// +0x10), with its constructors and destructor.
void fn_7100066a50(void* self, const char32_t* text);
void fn_7100066cac(void* self, const void* other);
void fn_7100066e44(void* text);
struct UnkLanguageText {
    u8 unk_0x0[0x8];
    const char32_t* chars;
    s32 count;
    u8 unk_0x14[0xc];

    explicit UnkLanguageText(const char32_t* text) { fn_7100066a50(this, text); }
    UnkLanguageText(const UnkLanguageText& other) { fn_7100066cac(this, &other); }
    ~UnkLanguageText() { fn_7100066e44(this); }
};
static_assert(sizeof(UnkLanguageText) == 0x20, "UnkLanguageText placeholder layout");

// A node's key: a string id released in two steps (the original reloads the id and its count after
// the decrement).
struct UnkLanguageKey {
    UnkStringData* data;

    explicit UnkLanguageKey(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkLanguageKey() {
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

// A node of the map (0x40 bytes): linked in insertion order and in its bucket's chain.
struct UnkLanguageNode {
    UnkLanguageKey key;
    UnkLanguageText text;
    UnkLanguageNode* next;
    UnkLanguageNode* prev;
    UnkLanguageNode* chain;

    UnkLanguageNode(const CStrId& key, const UnkLanguageText& text)
        : key(key), text(text), next(nullptr), prev(nullptr), chain(nullptr) {}
};
static_assert(sizeof(UnkLanguageNode) == 0x40, "UnkLanguageNode placeholder layout");

// A key's hash: its string's CRC-64, the halves xored.
static inline u32 Hash(const CStrId& key) {
    const UnkStringEntry* e = reinterpret_cast<const UnkStringEntry*>(key.data);
    return (u32)e->buffer.key ^ (u32)((u64)e->buffer.key >> 32);
}

// The bucket array's slot for a hash.
static inline UnkLanguageNode** Slot(base::UnkVector<UnkLanguageNode*>& buckets, u32 h) {
    u32 i = h % buckets.capacity;
    return buckets.data + i;
}

struct UnkLanguage {
    base::UnkAllocator* allocator;  // the nodes'
    u32 flags;
    u32 count;
    base::UnkVector<UnkLanguageNode*> buckets;  // `capacity` is the hash modulus
    UnkLanguageNode* last;
    UnkLanguageNode* first;
    bool field_0x48;
    u8 unk_0x49[0x7];
    u64 field_0x50;
    u32 field_0x58;
    bool field_0x5c;
    bool field_0x5d;
    UnkLanguageText* cache[1024];
    u32 language;
    s32 field_0x2064;

    // The bucket a key hashes to; null while there are none.
    UnkLanguageNode** Bucket(const CStrId& key) {
        return buckets.count ? Slot(buckets, Hash(key)) : nullptr;
    }

    bool Contains(const CStrId& key) {
        if (UnkLanguageNode** b = Bucket(key))
            for (UnkLanguageNode* n = *b; n; n = n->chain)
                if (n->key.data == key.data)
                    return true;
        return false;
    }

    void Insert(const CStrId& key, UnkLanguageText text) {
        // Written out rather than through Bucket(): the original keeps the null bucket's path.
        UnkLanguageNode** b = buckets.count ? Slot(buckets, Hash(key)) : nullptr;
        UnkLanguageNode* head = *b;
        auto* node = static_cast<UnkLanguageNode*>(allocator->vf_0x40(sizeof(UnkLanguageNode), 8, flags));
        if (node)
            new (node) UnkLanguageNode(key, text);
        node->chain = head;
        *b = node;
        // The tail is read through a reference: the original reloads it after linking the node.
        UnkLanguageNode*& tail = last;
        if (!first) {
            first = node;
        } else if (tail) {
            node->prev = tail;
            tail->next = node;
        }
        tail = node;
        ++count;
    }
};
static_assert(sizeof(UnkLanguage) == 0x2068, "UnkLanguage placeholder layout");

// Frees the cached texts and, unless `keep_texts`, every text of the map.
void fn_710119c208(UnkLanguage* self, bool keep_texts) {
    for (u64 i = 0; i < 1024; ++i) {
        if (UnkLanguageText* text = self->cache[i]) {
            delete text;
            self->cache[i] = nullptr;
        }
    }
    self->field_0x58 = 0;
    self->field_0x50 = 0;
    if (!keep_texts && self->count) {
        UnkLanguageNode* node = self->first;
        while (node) {
            UnkLanguageNode* next = node->next;
            node->text.~UnkLanguageText();
            node->key.~UnkLanguageKey();
            self->allocator->vf_0x48(node);
            node = next;
        }
        memset(self->buckets.data, 0, self->buckets.capacity * u64(8));
        self->count = 0;
        self->last = nullptr;
        self->first = nullptr;
    }
}

// The file reader the language file is read with: the file held in memory and two words.
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};
// The id and path pair (base/UnkStrIdPathPair.h) fn_710008d680 builds from a path.
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};
void fn_710008d680(UnkStrPair_0x10* s, const char* path);
bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
// Closes the file.
void fn_710007c144(UnkHashStream_0x38* s);
// Reads values from the file's position as the format says ("s" a string, "w" a UTF-32 string);
// returns the bytes read.
u64 fn_710007d788(base::global::CRntFile* file, const char* format, ...);
// Adds a credits text (keys with "CREDIT_" in them).
void fn_710119ce68(UnkLanguage* self, const CStrId& key, const UnkLanguageText& text);

// Loads the language file at `path`: each entry is a key and a UTF-32 text. Keys the map has, empty
// keys and empty texts are skipped; keys with "CREDIT_" in them go to fn_710119ce68, the others into
// the map (unless `keep_texts`, which keeps the map as it was).
// NON_MATCHING(minor): every stack slot sits 8 bytes lower (the original has an unused 8-byte slot
// at the bottom of the frame, likely the register scavenger's), and the bucket-index blocks load the
// bucket array before the msub instead of after it (scheduling and register numbers only).
void fn_710119cb44(UnkLanguage* self, const char* path, bool keep_texts) {
    fn_710119c208(self, keep_texts);
    self->field_0x5c = false;
    UnkFileReader reader;
    fn_710007be8c(&reader.file, 0x16, nullptr);
    {
        UnkStrPair_0x10 s;
        fn_710008d680(&s, path);
        {
            CRntString mode("rb");
            fn_710007c1ac(&reader, &s, &mode);
        }
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    fn_710007c00c(&reader.file, 8, 0);
    while (reader.file.position < reader.file.size) {
        char key[0x200];
        char32_t value[0x1000];
        fn_710007d788(&reader.file, "sw", key, value);
        UnkLanguageText text(value);
        CStrId id(key, true);
        if (!text.chars || !text.count)
            continue;
        UnkStringEntry* e = reinterpret_cast<UnkStringEntry*>(id.data);
        if (!e->buffer.length || self->Contains(id))
            continue;
        // The id's string (counting the use), null for the empty id.
        ++e->tail.field_0x44;
        const CRntString* str = e == reinterpret_cast<UnkStringEntry*>(&g_unk_7101d47108->empty) ? nullptr : &e->buffer;
        if (str && fn_710007fb9c(str, "CREDIT_")) {
            fn_710119ce68(self, id, text);
        } else if (!keep_texts) {
            self->Insert(id, text);
        }
    }
    fn_710007c144(&reader.file);
    fn_710007c0e4(&reader.file);
}
