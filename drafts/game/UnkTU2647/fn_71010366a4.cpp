// Unit 2647 (sound::CAudioPreset): makes an object's file-path list the same as another: paths the other
// lacks are released from the sound system and removed, then the other's new non-empty paths are taken
// by the sound system and appended. Names and layouts are placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkResPathVec.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU23.h"
#include "types.h"

using base::global::CFilePathStrId;

using UnkPathIdMember = base::UnkStrIdMemberOf<g_unk_7101d47200>;  // a file-path id
UnkPathIdMember* fn_710008c174(base::global::CRntVector<UnkPathIdMember>* self, u32 index);

struct UnkPathOwner_l0366a4 {
    u8 pad_0x0[0x28];
    UnkResPathVec paths;  // 0x28
};

// The id at `index` (out of line).
static inline CFilePathStrId* PathAt(UnkResPathVec* list, u32 index) {
    return reinterpret_cast<CFilePathStrId*>(
        fn_710008c174(reinterpret_cast<base::global::CRntVector<UnkPathIdMember>*>(list), index));
}

static inline bool Contains(const UnkResPathVec* list, const CFilePathStrId& id) {
    for (u32 i = 0; i < list->count; ++i)
        if (list->data[i] == id)
            return true;
    return false;
}

// Releases the id at `index` and moves the later ones down.
static inline void Erase(UnkResPathVec* list, u32 index) {
    if (index < list->count) {
        list->data[index].~CFilePathStrId();
        for (u32 i = index; i < list->count - 1; ++i) {
            list->data[i].data = list->data[i + 1].data;
            list->data[i + 1].data = &g_unk_7101d47200->empty;
            list->data[i + 1].~CFilePathStrId();
        }
        list->count = list->count - 1;
    }
}

// Appends a copy of an id (the list grown by its step first).
static inline void Add(UnkResPathVec* list, const CFilePathStrId& id) {
    u32 n = list->count;
    u32 capacity = list->capacity;
    if (capacity <= n && list->step != 0)
        capacity = (n / list->step + 1) * list->step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    CAsset_ReserveFilePathIdVec(list, capacity);
    new (&list->data[list->count]) CFilePathStrId(id);
    ++list->count;
}

void fn_71010366a4(UnkPathOwner_l0366a4* self, UnkResPathVec* other) {
    for (s32 i = self->paths.count; i-- > 0;) {
        CFilePathStrId* id = PathAt(&self->paths, i);
        if (Contains(other, *id))
            continue;
        fn_7100108094(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(id));
        Erase(&self->paths, i);
    }
    for (u32 i = 0; i < other->count; ++i) {
        CFilePathStrId* id = PathAt(other, i);
        if (id->data == &g_unk_7101d47200->empty)
            continue;
        if (Contains(&self->paths, *id))
            continue;
        fn_710010808c(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(id));
        Add(&self->paths, *id);
    }
}
