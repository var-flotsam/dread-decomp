#pragma once

// The engine's dictionary, base::global::CRntDictionary (the name reflection gives it): a chained hash map
// whose nodes are also linked in one list, the list reflection walks. Nodes come from the dictionary's
// allocator; the buckets are a CRntVector of node pointers, indexed by the key's hash modulo their capacity.
// Layout as base/UnkHashMap.h describes it.

#include <new>

#include "base/global/CName.h"
#include "base/global/CRntVector.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

namespace base::global {

// The hash a key's bucket comes from (taken modulo the bucket count). A string id (CStrId, CFilePathStrId,
// base::UnkStrIdMemberOf): the two halves of its string's 64-bit hash xored.
template <class K>
struct CRntDictionaryHash {
    static u32 Of(const K& key) { return key.data->hash[0] ^ key.data->hash[1]; }
};

// A pointer key: its low 32 bits.
template <class T>
struct CRntDictionaryHash<T*> {
    static u32 Of(T* key) { return (u32)(u64)key; }
};

// A 32-bit integer key: itself.
template <>
struct CRntDictionaryHash<u32> {
    static u32 Of(u32 key) { return key; }
};

// A 64-bit integer key: its low 32 bits.
template <>
struct CRntDictionaryHash<u64> {
    static u32 Of(u64 key) { return (u32)key; }
};

// A name: the two halves of its 64-bit hash xored, as a string id's.
template <>
struct CRntDictionaryHash<CName> {
    static u32 Of(const CName& key) { return (u32)(key.hash >> 32) ^ (u32)key.hash; }
};

template <class K, class V>
struct CRntDictionary {
    using key_type = K;
    using value_type = V;

    struct TNode {
        K key;
        V value;
        TNode* next;         // the list's next node
        TNode* prev;         // the list's previous node
        TNode* bucket_next;  // the next node of the same bucket

        // The list links, through accessors as the original's are: a store through one may alias the
        // dictionary's ends, which are read again after it.
        TNode*& Next() { return next; }
        TNode*& Prev() { return prev; }
    };

    UnkAllocator* allocator;  // the nodes'
    u32 flags;
    u32 count;
    CRntVector<TNode*> buckets;
    TNode* last;   // the list's last node
    TNode* first;  // where the walk starts

    CRntDictionary() = default;
    // An empty dictionary with no buckets yet, its nodes and buckets from the default allocator with
    // the given flags (fn_7100e1bca4).
    explicit CRntDictionary(u32 flags)
        : allocator(GetDefaultAllocator()), flags(flags), count(0), buckets(flags), last(nullptr), first(nullptr) {}

    // Sets the bucket count to n, clearing the new buckets; a new array's pointer is stored after its
    // capacity (fn_7100e1bca4).
    void ResizeBuckets(u32 n) {
        if (buckets.count < n) {
            if (buckets.capacity < n) {
                TNode** p =
                    static_cast<TNode**>(buckets.allocator->vf_0x40((u64)n * sizeof(TNode*), 8, buckets.flags));
                if (buckets.data) {
                    __builtin_memcpy(p, buckets.data, (u64)buckets.count * sizeof(TNode*));
                    buckets.allocator->vf_0x48(buckets.data);
                }
                buckets.capacity = n;
                buckets.data = p;
            }
            __builtin_memset(&buckets.data[buckets.count], 0, (u64)(n - buckets.count) * sizeof(TNode*));
            buckets.count = n;
        } else if (n < buckets.count) {
            buckets.count = n;
        }
    }

    // The key's bucket, or null while there are no buckets.
    TNode* const* BucketFor(const K& key) const {
        if (buckets.count == 0)
            return nullptr;
        u32 index = key.Hash() % buckets.capacity;
        return buckets.data + index;
    }

    // Destroys and frees every node; the buckets stay, emptied.
    void Clear() {
        if (count == 0)
            return;
        for (TNode* node = first; node;) {
            TNode* next = node->next;
            node->~TNode();
            allocator->vf_0x48(node);
            node = next;
        }
        __builtin_memset(buckets.data, 0, (u64)buckets.capacity * sizeof(TNode*));
        count = 0;
        last = nullptr;
        first = nullptr;
    }

    // The value under key, or null.
    V* Find(const K& key) const {
        TNode* const* bucket = BucketFor(key);
        if (!bucket)
            return nullptr;
        for (TNode* node = *bucket; node; node = node->bucket_next)
            if (node->key == key)
                return &node->value;
        return nullptr;
    }

    // The key's node, or null.
    TNode* FindNode(const K& key) const {
        TNode* const* bucket = BucketFor(key);
        if (!bucket)
            return nullptr;
        for (TNode* node = *bucket; node; node = node->bucket_next)
            if (node->key == key)
                return node;
        return nullptr;
    }

    // Adds a node for key and value at the front of the key's bucket and at the end of the list, and
    // returns it. With no buckets there is no bucket, and the original reads it all the same
    // (fn_71008ebca8).
    TNode* Insert(const K& key, const V& value) {
        TNode** bucket = nullptr;
        if (buckets.count != 0) {
            u32 index = CRntDictionaryHash<K>::Of(key) % buckets.capacity;
            bucket = buckets.data + index;
        }
        return Link(bucket, key, value);
    }

    // Insert, moving the value into the node (fn_7100f91fe4).
    TNode* Insert(const K& key, V&& value) {
        TNode** bucket = nullptr;
        if (buckets.count != 0) {
            u32 index = CRntDictionaryHash<K>::Of(key) % buckets.capacity;
            bucket = buckets.data + index;
        }
        return Link(bucket, key, static_cast<V&&>(value));
    }

    // Insert, with the bucket vector's bounds check (the assertion of its operator[]); the original's
    // inserts of objects into pools use it (fn_7100169e28).
    TNode* InsertChecked(const K& key, const V& value) {
        TNode** bucket = nullptr;
        if (buckets.count != 0) {
            u32 index = CRntDictionaryHash<K>::Of(key) % buckets.capacity;
            if (buckets.count <= index)
                __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
            bucket = buckets.data + index;
        }
        return Link(bucket, key, value);
    }

private:
    // A new node at the front of bucket and at the end of the list, its value copied or moved in.
    template <class Value>
    TNode* Link(TNode** bucket, const K& key, Value&& value) {
        TNode* old = *bucket;
        auto* node = static_cast<TNode*>(allocator->vf_0x40(sizeof(TNode), 8, flags));
        if (node)
            new (node) TNode{key, static_cast<Value&&>(value), nullptr, nullptr, nullptr};
        node->bucket_next = old;
        *bucket = node;
        if (!first)
            first = node;
        else if (last) {
            node->Prev() = last;
            last->Next() = node;
        }
        last = node;
        ++count;
        return node;
    }
};

}  // namespace base::global
