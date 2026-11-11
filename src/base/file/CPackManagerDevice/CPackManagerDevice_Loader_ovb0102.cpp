// base::file::CPackManagerDevice's unit (tools/tu_map.py unit 66): more of the pack sets object
// (base/file/UnkFileDevice.h): opening a pack set table through the pack manager's device, the
// loader's service of the request ring, and unmounting a pack set's packs. Names and layouts are
// placeholders.
#include <nn/os.h>

#include "base/CDeviceQueue.h"
#include "base/UnkDefaultAllocator.h"
#include "base/UnkDestructors.h"
#include "base/UnkRegistry.h"
#include "base/UnkVector.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU63.h"
#include "types.h"

using base::UnkStrIdMemberOf;
using base::file::UnkDevice;
using base::file::UnkFileDevice;
using base::global::CFilePathStrId;

// The registry that reports an open's failure (src/base/file/CPackManagerDevice_3.cpp).
extern UnkRegistry_0x70* g_unk_7101d4b5a0;
void fn_7100087dc8(UnkRegistry_0x70* self, void* entry, u32 kind);

// A file the source device opens (deleted through its slot 1).
struct UnkOpenedFile {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

// The device the pack manager reads from: its slot 8 opens a file.
struct UnkPackSetSourceDevice {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual u32 vf_0x40(const CFilePathStrId& path, UnkOpenedFile** file, u32 mode, void* allocator, u32 arg,
                        u32 flags);
};

// The pack manager as this file reads it.
struct UnkPackSetManager {
    u8 unk_0x0[0x18];
    UnkPackSetSourceDevice* device;  // +0x18
};

// Reads the pack set table from an opened file (the base object's).
bool fn_710008badc(void* base_object, UnkOpenedFile* file);

// Opens the pack set table at `path` through the pack manager's device and reads it; a failed open
// other than status 1 is reported to the registry.
bool fn_7100089c1c(UnkFileDevice* self, const CFilePathStrId& path) {
    UnkOpenedFile* file = nullptr;
    UnkPackSetManager* packs = reinterpret_cast<UnkPackSetManager*>(self->packs);
    u32 status = packs->device->vf_0x40(path, &file, 1, nullptr, 0, 3);
    bool ok = false;
    if (status != 1) {
        if (status == 0) {
            ok = fn_710008badc(self->base_0x8, file);
        } else {
            UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
            if (!reg) {
                reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
                fn_7100087388(reg);
                g_unk_7101d4b5a0 = reg;
            }
            fn_7100087dc8(reg, self->packs, status);
        }
    }
    if (file) {
        file->vf_0x8();
        file = nullptr;
    }
    return ok;
}

struct UnkPackManagerView;
struct UnkPackPathKey;
// The pack manager's mount and unmount (src/base/file/CPackManagerDevice_3.cpp, where the mount
// returns whether a pack is mounted; src/engine/UnkEngineStart.cpp declares it without the result).
bool fn_710008aa90(UnkDevice* packs, const CFilePathStrId& path);
using UnkPackMountFn = bool (*)(UnkDevice* packs, const CFilePathStrId& path);
bool fn_710008ad4c(UnkPackManagerView* self, const UnkPackPathKey& path, bool force);

// A load request as the loader copies it out of the ring: the path, the flag and value, and the
// callback told whether the loads so far succeeded.
struct UnkPackRequestCallback {
    u64 context;
    void (*function)(u64 context, bool ok);
};
struct UnkPackRequest {
    CFilePathStrId path;
    u64 field_0x8;  // the flag and the value
    UnkPackRequestCallback callback;

    UnkPackRequest(const UnkPackRequest& other)
        : path(other.path), field_0x8(other.field_0x8), callback{other.callback.context, other.callback.function} {}
};

// Serves the queued requests: each non-empty path is mounted (unless the base object's flag is
// set) and the request's callback runs. True when every mount succeeded.
bool fn_71000898c8(UnkFileDevice* self) {
    fn_710006691c(&self->mutex_0x10f0);
    fn_710006691c(&self->mutex_0x1120);
    u8* busy = reinterpret_cast<u8*>(self) + 0x1119;
    *busy = 1;
    fn_7100066920(&self->mutex_0x1120);
    bool ok = true;
    for (;;) {
        fn_710006691c(&self->mutex);
        u32 queued = self->queued;
        fn_7100066920(&self->mutex);
        if (queued == 0)
            break;
        fn_710006691c(&self->mutex);
        UnkPackRequest& slot = reinterpret_cast<UnkPackRequest&>(self->slots[self->field_0x10c4]);
        self->field_0x10c4 = (self->field_0x10c4 + 1) & 0x7f;
        --self->queued;
        UnkPackRequest request = slot;
        fn_7100066920(&self->mutex);
        if (request.path.data != &g_unk_7101d47200->empty) {
            bool mounted = self->field_0xb8 ? false
                                                                                        : reinterpret_cast<UnkPackMountFn>(fn_710008aa90)(
                                                  reinterpret_cast<UnkDevice*>(self->packs), request.path);
            ok &= mounted;
        }
        if (request.callback.function)
            request.callback.function(request.callback.context, ok);
    }
    fn_710006691c(&self->mutex_0x1120);
    *busy = 0;
    fn_7100066920(&self->mutex_0x1120);
    fn_7100066920(&self->mutex_0x10f0);
    return ok;
}

// A listed path (a node of UnkFilePathList).
struct UnkListedPath {
    UnkStrIdMemberOf<g_unk_7101d47200> id;
    UnkListedPath* next;
};
// A list of path ids (as src/base/file/CPackManagerDevice.cpp has it).
struct UnkFilePathList {
    void** vtable;
    u32 count;
    UnkListedPath* head;
    UnkListedPath* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkFilePathList() {
        vtable = &g_unk_7101a1b030[2];
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkFilePathList() {
        vtable = &g_unk_7101a1b030[2];
        for (UnkListedPath* n = head; n;) {
            UnkListedPath* next = n->next;
            n->id.~UnkStrIdMemberOf();
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

// Lists the files under `path` into `out` (the base object's).
bool fn_710008c3bc(void* base_object, const CFilePathStrId& path, UnkFilePathList* out);

// Unmounts every pack of the pack set `path` (without forcing). False when the set can't be listed.
bool fn_7100089ef4(base::file::UnkFileDevice* self, const base::global::UnkFilePathStrIdBase* path) {
    UnkFilePathList list;
    bool ok = fn_710008c3bc(self->base_0x8, *static_cast<const CFilePathStrId*>(path), &list);
    if (ok) {
        for (UnkListedPath* n = list.head; n; n = n->next)
            fn_710008ad4c(reinterpret_cast<UnkPackManagerView*>(self->packs),
                          reinterpret_cast<const UnkPackPathKey&>(n->id), false);
    }
    return ok;
}
