// CWaterPlatformUsableComponent's unit (tools/tu_map.py unit 1608): tells whether no registered E.M.M.I.
// stands on (or is pathing through) a navigation-mesh side whose cells the component lists. Names and
// layouts are placeholders.
#include "base/UnkEmptyDtorElem.h"
#include "base/UnkIdKindKey.h"
#include "base/global/CRntVector.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CEnemyMovement.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU362.h"
#include "game/UnkTU376.h"
#include "types.h"

void fn_71002874bc(void* self);

// A cell id and side (0xffff and 0xff for none), as UnkIdKindKey; built empty out of line.
struct UnkNavSide_laa5980 {
    u16 cell;
    u8 side;
    u8 pad_0x3;

    UnkNavSide_laa5980() { fn_71002874bc(this); }
    ~UnkNavSide_laa5980() { fn_71002874dc(this); }
    void Set(u16 cell_, u8 side_) {
        cell = cell_;
        side = side_;
    }
    bool Valid() const { return cell != 0xffff && side != 0xff; }
};

// Where a movement stands on the mesh: a cell and side, then more (12 bytes).
struct UnkNavPlace_laa5980 {
    u16 cell;
    u8 side;
    u8 pad_0x3[0x9];

    ~UnkNavPlace_laa5980() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }
};

// A navigation path: the side it leaves by and the cells it starts and ends in (0xffff for none).
struct UnkNavPath_laa5980 {
    u8 pad_0x0;
    u8 side;   // 0x1
    u16 from;  // 0x2
    u8 pad_0x4[0x1e - 0x4];
    u16 to;  // 0x1e
    u8 pad_0x20[0x100 - 0x20];

    UnkNavPath_laa5980() { fn_71002b57bc(Object()); }
    ~UnkNavPath_laa5980() { fn_71002b6094(Object()); }
    UnkNavPath_laa5980& operator=(const UnkNavPath_laa5980& other) {
        fn_71002b5c90(Object(), other.Object());
        return *this;
    }
    UnkObject_0x100* Object() { return reinterpret_cast<UnkObject_0x100*>(this); }
    const UnkObject_0x100* Object() const { return reinterpret_cast<const UnkObject_0x100*>(this); }
};
static_assert(sizeof(UnkNavPath_laa5980) == 0x100, "layout");

struct UnkEmmyMovement_laa5980 {
    void** vtable;
    u8 pad_0x8[0x1870 - 0x8];
    UnkNavPath_laa5980 path;  // 0x1870
    bool has_path;            // 0x1970

    UnkNavPlace_laa5980 vf_0x2d8() {
        return reinterpret_cast<UnkNavPlace_laa5980 (*)(UnkEmmyMovement_laa5980*)>(vtable[0x2d8 / 8])(this);
    }
    bool GetPath(UnkNavPath_laa5980* out) {
        if (has_path)
            *out = path;
        return has_path;
    }
};
static_assert(__builtin_offsetof(UnkEmmyMovement_laa5980, has_path) == 0x1970, "layout");

struct UnkEmmyOwner_laa5980 {
    u8 pad_0x0[0x218];
    UnkEmmyMovement_laa5980* movement;  // 0x218
};

struct UnkEmmyActor_laa5980 {
    u8 pad_0x0[0x30];
    UnkEmmyOwner_laa5980* owner;  // 0x30
};

struct UnkEmmyEntry_laa5980 {
    u8 pad_0x0[0x10];
    UnkEmmyActor_laa5980* actor;  // 0x10
};

struct UnkEmmyList_laa5980 {
    u8 pad_0x0[8];
    u32 count;
    u32 entries[1];  // UnkEmmyEntry_laa5980* entries, 4-byte aligned
    UnkEmmyEntry_laa5980** Entries() { return reinterpret_cast<UnkEmmyEntry_laa5980**>(entries); }
};

// The cell's view of its tag at +0x68.
struct UnkNavCellTag_laa5980 {
    u8 pad_0x0[0x68];
    void* tag;  // 0x68
};

static inline void* CellTag(UnkNavMeshCell* cell) { return reinterpret_cast<UnkNavCellTag_laa5980*>(cell)->tag; }

struct UnkWaterPlatformUsable_laa5980 {
    u8 pad_0x0[0x5e0];
    void** blocked;     // 0x5e0, cell tags
    u32 blocked_count;  // 0x5e8
};

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

bool fn_7100aa5980(UnkWaterPlatformUsable_laa5980* self) {
    UnkNavMesh* mesh = NavMeshManager()->navmesh;
    if (!mesh)
        return true;
    UnkEmmyList_laa5980* list = reinterpret_cast<UnkEmmyList_laa5980*>(g_unk_71726af0a8);
    UnkEmmyEntry_laa5980** begin = list->Entries();
    UnkEmmyEntry_laa5980** end = begin + list->count;
    for (UnkEmmyEntry_laa5980** p = begin; p != end; ++p) {
        UnkEmmyActor_laa5980* actor = *p ? (*p)->actor : nullptr;
        UnkNavSide_laa5980 side;
        // Where the E.M.M.I. is on the mesh: the side its path leaves by while it follows one (state
        // 4), otherwise the place its movement reports.
        if (UnkEmmyOwner_laa5980* owner = actor->owner) {
            if (UnkEmmyMovement_laa5980* movement = owner->movement) {
                if (fn_71010cc3a4(movement) == 4) {
                    UnkNavPath_laa5980 path;
                    if (movement->GetPath(&path) && path.from != 0xffff && path.to != 0xffff) {
                        UnkIdKindKey key(path.to, path.side);
                        side.Set(key.id, key.kind);
                    }
                } else {
                    UnkNavPlace_laa5980 place = movement->vf_0x2d8();
                    side.Set(place.cell, place.side);
                }
            }
        }
        if (!side.Valid())
            continue;
        UnkNavMeshCell* cell = fn_710027a328(mesh, side.cell);
        UnkNavMeshEdge* edge = fn_71002afbe4(cell, &side.side);
        UnkNavMeshCell* other = edge->field_0x2 != 0xffff ? fn_710027a328(mesh, edge->field_0x2) : nullptr;
        for (u32 i = 0; i < self->blocked_count; ++i) {
            void* tag = self->blocked[i];
            if (tag == CellTag(cell) || (other && CellTag(other) == tag))
                return false;
        }
    }
    return true;
}
