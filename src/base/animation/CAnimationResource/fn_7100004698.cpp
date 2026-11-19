// Unit 11 (base::animation::CAnimationResource hint): builds a record of a string id and three
// vectors from a list of entries: the entries themselves, their non-empty second ids, and all of
// their listed ids. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdEntry_004698 {
    CStrId field_0x0;
    CStrId field_0x8;
    base::UnkStrIdVec ids;  // 0x10
};
static_assert(sizeof(UnkIdEntry_004698) == 0x38, "UnkIdEntry_004698 size");

struct UnkIdEntryVec_004698;
void fn_7100005534(UnkIdEntryVec_004698* self, u32 capacity);

// A vector of entries (CRntVector's layout) whose Reserve is out of line.
struct UnkIdEntryVec_004698 {
    UnkIdEntry_004698* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Reserve(u32 n) { fn_7100005534(this, n); }
    void PushBack(const UnkIdEntry_004698& v) {
        u32 n = count;
        u32 c = capacity;
        if (c <= n && step != 0)
            c = (n / step + 1) * step;
        if ((s32)n >= (s32)c)
            for (;;) {
            }
        Reserve(c);
        new (&data[count]) UnkIdEntry_004698(v);
        ++count;
    }
};

struct UnkIdEntryList_004698 {
    CStrId name;
    UnkIdEntryVec_004698 entries;  // 0x8
    base::UnkStrIdVec seconds;     // 0x30
    base::UnkStrIdVec ids;         // 0x58
};

void fn_7100004698(UnkIdEntryList_004698* self, const char* name, const UnkIdEntry_004698* entries, u64 count) {
    new (&self->name) CStrId(name, true);

    self->entries.data = nullptr;
    self->entries.count = 0;
    self->entries.capacity = 0;
    self->entries.step = 10;
    self->entries.field_0x14 = true;
    self->entries.flags = 0xf;
    self->entries.allocator = fn_7100083990();
    self->entries.Reserve(count);

    self->seconds.data = nullptr;
    self->seconds.count = 0;
    self->seconds.capacity = 0;
    self->seconds.step = 10;
    self->seconds.field_0x14 = true;
    self->seconds.flags = 0xf;
    self->seconds.allocator = fn_7100083990();
    self->seconds.Reserve(count);

    self->ids.data = nullptr;
    self->ids.count = 0;
    self->ids.capacity = 0;
    self->ids.step = 10;
    self->ids.field_0x14 = true;
    self->ids.flags = 0xf;
    self->ids.allocator = fn_7100083990();

    for (const UnkIdEntry_004698* e = entries; e != entries + count; ++e) {
        self->entries.PushBack(*e);
        if (e->field_0x8.data != &g_unk_7101d47108->empty)
            self->seconds.PushBack(e->field_0x8);
        int n = e->ids.count;
        for (int i = 0; i < n; ++i)
            self->ids.PushBack(e->ids.data[i]);
    }
}

