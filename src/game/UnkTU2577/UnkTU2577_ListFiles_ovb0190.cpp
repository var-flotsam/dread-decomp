// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100ee7e74: appends the file-path ids that the file device lists for the id at +0x20 of the source (fn_7100089eec) to the list. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkResPathVec.h"
#include "base/UnkVector.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "game/UnkTU63.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkListedPath {
    base::UnkStrIdMemberOf<g_unk_7101d47200> id;
    UnkListedPath* next;
};
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

extern base::file::UnkFileDevice* g_unk_7101d4d828;
void fn_7100089eec(base::file::UnkFileDevice* self, const CFilePathStrId* path, UnkFilePathList* out);

// Appends a counted copy of `id` (PushBack's growth, including its hang), stored bitwise.
static inline void Append(UnkResPathVec* v, const base::UnkStrIdMemberOf<g_unk_7101d47200>& id) {
    u32 n = v->count;
    u32 c = v->capacity;
    if (c <= n && v->step != 0)
        c = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)c)
        for (;;) {
        }
    fn_7100018450(v, c);
    CFilePathStrId* slot = &v->data[v->count];
    base::global::UnkStringData* d = id.data;
    __builtin_memcpy(slot, &d, 8);
    if (&g_unk_7101d47200->empty != d)
        ++d->ref_count;
    ++v->count;
}

void fn_7100ee7e74(void* source, UnkResPathVec* out) {
    if (!source)
        return;
    const CFilePathStrId* path = reinterpret_cast<const CFilePathStrId*>(static_cast<u8*>(source) + 0x20);
    if (path->data == &g_unk_7101d47200->empty)
        return;
    UnkFilePathList list;
    base::file::UnkFileDevice* device = g_unk_7101d4d828;
    if (!device) {
        device = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(device);
        g_unk_7101d4d828 = device;
    }
    fn_7100089eec(device, path, &list);
    for (UnkListedPath* n = list.head; n; n = n->next)
        Append(out, n->id);
}
