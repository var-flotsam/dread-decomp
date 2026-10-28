// base::file::CPackManagerDevice's unit (tools/tu_map.py unit 66: 0x7100089284-0x710008b6d4): the
// device over the game's .pkg packs ("packs", 0x88 bytes; the name comes from reflection) and the
// pack sets object that queues pack loads for its "PackLoaderThread" (0x1348 bytes,
// base/file/UnkFileDevice.h). Names and layouts are placeholders inferred from the code.

#include <stdio.h>

#include <nn/os.h>

#include "base/CDeviceQueue.h"
#include "base/UnkDefaultAllocator.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

namespace base::file {

// The key a pack looks a file up by (the path id's hash). Its copy is user-defined, so it goes
// through memory when passed by value.
struct UnkPackKey {
    u64 hash;
    UnkPackKey(const UnkPackKey& other) : hash(other.hash) {}
};

// What a lookup asks for: bit 0 asks for the file's entry (a weak function-local static gives the
// default, zero).
struct UnkPackFindFlags {
    u32 value;
    UnkPackFindFlags() : value(0) {}
};

inline const UnkPackFindFlags& DefaultFindFlags() {
    static UnkPackFindFlags s_flags;
    return s_flags;
}

// What a lookup reports: whether the pack has the file, and (with bit 0 asked) its entry.
struct UnkPackFindResult {
    bool found;
    u32 flags;
    u8 entry[0x28];
};

// A mounted pack (a .pkg file; unit 65). Only what this unit reads is named.
struct UnkPack {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual u32 vf_0x20();  // its reference count
    u8 unk_0x8[0x10];
    CFilePathStrId path;  // +0x18
    u8 unk_0x20[0xa4];
    u32 size;  // +0xc4
    bool field_0xc8;
    u8 pad_0xc9[3];
    s32 file_refs;  // +0xcc
};

struct UnkCallback {
    void* object;
    void (*function)();
};

// The device (vtable via GOT 0x7101cbc4d8; slots as UnkDevice). The packs are kept in mount order.
struct CPackManagerDevice {
    void* vtable;
    UnkDeviceName name;  // +0x8
    bool mounted;        // +0x10
    u8 field_0x11;
    u8 pad_0x12[6];
    UnkDevice* device;        // +0x18: the device the packs are read from
    UnkAllocator* allocator;  // +0x20
    u32 flags;                // +0x28
    u8 pad_0x2c[4];
    UnkVector<UnkPack*> packs;  // +0x30
    UnkCallback callback;       // +0x58
    nn::os::MutexType mutex;    // +0x68
};
static_assert(sizeof(CPackManagerDevice) == 0x88, "CPackManagerDevice layout");

}  // namespace base::file

using base::UnkStrIdMemberOf;
using base::file::CPackManagerDevice;
using base::file::DefaultFindFlags;
using base::file::UnkFileDevice;
using base::file::UnkPack;
using base::file::UnkPackFindFlags;
using base::file::UnkPackFindResult;
using base::file::UnkPackKey;
using base::file::UnkPathRequestArgs;
using base::file::UnkPathSlot;

// The pooled data behind an id as its text accessor sees it: the text at +0x10 and a count of the
// reads at +0x44.
struct UnkIdTextData {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    // A union member: the original reloads the id's data pointer after the increment.
    union {
        s32 reads;
    };
};

static inline const char* PathText(const CFilePathStrId& id) {
    ++reinterpret_cast<UnkIdTextData*>(id.data)->reads;
    const char* s = reinterpret_cast<UnkIdTextData*>(id.data)->str.data;
    return s ? s : g_unk_710162e2ed;
}

// ---------------------------------------------------------------------------------------------
// The pack sets object (base/file/UnkFileDevice.h).

// The pack sets singleton (created on first use).
extern base::file::UnkFileDevice* g_unk_7101d4d828;

static inline UnkFileDevice* PackSets() {
    if (!g_unk_7101d4d828) {
        auto* p = static_cast<UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        UnkFileDeviceConstruct(p);
        g_unk_7101d4d828 = p;
    }
    return g_unk_7101d4d828;
}

// The destructor of its base object.
void fn_710008b924(void* base_object);

// The destructor's body: the ring's paths are released last to first, then the base object.
static inline void Destroy(UnkFileDevice* self) {
    self->vtable = g_unk_7101a1b0e8 + 0x10;
    fn_7100066918(&self->mutex);
    for (UnkPathSlot* slot = &self->slots[128]; slot != self->slots;)
        (--slot)->path.~UnkStrIdMemberOf();
    fn_710008b924(self->base_0x8);
}

// Its destructor.
void fn_7100089460(UnkFileDevice* self) {
    Destroy(self);
}

// Its deleting destructor.
void fn_7100089510(UnkFileDevice* self) {
    Destroy(self);
    operator delete(self);
}

// Puts a request for `path` in the next ring slot (overwriting the oldest).
static inline void Enqueue(UnkFileDevice* self, const UnkStrIdMemberOf<g_unk_7101d47200>& path, bool a, u32 b,
                           const UnkPathRequestArgs& args) {
    fn_710006691c(&self->mutex);
    UnkPathSlot& slot = self->slots[self->next];
    self->next = (self->next + 1) & 0x7f;
    ++self->queued;
    slot.path = path;
    slot.args = args;
    slot.field_0x8 = a;
    slot.field_0xc = b;
    fn_7100066920(&self->mutex);
}

// Queues a request with the default path (g_unk_7101d470e8, base/global/CFilePathStrId.h; value
// 0x7fffffff) and the given words.
// NON_MATCHING(minor): scheduling and register allocation only: the original computes the slot's
// address right after loading the ring index (before the index and count stores) and keeps it in
// x22, ours computes it after them into the device's register. fn_7100089cf8 inlines the same
// Enqueue and matches.
bool fn_7100089668(UnkFileDevice* self, const UnkPathRequestArgs& args) {
    Enqueue(self, reinterpret_cast<const UnkStrIdMemberOf<g_unk_7101d47200>&>(g_unk_7101d470e8), false, 0x7fffffff,
            args);
    return true;
}

// Waits on a light event (the engine's wrapper).
void fn_710006669c(nn::os::LightEventType* event);
// Serves the queued requests.
void fn_71000898c8(UnkFileDevice* self);

// The "PackLoaderThread" entry (fn_71000895c8 starts it): serves the requests each time the event
// is signalled, while the object runs.
void fn_7100089af8(void* arg) {
    UnkFileDevice* self = PackSets();
    while (self->running) {
        fn_710006669c(&self->event);
        fn_71000898c8(self);
    }
}

// A listed path (a node of UnkFilePathList).
struct UnkListedPath {
    UnkStrIdMemberOf<g_unk_7101d47200> id;
    UnkListedPath* next;
};
extern void* g_unk_7101a1b030[];
// A list of path ids (its destructor is fn_7100087cf8; src/game/UnkResourceGroup.cpp has the same).
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

// Queues every file of the pack set `path` (value 9, no words). False when the set can't be listed.
bool fn_7100089cf8(UnkFileDevice* self, const CFilePathStrId& path) {
    UnkFilePathList list;
    if (!fn_710008c3bc(self->base_0x8, path, &list))
        return false;
    for (UnkListedPath* n = list.head; n; n = n->next)
        Enqueue(self, n->id, false, 9, UnkPathRequestArgs());
    return true;
}

// ---------------------------------------------------------------------------------------------
// The pack manager device.

extern char g_unk_7101a1b108[];  // its vtable (GOT 0x7101cbc4d8)

// Looks `key` up in the pack (unit 65); releases a file reference.
bool fn_7100089124(UnkPack* pack, const UnkPackKey& key, UnkPackFindResult* out, const UnkPackFindFlags& flags);
bool fn_71000890e0(UnkPack* pack, const UnkPackKey& key, bool flag);
// The vector's element accessor, out of line (base/UnkVector.h, GrowAt).
UnkPack** fn_710008aecc(base::UnkVector<UnkPack*>* v, u32 index);
// Unmounts every pack, last first (stops at the first that won't).
bool fn_710008a52c(CPackManagerDevice* self, bool flag);

// Its destructor: an unmount of what is still mounted.
void fn_710008a378(CPackManagerDevice* self) {
    self->vtable = g_unk_7101a1b108 + 0x10;
    if (self->mounted) {
        if (self->callback.function) {
            self->callback.object = nullptr;
            self->callback.function = nullptr;
        }
        fn_710008a52c(self, true);
        self->mounted = false;
    }
    fn_7100066918(&self->mutex);
    self->packs.Free();
    self->name.~UnkDeviceName();
}

// The pack that has `key`: with `any`, the first that has it; otherwise the first in use (a
// reference count) and not flagged at +0xc8.
UnkPack* fn_710008a648(CPackManagerDevice* self, const UnkPackKey& key, bool any) {
    fn_710006691c(&self->mutex);
    UnkPackFindResult result;
    result.found = false;
    result.flags = 0;
    UnkPack* found;
    base::UnkVector<UnkPack*>* packs = &self->packs;
    if (!any) {
        for (u32 i = 0; i < self->packs.count; ++i) {
            UnkPack* pack = *fn_710008aecc(packs, i);
            if (!pack->field_0xc8 && pack->vf_0x20() != 0) {
                UnkPackFindFlags flags = DefaultFindFlags();
                fn_7100089124(pack, key, &result, flags);
                if (result.found) {
                    found = pack;
                    goto done;
                }
            }
        }
    } else {
        for (u32 i = 0; i < self->packs.count; ++i) {
            UnkPack* pack = *fn_710008aecc(packs, i);
            UnkPackFindFlags flags = DefaultFindFlags();
            fn_7100089124(pack, key, &result, flags);
            found = result.found ? pack : nullptr;
            if (found)
                goto done;
        }
    }
    found = nullptr;
done:
    fn_7100066920(&self->mutex);
    return found;
}

// Releases a file reference of `key` in `pack`.
bool fn_710008a938(CPackManagerDevice* self, UnkPack* pack, const UnkPackKey& key, bool flag) {
    fn_710006691c(&self->mutex);
    bool released = fn_71000890e0(pack, key, flag);
    fn_7100066920(&self->mutex);
    return released;
}

// Releases a file reference of `key` in the first pack that has it. A status: 1 when no pack has
// it or the release reports false, else 0.
s32 fn_710008a8a4(CPackManagerDevice* self, const UnkPackKey& key, bool flag) {
    fn_710006691c(&self->mutex);
    s32 status = 1;
    if (UnkPack* pack = fn_710008a648(self, key, true))
        status = !fn_710008a938(self, pack, key, flag);
    fn_7100066920(&self->mutex);
    return status;
}

// Formats a line per mounted pack (the log call itself is compiled out).
void fn_710008b248(CPackManagerDevice* self) {
    for (UnkPack **it = self->packs.data, **end = it + self->packs.count; it != end; ++it) {
        UnkPack* pack = *it;
        if (!pack)
            continue;
        char line[0x100];
        sprintf(line, "Loaded: %s, PackRef: %u, FileRef: %d, Size: %d Bytes", PathText(pack->path), pack->vf_0x20(),
                pack->file_refs, pack->size);
    }
}

// Builds `dst` as a copy of `src`: the name retained, the pack list copied into its own array.
static inline void CopyConstruct(CPackManagerDevice* dst, const CPackManagerDevice* src) {
    dst->vtable = g_unk_7101a1b108 + 0x10;
    dst->name.data = src->name.data;
    if (&g_unk_7101d47108->empty != dst->name.data)
        ++dst->name.data->ref_count;
    __builtin_memcpy(&dst->mounted, &src->mounted, 0x1c);
    base::UnkVector<UnkPack*>& v = dst->packs;
    const base::UnkVector<UnkPack*>& o = src->packs;
    v.data = nullptr;
    v.count = 0;
    v.capacity = 0;
    v.step = o.step;
    v.field_0x14 = o.field_0x14;
    v.flags = o.flags;
    v.allocator = o.allocator;
    v.Reserve(o.capacity);
    __builtin_memcpy(v.data, o.data, (u64)o.count * sizeof(UnkPack*));
    v.count = o.count;
    dst->callback.object = src->callback.object;
    dst->callback.function = src->callback.function;
    dst->mutex = src->mutex;
}

// The reflected class's construction helpers at descriptor slots 0x30 and 0x38
// (src/reflection/base_file_CPackManagerDevice.cpp): both build a copy.
void fn_710008b3a4(void* dst, const void* src) {
    CopyConstruct(static_cast<CPackManagerDevice*>(dst), static_cast<const CPackManagerDevice*>(src));
}

void fn_710008b4ec(void* dst, const void* src) {
    CopyConstruct(static_cast<CPackManagerDevice*>(dst), static_cast<const CPackManagerDevice*>(src));
}
