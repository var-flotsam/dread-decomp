// Unit 2739: the variants of a file path, cached by path: for each of the 16 variant names the path
// with "_<name>" inserted before its extension, kept when the file registry knows the file. The list
// is built once and kept in a chained map (g_unk_7172a0c400). Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkChainedMap.h"
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/animation/CAnimationResource.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "../../../src/game_aca57c/UnkBufferString.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CRntVector;
using base::global::UnkFilePathStrIdBase;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkPathEntry_l1aaaf0;
using UnkPathEntryVector_l1aaaf0 = base::global::CRntVector<UnkPathEntry_l1aaaf0>;
void fn_71011aaaf0(UnkPathEntryVector_l1aaaf0* self, const UnkPathEntryVector_l1aaaf0* other);

extern UnkChainedMap g_unk_7172a0c400;
// The 16 variant names.
extern const char* const g_unk_7101cb5738[16] __attribute__((visibility("hidden")));

using UnkString_l1a7fcc = UnkBufferString<0x100, g_unk_7101a194b8>;

// A file path whose move leaves the source empty.
struct UnkPath_l1a7fcc : UnkFilePathStrIdBase {
    explicit UnkPath_l1a7fcc(const CRntString& str) { fn_710001a270(this, str, true); }
    UnkPath_l1a7fcc(const UnkPath_l1a7fcc& other) : UnkFilePathStrIdBase(other) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkPath_l1a7fcc(UnkPath_l1a7fcc&& other) : UnkFilePathStrIdBase(other) {
        other.data = &g_unk_7101d47200->empty;
    }
    ~UnkPath_l1a7fcc() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// A variant: its index among the names and its path. The index is a bit-field, read and written
// with no type of its own, so a store to it may alias the list's count and the pool.
struct UnkVariant_l1a7fcc {
    u32 index : 32;
    UnkPath_l1a7fcc path;

    UnkVariant_l1a7fcc(u32 index, const UnkPath_l1a7fcc& path) : index(index), path(path) {}
    UnkVariant_l1a7fcc(UnkVariant_l1a7fcc&& other)
        : index(other.index), path(static_cast<UnkPath_l1a7fcc&&>(other.path)) {}
};

using UnkVariantList_l1a7fcc = CRntVector<UnkVariant_l1a7fcc>;

// A map node: the path, its variants, the node list and the bucket chain. The variants are built in
// place by the list's copy (fn_71011aaaf0), so the node holds them as plain fields.
struct UnkVariantNode_l1a7fcc {
    struct Variants {
        UnkVariant_l1a7fcc* data;
        u32 count;
        u32 capacity;
        u32 step;
        bool field_0x14;
        u32 flags;
        base::UnkAllocator* allocator;
    };

    UnkPath_l1a7fcc key;
    Variants variants;                  // 0x8
    UnkVariantNode_l1a7fcc* next;       // 0x30
    UnkVariantNode_l1a7fcc* prev;       // 0x38
    UnkVariantNode_l1a7fcc* chain;      // 0x40

    UnkVariantNode_l1a7fcc(const UnkPath_l1a7fcc& key, const UnkVariantList_l1a7fcc& variants) : key(key) {
        fn_71011aaaf0(reinterpret_cast<UnkPathEntryVector_l1aaaf0*>(&this->variants),
                      reinterpret_cast<const UnkPathEntryVector_l1aaaf0*>(&variants));
        prev = nullptr;
        chain = nullptr;
        next = nullptr;
    }

    UnkVariantList_l1a7fcc* List() { return reinterpret_cast<UnkVariantList_l1a7fcc*>(&variants); }
};
static_assert(sizeof(UnkVariantNode_l1a7fcc) == 0x48, "node layout");

// The id and path pair the file registry is asked with: built by fn_710008d2ec, destroyed by
// fn_710008d8e8.
struct UnkFileKey_l1a7fcc {
    UnkStringData* id;
    UnkStringData* path;
};

// The bucket of a path, or none while the map has no buckets.
inline UnkVariantNode_l1a7fcc** Bucket(const UnkPath_l1a7fcc& key) {
    UnkChainedMap& map = g_unk_7172a0c400;
    if (map.buckets.count == 0)
        return nullptr;
    u32 hash = key.data->hash[0] ^ key.data->hash[1];
    return reinterpret_cast<UnkVariantNode_l1a7fcc**>(&map.buckets.data[hash % map.buckets.capacity]);
}

// The string of an id, counting the read.
inline const CRntString& IdString(const UnkFilePathStrIdBase& id) {
    ++id.data->reads;
    return id.data->String();
}

inline const char* VariantName(u32 index) {
    if (index < 16)
        return g_unk_7101cb5738[index];
    return g_unk_710162e2ed;
}

// Copies `src` to `dst` in lower case.
inline void ToLower(char* dst, const char* src) {
    dst[0] = '\0';
    u32 length = strlen(src);
    if (length != 0) {
        for (u32 i = 0; i < length; ++i) {
            u8 c = src[i];
            dst[i] = (u8)(c - 'A') < 26 ? c + ('a' - 'A') : c;
        }
        dst[length] = '\0';
    }
}

// Appends `text` in lower case.
inline void AppendLowerCase(CRntString* str, const char* text) {
    char lower[0x100];
    ToLower(lower, text);
    UnkString_l1a7fcc part(lower);
    CRntString_Append(str, part.Str());
}

// The node of a path, or none.
inline UnkVariantNode_l1a7fcc* Find(const UnkPath_l1a7fcc& key) {
    if (UnkVariantNode_l1a7fcc** bucket = Bucket(key)) {
        for (UnkVariantNode_l1a7fcc* node = *bucket; node; node = node->chain) {
            if (node->key.data == key.data)
                return node;
        }
    }
    return nullptr;
}

UnkVariantList_l1a7fcc* fn_71011a7fcc(const UnkPath_l1a7fcc* key) {
    UnkVariantNode_l1a7fcc* node = Find(*key);
    if (!node) {
        UnkVariantList_l1a7fcc variants(0xf);
        {
            const CRntString& path_text = IdString(*key);
            UnkString_l1a7fcc path;
            CRntString_Assign(path.Str(), &path_text);
            s32 dot = CRntString_FindLastChar(path.Str(), '.', -1);
            if (dot != -1) {
                // The path up to its extension, and the extension.
                const char* text = path.Str()->data;
                UnkString_l1a7fcc stem;
                CRntString_AssignPrefix(stem.Str(), text, dot);
                u32 rest = path.Str()->length - dot;
                const char* extension_text = path.Str()->data + (u32)dot;
                UnkString_l1a7fcc extension;
                CRntString_AssignPrefix(extension.Str(), extension_text, rest);

                for (u32 i = 0; i != 16; ++i) {
                    UnkString_l1a7fcc name(stem);
                    CRntString_AppendCString(name.Str(), "_");
                    AppendLowerCase(name.Str(), VariantName(i));
                    CRntString_Append(name.Str(), extension.Str());
                    UnkPath_l1a7fcc variant(*name.Str());
                    // Kept when the file registry knows the file.
                    UnkRegistry_0x70* registry = g_unk_7101d4b5a0;
                    if (!registry) {
                        registry = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
                        fn_7100087388(registry);
                        g_unk_7101d4b5a0 = registry;
                    }
                    bool found;
                    {
                        UnkFileKey_l1a7fcc file;
                        fn_710008d2ec(reinterpret_cast<UnkStrPair_0x10*>(&file),
                                      static_cast<const CFilePathStrId*>(static_cast<const UnkFilePathStrIdBase*>(&variant)));
                        found = fn_7100087850(registry, reinterpret_cast<const UnkStrPair_0x10*>(&file));
                        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&file));
                    }
                    if (found)
                        variants.EmplaceBack(UnkVariant_l1a7fcc(i, variant));
                }
            }

            UnkChainedMap& map = g_unk_7172a0c400;
            UnkVariantNode_l1a7fcc** bucket = Bucket(*key);
            UnkVariantNode_l1a7fcc* chain = *bucket;
            node = static_cast<UnkVariantNode_l1a7fcc*>(map.allocator->vf_0x40(sizeof(UnkVariantNode_l1a7fcc), 8, map.flags));
            if (node)
                new (node) UnkVariantNode_l1a7fcc(*key, variants);
            node->chain = chain;
            *bucket = node;
            UnkVariantNode_l1a7fcc*& first = reinterpret_cast<UnkVariantNode_l1a7fcc*&>(map.tail);
            UnkVariantNode_l1a7fcc*& last = reinterpret_cast<UnkVariantNode_l1a7fcc*&>(map.head);
            if (!first) {
                first = node;
            } else if (last) {
                node->prev = last;
                last->next = node;
            }
            last = node;
            ++map.count;
        }
        variants.Free();
    }
    return node->List();
}
