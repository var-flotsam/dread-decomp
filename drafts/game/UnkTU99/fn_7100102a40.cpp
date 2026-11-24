// Unit 99 of tools/tu_map.py (base::snd::CSoundSystemATK's range): fn_7100102a40 starts the sound of an
// id: it adds the id to the sound map at +0x4007d0 when missing, makes the id's sound from the engine heap
// (or resets the one it has), prepares it from the resource the request names and starts it, telling the
// sound thread what happened. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntVector.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "game/UnkTU58.h"
#include "game/UnkTU99.h"

using base::snd::UnkSoundSystemATK;

struct UnkSoundStart;
void CSoundSystemATK_PushCommandToSecondQueue(UnkSoundSystemATK* sys, s32 command, const void* payload);
void fn_71000f4414(void* sound);
void fn_71000f43c0(void* sound);
void fn_71000f4488(void* self, void* target, u32 id, u64 a, u32 b, u32 c, u32 d);
bool fn_71000f4ebc(UnkSoundStart* self, u32 id, u32 c, f32 a, f32 b);
extern u8 g_unk_7108b6b600[] __attribute__((visibility("hidden")));

template <class K>
struct UnkSndNode_l102a40 {
    K key;                      // 0x0
    void* value;                // 0x8
    UnkSndNode_l102a40* next;   // 0x10: the list of all nodes
    UnkSndNode_l102a40* prev;   // 0x18
    UnkSndNode_l102a40* chain;  // 0x20: the next node of the bucket

    // The back link, through an accessor (its store may alias the map's own links).
    UnkSndNode_l102a40*& Prev() { return prev; }
};

template <class K>
struct UnkSndMap_l102a40 {
    typedef UnkSndNode_l102a40<K> Node;

    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    Node** buckets;                 // 0x10
    u32 used;                       // 0x18
    u32 bucket_count;               // 0x1c
    u8 pad_0x20[0x18];
    Node* tail;  // 0x38
    Node* head;  // 0x40

    // The bucket of `key` (its low 32 bits modulo the bucket count); null while the map is unused.
    Node** BucketOf(K key) {
        if (!used)
            return nullptr;
        return static_cast<u32>(key) % bucket_count + buckets;
    }

    // The node of `key` in its bucket `bucket` (from BucketOf).
    static Node* FindIn(Node** bucket, K key) {
        if (!bucket)
            return nullptr;
        Node* n = *bucket;
        for (; n; n = n->chain) {
            if (n->key == key)
                break;
        }
        return n;
    }

    Node* Find(K key) { return FindIn(BucketOf(key), key); }

    bool Contains(K key) { return Find(key) != nullptr; }

    // The value of a key known to be there.
    void* At(K key) {
        Node* n = *BucketOf(key);
        while (n->key != key)
            n = n->chain;
        return n->value;
    }

    // Adds a node for `key` (its value null) at the head of its bucket `bucket` (from BucketOf) and at the
    // end of the list.
    void Add(const K& key) {
        Node** bucket = used ? static_cast<u32>(key) % bucket_count + buckets : nullptr;
        Node* old_head = *bucket;
        Node* n = static_cast<Node*>(allocator->vf_0x40(sizeof(Node), 8, flags));
        if (n) {
            n->key = key;
            n->value = nullptr;
            n->next = nullptr;
            n->prev = nullptr;
            n->chain = nullptr;
        }
        n->chain = old_head;
        *bucket = n;
        Node** link = &head;
        if (!*link) {
            *link = n;
        } else if (tail) {
            n->Prev() = tail;
            link = &tail->next;
            *link = n;
        }
        tail = n;
        ++count;
    }
};
static_assert(sizeof(UnkSndMap_l102a40<u32>) == 0x48, "layout");

// The resource a request names: its handle and id.
struct UnkSndResource_l102a40 {
    u64 handle;  // 0x0
    u32 id;      // 0x8
};

struct UnkSndSys_l102a40 {
    u8 pad_0x0[0x4007d0];
    UnkSndMap_l102a40<u32> sounds;  // 0x4007d0
    u8 pad_0x400818[0x400880 - 0x400818];
    UnkSndMap_l102a40<u64>* resources;  // 0x400880
};
static_assert(__builtin_offsetof(UnkSndSys_l102a40, resources) == 0x400880, "layout");

struct UnkSndStartRequest_l102a40 {
    u32 id;  // 0x0
    u32 pad_0x4;
    u64 resource;  // 0x8
    u32 start_id;  // 0x10
    u32 pad_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 start_c;  // 0x24
    u32 pad_0x28;
    f32 start_b;  // 0x2c
    f32 start_a;  // 0x30
};

// The engine heap as the sounds use it: a sub-allocator, a hook after each allocation (which may replace
// the result) and one before each free.
struct UnkAtkHeapHooks_l102a40 {
    u64 field_0x0;
    base::UnkSubAllocator allocator;  // 0x8
    u8 heap[8];                       // 0x40
    void* last;                       // 0x48
    void (*on_alloc)(void* arg, UnkAtkHeapHooks_l102a40* heap);  // 0x50
    void* on_alloc_arg;                                          // 0x58
    void (*on_free)(void* arg, UnkAtkHeapHooks_l102a40* heap);   // 0x60
    void* on_free_arg;                                           // 0x68
};

static inline UnkAtkHeapHooks_l102a40* Heap() { return reinterpret_cast<UnkAtkHeapHooks_l102a40*>(&g_unk_7108b69038); }

// A command's payload for the second queue: an id and two words.
struct UnkSndIdPayload_l102a40 {
    u32 id;
    u32 a;
    u32 b;
};

static inline void Notify(UnkSndSys_l102a40* self, s32 command, u32 id, u32 a, u32 b) {
    nn::os::GetCurrentThread();
    UnkSndIdPayload_l102a40 payload = {id, a, b};
    CSoundSystemATK_PushCommandToSecondQueue(reinterpret_cast<UnkSoundSystemATK*>(self), command, &payload);
}

void fn_7100102a40(UnkSndSys_l102a40* self, const UnkSndStartRequest_l102a40* request) {
    UnkSndMap_l102a40<u32>& sounds = self->sounds;
    UnkSndNode_l102a40<u32>* node = sounds.Find(request->id);
    if (!node) {
        sounds.Add(request->id);
        node = sounds.Find(request->id);
        if (!node)
            return;
    }
    UnkSndMap_l102a40<u64>& resources = *self->resources;
    u64 key = request->resource;
    UnkSndResource_l102a40* resource = nullptr;
    if (resources.Contains(key))
        resource = static_cast<UnkSndResource_l102a40*>(resources.At(key));
    if (!resource) {
        Notify(self, 0, request->id, 1, 1);
        return;
    }
    if (node->value) {
        fn_71000f4414(node->value);
    } else {
        nn::os::GetCurrentThread();
        UnkAtkHeapHooks_l102a40* heap = Heap();
        heap->last = fn_71000813e4(&heap->allocator, 0x178, 8, 0xb);
        if (heap->on_alloc)
            heap->on_alloc(heap->on_alloc_arg, heap);
        void* sound = heap->last;
        fn_71000f43c0(sound);
        node->value = sound;
        if (!sound)
            return;
        Notify(self, 0, request->id, 1, 0);
    }
    fn_71000f4488(node->value, g_unk_7108b6b600, resource->id, resource->handle, request->field_0x1c,
                  request->field_0x20, request->field_0x18);
    if (fn_71000f4ebc(static_cast<UnkSoundStart*>(node->value), request->start_id, request->start_c,
                      request->start_a, request->start_b)) {
        Notify(self, 3, request->id, resource->id, 1);
        return;
    }
    void* sound = node->value;
    nn::os::GetCurrentThread();
    if (sound) {
        fn_71000f4414(sound);
        UnkAtkHeapHooks_l102a40* heap = Heap();
        if (heap->on_free)
            heap->on_free(heap->on_free_arg, heap);
        fn_7100081420(&heap->allocator, sound);
    }
    Notify(self, 0, request->id, 1, 1);
}
