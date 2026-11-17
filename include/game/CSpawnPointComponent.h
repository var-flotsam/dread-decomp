#pragma once

// The spawn point component (reflection: CSpawnPointComponent, 0x3b8 bytes, base CComponent of 0x40; unit 2704
// of tools/tu_map.py), as far as the hand-written sources know it: one layout, grown by each source that reads
// a member it does not have yet. The reflected members keep their reflected names (tools/gen_fields.py
// --class CSpawnPointComponent); the others are placeholders. The owning actor (the base component's +0x30)
// is left untyped: each source reads it through its own view, until the actor has one layout. The members the
// destructor (fn_7101125c90) destroys have the types it destroys them as: the links are CGameLink<CActor>
// (destroyed inline as members, through their vtable as array elements), +0x290 an owned id table.
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CGameLink.h"
#include "game/UnkComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct CActor;
struct CSpawnerActorBlueprint;

struct CSpawnPointComponent : UnkBaseComponent {
    enum EXCellSpawnPositionMode : s32 {};
    enum EDynamicSpawnPositionMode : s32 {};

    base::UnkRefPtrMember field_0x40;
    base::global::CStrId field_0x48;
    UnkCountedResource* field_0x50;  // released by the destructor (fn_710001737c at the last reference)
    base::global::CStrId field_0x58;
    CGameLink<CActor> field_0x60;  // not reflected: the object told of each spawn (its vtable slot 0x178)
    base::global::CStrId sStartAnimation;         // 0xa0
    base::global::CRntString sOnBeforeGenerate;   // 0xa8
    base::global::CRntString sOnEntityGenerated;  // 0xd8: the Lua function called with the owner and the actor
    bool bSpawnOnFloor;                           // 0x108
    bool bEntityCheckFloor;                       // 0x109
    bool bCheckCollisions;                        // 0x10a
    u8 pad_0x10b[0x1];
    f32 fTimeToActivate;     // 0x10c
    s32 iMaxNumToGenerate;   // 0x110
    bool bAllowSpawnInFrustum;  // 0x114
    bool bStartEnabled;         // 0x115
    u8 pad_0x116[0x2];
    void* field_0x118;  // the collider made for the scene shape (src/game/UnkTU2704 reads it)
    CGameLink<CActor> wpSceneShapeId;  // 0x120
    void* field_0x160;  // what the scene shape is tracked as
    void* field_0x168;  // the collider made for the collision scene shape
    CGameLink<CActor> wpCollisionSceneShapeId;  // 0x170
    void* field_0x1b0;  // what the collision scene shape is tracked as
    CGameLink<CActor> wpNavigableShape;      // 0x1b8
    CGameLink<CActor> wpAreaOfInterest;      // 0x1f8
    CGameLink<CActor> wpAreaOfInterestEnd;   // 0x238
    f32 fTimeOnAOIEndToUseAsMainAOI;         // 0x278
    bool bAutomanaged;                       // 0x27c
    u8 pad_0x27d[0x3];
    // What fn_7101127f2c registers under the owner's name (fn_7101127e78): set when registered; it goes
    // on into field_0x288 and field_0x290.
    bool field_0x280;
    u8 pad_0x281[0x7];
    base::global::CStrId field_0x288;
    // The blackboard properties by name: the keys are the properties' name ids, each value's id the
    // property's (fn_71011269c0, fn_7101126af4).
    UnkOwnedStrIdTable field_0x290;
    bool field_0x2b8;  // cleared when a spawn finds the field_0x60 object's +0x13a set
    u8 pad_0x2b9[0x3];
    f32 fSpawnFromXCellProbability;            // 0x2bc
    f32 fSpawnFromXCellProbabilityAfterFirst;  // 0x2c0
    EXCellSpawnPositionMode eXCellSpawnPositionMode;              // 0x2c4
    base::global::CRntVector<CGameLink<CActor>> tXCellTransformTargets;    // 0x2c8
    CGameLink<CActor> wpXCellActivationAreaShape;                 // 0x2f0
    // Set by fn_7101127c24.
    bool field_0x330;
    bool field_0x331;  // when set, field_0x2b8 is cleared
    bool field_0x332;
    u8 pad_0x333[0x1];
    u32 field_0x334;
    bool field_0x338;
    u8 pad_0x339[0x340 - 0x339];
    UnkWeakHandle* field_0x340;  // fn_7101127c24's last target's weak handle
    UnkWeakHandle* field_0x348;  // fn_7101127c24's first target's weak handle
    bool field_0x350;  // whether the owner is on the Emmy zone's shape (set by fn_7101126c34)
    bool bUseDynamicSpawnPosition;  // 0x351
    u8 pad_0x352[0x2];
    EDynamicSpawnPositionMode eDynamicSpawnPositionMode;          // 0x354
    base::global::CRntVector<CGameLink<CActor>> tDynamicSpawnPositions;    // 0x358
    UnkWeakHandle* field_0x380;  // the weak handle of the object found by fn_710112c4d8
    u8 pad_0x388[0x390 - 0x388];
    base::global::CRntVector<CSpawnerActorBlueprint> voActorBlueprint;     // 0x390

    // Resolves one of the component's links the way this unit resolves links (fn_710023b580,
    // fn_7100234f78): once, while its state is 1.
    static void ResolveLink(CGameLink<CActor>& link) {
        if (link.state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(&link.callbacks);
                // The reference's pointer is the handle's.
                fn_7100234f78(&link.handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            link.state = 2;
        }
    }
    void ResolveField0x60() { ResolveLink(field_0x60); }

    // The owning actor (the base component's +0x30), through the view the caller has of it.
    template <class T>
    T* Owner() const {
        return static_cast<T*>(field_0x30);
    }
};
static_assert(__builtin_offsetof(CSpawnPointComponent, field_0x60) == 0x60, "CSpawnPointComponent layout");
static_assert(__builtin_offsetof(CSpawnPointComponent, iMaxNumToGenerate) == 0x110, "CSpawnPointComponent layout");
static_assert(__builtin_offsetof(CSpawnPointComponent, wpSceneShapeId) == 0x120, "CSpawnPointComponent layout");
static_assert(__builtin_offsetof(CSpawnPointComponent, wpNavigableShape) == 0x1b8, "CSpawnPointComponent layout");
static_assert(__builtin_offsetof(CSpawnPointComponent, bAutomanaged) == 0x27c, "CSpawnPointComponent layout");
static_assert(__builtin_offsetof(CSpawnPointComponent, field_0x290) == 0x290, "CSpawnPointComponent layout");
static_assert(__builtin_offsetof(CSpawnPointComponent, field_0x340) == 0x340, "CSpawnPointComponent layout");
static_assert(__builtin_offsetof(CSpawnPointComponent, field_0x2b8) == 0x2b8, "CSpawnPointComponent layout");
static_assert(__builtin_offsetof(CSpawnPointComponent, tXCellTransformTargets) == 0x2c8, "CSpawnPointComponent layout");
static_assert(__builtin_offsetof(CSpawnPointComponent, wpXCellActivationAreaShape) == 0x2f0, "CSpawnPointComponent layout");
static_assert(__builtin_offsetof(CSpawnPointComponent, tDynamicSpawnPositions) == 0x358, "CSpawnPointComponent layout");
static_assert(__builtin_offsetof(CSpawnPointComponent, voActorBlueprint) == 0x390, "CSpawnPointComponent layout");
static_assert(sizeof(CSpawnPointComponent) == 0x3b8, "CSpawnPointComponent layout");
