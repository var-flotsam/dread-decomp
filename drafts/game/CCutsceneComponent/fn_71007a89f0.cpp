// CCutsceneComponent's unit: appends a new, empty entry to the list at 0xd0 and returns it. When the
// component has a source (0x200), the entry takes it and gets that many zeroed bytes.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CCutsceneComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CRntVector;
using base::global::UnkStringData;

struct UnkCutsceneEntryRef_ovb0077;
void CCutsceneComponent_PushBackActorEntryCopy(CRntVector<UnkCutsceneEntryRef_ovb0077>* self, const UnkCutsceneEntryRef_ovb0077* value);

extern void* g_unk_7101a700a0[];

struct UnkCutsceneSource_l7a89f0 {
    u8 pad_0x0[0x130];
    u32 size;  // 0x130
};

struct UnkCutsceneCallbacks_l7a89f0 {
    void** vtable = g_unk_7101a20950 + 2;
    CRntVector<void*> list{15};  // 0x8
    u32 field_0x30 = 0;
    u64 field_0x38 = 0;
    UnkCutsceneSource_l7a89f0* source = nullptr;  // 0x40
    u8 field_0x48 = 0;
    u8 field_0x49 = 1;
    u8 field_0x4a = 1;
};

struct UnkCutsceneEntry_l7a89f0 {
    void** vtable = g_unk_7101a700a0 + 2;
    CRntVector<u8> bytes{15};  // 0x8
    UnkEmptyStrId name;  // 0x30
    UnkCutsceneCallbacks_l7a89f0 callbacks;  // 0x38
};
static_assert(sizeof(UnkCutsceneEntry_l7a89f0) == 0x88, "layout");

struct UnkCutsceneComponent_l7a89f0 {
    u8 pad_0x0[0xd0];
    CRntVector<UnkCutsceneEntry_l7a89f0> entries;  // 0xd0
    u8 pad_0xf8[0x108];
    UnkCutsceneSource_l7a89f0* source;  // 0x200
};
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7a89f0, source) == 0x200, "layout");

static inline UnkCutsceneEntry_l7a89f0& Back(CRntVector<UnkCutsceneEntry_l7a89f0>& v) {
    UnkCutsceneEntry_l7a89f0* end = &v.data[v.count];
    return v.count ? end[-1] : *end;
}

// Sets the array's count, zero-filling new bytes.
static inline void Resize(CRntVector<u8>& v, u32 n) {
    if (v.count < n) {
        v.Reserve(n);
        __builtin_memset(&v.data[v.count], 0, n - v.count);
        v.count = n;
    } else if (v.count > n) {
        v.count = n;
    }
}

UnkCutsceneEntry_l7a89f0* fn_71007a89f0(UnkCutsceneComponent_l7a89f0* self) {
    UnkCutsceneEntry_l7a89f0 entry = UnkCutsceneEntry_l7a89f0();
    CCutsceneComponent_PushBackActorEntryCopy(reinterpret_cast<CRntVector<UnkCutsceneEntryRef_ovb0077>*>(&self->entries),
                  reinterpret_cast<const UnkCutsceneEntryRef_ovb0077*>(&entry));
    fn_71007a8eec(&entry);
    if (self->source) {
        Back(self->entries).callbacks.source = self->source;
        u32 n = self->source->size;
        Resize(Back(self->entries).bytes, n);
        UnkCutsceneEntry_l7a89f0& last = Back(self->entries);
        for (u32 i = 0; i < n; ++i)
            last.bytes.GrowAt(i) = 0;
    }
    return &Back(self->entries);
}
