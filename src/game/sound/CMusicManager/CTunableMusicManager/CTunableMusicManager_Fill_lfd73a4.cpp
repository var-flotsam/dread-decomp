// sound::CMusicManager::CTunableMusicManager's unit: fills the two requests a music id stands for. The id
// is looked up in the manager's first map (definitions held by pointer), then in its third (held in
// place); each part of the definition fills the first or the second request and appends its file
// paths. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/global/UnkPooledId.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkTU2640.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/UnkMusicPathVec.h"
#include "types.h"

using base::global::CStrId;

struct UnkMusicRequest;

// One part of a music definition (0x50 bytes): which request it fills, its file paths and the
// request's settings.
struct UnkMusicPart_fd73a4 {
    u32 kind;  // 0: the first request, otherwise the second
    u8 pad_0x4[0x4];
    UnkPathVec_b0019 paths;  // 0x8
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    bool field_0x4c;
};
static_assert(sizeof(UnkMusicPart_fd73a4) == 0x50, "UnkMusicPart_fd73a4 layout");

// A music definition: its parts and its name.
struct UnkMusicDef_fd73a4 {
    UnkMusicPart_fd73a4* parts;
    u32 count;
    u8 pad_0xc[0x1c];
    CStrId name;  // 0x28
};

// The definition's name (out of line).
struct UnkIdObject_lfd9aa0;

// The first map's nodes hold a definition by pointer, the third map's in place.
struct UnkMusicDefRefNode_fd73a4 {
    CStrId key;
    UnkMusicDef_fd73a4* value;
    u8 pad_0x10[0x10];
    UnkMusicDefRefNode_fd73a4* next;  // 0x20
};
struct UnkMusicDefNode_fd73a4 {
    CStrId key;
    u8 pad_0x8[0x8];
    UnkMusicDef_fd73a4 value;  // 0x10
    u8 pad_0x40[0x40];
    UnkMusicDefNode_fd73a4* next;  // 0x80
};

// The request as this function fills it.
struct UnkMusicRequestView_fd73a4 {
    u32 channel;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u8 pad_0x24;
    bool field_0x25;
    UnkPathVec_b0019 files;  // 0x28
    u64 field_0x50;
    CStrId name;  // 0x58
};

template <class Node>
static inline Node* FindNode(UnkChainedMap& map, const CStrId& id) {
    if (map.buckets.count == 0)
        return nullptr;
    u32 index = id.Hash() % map.buckets.capacity;
    UnkMapNode** bucket = map.buckets.data + index;
    if (!bucket)
        return nullptr;
    for (Node* node = reinterpret_cast<Node*>(*bucket); node; node = node->next) {
        if (node->key == id)
            return node;
    }
    return nullptr;
}

// Assigns a file path id (CStrId's assignment, on the file-path pool).
inline void AssignPath_fd73a4(UnkPooledId<&g_unk_7101d47200>& dst, const UnkPooledId<&g_unk_7101d47200>& src) {
    using base::global::UnkStringData;
    using base::global::UnkStringPool;
    if (dst.data != src.data) {
        UnkStringData* d = src.data;
        UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (dst.data != &pool->empty) {
            --dst.data->ref_count;
            if (dst.data->ref_count == 0)
                StringPool_Release(pool, dst.data);
        }
        dst.data = d;
    }
}

// Appends a default record and returns it.
static inline UnkPathRecord_b0019& AddPath(UnkPathVec_b0019& v) {
    u32 n = v.count + 1;
    fn_7100456980(&v, n);
    for (u32 i = v.count; i < n; ++i)
        fn_7100456ac0(reinterpret_cast<UnkPathRecord*>(&v.data[i]));
    v.count = n;
    UnkPathRecord_b0019* end = &v.data[n];
    return n ? end[-1] : *end;
}

void fn_7100fd73a4(UnkMusicManager* self, const CStrId* id, UnkMusicRequest* first, UnkMusicRequest* second) {
    UnkMusicDef_fd73a4* def;
    if (UnkMusicDefRefNode_fd73a4* node = FindNode<UnkMusicDefRefNode_fd73a4>(self->map_0, *id)) {
        def = node->value;
        if (!def)
            return;
    } else if (FindNode<UnkMusicDefNode_fd73a4>(self->map_2, *id)) {
        def = &FindNode<UnkMusicDefNode_fd73a4>(self->map_2, *id)->value;
    } else {
        return;
    }
    if (def->count == 0)
        return;

    for (u64 i = 0; i < def->count; ++i) {
        const UnkMusicPart_fd73a4& part = def->parts[i];
        auto* request = reinterpret_cast<UnkMusicRequestView_fd73a4*>(part.kind == 0 ? first : second);
        request->name = *fn_7100fed648(reinterpret_cast<UnkIdObject_lfd9aa0*>(def));
        request->field_0x4 = part.field_0x48;
        request->field_0x10 = part.field_0x30;
        request->field_0xc = part.field_0x34;
        request->field_0x8 = part.field_0x38;
        request->field_0x14 = part.field_0x3c;
        request->field_0x18 = part.field_0x40;
        request->field_0x1c = part.field_0x44;
        request->field_0x25 = part.field_0x4c;
        request->field_0x50 = 0;
        request->field_0x20 = 0;
        request->channel = 2;
        for (u64 j = 0; j < part.paths.count; ++j) {
            const UnkPathRecord_b0019* paths = part.paths.data;
            UnkPathRecord_b0019& path = AddPath(request->files);
            AssignPath_fd73a4(path.id, paths[j].id);
            path.field_0x8.z = paths[j].field_0x8.z;
            path.field_0x8.x = paths[j].field_0x8.x;
            path.field_0x8.y = paths[j].field_0x8.y;
        }
    }
}
