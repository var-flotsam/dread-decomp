// base::reflection unit (tools/tu_map.py): merges one record into another. The kind id and a flag are
// always copied; when the kind is the id table's default kind (+0xd80) every field is copied, otherwise
// only those the source sets. Names and layouts are placeholders.
#include "base/UnkResPathVec.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// The list at +0x78, copied out of line (fn_71000ce19c).
struct UnkMergeList_lfa5c40 {
    void* data;
    u32 count;
    u8 pad_0xc[0x28 - 0xc];
};

struct UnkMergeRecord_lfa5c40 {
    u8 pad_0x0[0x18];
    CStrId kind;               // 0x18
    CFilePathStrId path;       // 0x20
    CStrId field_0x28;
    CStrId ids[9];             // 0x30
    UnkMergeList_lfa5c40 list; // 0x78
    UnkResPathVec paths;       // 0xa0
    f32 field_0xc8;
    u8 field_0xcc;
    u8 field_0xcd;
};
static_assert(__builtin_offsetof(UnkMergeRecord_lfa5c40, field_0xcd) == 0xcd, "layout");

static inline bool IsSet(const CStrId& id) { return &g_unk_7101d47108->empty != id.data; }
static inline bool IsSet(const CFilePathStrId& id) { return &g_unk_7101d47200->empty != id.data; }

void fn_7100fa5c40(void* self_, void* arg) {
    UnkMergeRecord_lfa5c40* self = static_cast<UnkMergeRecord_lfa5c40*>(self_);
    const UnkMergeRecord_lfa5c40* other = static_cast<const UnkMergeRecord_lfa5c40*>(arg);
    if (self == other)
        return;

    bool all = other->kind == GameManagerIdTable().field_0xd80;
    self->kind = other->kind;
    self->field_0xcd = other->field_0xcd;

    for (int i = 0; i < 9; ++i) {
        if (all || IsSet(other->ids[i]))
            self->ids[i] = other->ids[i];
    }
    if (all || other->list.count != 0)
        fn_71000ce19c(reinterpret_cast<UnkStruct_0x28*>(&self->list),
                      reinterpret_cast<const UnkStruct_0x28*>(&other->list));
    if (all || IsSet(other->field_0x28))
        self->field_0x28 = other->field_0x28;
    if (all || other->paths.count != 0)
        fn_71000d0958(&self->paths, &other->paths);
    if (IsSet(other->path))
        self->path = other->path;
    if (other->field_0xc8 > 0.0f)
        self->field_0xc8 = other->field_0xc8;
    self->field_0xcc = other->field_0xcc;
}
