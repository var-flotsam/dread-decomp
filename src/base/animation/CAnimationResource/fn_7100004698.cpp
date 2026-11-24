// The resource type's constructor (base/UnkResType.h): its name, a copy of each of its formats,
// each format's extension when it has one, and every format's source extensions.
#include <new>

#include "base/UnkResFormatMembers.h"
#include "base/UnkResType.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// The resource type's vector of formats (CRntVector's layout), whose Reserve is out of line.
struct UnkResFormatVec {
    UnkResFormatMembers* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    void Reserve(u32 n) { fn_7100005534(reinterpret_cast<base::global::CRntVector<UnkResFormat>*>(this), n); }
    void PushBack(const UnkResFormatMembers& v) {
        u32 n = count;
        u32 c = capacity;
        if (c <= n && step != 0)
            c = (n / step + 1) * step;
        if ((s32)n >= (s32)c)
            for (;;) {
            }
        Reserve(c);
        new (&data[count]) UnkResFormatMembers(v);
        ++count;
    }
};

// UnkResType with its members' own types.
struct UnkResTypeMembers {
    CStrId name;
    UnkResFormatVec formats;        // 0x8
    base::UnkStrIdVec extensions;  // 0x30
    base::UnkStrIdVec sources;     // 0x58
};
static_assert(sizeof(UnkResTypeMembers) == sizeof(UnkResType), "UnkResTypeMembers layout");

static inline void Construct(UnkResTypeMembers* self, const char* name, const UnkResFormatMembers* formats, u64 count) {
    new (&self->name) CStrId(name, true);

    self->formats.data = nullptr;
    self->formats.count = 0;
    self->formats.capacity = 0;
    self->formats.step = 10;
    self->formats.field_0x14 = true;
    self->formats.flags = 0xf;
    self->formats.allocator = GetDefaultAllocator();
    self->formats.Reserve(count);

    self->extensions.data = nullptr;
    self->extensions.count = 0;
    self->extensions.capacity = 0;
    self->extensions.step = 10;
    self->extensions.field_0x14 = true;
    self->extensions.flags = 0xf;
    self->extensions.allocator = GetDefaultAllocator();
    self->extensions.Reserve(count);

    self->sources.data = nullptr;
    self->sources.count = 0;
    self->sources.capacity = 0;
    self->sources.step = 10;
    self->sources.field_0x14 = true;
    self->sources.flags = 0xf;
    self->sources.allocator = GetDefaultAllocator();

    for (const UnkResFormatMembers* f = formats; f != formats + count; ++f) {
        self->formats.PushBack(*f);
        if (f->extension.data != &g_unk_7101d47108->empty)
            self->extensions.PushBack(f->extension);
        int n = f->sources.count;
        for (int i = 0; i < n; ++i)
            self->sources.PushBack(f->sources.data[i]);
    }
}

void CAnimationResource_ConstructResourceTypeFromFormats(void* self, const char* name, UnkResFormat* formats, u64 count) {
    Construct(static_cast<UnkResTypeMembers*>(self), name, reinterpret_cast<UnkResFormatMembers*>(formats), count);
}

