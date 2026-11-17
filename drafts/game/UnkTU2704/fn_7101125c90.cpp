// UnkTU2704 (CSpawnPointComponent's unit): the spawn point component's destructor. It runs two cleanup
// steps, then destroys the members last first and ends with the base component's destructor.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkComponent.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::UnkRefPtrMember;
using base::global::CRntString;
using base::global::CStrId;

// A link to an object of the scene (CGameLink<CActor>): a callback set holding the counted handle.
struct UnkGameLink_l125c90;
template <>
struct base::UnkCallbackTraits<UnkGameLink_l125c90> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkSpawnLink_l125c90 = base::UnkCallbackListShort<UnkGameLink_l125c90>;

// An element of the link arrays, destroyed through its vtable.
struct UnkSpawnLinkElem_l125c90 {
    virtual ~UnkSpawnLinkElem_l125c90();
    u8 pad_0x8[0x38];
};

struct CSpawnerActorBlueprint;

// The id table and the property ids the component keeps per key.
struct UnkSpawnIdTable_l125c90 {
    UnkStrIdTable table;

    ~UnkSpawnIdTable_l125c90() {
        fn_71004f11f8(&table);
        if (table.keys)
            table.allocator->vf_0x48(table.keys);
        if (table.values)
            table.allocator->vf_0x48(table.values);
    }
};

struct UnkSpawnPoint_l125c90 : UnkBaseComponent {
    UnkRefPtrMember field_0x40;
    CStrId field_0x48;
    UnkCountedResource* field_0x50;
    CStrId field_0x58;
    UnkSpawnLink_l125c90 field_0x60;
    CStrId sStartAnimation;         // 0xa0
    CRntString sOnBeforeGenerate;   // 0xa8
    CRntString sOnEntityGenerated;  // 0xd8
    u8 pad_0x108[0x18];
    UnkSpawnLink_l125c90 wpSceneShapeId;  // 0x120
    u8 pad_0x160[0x10];
    UnkSpawnLink_l125c90 wpCollisionSceneShapeId;  // 0x170
    u8 pad_0x1b0[0x8];
    UnkSpawnLink_l125c90 wpNavigableShape;     // 0x1b8
    UnkSpawnLink_l125c90 wpAreaOfInterest;     // 0x1f8
    UnkSpawnLink_l125c90 wpAreaOfInterestEnd;  // 0x238
    u8 pad_0x278[0x10];
    CStrId field_0x288;
    UnkSpawnIdTable_l125c90 field_0x290;
    u8 pad_0x2b8[0x10];
    base::global::CRntVector<UnkSpawnLinkElem_l125c90> tXCellTransformTargets;  // 0x2c8
    UnkSpawnLink_l125c90 wpXCellActivationAreaShape;                           // 0x2f0
    u8 pad_0x330[0x10];
    UnkRefPtrMember field_0x340;
    UnkRefPtrMember field_0x348;
    u8 pad_0x350[0x8];
    base::global::CRntVector<UnkSpawnLinkElem_l125c90> tDynamicSpawnPositions;  // 0x358
    UnkRefPtrMember field_0x380;
    u8 pad_0x388[0x8];
    base::global::CRntVector<CSpawnerActorBlueprint> voActorBlueprint;  // 0x390
};
static_assert(__builtin_offsetof(UnkSpawnPoint_l125c90, field_0x60) == 0x60, "layout");
static_assert(__builtin_offsetof(UnkSpawnPoint_l125c90, wpSceneShapeId) == 0x120, "layout");
static_assert(__builtin_offsetof(UnkSpawnPoint_l125c90, field_0x288) == 0x288, "layout");
static_assert(__builtin_offsetof(UnkSpawnPoint_l125c90, tXCellTransformTargets) == 0x2c8, "layout");
static_assert(__builtin_offsetof(UnkSpawnPoint_l125c90, field_0x340) == 0x340, "layout");
static_assert(__builtin_offsetof(UnkSpawnPoint_l125c90, voActorBlueprint) == 0x390, "layout");

extern void* g_unk_7101cb28c8[];
void fn_71011264b4(UnkSpawnPoint_l125c90* self);
void fn_71011265c0(UnkSpawnPoint_l125c90* self);
void fn_71011266cc(base::global::CRntVector<CSpawnerActorBlueprint>* self);

void fn_7101125c90(char* p) {
    UnkSpawnPoint_l125c90* self = reinterpret_cast<UnkSpawnPoint_l125c90*>(p);
    self->vtable = g_unk_7101cb28c8 + 2;
    fn_71011264b4(self);
    fn_71011265c0(self);
    fn_71011266cc(&self->voActorBlueprint);
    self->field_0x380.~UnkRefPtrMember();
    self->tDynamicSpawnPositions.Free();
    self->field_0x348.~UnkRefPtrMember();
    self->field_0x340.~UnkRefPtrMember();
    self->wpXCellActivationAreaShape.~UnkSpawnLink_l125c90();
    self->tXCellTransformTargets.Free();
    self->field_0x290.~UnkSpawnIdTable_l125c90();
    self->field_0x288.~CStrId();
    self->wpAreaOfInterestEnd.~UnkSpawnLink_l125c90();
    self->wpAreaOfInterest.~UnkSpawnLink_l125c90();
    self->wpNavigableShape.~UnkSpawnLink_l125c90();
    self->wpCollisionSceneShapeId.~UnkSpawnLink_l125c90();
    self->wpSceneShapeId.~UnkSpawnLink_l125c90();
    self->sOnEntityGenerated.~CRntString();
    self->sOnBeforeGenerate.~CRntString();
    self->sStartAnimation.~CStrId();
    self->field_0x60.~UnkSpawnLink_l125c90();
    self->field_0x58.~CStrId();
    if (UnkCountedResource* resource = self->field_0x50) {
        if (--resource->ref_count == 0)
            fn_710001737c(resource);
    }
    self->field_0x48.~CStrId();
    self->field_0x40.~UnkRefPtrMember();
    fn_71010c4194(self);
}
