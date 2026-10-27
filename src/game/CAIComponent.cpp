// Translation unit 2702 of data/tu_map.csv (0x71010f41a0-0x71011173fc), named after the
// CAIComponent reflection data it registers: an AI component's path request toward its target
// (fn_7101103080). The navigation query and its types repeat the ones of src/game/UnkNavQuery.cpp
// and src/game/navmesh/CNavMesh.cpp (the same definitions). Names and layouts are placeholders.
#include <functional>
#include <math.h>
#include <new>

#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

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

struct UnkNavMeshSideSegment;   // src/game/navmesh/CNavMesh.cpp

template <class R, class... A>
struct UnkNavMeshCallback {
    void* ctx;
    R (*fn)(void* ctx, A... args);

    explicit operator bool() const { return fn != nullptr; }
    R operator()(A... args) const { return fn(ctx, args...); }
};

using UnkNavSideFilter = UnkNavMeshCallback<bool, u16, u8, void*, void*, u64, u32, u32>;
using UnkNavSideVerdict = UnkNavMeshCallback<s32, UnkNavMeshSideSegment*>;

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

// A side of the path and a position with it (0x14 bytes): the request's three candidates.
struct UnkAIPathCandidate {
    UnkNavMeshHit hit;
    CVector2D pos;

    UnkAIPathCandidate() : pos{0.0f, 0.0f} {}
    void Reset() {
        hit.cell = 0xffff;
        hit.side = 0xff;
        hit.point.x = 0.0f;
        hit.point.y = 0.0f;
        pos.x = 0.0f;
        pos.y = 0.0f;
    }
    bool IsValid() const { return (s16)hit.cell != -1 && hit.side != 0xff; }
};
static_assert(sizeof(UnkAIPathCandidate) == 0x14, "UnkAIPathCandidate placeholder layout");

struct UnkAIPathCandidates {
    UnkAIPathCandidate c[3];
};

// The list of sides the query starts from (up to 16).
struct UnkAIPathHits {
    UnkNavMeshHits hits;
    u8 items[0xd4];

    UnkAIPathHits() { hits.count = 0; }
    ~UnkAIPathHits() { hits.Clear(); }
};

// Where the request ends up, for the caller: the point and whether it is set.
struct UnkAIPathPoint {
    f32 x, y;
    bool set;
};

// The follower at +0x68 of the component: it is ready when its count is above one, or one with
// its flag clear.
struct UnkAIPathFollower {
    u8 pad_0x0[0x8];
    u32 count;
    u8 pad_0xc[0x20];
    u8 flag;

    bool IsReady() const { return count != (u32)((count != 0) & !flag); }
};

// What a side search fills (0x14 bytes).
struct UnkAIPathSearchOut {
    u64 a;
    u64 b;
    f32 distance;

    UnkAIPathSearchOut() : a(0), b(0), distance(-1.0f) {}
};

// The side search from `from` toward `to` (fn_710035e8d8): `blocked` tells whether it stopped on
// a side, `point` where.
bool fn_710035e8d8(UnkAIPathFollower* self, const CVector2D* from, const CVector2D* to, const CVector2D* dir_a,
                   const CVector2D* dir_b, bool flag, CVector2D* point, u32* out_0x7, f32 limit, bool* blocked,
                   f32* out_a, f32* out_b, u32* out_c, UnkAIPathSearchOut* out, std::function<void()> filter);

struct UnkAIPathFlags {
    u8 pad_0x0[0x22c];
    bool enabled;
};

// The target actor's tracked entry (its +0x1f8): the side it was last on (vf_0x2d8).
struct UnkAITracked;
struct UnkAITrackedVtbl {
    u8 pad_0x0[0x2d8];
    UnkNavMeshHit (*vf_0x2d8)(UnkAITracked* self);
};
struct UnkAITracked {
    UnkAITrackedVtbl* vtable;
};

// Its reach (the first float is the radius).
struct UnkAIReach {
    f32 radius;
    f32 field_0x4;

    UnkAIReach() : radius(0.0f), field_0x4(0.0f) {}
    UnkAIReach(const UnkAIReach& other);
};

struct UnkAIComponent;
struct UnkAIComponentVtbl {
    u8 pad_0x0[0x7a8];
    bool (*vf_0x7a8)(UnkAIComponent* self, UnkAIPathCandidates* candidates);
    s32 (*vf_0x7b0)(UnkAIComponent* self, UnkAIPathCandidates* candidates, UnkAIPathPoint* point);
};

struct UnkAIComponent {
    UnkAIComponentVtbl* vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;
    u8 pad_0x38[0x30];
    UnkAIPathFollower follower;
    u8 pad_0x98[0x1568];
    UnkNavMeshHit last_side;
    u8 pad_0x160c[0x4];
    UnkAIPathFlags* field_0x1610;
};
static_assert(__builtin_offsetof(UnkAIComponent, follower) == 0x68, "UnkAIComponent placeholder layout");
static_assert(__builtin_offsetof(UnkAIComponent, last_side) == 0x1600, "UnkAIComponent placeholder layout");
static_assert(__builtin_offsetof(UnkAIComponent, field_0x1610) == 0x1610, "UnkAIComponent placeholder layout");

struct UnkStruct_7100286798;  // the query as src/game_lo/fn_7100286798.cpp names it
struct UnkActor_7100286798;
struct UnkStruct_7100286624;  // the query as src/game_lo/fn_7100286624.cpp names it

void fn_710028659c(UnkNavQuery* self);
void fn_7100286798(UnkStruct_7100286798* self, UnkActor_7100286798* actor);
void fn_7100286624(UnkStruct_7100286624* self);
void fn_7100286920(UnkNavQuery* self, const CVector2D* point, u16 cell, u8 side);
void fn_7100286964(UnkNavQuery* self, const CVector2D* start, u16 cell, UnkNavMeshHits* hits);
bool fn_7100ebfa6c(UnkNavMeshManager* self, UnkNavQuery* query, void* path);
bool fn_7101109e8c(UnkAIComponent* self, UnkNavMeshHit* side);
bool fn_7100979ef0(void* tracked);
UnkAIReach fn_7100977b90(void* tracked);

// The component's handlers the query calls back.
void fn_7100708034();
void fn_71003809ec();
void fn_7100380a04();
void fn_7100380a10();
void fn_71003809f8();

// A callback on `obj`: none without one.
template <class C>
static inline void UnkBind(C* cb, void* obj, void (*fn)()) {
    if (obj) {
        cb->ctx = obj;
        cb->fn = reinterpret_cast<decltype(cb->fn)>(fn);
    } else {
        cb->ctx = nullptr;
        cb->fn = nullptr;
    }
}

static inline CVector3D UnkTranslation(const UnkActor* actor) {
    CVector3D v;
    v.x = actor->field_0x44;
    v.y = actor->field_0x54;
    v.z = actor->field_0x64;
    return v;
}


// Plans a path from the owner to its target into `path`: the target's side (or the last side
// remembered when the flags at +0x1610 ask for it and the target is tracked), the three candidate
// sides the component picks (vf_0x7a8, vf_0x7b0; one is moved onto the target's tracked side when
// the second side search reaches it within half its reach), copied to `candidates` when given, and
// the chosen one's position to `point`. False without a target or a side for it.
// NON_MATCHING(minor): register allocation (the frame is 0x10 larger), and the callbacks are stored
// in another order, the null case without the original's shared tail; the same calls with the same
// arguments in the same order, and the same locals at the same offsets.
bool fn_7101103080(UnkAIComponent* self, void* path, UnkAIPathPoint* point, UnkAIPathCandidates* candidates) {
    if (candidates)
        for (UnkAIPathCandidate& c : candidates->c)
            c.Reset();
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return false;
    UnkAIPathHits hits;
    CVector3D pos = UnkTranslation(target);
    UnkNavMeshHit side;
    bool result = false;
    if (fn_7101109e8c(self, &side)) {
        UnkNavQuery query;
        fn_710028659c(&query);
        fn_7100286798(reinterpret_cast<UnkStruct_7100286798*>(&query),
                      reinterpret_cast<UnkActor_7100286798*>(self->owner));
        fn_7100286920(&query, &side.point, side.cell, side.side);
        void* component = self->owner->field_0x240;
        UnkBind(&query.field_0x0, component, &fn_7100708034);
        UnkBind(&query.filter, component, &fn_71003809ec);
        UnkBind(&query.field_0x20, component, &fn_7100380a04);
        UnkBind(&query.field_0x30, component, &fn_7100380a10);
        UnkBind(&query.verdict, component, &fn_71003809f8);
        u16 cell;
        UnkAIPathFlags* flags = self->field_0x1610;
        if (flags && flags->enabled && fn_7100979ef0(target->field_0x1f8)) {
            const UnkNavMeshHit& last = self->last_side;
            hits.hits.Push(last);
            cell = last.cell;
            pos.x = last.point.x;
            pos.y = last.point.y;
            pos.z = 0.0f;
        } else {
            UnkAIPathCandidates c;
            if (self->vtable->vf_0x7a8(self, &c)) {
                CVector2D from;
                fn_710007b430(&from, &static_cast<const CVector3D&>(UnkTranslation(self->owner)));
                const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
                CVector2D dir;
                fn_710007b430(&dir, &static_cast<const CVector3D&>(CVector3D{m.m[2], m.m[6], m.m[10]}));
                if (c.c[0].IsValid() && c.c[2].IsValid() && self->follower.IsReady() &&
                    self->owner->field_0x1f8) {
                    CVector2D point_b, point_a;
                    u32 out7_b, out7_a;
                    bool blocked_b, blocked_a;
                    f32 a_b, b_b, a_a, b_a;
                    u32 c_b, c_a;
                    point_a.x = point_a.y = 0.0f;
                    point_b.x = point_b.y = 0.0f;
                    UnkAIPathSearchOut out_b, out_a;
                    fn_710035e8d8(&self->follower, &from, &c.c[2].hit.point, &dir, &dir, false, &point_b, &out7_b,
                                  0.0f, &blocked_b, &a_b, &b_b, &c_b, &out_b, std::function<void()>());
                    fn_710035e8d8(&self->follower, &from, &c.c[0].hit.point, &dir, &dir, false, &point_a, &out7_a,
                                  0.0f, &blocked_a, &a_a, &b_a, &c_a, &out_a, std::function<void()>());
                    if (!blocked_b && blocked_a) {
                        UnkAIReach reach =
                            self->owner->field_0x1f8 ? fn_7100977b90(target->field_0x1f8) : UnkAIReach();
                        f32 dx = c.c[0].hit.point.x - point_a.x;
                        f32 dy = c.c[0].hit.point.y - point_a.y;
                        if (sqrtf(dx * dx + dy * dy) <= reach.radius * 0.5f) {
                            {
                                UnkAITracked* tracked = static_cast<UnkAITracked*>(self->owner->field_0x1f8);
                                UnkNavMeshHit h = tracked->vtable->vf_0x2d8(tracked);
                                c.c[0].hit.cell = h.cell;
                                c.c[0].hit.side = h.side;
                                c.c[0].hit.point.x = h.point.x;
                                c.c[0].hit.point.y = h.point.y;
                            }
                            CVector2D p;
                            fn_710007b430(&p, &static_cast<const CVector3D&>(UnkTranslation(self->owner)));
                            c.c[0].pos.x = p.x;
                            c.c[0].pos.y = p.y;
                        }
                    }
                }
                s32 which = self->vtable->vf_0x7b0(self, &c, point);
                if (candidates) {
                    for (u32 i = 0; i < 3; ++i) {
                        candidates->c[i].hit.cell = c.c[i].hit.cell;
                        candidates->c[i].hit.side = c.c[i].hit.side;
                        candidates->c[i].hit.point.x = c.c[i].hit.point.x;
                        candidates->c[i].hit.point.y = c.c[i].hit.point.y;
                        candidates->c[i].pos.x = c.c[i].pos.x;
                        candidates->c[i].pos.y = c.c[i].pos.y;
                    }
                }
                switch (which) {
                case 0:
                    hits.hits.Push(c.c[0].hit);
                    cell = c.c[0].hit.cell;
                    pos.x = c.c[0].pos.x;
                    pos.y = c.c[0].pos.y;
                    pos.z = 0.0f;
                    break;
                case 1:
                    hits.hits.Push(c.c[1].hit);
                    cell = c.c[1].hit.cell;
                    pos.x = c.c[1].pos.x;
                    pos.y = c.c[1].pos.y;
                    pos.z = 0.0f;
                    break;
                case 2:
                    hits.hits.Push(c.c[2].hit);
                    cell = c.c[2].hit.cell;
                    pos.x = c.c[2].pos.x;
                    pos.y = c.c[2].pos.y;
                    pos.z = 0.0f;
                    break;
                default:
                    cell = 0xffff;
                    break;
                }
            } else {
                cell = 0xffff;
            }
        }
        if (point) {
            CVector2D p;
            fn_710007b430(&p, &pos);
            point->x = p.x;
            point->y = p.y;
            point->set = true;
        }
        CVector2D start;
        fn_710007b430(&start, &pos);
        fn_7100286964(&query, &start, cell, &hits.hits);
        result = fn_7100ebfa6c(UnkNavManager(), &query, path);
        fn_7100286624(reinterpret_cast<UnkStruct_7100286624*>(&query));
    }
    return result;
}
