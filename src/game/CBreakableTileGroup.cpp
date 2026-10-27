// The CBreakableTileGroup unit (tools/tu_map.py: 0x7100c9da0c-0x7100ca3890, whose reflected class
// is CBreakableTileGroup): a group of breakable tiles (0xa8 bytes each, in an array at +0x48 of the
// group) and the tiles' state changes. The group is probably the reflected CBreakableTileGroup; the
// names and layouts here are placeholders.
// Holds fn_7100c9e2a8 (whether a tile may respawn), fn_7100c9e6ac (a tile's state change) and
// fn_7100ca2978 (the tiles a shape touches).
#include "base/UnkDeferredSet.h"
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

namespace base {
template <class Arg>
struct UnkDeferredCallback;
}

struct UnkTileGroup;

// One tile of a group. `state` is 0 (intact), 1 (hit) or 2 (broken).
struct UnkQueryObj {
    u32 field_0x0;  // the tile's index in its group
    u32 field_0x4;  // its type (the enum registered next to fn_7100ca0118's clone: 7 is "WEIGHT")
    u32 field_0x8;
    u32 field_0xc;
    f32 field_0x10;  // the respawn time (fn_7100ca6af8 of the type), unless field_0x3c is set
    u32 pad_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    base::global::CStrId field_0x20;  // a render layer, switched with the state when it is not empty
    f32 field_0x28;                   // x
    f32 field_0x2c;                   // y
    UnkTileGroup* field_0x30;
    s32 state;  // 0x38
    bool field_0x3c;
    u8 pad_0x3d[0xb];
    u64 field_0x48;  // a render object, 0xffffffff when there is none
    u8 pad_0x50[0x58];
};
static_assert(sizeof(UnkQueryObj) == 0xa8, "UnkQueryObj placeholder layout");

struct UnkTileGroup {
    u8 pad_0x0[0x10];
    u32 field_0x10;  // the group's id
    bool field_0x14;
    u8 pad_0x15[0xb];
    f32 field_0x20;  // z of every tile
    u8 pad_0x24[0x1c];
    u32 field_0x40;           // the tile count
    UnkQueryObj* field_0x48;  // the tiles
};

static const u64 kNoRenderObject = 0xffffffff;

// The deferred callbacks told of every state change, with an UnkTileStateEvent.
using UnkTileEventSet = base::UnkDeferredSet<base::UnkDeferredCallback<const void*>>;
extern UnkTileEventSet g_unk_7172795e08;

struct UnkTileStateEvent {
    u32 group;
    u32 tile;
    s32 old_state;
    s32 new_state;

    UnkTileStateEvent(u32 group_, u32 tile_, s32 old_state_, s32 new_state_)
        : group(group_), tile(tile_), old_state(old_state_), new_state(new_state_) {}
};

// A zero vector in .rodata (the rotation of a tile's render object).
extern const UnkVec3f g_unk_710164ba64 __attribute__((visibility("hidden")));

UnkMatrix44 fn_71000791a8(const UnkVec3f& translation, const UnkVec3f& angle, const UnkVec3f& scale);
void fn_7100219c6c(UnkRenderManager* self, bool enable, u64 layer, u32 arg3, bool arg4);
void fn_7100219e10(UnkRenderManager* self, u64 object);
void fn_7100219e64(UnkRenderManager* self, u64 object, const UnkMatrix44* m);
void fn_7100c9ea2c(UnkQueryObj* self);
void fn_7100c9eb08(UnkQueryObj* self, s32 old_state, s32 new_state, bool a, bool b);
void fn_7100ca0118(UnkTileEventSet* set, const void* event);
void fn_7100ca0c40(UnkTileGroup* self, u32 tile, UnkVec3f* pos);
void fn_7100ca0c78(UnkTileGroup* self, u32 tile, UnkVec3f* pos, UnkVec3f* scale);
void fn_7100ca34f0(UnkTileGroup* self);
void fn_7100ca34f4(UnkTileGroup* self, u32 a, u32 b);
f32 fn_7100ca6af8(u32 type);
void fn_7100ca7388(UnkGameManagerSub_0x1af8* self, u32 group, u32 a, u32 b);

// The tile size (a float in .rodata, reached through the GOT).
extern const f32 g_unk_710164ba70;

// A collision shape (0xc0 bytes) built from a 2D box (fn_7100d5e1f8) and destroyed by
// fn_7100d5e2c8.
struct UnkBox2D {
    f32 min_x, min_y, max_x, max_y;

    UnkBox2D(f32 min_x_, f32 min_y_, f32 max_x_, f32 max_y_)
        : min_x(min_x_), min_y(min_y_), max_x(max_x_), max_y(max_y_) {}
};
struct UnkCollisionShape {
    u8 data[0xc0];
};

// The hits of a collision query: an inline array of up to eight 0x48-byte records after the count,
// each starting with two reference-counted pointers (the hit object's handle first). The
// destructor (vtable g_unk_7101a23478) releases both, as fn_7100331700 does.
struct UnkHitComponent : base::reflection::UnkReflObject {
    u8 pad_0x8[0x28];
    void* field_0x30;
};
struct UnkHitEntity {
    u8 pad_0x0[0x10];
    UnkHitComponent* field_0x10;  // the hit component
};
struct UnkHitHandle : UnkRefObject {
    UnkHitEntity* field_0x10;
};
struct __attribute__((packed)) UnkHitRecord {
    UnkHitHandle* field_0x0;
    UnkRefObject* field_0x8;
    u8 pad_0x10[0x38];
};
extern void* g_unk_7101a23478[];

static inline void ReleaseRef(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

struct __attribute__((packed)) UnkHitList {
    void** vtable;
    u32 count;
    u8 storage[8 * sizeof(UnkHitRecord)];

    UnkHitRecord* items() { return reinterpret_cast<UnkHitRecord*>(storage); }

    UnkHitList() {
        vtable = g_unk_7101a23478 + 2;
        count = 0;
    }
    ~UnkHitList() {
        vtable = g_unk_7101a23478 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef(items()[i].field_0x8);
            ReleaseRef(items()[i].field_0x0);
        }
        count = 0;
    }
    UnkHitRecord* begin() { return items(); }
    UnkHitRecord* end() { return reinterpret_cast<UnkHitRecord*>(storage + count * sizeof(UnkHitRecord)); }
};

// The player's +0x208: a movement helper.
struct UnkHelper {
    void** vtable;

    bool vf_0x3b8() { return reinterpret_cast<bool (*)(UnkHelper*)>(vtable[0x3b8 / 8])(this); }
};

struct UnkPlayer {
    u8 pad_0x0[0x1f0];
    void* field_0x1f0;
    u8 pad_0x1f8[0x10];
    UnkHelper* field_0x208;
    u8 pad_0x210[0x50];
    void* field_0x260;
};

// The objects fn_7100552d7c lists: a tile may not come back while one of them has a +0x1b8 whose
// +0x70 is 2.
struct UnkListedState {
    u8 pad_0x0[0x70];
    s32 field_0x70;
};
struct UnkListedOwner {
    u8 pad_0x0[0x1b8];
    UnkListedState* field_0x1b8;
};
struct UnkListedEntity {
    u8 pad_0x0[0x30];
    UnkListedOwner* field_0x30;
};
struct UnkListedItem {
    u8 pad_0x0[0x10];
    UnkListedEntity* field_0x10;
};

void fn_7100d5e1f8(UnkCollisionShape* self, const UnkBox2D* box);
void fn_7100d5e2c8(UnkCollisionShape* self);
// The collision query (src/game_hi/fn_7100d5b484.cpp, whose names for the tree, the shape and the
// matrix these are).
struct UnkTree;
struct UnkShape;
struct UnkMtx44;
bool fn_7100d5b484(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, void* arg, u16 arg2);
void* fn_7100c9ff5c(void* p);
UnkPlayer* fn_7100dec788(UnkGameManager* manager);
bool fn_710113d83c(void* p);
bool fn_71009e12d0(UnkHelper* helper);
bool fn_71009e6434(UnkHelper* helper);
UnkVec3f fn_71009ebe68(UnkHelper* helper);
base::UnkVector<UnkListedItem*>* fn_7100552d7c();

static inline void SetIdentity(UnkMatrix44* m) {
    *m = UnkMatrix44{};
    m->m[15] = 1.0f;
    m->m[10] = 1.0f;
    m->m[5] = 1.0f;
    m->m[0] = 1.0f;
}

// Whether a broken tile may come back: nothing may overlap its square (for a type 7 tile, only
// collision components that fn_7100c9ff5c maps to something), the player must not be inside or
// just beside it, and none of fn_7100552d7c's objects may be in state 2.
// NON_MATCHING(minor): the type 7 loop over the hits: the original tests the hits' byte size
// (count * 0x48) for emptiness and leaves the loop through one ccmp of "result non-null and hits
// left"; ours tests the count and leaves through two branches, which shifts the rest by 24 bytes
// (the hit list's destructor loop is laid out in another block order). Also scheduling: ours keeps
// the hit list's vtable pointer with its +0x10 added in one register (the original adds it at each
// store) and orders the identity matrix's stores differently.
bool fn_7100c9e2a8(UnkQueryObj* self) {
    UnkGameManager* gm = g_unk_7101d47310;
    if (gm->field_0x608) {
        if (void* tracker = gm->field_0x608->field_0x820) {
            UnkCollisionShape shape;
            {
                f32 x = self->field_0x28;
                f32 y = self->field_0x2c;
                UnkBox2D box(x, y, x + g_unk_710164ba70, y + g_unk_710164ba70);
                fn_7100d5e1f8(&shape, &box);
            }
            bool ok;
            {
                UnkHitList hits;
                UnkMatrix44 identity;
                SetIdentity(&identity);
                if (!fn_7100d5b484(static_cast<UnkTree*>(tracker), 0x2000, reinterpret_cast<UnkShape*>(&shape),
                                   reinterpret_cast<const UnkMtx44*>(&identity), &hits, 0)) {
                    ok = true;
                } else if (self->field_0x4 == 7) {
                    ok = true;
                    UnkHitRecord* it = hits.begin();
                    UnkHitRecord* end = hits.end();
                    while (ok && it != end) {
                        UnkHitComponent* c = it->field_0x0->field_0x10->field_0x10;
                        if (!c || !fn_710009d50c(c->vf_0x0(), fn_710085fc44()))
                            ok = false;
                        else
                            ok = fn_7100c9ff5c(c->field_0x30) != nullptr;
                        ++it;
                    }
                } else {
                    ok = false;
                }
            }
            fn_7100d5e2c8(&shape);
            if (!ok)
                return false;
        }
    }
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (player && player->field_0x260 && fn_710113d83c(player->field_0x260))
        return false;
    player = fn_7100dec788(g_unk_7101d47310);
    if (player && player->field_0x1f0) {
        UnkHelper* h = player->field_0x208;
        if (h && fn_71009e12d0(h) && !fn_71009e6434(h)) {
            f32 size = g_unk_710164ba70;
            f32 half = size * 0.5f;
            UnkVec3f pos = fn_71009ebe68(h);
            f32 dy = half + self->field_0x2c - pos.y;
            bool tall = h->vf_0x3b8();
            if (dy >= -1.1920929e-07f && dy <= (tall ? size + size : size) + 1.1920929e-07f) {
                f32 dx = half + self->field_0x28 - pos.x;
                if (pos.x > self->field_0x28) {
                    if (dx > size * -1.1f && dx < 0.0f)
                        return false;
                } else {
                    if (dx < size * 1.1f && dx > 0.0f)
                        return false;
                }
            }
        }
    }
    bool result = true;
    base::UnkVector<UnkListedItem*>* items = fn_7100552d7c();
    for (UnkListedItem **it = items->data, **end = it + items->count; it != end; ++it) {
        UnkListedOwner* owner = (*it)->field_0x10->field_0x30;
        if (owner && owner->field_0x1b8)
            result &= owner->field_0x1b8->field_0x70 != 2;
    }
    return result;
}

static inline u64 LayerKey(const base::global::CStrId& id) {
    return *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(id.data) + 0x30);
}

static inline bool HasLayer(const UnkQueryObj* self) {
    return self->field_0x30->field_0x14 && self->field_0x20.data != &g_unk_7101d47108->empty;
}

static inline void ReleaseRenderObject(UnkQueryObj* self) {
    if (self->field_0x48 != kNoRenderObject) {
        fn_7100219e10(g_unk_7101d47300, self->field_0x48);
        self->field_0x48 = kNoRenderObject;
    }
}

// Switches a tile to `state`; a tile already there is only refreshed when it is hit (1).
void fn_7100c9e6ac(UnkQueryObj* self, u32 state, bool a, bool b) {
    if (self->state == (s32)state) {
        if (state == 1)
            fn_7100ca34f4(self->field_0x30, self->field_0x8, self->field_0xc);
        return;
    }
    if (self->state == 0 && HasLayer(self))
        fn_7100219c6c(g_unk_7101d47300, false, LayerKey(self->field_0x20), 0, true);
    s32 old_state = self->state;
    self->state = state;
    if (state == 1) {
        fn_7100c9ea2c(self);
        if (self->field_0x48 != kNoRenderObject) {
            UnkVec3f pos(0.0f, 0.0f, 0.0f);
            UnkVec3f scale(1.0f, 1.0f, 1.0f);
            fn_7100ca0c40(self->field_0x30, self->field_0x0, &pos);
            fn_7100ca0c78(self->field_0x30, self->field_0x0, &pos, &scale);
            UnkMatrix44 m = fn_71000791a8(pos, g_unk_710164ba64, scale);
            fn_7100219e64(g_unk_7101d47300, self->field_0x48, &m);
        }
    } else if (state == 2) {
        ReleaseRenderObject(self);
        if (!self->field_0x3c)
            self->field_0x10 = fn_7100ca6af8(self->field_0x4);
        self->field_0x1c = self->field_0x18;
    } else if (state == 0) {
        ReleaseRenderObject(self);
        if (HasLayer(self))
            fn_7100219c6c(g_unk_7101d47300, true, LayerKey(self->field_0x20), 0, false);
    }
    fn_7100c9eb08(self, old_state, self->state, a, b);
    if (state == 2) {
        fn_7100ca34f0(self->field_0x30);
        fn_7100ca7388(g_unk_7101d47310->field_0x1af8, self->field_0x30->field_0x10, self->field_0x8,
                      self->field_0xc);
    }
    UnkTileStateEvent event(self->field_0x30->field_0x10, self->field_0x0, old_state, self->state);
    fn_7100ca0118(&g_unk_7172795e08, &event);
}

// What fn_7100ca2978 reports for each tile a shape touches: the point (the tile's centre at the
// group's z), the direction from the tile's corner to it, the side of the tile it is on and flags.
struct UnkTileContact {
    base::math::CVector3D pos;
    base::math::CVector3D dir;
    base::math::CVector3D normal;
    u32 field_0x24;
    u32 flags;

    UnkTileContact(f32 x, f32 y) : dir(), normal(), field_0x24(0) {
        pos.x = x;
        pos.y = y;
        flags = 8;
    }
    UnkTileContact(const UnkTileContact& o) {
        pos.x = o.pos.x;
        pos.y = o.pos.y;
        pos.z = o.pos.z;
        dir.x = o.dir.x;
        dir.y = o.dir.y;
        dir.z = o.dir.z;
        normal.x = o.normal.x;
        normal.y = o.normal.y;
        normal.z = o.normal.z;
        field_0x24 = o.field_0x24;
        flags = o.flags;
    }
};
struct UnkTileHit {
    s32 id;
    UnkTileContact contact;

    UnkTileHit(s32 id_, const UnkTileContact& contact_) : id(id_), contact(contact_) {}
};
static_assert(sizeof(UnkTileHit) == 0x30, "UnkTileHit placeholder layout");

// The visitor's result, returned through x8 and dropped.
struct UnkTileVisitResult {
    u64 value;
    UnkTileVisitResult(const UnkTileVisitResult& other) : value(other.value) {}
};

struct UnkTileQueryShape {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual bool vf_0x50(const base::math::CVector2D& point);  // whether the shape holds the point
};

struct UnkTileVisitor {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkTileVisitResult vf_0x10(const UnkTileHit* hit);
};

extern "C" f32 sqrtf(f32);

// Reports every tile that is not broken and whose centre `shape` holds to `visitor`; whether there
// was one.
bool fn_7100ca2978(UnkTileGroup* self, UnkTileQueryShape* shape, void* unused, UnkTileVisitor* visitor) {
    u64 i = 0;
    bool any = false;
    for (; i < self->field_0x40; ++i) {
        UnkQueryObj* tile = &self->field_0x48[i];
        if (tile->state == 2)
            continue;
        f32 x = tile->field_0x28;
        f32 y = tile->field_0x2c;
        f32 cx = x + 50.0f;
        f32 cy = y + 50.0f;
        if (!shape->vf_0x50(base::math::CVector2D{cx, cy}))
            continue;
        f32 max_y = y + 100.0f;
        f32 max_x = x + 100.0f;
        UnkTileContact contact(cx, cy);
        contact.pos.z = self->field_0x20;
        base::math::CVector3D normal;
        {
            base::math::CVector2D p;
            fn_710007b430(&p, &contact.pos);
            f32 sum_x = x + max_x;
            f32 sum_y = y + max_y;
            f32 mid_x = sum_x * 0.5f;
            f32 mid_y = sum_y * 0.5f;
            f32 dx = p.x - mid_x;
            f32 dy = p.y - mid_y;
            f32 sx = dx < 0.0f ? -1.0f : 1.0f;
            f32 sy = dy < 0.0f ? -1.0f : 1.0f;
            if (__builtin_fabsf(p.x - mid_x) > __builtin_fabsf(p.y - mid_y))
                normal = base::math::CVector3D{sx, 0.0f, 0.0f};
            else
                normal = base::math::CVector3D{0.0f, sy, 0.0f};
        }
        f32 vx = contact.pos.x - x;
        f32 vy = contact.pos.y - y;
        f32 vz = 0.0f;
        f32 inv = 1.0f / (sqrtf(vx * vx + vy * vy + vz * vz) + 0.0001f);
        contact.dir = base::math::CVector3D{vx * inv, vy * inv, vz * inv};
        contact.normal = normal;
        contact.flags |= 0x30;
        UnkTileHit hit(-1, contact);
        visitor->vf_0x10(&hit);
        any = true;
    }
    return any;
}
