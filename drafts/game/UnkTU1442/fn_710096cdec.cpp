// UnkTU1442 (tools/tu_map.py unit 1442): gathers the smart-link steps that lie on the owner's path.
// It walks round the corner the path's current point sits on, then asks every smart-link rule of
// the owner's group for steps and keeps those whose cell and side the path passes, searching from
// the current point in the walk's direction. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkPath442.h"
#include "game/UnkTU1442.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU367.h"
#include "game/UnkTU376.h"
#include "game/UnkObject_0x100.h"
#include "types.h"

#include <new>

using namespace base::global;
using base::math::CVector2D;

// ---- The smart-link entries (as src/game/navmesh/CDynamicSmartLinkGroup/CDynamicSmartLinkGroup_Links_ovb0128.cpp) ----

struct UnkLinkPath_ovb0128 : UnkFilePathStrIdBase {
    UnkLinkPath_ovb0128() {
        data = nullptr;
        if (!g_unk_7101d47200)
            UnkEnsureFilePathPool();
        data = &g_unk_7101d47200->empty;
    }
    UnkLinkPath_ovb0128(const UnkLinkPath_ovb0128& other) : UnkFilePathStrIdBase(other) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkLinkPath_ovb0128(UnkLinkPath_ovb0128&& other) : UnkFilePathStrIdBase(other) {
        other.data = &g_unk_7101d47200->empty;
    }
    ~UnkLinkPath_ovb0128() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkSmartLinkRef_ovb0128 {
    UnkCountedResource* ptr;

    UnkSmartLinkRef_ovb0128() : ptr(nullptr) {}
    UnkSmartLinkRef_ovb0128(const UnkSmartLinkRef_ovb0128& other) : ptr(nullptr) {
        if (UnkCountedResource* p = other.ptr)
            Assign(p);
    }
    ~UnkSmartLinkRef_ovb0128() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }

    // Takes a reference to p (not null), dropping the held one.
    void Assign(UnkCountedResource* p) {
        ++p->ref_count;
        if (UnkCountedResource* old = ptr) {
            if (--old->ref_count == 0)
                fn_710001737c(old);
        }
        ptr = p;
    }
};

struct UnkSmartLinkEntry_ovb0128 {
    UnkLinkPath_ovb0128 path;
    UnkSmartLinkRef_ovb0128 object;
};

void fn_71002681a8(base::global::CRntVector<UnkSmartLinkEntry_ovb0128>* self,
                   const base::global::CRntVector<UnkSmartLinkEntry_ovb0128>* other);

// ---- Callees ----

struct UnkSource_7100286798;
u32 fn_7100413fd0(UnkSource_7100286798* self);
bool fn_710027c460(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side);
// The widths the owner's body needs to pass an opening.
UnkVec2Result fn_71010fb200(UnkComponent_7100286798* self);
// The entries of a smart-link group.
const base::global::CRntVector<UnkSmartLinkEntry_ovb0128>* fn_710026626c(void* group);
// Whether the path loops round.
bool fn_710035e53c(const UnkPath442* self);

// ---- Layouts ----

// A step of a smart link: the 0x100-byte object of game/UnkObject_0x100.h, read for the cell and side
// it starts from.
struct UnkSmartLinkStep_l96cdec {
    u8 pad_0x0[0x1c];
    u8 side;  // 0x1c
    u8 pad_0x1d;
    u16 cell;  // 0x1e
    u8 pad_0x20[0x100 - 0x20];

    UnkSmartLinkStep_l96cdec() { fn_71002b57bc(Object()); }
    UnkSmartLinkStep_l96cdec(const UnkSmartLinkStep_l96cdec& other) { fn_71002b58c0(Object(), other.Object()); }
    ~UnkSmartLinkStep_l96cdec() { fn_71002b6094(Object()); }
    UnkSmartLinkStep_l96cdec& operator=(const UnkSmartLinkStep_l96cdec& other) {
        fn_71002b5c90(Object(), other.Object());
        return *this;
    }
    UnkObject_0x100* Object() { return reinterpret_cast<UnkObject_0x100*>(this); }
    const UnkObject_0x100* Object() const { return reinterpret_cast<const UnkObject_0x100*>(this); }
};
static_assert(sizeof(UnkSmartLinkStep_l96cdec) == 0x100, "layout");

// Up to ten steps in place (the storage is constructed one step at a time).
struct UnkSmartLinkSteps_l96cdec {
    u32 count;
    u8 storage[10][0x100];

    UnkSmartLinkSteps_l96cdec() : count(0) {}
    ~UnkSmartLinkSteps_l96cdec() {
        for (u32 i = 0; i < count; ++i)
            At(i)->~UnkSmartLinkStep_l96cdec();
    }
    UnkSmartLinkStep_l96cdec* At(u32 i) { return reinterpret_cast<UnkSmartLinkStep_l96cdec*>(storage[i]); }
    UnkSmartLinkStep_l96cdec* begin() { return At(0); }
    UnkSmartLinkStep_l96cdec* end() { return At(count); }
    void Clear() {
        for (u32 i = 0; i < count; ++i)
            At(i)->~UnkSmartLinkStep_l96cdec();
        count = 0;
    }
    void PushBack(const UnkSmartLinkStep_l96cdec& step) {
        new (At(count)) UnkSmartLinkStep_l96cdec(step);
        ++count;
    }
};
static_assert(sizeof(UnkSmartLinkSteps_l96cdec) == 0xa04, "layout");

// A point of the path: the cell and the side of it the path crosses.
struct UnkPathPoint_l96cdec {
    u16 cell;
    u8 side;
};

// The owner's AI component.
struct UnkLinkAI_l96cdec {
    virtual const base::reflection::CClass* GetClass();  // vf_0x0
    u8 pad_0x8[0x1e0 - 0x8];
    void* group;  // 0x1e0: the smart-link group
    u8 pad_0x1e8[0x208 - 0x1e8];
    u32 layer_mask;  // 0x208
    u8 pad_0x20c[0x218 - 0x20c];
    u32 field_0x218;
    u8 pad_0x21c[0x228 - 0x21c];
    u32 field_0x228;
};

struct UnkLinkMover_l96cdec {
    u8 pad_0x0[0x10];
    UnkLinkAI_l96cdec* ai;  // 0x10
    u8 pad_0x18[0x68 - 0x18];
    UnkPath442 path;  // 0x68
};

struct UnkLinkOwner_l96cdec {
    u8 pad_0x0[0x240];
    UnkLinkMover_l96cdec* mover;  // 0x240
};

// A smart link of a rules object; vf_0x40 lists the steps it offers from a cell's side.
struct UnkSmartLink_l96cdec {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(UnkLinkOwner_l96cdec* owner, UnkNavMesh* mesh, u32 flags, u32 cell, u8 side,
                         const CVector2D* direction, const void* params, void* context,
                         UnkSmartLinkSteps_l96cdec* out, u32 a, u32 b);
};

// navmesh::CDynamicSmartLinkRules: a counted resource with its links at +0x68.
struct UnkSmartLinkRules_l96cdec {
    virtual const base::reflection::CClass* GetClass();  // vf_0x0
    u8 pad_0x8[0x68 - 0x8];
    UnkSmartLink_l96cdec** links;  // 0x68
    u32 count;                     // 0x70
};

struct UnkSmartLinkWalker_l96cdec {
    u8 pad_0x0[0x30];
    UnkLinkOwner_l96cdec* owner;  // 0x30
    u8 pad_0x38[0x9c - 0x38];
    u32 index;  // 0x9c: the current point of the path
    u8 pad_0xa0[0xf4 - 0xa0];
    CVector2D direction;  // 0xf4
    u8 params[0x140 - 0xfc];  // 0xfc
    UnkSmartLinkSteps_l96cdec steps;  // 0x140
    u8 pad_0xb44[4];
    UnkSmartLinkStep_l96cdec step;  // 0xb48
};
static_assert(__builtin_offsetof(UnkSmartLinkWalker_l96cdec, step) == 0xb48, "layout");

// A copy of a group's entries, laid out as base::global::CRntVector (fn_71002681a8 builds it).
struct UnkLinkList_l96cdec {
    UnkSmartLinkEntry_ovb0128* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    explicit UnkLinkList_l96cdec(const base::global::CRntVector<UnkSmartLinkEntry_ovb0128>* other) {
        fn_71002681a8(reinterpret_cast<base::global::CRntVector<UnkSmartLinkEntry_ovb0128>*>(this), other);
    }
    ~UnkLinkList_l96cdec() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                data[i].~UnkSmartLinkEntry_ovb0128();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
    UnkSmartLinkEntry_ovb0128* begin() { return data; }
    UnkSmartLinkEntry_ovb0128* end() { return data + count; }
};

// ---- Helpers ----

template <const base::reflection::CClass* (*GetBase)(), class T>
static inline bool IsA(T* object) {
    const base::reflection::CClass* cls = object->GetClass();
    return fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(cls),
                         reinterpret_cast<const base::reflection::CType*>(GetBase()));
}

static inline UnkLinkAI_l96cdec* AIOf(UnkLinkMover_l96cdec* mover) {
    if (!mover)
        return nullptr;
    UnkLinkAI_l96cdec* ai = mover->ai;
    return ai && IsA<fn_7100408b2c>(ai) ? ai : nullptr;
}

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

static inline bool IsNear(f32 a, f32 b) {
    return a + 0.001f >= b && a - 0.001f <= b;
}

static inline bool PassesThrough(const UnkPath442* path, u32 i, const UnkSmartLinkStep_l96cdec& step) {
    const UnkPathPoint_l96cdec* point = reinterpret_cast<const UnkPathPoint_l96cdec*>(fn_710035e580(path, i));
    return point->cell == step.cell && point->side == step.side;
}

// Whether the path passes the step's cell and side, searching from `start` by `delta` and, on a
// looped path, round again from the other end.
static inline bool IsOnPath(const UnkPath442* path, u32 start, s32 delta, bool forward,
                            const UnkSmartLinkStep_l96cdec& step) {
    bool looped = fn_710035e53c(path);
    bool found = false;
    u32 i = start;
    for (; i < path->SegmentCount(); i += delta) {
        if (PassesThrough(path, i, step)) {
            found = true;
            break;
        }
    }
    if (looped) {
        i = forward ? 0 : path->SegmentCount() - 1;
        while (!found && i != start) {
            if (PassesThrough(path, i, step))
                found = true;
            else
                i += delta;
        }
    }
    return found;
}

void fn_710096cdec(char* raw, bool backwards) {
    UnkSmartLinkWalker_l96cdec* self = reinterpret_cast<UnkSmartLinkWalker_l96cdec*>(raw);

    self->steps.Clear();
    self->step = UnkSmartLinkStep_l96cdec();

    UnkLinkMover_l96cdec* mover = self->owner->mover;
    UnkLinkAI_l96cdec* ai = AIOf(mover);
    if (!ai)
        return;
    void* group = ai->group;
    if (!group)
        return;
    UnkLinkMover_l96cdec* moving = self->owner->mover;
    UnkPath442* path = &moving->path;
    if (path->SegmentCount() == 0)
        return;

    u32 start = self->index;
    const UnkPathPoint_l96cdec* point = reinterpret_cast<const UnkPathPoint_l96cdec*>(fn_710035e580(path, start));
    CVector2D direction;
    if (backwards) {
        direction.x = -self->direction.x;
        direction.y = -self->direction.y;
    } else {
        direction.x = self->direction.x;
        direction.y = self->direction.y;
    }
    UnkLinkAI_l96cdec* layers = AIOf(self->owner->mover);
    u32 mask = layers ? layers->layer_mask : 0;

    u32 cell = point->cell;
    u8 side = point->side;
    bool forward = true;
    if (side != 0xff && cell != 0xffff) {
        if (UnkNavMesh* mesh = NavMeshManager()->navmesh) {
            u16 next_cell = 0xffff;
            u8 next_side = 0xff;
            UnkNavMeshSideSegment segment = {{0.0f, 0.0f}, {0.0f, 0.0f}, -1.0f};
            fn_71002afea8(fn_710027a33c(mesh, cell), &side, &segment);
            UnkVec2Result normal = fn_7100125e78(reinterpret_cast<const UnkSegment2D*>(&segment));
            f32 cross = normal.y * direction.x - normal.x * direction.y;
            forward = cross > 0.0f;
            UnkVec2Result widths = fn_71010fb200(reinterpret_cast<UnkComponent_7100286798*>(mover));
            bool more = fn_710027c460(mesh, cell, side, forward, reinterpret_cast<const CVector2D*>(&widths),
                                      fn_7100413fd0(reinterpret_cast<UnkSource_7100286798*>(ai)), &next_cell,
                                      &next_side);
            while (more) {
                UnkNavMeshCell* next = fn_710027a33c(mesh, next_cell);
                const UnkNavMeshEdge* edge = fn_71002afbe4(next, &next_side);
                if (!IsNear(normal.x, -edge->field_0x10) || !IsNear(normal.y, edge->field_0xc))
                    break;
                if (mask != 0 && ((fn_71002b009c(fn_710027a33c(mesh, cell)) ^ fn_71002b009c(next)) & mask) != 0)
                    break;
                side = next_side;
                cell = next_cell;
                more = fn_710027c460(mesh, cell, side, cross > 0.0f, reinterpret_cast<const CVector2D*>(&widths),
                                     fn_7100413fd0(reinterpret_cast<UnkSource_7100286798*>(ai)), &next_cell,
                                     &next_side);
            }
        }
    }

    s32 delta = forward ? 1 : -1;
    UnkLinkList_l96cdec links(fn_710026626c(group));
    for (UnkSmartLinkEntry_ovb0128 link : links) {
        UnkCountedResource* object = link.object.ptr;
        if (!object)
            continue;
        UnkSmartLinkRules_l96cdec* rules = reinterpret_cast<UnkSmartLinkRules_l96cdec*>(object);
        if (!IsA<fn_710026cad8>(rules))
            continue;
        for (UnkSmartLink_l96cdec** it = rules->links, **end = it + rules->count; it != end; ++it) {
            UnkSmartLink_l96cdec* smart = *it;
            UnkSmartLinkSteps_l96cdec offered;
            UnkNavMesh* mesh = NavMeshManager()->navmesh;
            smart->vf_0x40(self->owner, mesh, fn_7100413fd0(reinterpret_cast<UnkSource_7100286798*>(ai)), cell, side,
                           &direction, self->params, fn_71010fb398(reinterpret_cast<UnkComponent_7100286798*>(mover)),
                           &offered, ai->field_0x228, ai->field_0x218);
            for (UnkSmartLinkStep_l96cdec& step : offered) {
                if (IsOnPath(path, start, delta, forward, step))
                    self->steps.PushBack(step);
            }
        }
    }
}
