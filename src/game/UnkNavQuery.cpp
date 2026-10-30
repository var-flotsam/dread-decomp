// From translation unit 357 of data/tu_map.csv (0x710028659c-0x71002871cc, no class hint; its other
// functions are src/game_lo/fn_7100286624.cpp, the destructor, and fn_7100286798.cpp, which binds
// the actor): a navigation query of an actor, from a start point to a goal on the navigation mesh.
// fn_7100286c8c resolves both points' cells and collects the goal cell's sides that can be reached
// (asking the actor's component first, then the mesh near the goal). The navmesh types repeat the
// ones of src/game/navmesh/CNavMesh.cpp (the same definitions). Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;

// From src/game/navmesh/CNavMesh.cpp (the same definitions).
struct UnkNavMeshHit;
struct UnkElem_7100286624;  // the same 12-byte record (src/game_lo/fn_7100286624.cpp)
void fn_7100287598(UnkNavMeshHit* self);
void fn_710028760c(UnkElem_7100286624* self);

struct UnkNavMeshHit {
    u16 cell;
    u8 side;
    CVector2D point;

    UnkNavMeshHit() { fn_7100287598(this); }
    UnkNavMeshHit(const UnkNavMeshHit& other) {
        __builtin_memcpy(static_cast<void*>(&cell), &other.cell, 4);  // the cell, the side and the pad
        point.x = other.point.x;
        point.y = other.point.y;
    }
    ~UnkNavMeshHit() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }

    bool IsValid() const { return cell != 0xffff && side != 0xff; }
};
static_assert(sizeof(UnkNavMeshHit) == 0xc, "UnkNavMeshHit layout");

struct UnkNavMeshHits {
    u32 count;

    UnkNavMeshHit* items() { return reinterpret_cast<UnkNavMeshHit*>(&count + 1); }
    void Clear() {
        for (u64 i = 0; i < count; ++i)
            items()[i].~UnkNavMeshHit();
        count = 0;
    }
    // The hit at `i`, constructing the ones up to it first when the list is shorter.
    UnkNavMeshHit& At(u64 i) {
        if (i >= count) {
            for (u32 j = count; j < i + 1; ++j)
                new (&items()[j]) UnkNavMeshHit;
            count = i + 1;
        }
        return items()[i];
    }
    void Push(const UnkNavMeshHit& hit) {
        new (&items()[count]) UnkNavMeshHit(hit);
        ++count;
    }
};

struct UnkDirectionFilter;     // src/game/UnkDirectionFilter.cpp
struct UnkNavMeshSideSegment;  // src/game/navmesh/CNavMesh.cpp

template <class R, class... A>
struct UnkNavMeshCallback {
    void* ctx;
    R (*fn)(void* ctx, A... args);

    explicit operator bool() const { return fn != nullptr; }
    R operator()(A... args) const { return fn(ctx, args...); }
};

using UnkNavSideFilter = UnkNavMeshCallback<bool, u16, u8, void*, void*, u64, u32, u32>;
using UnkNavSideVerdict = UnkNavMeshCallback<s32, UnkNavMeshSideSegment*>;

bool fn_710027dab8(UnkNavMesh* mesh, const CVector2D* point, u16 cell, u32 flags, u64 filter_arg, u32 filter_arg2,
                   u32 filter_arg3, const UnkNavSideFilter& filter, UnkNavMeshHits* out);
bool fn_710027bbf0(UnkNavMesh* mesh, const CVector2D* point, u32 flags, u64 filter_arg, u32 filter_arg2,
                   u32 filter_arg3, UnkDirectionFilter* direction, UnkNavMeshHits* out, const void* arg_0x28,
                   const void* arg_0x30, const void* arg_0x38, f32 up, f32 down, f32 left, f32 right);
bool fn_710027b780(UnkNavMesh* mesh, const CVector2D* point, u32 flags, u64 filter_arg, u32 filter_arg2,
                   u32 filter_arg3, UnkDirectionFilter* direction, UnkNavMeshHits* out,
                   const UnkNavSideFilter& filter, const UnkNavSideVerdict& verdict);

// The navigation component of the actor (+0x240): its sides for a goal cell (vf_0x548), and the
// result handed back (vf_0x550).
struct UnkNavQuery;
struct UnkNavComponent;
struct UnkNavComponentVtbl {
    u8 pad_0x0[0x548];
    void (*vf_0x548)(UnkNavComponent* self, u16 cell, UnkNavQuery* query, UnkNavMeshHits* hits);
    void (*vf_0x550)(UnkNavComponent* self, u16 cell, UnkNavQuery* query, UnkNavMeshHits* hits);
};
struct UnkNavComponent {
    UnkNavComponentVtbl* vtable;
};

// What fn_710110c3d8 returns for the component: its smart-link group name at +0x30.
struct UnkNavGroupInfo {
    u8 pad_0x0[0x30];
    CStrId name;
};
UnkNavGroupInfo* fn_710110c3d8(UnkNavComponent* component);

struct UnkNavActor {
    u8 pad_0x0[0x240];
    UnkNavComponent* component;
};

struct UnkNavCallback {
    void* ctx;
    void* fn;
};

// A callback passed by value: the original copies it member by member (a user-provided copy), so
// the copies go in memory and their addresses are passed.
struct UnkNavCallbackCopy {
    void* ctx;
    void* fn;

    template <class T>
    explicit UnkNavCallbackCopy(const T& other) {
        ctx = other.ctx;
        fn = reinterpret_cast<void*>(other.fn);
    }
    template <class T>
    const T& As() const {
        return *reinterpret_cast<const T*>(this);
    }
};

struct UnkNavQuery {
    UnkNavCallback field_0x0;
    UnkNavSideFilter filter;
    UnkNavCallback field_0x20;
    UnkNavCallback field_0x30;
    UnkNavSideVerdict verdict;
    UnkNavActor* actor;
    u8 pad_0x58[0x8];
    u32 flags;
    u8 pad_0x64[0x4];
    u32 field_0x68;
    u32 field_0x6c;
    CVector2D start;
    s16 start_cell;
    u8 pad_0x7a[0x6];
    bool enabled;
    u8 pad_0x81[0x3];
    CVector2D goal;
    u16 goal_cell;
    u8 pad_0x8e[0x2];
    UnkNavMeshHits hits;  // up to 16
    u8 items[0xc0];
    u8 pad_0x154[0x4];
    u8 field_0x158;
    u8 pad_0x159[0x7];
    u8 direction[0x28];  // UnkDirectionFilter
    void* group;  // +0x188
    u64 field_0x190;
    void* field_0x198;
    u8 pad_0x1a0[0x18];
};
static_assert(__builtin_offsetof(UnkNavQuery, actor) == 0x50, "UnkNavQuery placeholder layout");
static_assert(__builtin_offsetof(UnkNavQuery, hits) == 0x90, "UnkNavQuery placeholder layout");
static_assert(__builtin_offsetof(UnkNavQuery, field_0x158) == 0x158, "UnkNavQuery placeholder layout");
static_assert(__builtin_offsetof(UnkNavQuery, group) == 0x188, "UnkNavQuery placeholder layout");

static inline UnkNavMeshManager* UnkNavManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

// Updates the query on `mesh`: the cells of the start and goal points (when not known yet), the
// actor's smart-link group (its component's, else "SP_Emmy"), and, when the query is enabled and
// has no sides yet, the goal cell's sides: from the component (vf_0x548), else from the goal cell,
// else from the side nearest the goal (flooding the cells around it, then through the tree) and
// the sides of the cells found. The component is told the result (vf_0x550). The actor is read
// without a check once the query is enabled, as in the game.
// NON_MATCHING(minor): register allocation and stack slots (the original keeps the "SP_Emmy" id
// and the first callback copy in separate slots and allocates the loop's registers differently);
// the same calls with the same arguments in the same order.
void fn_7100286c8c(UnkNavQuery* self, UnkNavMesh* mesh) {
    if (!mesh)
        return;
    if (self->start_cell == -1)
        self->start_cell = fn_710027a41c(mesh, &self->start, 0);
    UnkNavActor* actor = self->actor;
    UnkNavGroupInfo* info = actor && actor->component ? fn_710110c3d8(actor->component) : nullptr;
    UnkNavMeshManager* manager = UnkNavManager();
    if (info)
        self->group = fn_7100ec13cc(manager, &info->name);
    else {
        CStrId name("SP_Emmy", true);
        self->group = fn_7100ec13cc(manager, &name);
    }
    if (!self->enabled)
        return;
    u16 cell = self->goal_cell;
    if (cell == 0xffff) {
        cell = fn_710027a41c(mesh, &self->goal, self->flags);
        self->goal_cell = cell;
    }
    if (!(self->field_0x158 & 2))
        return;
    UnkDirectionFilter* direction = reinterpret_cast<UnkDirectionFilter*>(self->direction);
    if (self->hits.count == 0) {
        if (UnkNavComponent* c = actor->component)
            c->vtable->vf_0x548(c, cell, self, &self->hits);
        if (self->hits.count == 0) {
            fn_710027dab8(UnkNavManager()->navmesh, &self->goal, self->goal_cell, self->flags, self->field_0x190,
                          self->field_0x6c, self->field_0x68, self->filter, &self->hits);
            if (self->hits.count == 0) {
                UnkNavMesh* nav = UnkNavManager()->navmesh;
                UnkNavCallbackCopy a(self->field_0x0);
                UnkNavCallbackCopy b(self->filter);
                UnkNavCallbackCopy c(self->verdict);
                fn_710027bbf0(nav, &self->goal, self->flags, self->field_0x190, self->field_0x6c, self->field_0x68,
                              direction, &self->hits, &a, &b, &c, 1000.0f, 400.0f, 700.0f, 700.0f);
                if (self->hits.count == 0) {
                    UnkNavMesh* nav2 = UnkNavManager()->navmesh;
                    UnkNavCallbackCopy filter(self->filter);
                    UnkNavCallbackCopy verdict(self->verdict);
                    fn_710027b780(nav2, &self->goal, self->flags, self->field_0x190, self->field_0x6c, self->field_0x68,
                                  direction, &self->hits, filter.As<UnkNavSideFilter>(),
                                  verdict.As<UnkNavSideVerdict>());
                    if (self->hits.count == 0)
                        goto done;
                }
                u32 n = self->hits.count;
                for (u64 i = 0; i < n; ++i)
                    fn_710027dab8(UnkNavManager()->navmesh, &self->hits.At(i).point, self->hits.At(i).cell,
                                  self->flags, self->field_0x190, self->field_0x6c, self->field_0x68, self->filter,
                                  &self->hits);
            }
        }
    }
done:
    if (UnkNavComponent* c = actor->component)
        c->vtable->vf_0x550(c, self->goal_cell, self, &self->hits);
}
