// CCharClassFXComponent's unit: the component's destructor. Releases each of its effect paths from the
// effect cache of the lazily created model manager, frees the paths, then runs the base's destructor.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU2739.h"
#include "types.h"

using base::global::CFilePathStrId;

extern void* g_unk_7101a6f4b8[];  // the component's vtable

// An effect path and a word that starts at zero.
struct UnkFxEntry_l78f700 {
    CFilePathStrId path;
    u64 field_0x8;
};

// Grows the paths' array to hold `capacity` entries.
void fn_710078fe5c(base::global::CRntVector<UnkFxEntry_l78f700>* paths, u32 capacity);

struct UnkFXComponent_l78f700 {
    void** vtable;
    u8 pad_0x8[0x50];
    base::global::CRntVector<UnkFxEntry_l78f700> paths;  // 0x58
};

static inline UnkFxCache* GetCache() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return reinterpret_cast<UnkFxCache*>(g_unk_71726b0cd8);
}

// The entry at `index`, growing the paths to hold it (new entries get the empty path) when they may grow.
static inline UnkFxEntry_l78f700& GrowAt(base::global::CRntVector<UnkFxEntry_l78f700>& v, u32 index) {
    if (v.field_0x14 && v.count <= index) {
        u32 cap = v.capacity;
        if (cap <= index && v.step != 0)
            cap = v.GrownCapacity(index);
        fn_710078fe5c(&v, cap);
        fn_710078fe5c(&v, index + 1);
        for (u32 i = v.count; i < index + 1; ++i) {
            UnkFxEntry_l78f700* entry = &v.data[i];
            new (&entry->path) CFilePathStrId(g_unk_710162e2ed, true);
            entry->field_0x8 = 0;
        }
        v.count = index + 1;
    }
    return v.data[index];
}

void fn_710078f700(UnkFXComponent_l78f700* self) {
    self->vtable = &g_unk_7101a6f4b8[2];
    s32 count = self->paths.count;
    for (s32 i = 0; i < count; ++i) {
        UnkFxCache* cache = GetCache();
        CFilePathStrId path = GrowAt(self->paths, i).path;
        fn_71011aa5a4(cache, &path);
    }
    self->paths.Free();
    fn_71011acd74(reinterpret_cast<UnkIdObject*>(self));
}
