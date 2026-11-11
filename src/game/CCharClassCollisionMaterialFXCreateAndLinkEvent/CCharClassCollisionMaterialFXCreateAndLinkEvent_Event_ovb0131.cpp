// CCharClassCollisionMaterialFXCreateAndLinkEvent's unit (tools/tu_map.py unit 1759): the destructor
// (fn_7100ba0ef4): the vtable, the string ids at +0x60 and +0x30, the file path at +0x20 and the string
// id at +0x10 released, then the base destructor fn_7100067ed4 (as fn_7100bb1740 in
// src/game/CCharClassFXCreateTrailEvent/CCharClassFXCreateTrailEvent_Copy_ovb0121.cpp); the copy
// assignment (fn_7100ba1358: the plain fields copied, the ids taken with their counts adjusted); and
// fn_7100ba0d14, which links the event to the actor's FX (its +0x280, fn_7101154738) unless the event's
// effect path (+0x20) is one of the two ground landing effects (function-local static paths). Names and
// layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

template <UnkStringPool** Pool>
struct UnkReleasedId_ovb0131 {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedId_ovb0131() {
        if (UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};
using UnkReleasedStrId_ovb0131 = UnkReleasedId_ovb0131<&g_unk_7101d47108>;
using UnkReleasedPathId_ovb0131 = UnkReleasedId_ovb0131<&g_unk_7101d47200>;

void fn_7100067ed4(void* self);  // the base's destructor

extern char g_unk_7101aa08c8[];

struct UnkMaterialFXEvent_ovb0131 {
    char* vtable;
    u8 unk_0x8[0x8];
    UnkReleasedStrId_ovb0131 a;      // 0x10
    u8 unk_0x18[0x8];
    UnkReleasedPathId_ovb0131 path;  // 0x20
    u8 unk_0x28[0x8];
    UnkReleasedStrId_ovb0131 b;      // 0x30
    u8 unk_0x38[0x28];
    UnkReleasedStrId_ovb0131 c;      // 0x60

    __attribute__((always_inline)) ~UnkMaterialFXEvent_ovb0131() { vtable = g_unk_7101aa08c8 + 0x10; }
};

void fn_7100ba0ef4(UnkMaterialFXEvent_ovb0131* self) {
    self->~UnkMaterialFXEvent_ovb0131();
    fn_7100067ed4(self);
}

template <UnkStringPool** Pool>
struct UnkCopiedId_ovb0131 {
    UnkStringData* data;

    __attribute__((always_inline)) UnkCopiedId_ovb0131& operator=(const UnkCopiedId_ovb0131& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = *Pool;
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

struct UnkMaterialFXBlock_ovb0131 {
    u32 words[7];
};

struct UnkMaterialFXEventData_ovb0131 {
    void* vtable;
    u32 field_0x8;
    UnkCopiedId_ovb0131<&g_unk_7101d47108> a;     // 0x10
    u32 field_0x18;
    UnkCopiedId_ovb0131<&g_unk_7101d47200> path;  // 0x20
    u32 field_0x28;
    UnkCopiedId_ovb0131<&g_unk_7101d47108> b;     // 0x30
    u64 field_0x38;
    UnkMaterialFXBlock_ovb0131 block;             // 0x40
    UnkCopiedId_ovb0131<&g_unk_7101d47108> c;     // 0x60
    u64 field_0x68;
};

// The copy assignment (fn_7100ba1358).
UnkMaterialFXEventData_ovb0131* fn_7100ba1358(UnkMaterialFXEventData_ovb0131* self,
                                              const UnkMaterialFXEventData_ovb0131* other) {
    self->field_0x8 = other->field_0x8;
    self->a = other->a;
    self->field_0x18 = other->field_0x18;
    self->path = other->path;
    self->field_0x28 = other->field_0x28;
    self->b = other->b;
    self->field_0x38 = other->field_0x38;
    self->block = other->block;
    self->c = other->c;
    self->field_0x68 = other->field_0x68;
    return self;
}

using base::global::CFilePathStrId;

struct UnkLandingWorld_ovb0131;
struct UnkMaterialFXLinkEvent_ovb0131;
void fn_7101154738(UnkLandingWorld_ovb0131* world, UnkMaterialFXLinkEvent_ovb0131* event);

struct UnkMaterialFXLinkEvent_ovb0131 {
    u8 unk_0x0[0x20];
    UnkStringData* path;  // 0x20
};

struct UnkLandingActor_ovb0131 {
    u8 unk_0x0[0x280];
    UnkLandingWorld_ovb0131* fx;  // 0x280
};

void fn_7100ba0d14(UnkMaterialFXLinkEvent_ovb0131* self, UnkLandingActor_ovb0131** actor) {
    if (!*actor)
        return;
    static const CFilePathStrId landing("actors/characters/samus/fx/groundlanding.bcptl", true);
    static const CFilePathStrId landing_run("actors/characters/samus/fx/groundlandingrun.bcptl", true);
    if (self->path == *reinterpret_cast<UnkStringData* const*>(&landing))
        return;
    if (self->path == *reinterpret_cast<UnkStringData* const*>(&landing_run))
        return;
    UnkLandingWorld_ovb0131* fx = (*actor)->fx;
    if (!fx)
        return;
    fn_7101154738(fx, self);
}
