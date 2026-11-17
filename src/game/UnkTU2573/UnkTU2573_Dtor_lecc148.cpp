// Unit 2573: a shot's destructor (the shot CTunableShotManager's copy constructor fn_7100eddc68 builds):
// the path and its resource, the two vectors of counted references, then the counted references.
// Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CRntVector.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// A counted reference released on destruction.
struct UnkShotRef_lecc148 {
    UnkRefObject* p;
    ~UnkShotRef_lecc148() {
        if (UnkRefObject* r = p) {
            if (--r->ref_count == 0)
                r->vf_0x10();
        }
    }
};

struct UnkShotRecord18_lecc148 {
    UnkShotRef_lecc148 ref;
    u8 pad_0x8[0x10];
};

struct UnkShotRecord10_lecc148 {
    UnkShotRef_lecc148 ref;
    u8 pad_0x8[0x8];
};

struct UnkShot_lecc148 {
    void** vtable;
    u8 pad_0x8[0x20];
    UnkShotRef_lecc148 field_0x28;
    UnkShotRef_lecc148 field_0x30;
    u8 pad_0x38[0x30];
    UnkShotRef_lecc148 field_0x68;
    UnkShotRef_lecc148 field_0x70;
    UnkShotRef_lecc148 field_0x78;
    u8 pad_0x80[0x78];
    base::global::CRntVector<UnkShotRecord18_lecc148> field_0xf8;
    base::global::CRntVector<UnkShotRecord10_lecc148> field_0x120;
    u8 pad_0x148[0x10];
    UnkStrIdPathResource path;  // 0x158
};
static_assert(__builtin_offsetof(UnkShot_lecc148, field_0xf8) == 0xf8, "placeholder layout");
static_assert(__builtin_offsetof(UnkShot_lecc148, path) == 0x158, "placeholder layout");

void fn_7100ecc148(UnkShot_lecc148* self) {
    self->vtable = &g_unk_7101ca9fe0[2];
    fn_7100027044(&self->path);
    if (UnkCountedResource* r = self->path.resource) {
        if (--r->ref_count == 0)
            fn_710001737c(r);
    }
    self->path.ids.path.~UnkStrIdMemberOf();
    self->path.ids.id.~UnkStrIdMemberOf();
    self->field_0x120.Free();
    self->field_0xf8.Free();
    self->field_0x78.~UnkShotRef_lecc148();
    self->field_0x70.~UnkShotRef_lecc148();
    self->field_0x68.~UnkShotRef_lecc148();
    self->field_0x30.~UnkShotRef_lecc148();
    self->field_0x28.~UnkShotRef_lecc148();
}
