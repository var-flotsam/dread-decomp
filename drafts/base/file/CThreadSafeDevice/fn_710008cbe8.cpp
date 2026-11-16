// CThreadSafeDevice (unit 68): loads a pack's index from its file: two header words and a count,
// then that many entries (a 64-bit key and the begin and end offsets of its data), each put into
// the reader's chained set. On a short file the set is emptied again. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkHashMap.h"
#include "base/file/UnkFileCacheSet.h"
#include "types.h"

char** fn_710008d0c4(char* set, u32 index);
void fn_710008cef8(UnkFileCacheSet* self, u32 buckets, u32 nodes);

namespace {

// An index entry as the set keeps it: the key, where its data starts and how long it is.
struct UnkPackIndexNode_l08cbe8 {
    u64 key;
    s64 offset;
    s64 size;
    UnkPackIndexNode_l08cbe8* next;

    UnkPackIndexNode_l08cbe8(u64 key, s64 offset, s64 size) : key(key), offset(offset), size(size), next(nullptr) {}
};

// The file the index is read from: its contents and their size.
struct UnkPackIndexFile_l08cbe8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual const u8* vf_0x28();
    virtual s64 vf_0x30();
};

struct UnkPackIndex_l08cbe8 {
    bool loaded;
    UnkFileCacheSet set;
    u32 field_0x98;
    u32 field_0x9c;
};
static_assert(__builtin_offsetof(UnkPackIndex_l08cbe8, field_0x98) == 0x98, "UnkPackIndex layout");

struct UnkPackIndexTag_l08cbe8;
struct UnkPackIndexKey_l08cbe8 {};

// Reads values one after another, failing once the next would pass the end.
struct UnkBlobReader_l08cbe8 {
    const u8* pos;
    const u8* end;

    template <class T>
    bool Read(T* out) {
        if (pos + sizeof(T) > end)
            return false;
        *out = *reinterpret_cast<const T*>(pos);
        pos += sizeof(T);
        return true;
    }
};

}  // namespace

template <>
struct base::UnkChainedSetTraits<UnkPackIndexTag_l08cbe8> {
    static char** bucket(char* self, u32 i) { return fn_710008d0c4(self, i); }
};

namespace {

// Empties the set and marks the index unloaded.
inline void Reset(UnkPackIndex_l08cbe8* self) {
    base::refl_ChainedSetClear<UnkPackIndexTag_l08cbe8, UnkPackIndexKey_l08cbe8, 0x18>(
        reinterpret_cast<char*>(&self->set));
    self->loaded = false;
}

// Adds an entry to the set, keeping track of the lowest non-empty bucket and its first node.
inline void Insert(UnkFileCacheSet* set, u64 key, s32 begin, s32 end) {
    ++set->version;
    void* mem = set->node_allocator->vf_0x40(sizeof(UnkPackIndexNode_l08cbe8), 8, set->node_flags);
    auto* node = new (mem) UnkPackIndexNode_l08cbe8(key, begin, end - begin);
    u32 index = set->buckets.count ? static_cast<u32>(key) % set->buckets.capacity : u32(-1);
    u32 first = set->first_bucket;
    set->first_bucket = index < first ? index : first;
    if (index <= first)
        set->first_node = reinterpret_cast<UnkFileCacheSetNode*>(node);
    auto** bucket = reinterpret_cast<UnkPackIndexNode_l08cbe8**>(fn_710008d0c4(reinterpret_cast<char*>(set), index));
    node->next = *bucket;
    *bucket = node;
    ++set->node_count;
}

}  // namespace

void fn_710008cbe8(void* reader, void* file) {
    auto* self = static_cast<UnkPackIndex_l08cbe8*>(reader);
    auto* f = static_cast<UnkPackIndexFile_l08cbe8*>(file);
    if (self->loaded)
        Reset(self);
    const u8* data = f->vf_0x28();
    s64 size = f->vf_0x30();
    UnkBlobReader_l08cbe8 r{data, data + size};
    s32 count = 0;
    bool ok = r.Read(&self->field_0x98) && r.Read(&self->field_0x9c) && r.Read(&count);
    fn_710008cef8(&self->set, count, count);
    for (s32 i = 0; ok && i < count; ++i) {
        u64 key;
        s32 begin;
        s32 end;
        ok = r.Read(&key) && r.Read(&begin) && r.Read(&end);
        if (ok)
            Insert(&self->set, key, begin, end);
    }
    if (!ok)
        Reset(self);
    self->loaded = ok;
}
