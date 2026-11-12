// CStandaloneFXComponent (unit 1274): setting the effect's path. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/UnkVector.h"
#include "game/CActorComponent.h"
#include "game/CStandaloneFXComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU2739.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A file-path id copied in: `data` is read before `other.data`, and the released id is reloaded
// for the release call (as CBlendSpaceResource_Assign.cpp's UnkBlendPathId).
struct UnkStandaloneFxPath {
    UnkStringData* data;

    UnkStandaloneFxPath& operator=(const UnkStandaloneFxPath& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkStandaloneFxOwner {
    u8 unk_0x0[0x40];
    u32 field_0x40;
};

struct UnkStandaloneFx {
    UnkStandaloneFxOwner* owner;  // 0x0
    UnkStandaloneFxPath path;     // 0x8
    u8 unk_0x10[0x28];
    void* field_0x38;             // the effect the model manager holds for it
};

static inline UnkModelManager* GetManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

void fn_7100811e18(UnkStandaloneFx* self, const UnkStandaloneFxPath* path) {
    if (self->path.data == path->data) return;
    if (self->field_0x38) {
        fn_71011a91c4(GetManager(), self->field_0x38, self->owner->field_0x40);
        self->field_0x38 = nullptr;
    }
    self->path = *path;
    if (self->path.data != &g_unk_7101d47200->empty && fn_710024fbc8(self->owner)) fn_7100811f28(self);
}
