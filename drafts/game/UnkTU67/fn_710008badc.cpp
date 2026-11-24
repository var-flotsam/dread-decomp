// UnkTU67: reads the pack set table from an opened file: for each set, its name and the paths it
// lists. Each set becomes a node of the owner's chained hash map, holding the indices of its paths in
// the owner's path array (a path is added there the first time it is seen). Names and layouts are
// placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkResPathVec.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;

struct UnkOpenedFile;

// The opened table file: its contents and their size.
struct UnkPackSetTableFile {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual const u8* Data();  // vf_0x28
    virtual s64 Size();        // vf_0x30
};

// A cursor over the file's contents.
struct UnkTableReader {
    UnkPackSetTableFile* file;
    const u8* cur;
    const u8* end;

    bool ReadS32(s32& out) {
        bool ok = cur + 4 <= end;
        s32 value = 0;
        if (ok) {
            value = *reinterpret_cast<const s32*>(cur);
            cur += 4;
        }
        out = value;
        return ok;
    }
};

// Copies `size` bytes from the reader into `out`; false when fewer are left.
bool fn_71000812ac(UnkTableReader* reader, char* out, s64 size);
// Sets up the map's storage: `buckets` buckets and a pool of `nodes` nodes.
void fn_710008c020(char* map, u32 buckets, u32 nodes);

using UnkPathIdMember = base::UnkStrIdMemberOf<g_unk_7101d47200>;
UnkPathIdMember* fn_710008c174(base::global::CRntVector<UnkPathIdMember>*, u32);

// A pack set: its name and the indices of its paths.
struct UnkPathSetNode {
    CFilePathStrId key;
    CRntVector<u32> indices;  // 0x8
    UnkPathSetNode* next;     // 0x30
    UnkPathSetNode* prev;     // 0x38
    UnkPathSetNode* chain;    // 0x40

    explicit UnkPathSetNode(const CFilePathStrId& name) : key(name), indices(3), next(nullptr), prev(nullptr) {}
};
static_assert(sizeof(UnkPathSetNode) == 0x48, "UnkPathSetNode layout");

struct UnkPathSetTable {
    base::IAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    UnkPathSetNode** buckets;       // 0x10
    u32 bucket_count;               // 0x18
    u32 bucket_capacity;            // 0x1c
    u8 pad_0x20[0x18];
    UnkPathSetNode* tail;  // 0x38
    UnkPathSetNode* head;  // 0x40
    u8 pad_0x48[0x40];
    UnkResPathVec paths;  // 0x88

    // Adds a node for `name` at the front of its bucket and the end of the node list.
    UnkPathSetNode* Insert(const CFilePathStrId& name) {
        UnkPathSetNode** bucket = bucket_count ? &buckets[name.Hash() % bucket_capacity] : nullptr;
        UnkPathSetNode* chain = *bucket;
        UnkPathSetNode* node =
            new (allocator->vf_0x40(sizeof(UnkPathSetNode), 8, flags)) UnkPathSetNode(name);
        node->chain = chain;
        *bucket = node;
        if (!head) {
            head = node;
        } else if (tail) {
            node->prev = tail;
            tail->next = node;
        }
        ++count;
        tail = node;
        return node;
    }

    // The index of `path` in the path array, appending it when it is not there yet.
    s32 FindOrAddPath(const CFilePathStrId& path) {
        s32 n = paths.count;
        s32 i = 0;
        bool found = false;
        while (i < n && !found) {
            found = fn_710008c174(reinterpret_cast<CRntVector<UnkPathIdMember>*>(&paths), i)->data == path.data;
            if (!found)
                ++i;
        }
        if (!found) {
            i = n;
            u32 size = paths.count;
            u32 capacity = paths.capacity;
            if (capacity <= size && paths.step != 0)
                capacity = (size / paths.step + 1) * paths.step;
            if ((s32)size >= (s32)capacity)
                for (;;) {
                }
            CAsset_ReserveFilePathIdVec(&paths, capacity);
            new (&paths.data[paths.count]) CFilePathStrId(path);
            ++paths.count;
        }
        return i;
    }
};

bool fn_710008badc(void* base_object, UnkOpenedFile* opened) {
    UnkPathSetTable* self = static_cast<UnkPathSetTable*>(base_object);
    UnkPackSetTableFile* file = reinterpret_cast<UnkPackSetTableFile*>(opened);
    UnkTableReader reader;
    reader.file = file;
    const u8* data = file->Data();
    s64 size = file->Size();
    reader.end = data + size;
    reader.cur = data + 8;  // past the header
    s32 set_count;
    bool ok = reader.ReadS32(set_count);
    fn_710008c020(reinterpret_cast<char*>(self), set_count, set_count);
    char name[256];
    for (s32 i = 0; ok && i < set_count; ++i) {
        s32 length;
        ok = reader.ReadS32(length) && fn_71000812ac(&reader, name, length);
        name[length] = '\0';
        CFilePathStrId set_name(name, true);
        s32 path_count;
        reader.ReadS32(path_count);
        UnkPathSetNode* node = self->Insert(set_name);
        node->indices.Reserve(path_count);
        for (s32 j = 0; ok && j < path_count; ++j) {
            ok = reader.ReadS32(length) && fn_71000812ac(&reader, name, length);
            name[length] = '\0';
            CFilePathStrId path(name, true);
            node->indices.PushBack(self->FindOrAddPath(path));
        }
    }
    return ok;
}
