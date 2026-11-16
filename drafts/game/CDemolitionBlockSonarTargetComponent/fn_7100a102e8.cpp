// CDemolitionBlockSonarTargetComponent unit: grows the array of 0x20-byte entries (a vtable, two counted
// references and a word) to at least n entries, copying each over and destroying the old one. Names and
// layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern char g_unk_7101a8b8a0[];

struct UnkSonarPair {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    UnkWeakHandle* first;
    UnkWeakHandle* second;
    u64 field_0x18;

    UnkSonarPair(const UnkSonarPair& other) {
        *reinterpret_cast<void**>(this) = g_unk_7101a8b8a0 + 0x10;
        first = nullptr;
        AssignRef(&first, other.first);
        second = nullptr;
        AssignRef(&second, other.second);
        field_0x18 = other.field_0x18;
    }
};
static_assert(sizeof(UnkSonarPair) == 0x20, "UnkSonarPair layout");

struct UnkSonarPairs {
    UnkSonarPair* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100a102e8(UnkSonarPairs* pairs, u32 n) {
    if (pairs->capacity >= n)
        return;
    auto* p = static_cast<UnkSonarPair*>(pairs->allocator->vf_0x40((u64)n * sizeof(UnkSonarPair), 8, pairs->flags));
    if (UnkSonarPair* src = pairs->data) {
        for (u32 i = 0; i < pairs->count; ++i) {
            new (&p[i]) UnkSonarPair(src[i]);
            src[i].vf_0x30();
        }
        pairs->allocator->vf_0x48(pairs->data);
    }
    pairs->data = p;
    pairs->capacity = n;
}
