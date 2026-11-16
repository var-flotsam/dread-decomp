// Unit 2733 (the language singleton of src/game/UnkLanguage.cpp): the localized text for a key. The
// key is looked up as it is, then in upper case; a missing one gives "[<key>]". Before the texts are
// loaded every key gives the empty text. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "base/global/timeline/CEvent.h"
#include "game/UnkTU2733.h"
#include "game/nn/time.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringEntry;
using base::global::UnkStringPool;

using base::global::UnkStringPool;

// A localized text (0x20 bytes: the UTF-32 characters at +0x8, null when empty, and their count at
// +0x10), with its constructors and destructor.
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
static inline UnkLanguageNode** Slot(base::global::CRntVector<UnkLanguageNode*>& buckets, u32 h) {
    u32 i = h % buckets.capacity;
    return buckets.data + i;
}

struct UnkLanguage {
    base::UnkAllocator* allocator;  // the nodes'
    u32 flags;
    u32 count;
    base::global::CRntVector<UnkLanguageNode*> buckets;  // `capacity` is the hash modulus
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

struct UnkDialogText {
    u8 unk_0x0[0x20];
    ~UnkDialogText() { fn_7100066e44(this); }
};

struct UnkLayoutText;
struct UnkTextCommand;
void fn_71000673e8(UnkLayoutText* self, const UnkTextCommand* command);

extern const char32_t g_unk_7101648434[] __attribute__((visibility("hidden")));

// A string with 0x100 bytes of inline storage (0x148 bytes), as in src/base/UnkLuaDoString_b50.cpp.
struct UnkInlineString_l19d040 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_l19d040() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    explicit UnkInlineString_l19d040(const char* text) : UnkInlineString_l19d040() {
        fn_710007e77c(Str(), text);
    }
    ~UnkInlineString_l19d040() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

// A string id built from a string's text, out of line (fn_7100000850).
struct UnkTextStrId_l19d040 {
    UnkStringData* data;

    explicit UnkTextStrId_l19d040(const CRntString& text) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), text, true);
    }
    ~UnkTextStrId_l19d040() {
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
    const CStrId& Id() const { return *reinterpret_cast<const CStrId*>(this); }
};

// `src` with a-z turned into A-Z, as a string.
inline UnkInlineString_l19d040 UpperCase_l19d040(const char* src) {
    char upper[0x100];
    upper[0] = 0;
    u32 length = static_cast<u32>(__builtin_strlen(src));
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            if (static_cast<u32>(c - 'a') < 26)
                c -= 0x20;
            upper[i] = c;
        }
        upper[length] = 0;
    }
    return UnkInlineString_l19d040(upper);
}

// The node holding the string `data` (the string of `key`); null when the map has none.
inline UnkLanguageNode* FindNode_l19d040(UnkLanguage* language, const CStrId& key, UnkStringData* data) {
    if (UnkLanguageNode** b = language->Bucket(key))
        for (UnkLanguageNode* n = *b; n; n = n->chain)
            if (n->key.data == data)
                return n;
    return nullptr;
}

UnkDialogText fn_710119d040(UnkLanguage* language, const CStrId& key) {
    UnkDialogText text;
    if (language->field_0x5d) {
    UnkLanguageNode* node = FindNode_l19d040(language, key, key.data);
    if (!node) {
        UnkInlineString_l19d040 upper = UpperCase_l19d040(key.Text());
        UnkTextStrId_l19d040 id(*upper.Str());
        node = FindNode_l19d040(language, id.Id(), id.Id().data);
    }
    if (node) {
        fn_7100066cac(&text, &node->text);
    } else {
        wchar_t name[0x100];
        ++key.data->reads;
        fn_710006c000(&key.data->String(), name, 0x100);
        fn_7100066a50(&text, U"[");
        fn_71000673e8(reinterpret_cast<UnkLayoutText*>(&text), reinterpret_cast<const UnkTextCommand*>(name));
        fn_71000673e8(reinterpret_cast<UnkLayoutText*>(&text), reinterpret_cast<const UnkTextCommand*>(U"]"));
    }
    return text;
    }
    fn_7100066a50(&text, g_unk_7101648434);
    return text;
}
