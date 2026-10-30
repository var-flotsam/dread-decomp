// Translation unit 2572 of data/tu_map.csv (0x7100eca4ec-0x7100ecbddc, no class hint): a group of
// loaded resources: a chained hash set of scene objects at +8 (the iteration state at +0x30/+0x38
// and a change count at +0x50, as base/UnkHashMap.h's chained sets have them), another container
// at +0x58, the file-path ids of files it loaded at +0xa8 and a directory path at +0xd0 whose files
// it loaded. The types of the file device's list repeat src/game/CGameManager.cpp's.
// Holds fn_7100ecb00c (releases everything). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// The file device singleton (created on first use; src/game/CGameManager.cpp).
extern base::file::UnkFileDevice* g_unk_7101d4d828;
static inline base::file::UnkFileDevice* FileDevice() {
    if (!g_unk_7101d4d828) {
        auto* device = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(device);
        g_unk_7101d4d828 = device;
    }
    return g_unk_7101d4d828;
}

// Unloads the file `path` (same unit as the device, not decompiled yet).
void fn_710008987c(base::file::UnkFileDevice* self, const CFilePathStrId* path, u32 flags);

// The pooled data behind a string id as its text accessor sees it (src/game/CGameManager.cpp).
struct UnkIdTextData {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    union {
        s32 reads;
    };
};

// A path id whose release decrements and tests the count in two steps (src/game/CGameManager.cpp).
struct UnkListedPathId {
    UnkStringData* data;

    ~UnkListedPathId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// The same release for a path id that stays (it is left the empty id).
static inline void ResetPath(CFilePathStrId* id) {
    UnkStringData*& data = id->data;
    if (UnkStringPool* pool = g_unk_7101d47200) {
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = g_unk_7101d47200;
            }
        }
        data = &pool->empty;
    }
}

static inline const char* IdText(UnkListedPathId& id) {
    ++reinterpret_cast<UnkIdTextData*>(id.data)->reads;
    const CRntString& str = reinterpret_cast<UnkIdTextData*>(id.data)->str;
    return str.data ? str.data : g_unk_710162e2ed;
}

struct UnkListedPath {
    UnkListedPathId id;
    UnkListedPath* next;
};
extern void* g_unk_7101a1b030[];
// A list of path ids (its destructor is fn_7100087cf8).
struct UnkFilePathList {
    void** vtable;
    u32 count;
    UnkListedPath* head;
    UnkListedPath* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkFilePathList() {
        vtable = &g_unk_7101a1b030[2];
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkFilePathList() {
        vtable = &g_unk_7101a1b030[2];
        for (UnkListedPath* n = head; n;) {
            UnkListedPath* next = n->next;
            n->id.~UnkListedPathId();
            allocator->vf_0x48(n);
            n = next;
        }
    }
};
// Lists the files under `path` into `out` (src/game/CGameManager.cpp).
void fn_7100089eec(base::file::UnkFileDevice* self, const CFilePathStrId* path, UnkFilePathList* out);

// A scene object of the set: the render manager's node is its base at +8.
struct UnkItemBase_71011a91c4;

struct UnkResourceObject {
    void* vtable;
};

struct UnkResourceSetNode {
    u64 field_0x0;
    UnkResourceObject* value;
    UnkResourceSetNode* next;
};

struct UnkResourceSet {
    base::UnkVector<UnkResourceSetNode*> buckets;
    u32 first_bucket;  // where an iteration starts
    UnkResourceSetNode* first;
    u8 pad_0x38[0x10];
    u32 changes;  // counts removals: an iteration stops when it moves
};

struct UnkResourceGroup {
    u8 pad_0x0[0x8];
    UnkResourceSet objects;
    u8 others[0x50];
    base::UnkVector<UnkListedPathId> files;
    CFilePathStrId directory;
};
static_assert(__builtin_offsetof(UnkResourceGroup, objects.first_bucket) == 0x30, "UnkResourceGroup placeholder layout");
static_assert(__builtin_offsetof(UnkResourceGroup, objects.changes) == 0x50, "UnkResourceGroup placeholder layout");
static_assert(__builtin_offsetof(UnkResourceGroup, others) == 0x58, "UnkResourceGroup placeholder layout");
static_assert(__builtin_offsetof(UnkResourceGroup, files) == 0xa8, "UnkResourceGroup placeholder layout");
static_assert(__builtin_offsetof(UnkResourceGroup, directory) == 0xd0, "UnkResourceGroup placeholder layout");

// An iteration over the set, in bucket order.
struct UnkResourceSetIter {
    UnkResourceSet* set;
    UnkResourceSetNode* node;
    u32 bucket;
    u32 changes;

    explicit UnkResourceSetIter(UnkResourceSet* s)
        : set(s), node(s->first), bucket(s->first_bucket), changes(s->changes) {}
    bool Changed() const { return set->changes != changes; }
    void Next() {
        node = node->next;
        while (!node) {
            if (++bucket >= set->buckets.count)
                return;
            node = set->buckets.data[bucket];
        }
    }
};

// Clear the set and the container at +0x58 (same unit, not decompiled yet).
void fn_7100ecb3a4(UnkResourceSet* self);
void fn_7100ecb4ac(void* self);

// Hands every scene object of the set back to the render manager's pool (stopping if that changes
// the set), clears both containers, unloads the files and then the directory's files, and drops
// the ids.
// NON_MATCHING(minor): block layout of the first loop's entry: the original keeps the loop's null
// test on its back edges (redundant there) and does not peel the first load of the body into the
// preheader; the same loads, calls and exits.
void fn_7100ecb00c(UnkResourceGroup* self) {
    for (UnkResourceSetIter it(&self->objects); it.node; it.Next()) {
        UnkResourceObject* obj = it.node->value;
        fn_71002199d8(g_unk_7101d47300,
                      obj ? reinterpret_cast<UnkItemBase_71011a91c4*>(reinterpret_cast<char*>(obj) + 8) : nullptr, 0);
        if (it.Changed())
            break;
    }
    fn_7100ecb3a4(&self->objects);
    fn_7100ecb4ac(self->others);
    for (UnkListedPathId* it = self->files.data, *end = it + self->files.count; it != end; ++it)
        fn_710008987c(FileDevice(), reinterpret_cast<CFilePathStrId*>(it), 0);
    self->files.Clear();
    if (self->directory.data != &g_unk_7101d47200->empty) {
        UnkFilePathList list;
        fn_7100089eec(FileDevice(), &self->directory, &list);
        for (UnkListedPath* n = list.head; n; n = n->next) {
            base::file::UnkFileDevice* device = FileDevice();
            CFilePathStrId id(IdText(n->id), true);
            fn_710008987c(device, &id, 0);
        }
        ResetPath(&self->directory);
    }
}
