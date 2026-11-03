// base::file::CPackManagerDevice's unit (tools/tu_map.py unit 66), more of the device of
// src/base/file/CPackManagerDevice.cpp (its packs at +0x30, count at +0x38, the mutex at +0x68): the
// lookup of a mounted pack by its path id (a pack's id at +0x18), used under the mutex, a setter of a
// pack's flag, the mount and the unmount. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkRegistry.h"
#include "base/UnkVector.h"
#include "types.h"


struct UnkPackPathKey {
    u64 hash;
};

struct UnkMountedPack {
    virtual void vf_0x0();
    virtual void vf_0x8();  // deleting destructor
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual u32 vf_0x20();  // its reference count
    u8 unk_0x8[0x10];
    UnkPackPathKey path;  // +0x18
    u8 unk_0x20[0xa8];
    bool locked;  // +0xc8
    u8 pad_0xc9[3];
    s32 file_refs;  // +0xcc
};

// Closes (or reopens) a pack's file.
void fn_710008918c(UnkMountedPack* pack);

struct UnkPackManagerView;
// A pack's constructor over an opened file, and the device's append of a pack.
void fn_7100088dfc(UnkMountedPack* pack, UnkPackManagerView* device, const UnkPackPathKey* path, void* file);
void fn_710008abd4(UnkPackManagerView* self, UnkMountedPack* pack);

extern u32 g_unk_7101a1b050;
// The registry that reports an open's failure (src/game/CSubAreaManager.cpp creates it the same way).
extern UnkRegistry_0x70* g_unk_7101d4b5a0;
void fn_7100087dc8(UnkRegistry_0x70* self, void* entry, u32 kind);

struct UnkPackCallback {
    void* object;
    void (*function)(void* object);
};

// The device the packs are read from: its slot 8 opens a file.
struct UnkPackSourceDevice {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual u32 vf_0x40(const UnkPackPathKey* path, void** file, u32 mode, base::UnkAllocator* allocator,
                        u32 arg, u32 flags);
};

struct UnkPackManagerView {
    u8 unk_0x0[0x18];
    UnkPackSourceDevice* device;    // +0x18
    base::UnkAllocator* allocator;  // +0x20
    u32 flags;                      // +0x28
    u8 pad_0x2c[4];
    base::UnkVector<UnkMountedPack*> packs;  // +0x30
    UnkPackCallback callback;                // +0x58
    nn::os::MutexType mutex;                 // +0x68

    UnkMountedPack* Find(const UnkPackPathKey& path) const {
        for (UnkMountedPack **it = packs.data, **end = it + packs.count; it != end; ++it) {
            if ((*it)->path.hash == path.hash)
                return *it;
        }
        return nullptr;
    }
};

// Sets the flag at +0xc8 of the pack mounted from `path`; false when there is none.
bool fn_710008acc0(UnkPackManagerView* self, const UnkPackPathKey& path, bool locked) {
    fn_710006691c(&self->mutex);
    bool found = false;
    if (UnkMountedPack* pack = self->Find(path)) {
        pack->locked = locked;
        found = true;
    }
    fn_7100066920(&self->mutex);
    return found;
}

// Unmounts the pack mounted from `path` when it is idle (not in use and no file references) or when
// `force` is set (a pack in use first gets its vf_0x18; with `force`, one with file references runs
// the callback between two calls of fn_710008918c); a pack left mounted and not in use gets fn_710008918c. True when it was unmounted.
bool fn_710008ad4c(UnkPackManagerView* self, const UnkPackPathKey& path, bool force) {
    fn_710006691c(&self->mutex);
    bool unmounted = false;
    if (UnkMountedPack* pack = self->Find(path)) {
        if (pack->vf_0x20() != 0)
            pack->vf_0x18();
        if (force && pack->file_refs > 0 && self->callback.function) {
            fn_710008918c(pack);
            self->callback.function(self->callback.object);
            fn_710008918c(pack);
        }
        bool idle = pack->vf_0x20() != 0 ? false : pack->file_refs < 1;
        u32 refs = pack->vf_0x20();
        if (idle || force) {
            self->packs.Remove(pack);
            pack->vf_0x8();
            unmounted = true;
        } else if (refs == 0) {
            fn_710008918c(pack);
        }
    }
    fn_7100066920(&self->mutex);
    return unmounted;
}

// Mounts the pack at `path` (an already mounted one is reused): the device opens the file and a new
// pack (0xd8 bytes) is appended; a failed open other than status 1 is reported to the registry.
// The pack then gets its vf_0x10. True when a pack is mounted.
bool fn_710008aa90(UnkPackManagerView* self, const UnkPackPathKey& path) {
    fn_710006691c(&self->mutex);
    bool mounted = false;
    UnkMountedPack* pack = self->Find(path);
    if (!pack) {
        void* file = nullptr;
        u32 status = self->device->vf_0x40(&path, &file, 1, self->allocator, g_unk_7101a1b050, self->flags);
        if (status != 0) {
            if (status != 1) {
                UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
                if (!reg) {
                    reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
                    fn_7100087388(reg);
                    g_unk_7101d4b5a0 = reg;
                }
                fn_7100087dc8(reg, self->device, status);
            }
            goto done;
        }
        pack = static_cast<UnkMountedPack*>(fn_71000839a0(0xd8, 3));
        fn_7100088dfc(pack, self, &path, file);
        fn_710008abd4(self, pack);
        if (!pack)
            goto done;
    }
    pack->vf_0x10();
    mounted = true;
done:
    fn_7100066920(&self->mutex);
    return mounted;
}
