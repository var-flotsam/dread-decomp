// CCharClassSwarmControllerComponent's unit: the component's constructor. Runs the base constructor,
// sets the defaults, and constructs its id, damage factors, strings, path ids and id lists.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "game/CCharClassSwarmControllerComponent.h"
#include "game/CDamageSourceFactor.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

// An id and a path id (taken from the empty defaults) and no resource yet.
struct UnkSwarmPath_l736b6c {
    UnkStringData* id;
    UnkStringData* path;
    void* resource;

    void Init(UnkStringData* default_id, UnkStringData* empty_id, UnkStringData* default_path,
              UnkStringData* empty_path) {
        id = default_id;
        if (empty_id != default_id)
            ++default_id->ref_count;
        path = default_path;
        if (empty_path != default_path)
            ++default_path->ref_count;
        resource = nullptr;
    }
};

// Up to ten ids stored in place after their count.
struct UnkSwarmIdList_l736b6c {
    u32 count;
    u8 storage[10 * sizeof(CStrId)];
};

struct UnkSwarmController_l736b6c {
    void* vtable;
    u8 pad_0x8[0x50];
    bool field_0x58;
    u8 pad_0x59[0x3];
    f32 field_0x5c;
    u32 field_0x60;
    f32 field_0x64;
    u32 field_0x68;
    u8 pad_0x6c[0x4];
    CStrId sIndividualLife;
    u8 pad_0x78[0x58];
    u32 field_0xd0;
    bool bWantsMigration;
    u8 pad_0xd5[0x3];
    f32 fMaxDistToMigrate;
    u8 pad_0xdc[0x34];
    u8 oDamageSourceFactor[0x70];
    CRntString sImpactSoundSmallGroup;
    CRntString sImpactSoundMediumGroup;
    CRntString sImpactSoundLargeGroup;
    CRntString sMoveSoundSmallGroup;
    CRntString sMoveSoundMediumGroup;
    CRntString sMoveSoundLargeGroup;
    UnkSwarmPath_l736b6c paths[6];  // 0x2a0
    u32 iTotalDrops;
    UnkSwarmIdList_l736b6c field_0x334;
    UnkSwarmIdList_l736b6c field_0x388;
    UnkSwarmIdList_l736b6c field_0x3dc;
};
static_assert(__builtin_offsetof(UnkSwarmController_l736b6c, field_0xd0) == 0xd0, "layout");
static_assert(__builtin_offsetof(UnkSwarmController_l736b6c, paths) == 0x2a0, "layout");
static_assert(__builtin_offsetof(UnkSwarmController_l736b6c, field_0x334) == 0x334, "layout");

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_7100736b6c(UnkSwarmController_l736b6c* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a6d688) + 0x10;
    self->field_0x58 = false;
    self->field_0x5c = 2.0f;
    self->field_0x60 = 0;
    self->field_0x64 = 0.01f;
    self->field_0x68 = 3;
    new (&self->sIndividualLife) CStrId(g_unk_7101d470e0);
    self->bWantsMigration = false;
    self->field_0xd0 = 0;
    self->fMaxDistToMigrate = 200.0f;
    fn_71008c2da8(self->oDamageSourceFactor);
    fn_710007e6c4(&self->sImpactSoundSmallGroup);
    fn_710007e6c4(&self->sImpactSoundMediumGroup);
    fn_710007e6c4(&self->sImpactSoundLargeGroup);
    fn_710007e6c4(&self->sMoveSoundSmallGroup);
    fn_710007e6c4(&self->sMoveSoundMediumGroup);
    fn_710007e6c4(&self->sMoveSoundLargeGroup);
    UnkStringData* empty_id = &g_unk_7101d47108->empty;
    UnkStringData* default_id = g_unk_7101d470e0.data;
    UnkStringData* default_path = g_unk_7101d470e8.data;
    UnkStringData* empty_path = &g_unk_7101d47200->empty;
    for (int i = 0; i < 6; ++i)
        self->paths[i].Init(default_id, empty_id, default_path, empty_path);
    self->iTotalDrops = 1;
    self->field_0x334.count = 0;
    self->field_0x388.count = 0;
    self->field_0x3dc.count = 0;
}
