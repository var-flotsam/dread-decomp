// Unit 99 of tools/tu_map.py (base::snd::CSoundSystemATK's range): plays a sound of the second id map,
// creating it on the engine heap on first use, with up to eight tracks looked up by key, and tells the
// sound thread the outcome. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntVector.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "game/UnkTU58.h"
#include "game/UnkTU99.h"

using base::snd::UnkSoundSystemATK;

struct UnkSoundCommand;
struct UnkSoundStart;

void CSoundSystemATK_PushCommandToSecondQueue(UnkSoundSystemATK* sys, s32 command, const void* payload);
void fn_71000f4414(void* sound);
void fn_71000f43c0(void* sound);
void fn_71000f44cc(void* self, void* target, UnkSoundCommand* command);
bool fn_71000f4ebc(UnkSoundStart* self, u32 id, u32 c, f32 a, f32 b);
extern u8 g_unk_7108b6b600[] __attribute__((visibility("hidden")));

template <class K>
struct UnkPlayNode_l102d50 {
    K key;                       // 0x0
    void* value;                 // 0x8
    UnkPlayNode_l102d50* next;   // 0x10: the list of all nodes
    UnkPlayNode_l102d50* prev;   // 0x18
    UnkPlayNode_l102d50* chain;  // 0x20: the next node of the bucket

    UnkPlayNode_l102d50*& Prev() { return prev; }
};

template <class K>
struct UnkPlayMap_l102d50 {
    typedef UnkPlayNode_l102d50<K> Node;

    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    Node** buckets;                 // 0x10
    u32 used;                       // 0x18
    u32 bucket_count;               // 0x1c
    u8 pad_0x20[0x18];
    Node* tail;  // 0x38
    Node* head;  // 0x40

    Node** Slot(K key) { return static_cast<u32>(key) % bucket_count + buckets; }

    Node* Find(K key) {
        if (!used)
            return nullptr;
        Node** bucket = Slot(key);
        if (!bucket)
            return nullptr;
        for (Node* n = *bucket; n; n = n->chain) {
            if (n->key == key)
                return n;
        }
        return nullptr;
    }

    // Adds a node for `key` (its value null) at the head of its bucket and the end of the list.
    void Add(const K& key) {
        Node** bucket = used ? Slot(key) : nullptr;
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
        if (!head) {
            head = n;
        } else if (tail) {
            n->Prev() = tail;
            tail->next = n;
        }
        tail = n;
        ++count;
    }
};

// What a track key maps to: the track's id at +0x8.
struct UnkPlayBank_l102d50 {
    u64 field_0x0;
    u32 id;
};

struct UnkPlaySys_l102d50 {
    u8 pad_0x0[0x4007d0];
    UnkPlayMap_l102d50<u32> sounds;  // 0x4007d0
    u8 pad_0x400818[0x400880 - 0x400818];
    UnkPlayMap_l102d50<u64>* banks;  // 0x400880
};
static_assert(__builtin_offsetof(UnkPlaySys_l102d50, banks) == 0x400880, "layout");

// The engine heap (g_unk_7108b69038): its sub-allocator and the hooks run after an allocation and
// before a free.
struct UnkPlayHeap_l102d50 {
    u64 field_0x0;
    base::UnkSubAllocator allocator;  // 0x8
    u8 heap[8];                       // 0x40
    void* last;                       // 0x48
    void (*alloc_hook)(void* arg, UnkPlayHeap_l102d50* heap);
    void* alloc_hook_arg;
    void (*free_hook)(void* arg, UnkPlayHeap_l102d50* heap);
    void* free_hook_arg;
};
static_assert(__builtin_offsetof(UnkPlayHeap_l102d50, free_hook_arg) == 0x68, "layout");

struct UnkPlayTrackRequest_l102d50 {
    u64 key;
    u32 field_0x8;
    s32 field_0xc;
    s32 field_0x10;
    u32 pad_0x14;
};

struct UnkPlayRequest_l102d50 {
    u32 id;
    u32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    u32 field_0x10;
    u32 track_count;
    UnkPlayTrackRequest_l102d50 tracks[8];
};

#pragma pack(push, 4)
struct UnkPlayTrack_l102d50 {
    u32 id;
    u32 field_0x4;
    u64 key;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
};
#pragma pack(pop)

struct UnkPlayTracks_l102d50 {
    UnkPlayTracks_l102d50() : count(0) {}

    u32 count;
    UnkPlayTrack_l102d50 entries[8];
};

struct UnkPlayPayload_l102d50 {
    u32 id;
    u32 a;
    u32 b;
};

static inline UnkPlayHeap_l102d50* Heap() { return reinterpret_cast<UnkPlayHeap_l102d50*>(&g_unk_7108b69038); }

static inline void Send(UnkPlaySys_l102d50* self, s32 command, u32 id, u32 a, u32 b) {
    nn::os::GetCurrentThread();
    UnkPlayPayload_l102d50 payload = {id, a, b};
    CSoundSystemATK_PushCommandToSecondQueue(reinterpret_cast<UnkSoundSystemATK*>(self), command, &payload);
}

void fn_7100102d50(UnkPlaySys_l102d50* self, const UnkPlayRequest_l102d50* request) {
    typedef UnkPlayNode_l102d50<u32> Node;
    UnkPlayMap_l102d50<u32>& sounds = self->sounds;
    Node* node = sounds.Find(request->id);
    if (!node) {
        sounds.Add(request->id);
        node = sounds.Find(request->id);
        if (!node)
            return;
    }

    if (void* sound = node->value) {
        fn_71000f4414(sound);
    } else {
        nn::os::GetCurrentThread();
        UnkPlayHeap_l102d50* heap = Heap();
        heap->last = fn_71000813e4(&heap->allocator, 0x178, 8, 0xb);
        if (heap->alloc_hook)
            heap->alloc_hook(heap->alloc_hook_arg, heap);
        sound = heap->last;
        fn_71000f43c0(sound);
        node->value = sound;
        if (!sound)
            return;
        Send(self, 0, request->id, 1, 0);
    }

    u32 n = request->track_count;
    if (n == 0)
        return;
    UnkPlayTracks_l102d50 tracks;
    if (n > 8)
        return;
    s32 first = -1;
    UnkPlayMap_l102d50<u64>* banks = self->banks;
    for (u32 i = 0; i < n; ++i) {
        const UnkPlayTrackRequest_l102d50& req = request->tracks[i];
        u64 key = req.key;
        UnkPlayBank_l102d50* bank = nullptr;
        if (banks->Find(key)) {
            UnkPlayNode_l102d50<u64>* q = *banks->Slot(key);
            while (q->key != key)
                q = q->chain;
            bank = static_cast<UnkPlayBank_l102d50*>(q->value);
        }
        if (!bank) {
            Send(self, 0, request->id, 1, 1);
            return;
        }
        UnkPlayTrack_l102d50& track = tracks.entries[i];
        tracks.count = i + 1;
        track = UnkPlayTrack_l102d50();
        track.key = key;
        track.id = bank->id;
        track.field_0x18 = req.field_0x8;
        track.field_0x10 = req.field_0xc < 0 ? 0 : req.field_0xc;
        track.field_0x14 = req.field_0x10 < 0 ? 0 : req.field_0x10;
        if (first == -1)
            first = bank->id;
    }

    fn_71000f44cc(node->value, g_unk_7108b6b600, reinterpret_cast<UnkSoundCommand*>(&tracks));
    if (fn_71000f4ebc(static_cast<UnkSoundStart*>(node->value), request->field_0x4, request->field_0x10,
                      request->field_0x8, request->field_0xc)) {
        Send(self, 3, request->id, first, 1);
        return;
    }
    void* sound = node->value;
    nn::os::GetCurrentThread();
    if (sound) {
        fn_71000f4414(sound);
        UnkPlayHeap_l102d50* heap = Heap();
        if (heap->free_hook)
            heap->free_hook(heap->free_hook_arg, heap);
        fn_7100081420(&heap->allocator, sound);
    }
    Send(self, 0, request->id, 1, 1);
}
