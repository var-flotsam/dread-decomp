// engine::render::anim::CSkeletalAnimTree's unit (tools/tu_map.py unit 150): two copies of one
// evaluation pass, for owners whose fields sit 8 bytes apart (fn_710014c0b8, fn_710014bf68). The
// matrix buffer passed in is reset and refilled (fn_71000155cc) with the matrix of each 0x98-byte
// bone of the skeleton reached through +0x68/+0x70 (its +0x80), then marked full; the root node
// (first of the array at +0x10/+0x18) is evaluated into it (slot 0x38) with the busy flag at
// +0x60/+0x68 set, then every child of the array at +0xe8/+0xf0 whose weight (+0x8) is above zero
// (slot 0x10); last the skeleton's dirty flag (+0x30) is cleared. Names and layouts are placeholders.
#include <new>

#include "types.h"
#include "base/UnkVector.h"

struct UnkFixedArray_71000155cc;
struct UnkElem_0x40;
s32 fn_71000155cc(UnkFixedArray_71000155cc* self, const UnkElem_0x40* e);

struct UnkMatrix_b0119 {
    f32 m[16];
};

struct UnkMatrixBuffer_b0119 {
    u8 pad_0x0[0x8];
    UnkMatrix_b0119* begin;  // 0x8
    u8 pad_0x10[0x8];
    u32 capacity;  // 0x18
    u8 pad_0x1c[0x14];
    UnkMatrix_b0119* end;  // 0x30
};

struct UnkBone_b0119 {
    u8 pad_0x0[0x8];
    UnkMatrix_b0119* matrix;  // 0x8
    u8 pad_0x10[0x88];
};
static_assert(sizeof(UnkBone_b0119) == 0x98, "UnkBone_b0119 size");

struct UnkSkeleton_b0119 {
    u8 pad_0x0[0x8];
    UnkBone_b0119* bones;  // 0x8
    u8 pad_0x10[0x8];
    u32 count;  // 0x18
    u8 pad_0x1c[0x14];
    bool dirty;  // 0x30
};

struct UnkSkeletonHolder_b0119 {
    u8 pad_0x0[0x80];
    UnkSkeleton_b0119* skeleton;  // 0x80
};

struct UnkAnimNode_b0119 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(UnkMatrixBuffer_b0119* out);
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(UnkMatrixBuffer_b0119* out);
    f32 weight;  // 0x8
};

template <u32 Shift>
struct UnkAnimTree_b0119 {
    u8 pad_0x0[0x10 + Shift];
    UnkAnimNode_b0119** roots;  // 0x10
    u32 root_count;              // 0x18
    u8 pad_0x1c[0x44];
    bool busy;  // 0x60
    u8 pad_0x61[0x7];
    UnkSkeletonHolder_b0119* holder;  // 0x68
    u8 pad_0x70[0x78];
    UnkAnimNode_b0119** children;  // 0xe8
    u32 child_count;               // 0xf0
};

template <u32 Shift>
__attribute__((always_inline)) static inline void Evaluate(UnkAnimTree_b0119<Shift>* self,
                                                           UnkMatrixBuffer_b0119* out) {
    UnkSkeleton_b0119* skeleton = self->holder->skeleton;
    out->end = out->begin;
    u32 count = skeleton->count;
    for (u32 i = 0; i < count; ++i)
        fn_71000155cc(reinterpret_cast<UnkFixedArray_71000155cc*>(out),
                      reinterpret_cast<const UnkElem_0x40*>(skeleton->bones[i].matrix));
    out->end = out->begin + out->capacity;
    self->busy = true;
    if (self->root_count)
        self->roots[0]->vf_0x38(out);
    self->busy = false;
    UnkAnimNode_b0119** end = self->children + self->child_count;
    for (UnkAnimNode_b0119** p = self->children; p != end; ++p) {
        if ((*p)->weight > 0.0f)
            (*p)->vf_0x10(out);
    }
    self->holder->skeleton->dirty = false;
}

void fn_710014bf68(UnkAnimTree_b0119<8>* self, UnkMatrixBuffer_b0119* out) { Evaluate(self, out); }

void fn_710014c0b8(UnkAnimTree_b0119<0>* self, UnkMatrixBuffer_b0119* out) { Evaluate(self, out); }

// engine::render::anim::CSkeletalAnimTree's unit (tools/tu_map.py unit 150): fn_710014bb58 sets a
// key's flag in the chained hash map at +0xa8 (the map fn_710014bdec reads; a missing key reads as
// true): true erases the key's node (fn_710014bca8), false adds a node with the flag clear when the
// key has none, at the front of its bucket and at the end of the list of all nodes. Names and
// layouts are placeholders.
extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkMapNode_b54a1;
template <int Tag>
struct UnkChainedMap_b54a1;
UnkMapNode_b54a1* fn_710014bca8(UnkChainedMap_b54a1<4>* self, const u64* key);

struct UnkFlagNode_b0119 {
    u64 key;
    bool flag;
    UnkFlagNode_b0119* next;   // 0x10
    UnkFlagNode_b0119* prev;   // 0x18
    UnkFlagNode_b0119* chain;  // 0x20

    UnkFlagNode_b0119(u64 k) : key(k), flag(false), chain(nullptr) { next = prev = nullptr; }

    // The list links, through an accessor (its store may alias the map's ends).
    UnkFlagNode_b0119*& Prev() { return prev; }
};

struct UnkFlagMap_b0119 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkFlagNode_b0119** buckets;  // 0x10 (base::global::CRntVector)
    u32 bucket_count;             // 0x18
    u32 bucket_capacity;          // 0x1c
    u8 pad_0x20[0x18];
    UnkFlagNode_b0119* tail;  // 0x38
    UnkFlagNode_b0119* head;  // 0x40

    UnkFlagNode_b0119** At(u32 i) {
        if (!(i < bucket_count))
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return &buckets[i];
    }

    UnkFlagNode_b0119* Find(u64 key) {
        if (bucket_count == 0)
            return nullptr;
        UnkFlagNode_b0119** bucket = At((u32)key % bucket_capacity);
        if (!bucket)
            return nullptr;
        UnkFlagNode_b0119* n = *bucket;
        while (n && n->key != key)
            n = n->chain;
        return n;
    }

    void Add(u64 key) {
        UnkFlagNode_b0119** bucket = bucket_count ? At((u32)key % bucket_capacity) : nullptr;
        UnkFlagNode_b0119* first = *bucket;
        UnkFlagNode_b0119* n = static_cast<UnkFlagNode_b0119*>(allocator->vf_0x40(sizeof(UnkFlagNode_b0119), 8, flags));
        if (n)
            new (n) UnkFlagNode_b0119(key);
        n->chain = first;
        *bucket = n;
        if (head) {
            if (tail) {
                n->Prev() = tail;
                tail->next = n;
            }
        } else {
            head = n;
        }
        tail = n;
        ++count;
    }
};

struct UnkFlagMapOwner_b0119 {
    u8 pad_0x0[0xa8];
    UnkFlagMap_b0119 map;  // 0xa8
};

void fn_710014bb58(UnkFlagMapOwner_b0119* self, u64 key, bool set) {
    UnkFlagNode_b0119* n = self->map.Find(key);
    if (set) {
        u64 k = key;
        fn_710014bca8(reinterpret_cast<UnkChainedMap_b54a1<4>*>(&self->map), &k);
        return;
    }
    if (!n)
        self->map.Add(key);
}
