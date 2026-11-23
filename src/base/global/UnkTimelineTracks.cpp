// From unit 45 of tools/tu_map.py (0x710006b6a4-0x710006d2a0): a timeline's tracks
// (base::global::timeline::CTrack, its class getter fn_710006ad5c), read from a file. Each track
// is stored by its class's 64-bit id; the class is looked up, created through its descriptor and
// read in place. Names and layouts are placeholders.
// Holds fn_710006b6e4.
#include "base/global/CRntVector.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntFile.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU83.h"
#include "types.h"

// The registry of reflected types by 64-bit id (as src/base/animation/CBlendSpaceResource.cpp
// declares it; its entries are the types' descriptors).
struct UnkVersionRegistry;
struct UnkVersionEntry;

// A track: polymorphic, destroyed (and freed) through its vtable's slot 2.
struct UnkTrack_06b6e4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // deleting destructor
};

// What a type's descriptor gives: the object's size and its in-place constructor.
struct UnkTypeView {
    u8 pad_0x0[0x18];
    u64 size;
    u8 pad_0x20[0x8];
    void (*construct)(void* object);
};

// Reads the rest of a track from the file.
void fn_710006afd4(UnkTrack_06b6e4* track, base::global::CRntFile* file);

// The array's slots are written as raw words (the original's stores may alias the array's own
// fields: its count is read again after each one).
static inline void StoreSlot(UnkTrack_06b6e4** slot, UnkTrack_06b6e4* value) {
    __builtin_memcpy(slot, &value, sizeof(value));
}

// The tracks' array: owning pointers, moved when it grows.
struct UnkTrackArray : base::global::CRntVector<UnkTrack_06b6e4*> {
    void Grow(u32 n) {
        if (capacity < n) {
            auto** p = static_cast<UnkTrack_06b6e4**>(allocator->vf_0x40((u64)n * 8, 8, flags));
            if (data) {
                UnkTrack_06b6e4** src = data;
                u64 k = 0;
                for (u32 i = 0; i < count; ++i, ++k, ++src) {
                    UnkTrack_06b6e4* t = *src;
                    StoreSlot(src, nullptr);
                    StoreSlot(&p[k], t);
                    UnkTrack_06b6e4* old = *src;
                    StoreSlot(src, nullptr);
                    if (old)
                        old->vf_0x10();
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }
};

// Reads the track count, then each track: its class id, and the track itself when the class is a
// CTrack the registry can create.
void fn_710006b6e4(UnkTrackArray* self, base::global::CRntFile* file) {
    u32 n;
    CRntFile_Read(file, &n, 4, 1);
    self->Grow(n);
    for (u32 i = 0; i < n; ++i) {
        u64 raw;
        CRntFile_Read(file, &raw, 8, 1);
        UnkVersionRegistry* registry = g_unk_7101d4c8d0;
        u64 id = raw;
        auto* type = reinterpret_cast<const base::reflection::CType*>(fn_71000a5118(registry, &id));
        if (!type)
            continue;
        if (!fn_710009d50c(type, reinterpret_cast<const base::reflection::CType*>(fn_710006ad5c())))
            continue;
        const auto* view = reinterpret_cast<const UnkTypeView*>(type);
        if (!view->construct)
            continue;
        void* p = GetDefaultAllocator()->vf_0x40(view->size, 8, 0x58);
        if (!p)
            continue;
        view->construct(p);
        u32 c = self->count;
        u32 cap = self->capacity;
        if (cap <= c && self->step != 0)
            cap = self->GrownCapacity(c);
        if (!((s32)c < (s32)cap))
            for (;;) {
            }
        self->Grow(cap);
        StoreSlot(&self->data[self->count], static_cast<UnkTrack_06b6e4*>(p));
        ++self->count;
        fn_710006afd4(static_cast<UnkTrack_06b6e4*>(p), file);
    }
}
