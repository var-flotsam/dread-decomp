// Unit 56 (tools/tu_map.py; base::global::CRntFile's neighbours): opens a file through the registry
// with a mode string ("w", "a", "b"); one opened for reading only reads the opened object's contents
// in place, any other is copied into a block of the file's own and the opened object released.
// Names and layouts are placeholders.
#include <string.h>

#include "base/UnkFileReader.h"
#include "base/UnkRegistry.h"
#include "base/UnkStrPair.h"
#include "base/animation/CAnimationResource.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "types.h"

// The object the registry opens: its contents and their size.
struct UnkOpenedFile_l07c1ac {
    virtual ~UnkOpenedFile_l07c1ac();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void* vf_0x28();  // the contents
    virtual u64 vf_0x30();    // their size
};

struct UnkFileOpener_l07c1ac {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual bool vf_0x28(const UnkStrPair_0x10* path, UnkOpenedFile_l07c1ac** out, bool binary, u64 size, u32 align,
                         u32 tag);
};

// CRntFile with the members the opener sets.
struct UnkFile_l07c1ac {
    base::UnkAllocator* allocator;  // 0x0
    u64 position;                   // 0x8
    u32 alloc_flags;                // 0x10
    void* data;                     // 0x18
    u64 size;                       // 0x20
    UnkOpenedFile_l07c1ac* opened;  // 0x28
    u8 alloc_align;                 // 0x30
    u8 swap;                        // 0x31
    u8 in_place;                    // 0x32: reads the opened object's contents
    u8 binary;                      // 0x33
    u8 is_open;                     // 0x34

    base::UnkAllocator* Allocator() {
        if (!allocator)
            allocator = fn_7100083990();
        return allocator;
    }
};

bool fn_710007c1ac(UnkFileReader* reader, const UnkStrPair_0x10* path, const base::global::CRntString* mode) {
    UnkFile_l07c1ac* self = reinterpret_cast<UnkFile_l07c1ac*>(reader);
    if (self->is_open)
        return false;

    self->in_place = !fn_710007fb9c(mode, "w") && !fn_710007fb9c(mode, "a");
    self->binary = fn_710007fb9c(mode, "b");

    UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
    if (!reg) {
        reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(reg);
        g_unk_7101d4b5a0 = reg;
    }
    if (!reinterpret_cast<UnkFileOpener_l07c1ac*>(reg)->vf_0x28(path, &self->opened, self->binary != 0, 0, 8, 3))
        return false;

    if (self->in_place) {
        self->data = self->opened->vf_0x28();
        self->size = self->opened->vf_0x30();
    } else {
        u64 size = self->opened->vf_0x30();
        void* data;
        if (base::UnkAllocator* allocator = self->Allocator()) {
            data = allocator->vf_0x40(size, self->alloc_align, self->alloc_flags);
            self->data = data;
            if (data) {
                self->size = size;
                self->position = 0;
            }
        } else {
            data = nullptr;
            self->data = nullptr;
        }
        memcpy(data, self->opened->vf_0x28(), self->opened->vf_0x30());
        if (self->opened) {
            delete self->opened;
            self->opened = nullptr;
        }
        self->in_place = false;
    }
    return true;
}
