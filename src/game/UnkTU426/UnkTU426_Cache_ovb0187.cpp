// Unit 426 (tools/tu_map.py, no name hint): looks up a counted resource through the member at +0x8 (its first
// virtual, handing back a reference) and, when it has none, builds one (fn_71002f6ebc); a resource of the
// wanted kind (g_unk_71727b9e30) is also added to the cache's two parallel arrays (references at +0x68, ids
// at +0x70, count +0x78, capacity +0x7c, growth +0x80; fn_71002f7c08 grows them). Names and layouts are
// placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2527.h"
#include <new>

#include "types.h"

using base::global::CStrId;

struct UnkResRef_b0187q {
    UnkCountedResource* ptr;
    ~UnkResRef_b0187q();
};

struct UnkResSource_b0187q {
    virtual UnkResRef_b0187q vf_0x0(const char* const* key, u32 flags, void* extra);
};

struct UnkResKeyHolder_b0187q;
UnkCountedResource* fn_71002f6ebc(UnkResSource_b0187q* source, const char* const* key, u32 flags, void* extra);
CStrId* fn_71002ec88c(UnkCountedResource* resource);
void fn_71002f7c08(void* arrays, u32 capacity);

struct UnkResCache_b0187q {
    u8 pad_0x0[0x8];
    u8 source[0x60];  // 0x8, a polymorphic member
    u8 pad_0x68_unused[0];
    UnkCountedResource** refs;  // 0x68
    CStrId* ids;                // 0x70
    u32 count;                  // 0x78
    u32 capacity;               // 0x7c
    u32 step;                   // 0x80
};

UnkResRef_b0187q fn_71002f6d18(UnkResCache_b0187q* self, const char* const* key, u32 flags, void* extra) {
    UnkResSource_b0187q* source = reinterpret_cast<UnkResSource_b0187q*>(self->source);
    UnkResRef_b0187q result = source->vf_0x0(key, flags, extra);
    if (result.ptr)
        return result;
    UnkCountedResource* created = fn_71002f6ebc(source, key, flags, extra);
    if (result.ptr != created) {
        if (created)
            ++created->ref_count;
        UnkCountedResource* old = result.ptr;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        result.ptr = created;
    }
    if (!created)
        return result;
    if (*reinterpret_cast<const u64*>(key) != *reinterpret_cast<const u64*>(g_unk_71727b9e30))
        return result;
    ++created->ref_count;
    CStrId* id = fn_71002ec88c(created);
    if (self->count == self->capacity)
        fn_71002f7c08(&self->refs, self->step + self->count);
    CStrId* slot = &self->ids[self->count];
    base::global::UnkStringData* d = id->data;
    __builtin_memcpy(slot, &d, sizeof(d));
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
    UnkCountedResource** refs = self->refs;
    u32 n = self->count;
    refs[n] = nullptr;
    ++created->ref_count;
    UnkCountedResource* old = refs[n];
    if (old && --old->ref_count == 0)
        fn_710001737c(old);
    refs[n] = created;
    self->count++;
    if (--created->ref_count == 0)
        fn_710001737c(created);
    return result;
}
