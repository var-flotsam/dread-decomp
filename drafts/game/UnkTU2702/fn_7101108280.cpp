// Unit 2702 (a CAIComponent unit): sends the owner's action named for the side the target is on (or the
// plain one), then, when the owner has a movement, probes the floor below it and launches it toward the
// target, faced its way. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/CMovementComponent.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2517.h"
#include "game/UnkTU365.h"
#include "types.h"

#include "../../game_aca57c/UnkBufferString.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

// A string on 0x80 bytes of inline storage.
struct UnkString80_l108280 : UnkBufferString<0x80, g_unk_7101a1b750> {
    explicit UnkString80_l108280(const CRntString& str) { fn_710007eaa0(Str(), &str); }
};

// The id's string, counting the read.
static inline const CRntString& Text(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// The two id slots this unit reads that the table's header does not name yet.
static inline const CStrId& Slot(const UnkGameManagerIdTable& table, u32 offset) {
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<const u8*>(&table) + offset);
}

struct UnkLeapMovement_l108280 {
    u8 pad_0x0[0xf8];
    bool field_0xf8;
    u8 pad_0xf9[0xb];
    bool field_0x104;
    u8 pad_0x105[0x7];
    f32 speed;  // 0x10c
    f32 jump;   // 0x110
};

struct UnkLeapActor_l108280 {
    void** vtable;
    u8 pad_0x8[0x3c];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x188];
    UnkLeapMovement_l108280* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    void* actions;  // 0x220
    u8 pad_0x228[0x164];
    f32 height;  // 0x38c

    void vf_0xd8(const CVector3D* rotation, bool flag) {
        reinterpret_cast<void (*)(UnkLeapActor_l108280*, const CVector3D*, bool)>(vtable[0xd8 / 8])(this, rotation,
                                                                                                    flag);
    }
};
static_assert(__builtin_offsetof(UnkLeapActor_l108280, movement) == 0x1f0, "layout");
static_assert(__builtin_offsetof(UnkLeapActor_l108280, actions) == 0x220, "layout");
static_assert(__builtin_offsetof(UnkLeapActor_l108280, height) == 0x38c, "layout");

struct UnkLeapAI_l108280 {
    u8 pad_0x0[0x30];
    UnkLeapActor_l108280* owner;  // 0x30
};

// ---- the collision-tree segment query (as src/game/UnkTU1501/UnkTU1501_FloorProbe_l9ee384.cpp spells it) ----

struct UnkHitRef_l108280 {
    base::UnkRefCounted* ptr;

    UnkHitRef_l108280() : ptr(nullptr) {}
    ~UnkHitRef_l108280() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_l108280 {
    f32 x, y, z;
    UnkHitVec_l108280(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkHit_l108280 {
    UnkHitRef_l108280 object;
    UnkHitRef_l108280 field_0x8;
    s32 id;
    UnkHitVec_l108280 point;
    UnkHitVec_l108280 field_0x20;
    UnkHitVec_l108280 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkHit_l108280()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

struct UnkSegmentQuery_l108280 {
    void** vtable;
    UnkHit_l108280 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentQuery_l108280(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentQuery_l108280() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkSegmentShape {
    u8 data[0xc0];
};
struct UnkTree;
struct UnkShape;
struct UnkMtx44;

static inline void SetIdentity(UnkMatrix44* m) {
    *m = UnkMatrix44{};
    m->m[15] = 1.0f;
    m->m[10] = 1.0f;
    m->m[5] = 1.0f;
    m->m[0] = 1.0f;
}

static inline CVector3D Position(const UnkLeapActor_l108280* actor) {
    CVector3D pos;
    pos.x = actor->x;
    pos.y = actor->y;
    pos.z = actor->z;
    return pos;
}

void fn_7101108280(UnkLeapAI_l108280* self, UnkLeapActor_l108280* target) {
    void* actions = self->owner->actions;
    if (!actions)
        return;

    f32 side = self->owner->x - target->x;
    const CRntString* text;
    if (side >= 0.0f)
        text = &Text(Slot(GameManagerIdTable(), 0x1df0));
    else
        text = &Text(Slot(GameManagerIdTable(), 0x15f0));
    UnkString80_l108280 name(*text);
    fn_710007f3b4(name.Str(), &Text(GameManagerIdTable().field_0x13f8));

    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    if (fn_7101176690(actions, name.Str()->key, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask))
        return;
    u64 plain = GameManagerIdTable().field_0x13f8.data->Key();
    base::reflection::UnkReflTypeIdArg plainMode;
    plainMode.id = 1;
    base::reflection::UnkReflTypeIdArg plainMask;
    plainMask.id = UnkAll_SpitSend().value;
    if (!fn_7101176690(actions, plain, 0xf4061ff25a2163d6, &plainMode, &g_unk_710164b958, &g_unk_7172769740,
                       &plainMask))
        return;

    UnkLeapMovement_l108280* movement = self->owner->movement;
    if (!movement)
        return;

    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    CVector2D from;
    {
        CVector3D pos = Position(self->owner);
        fn_710007b430(&from, &pos);
    }
    UnkSegmentShape shape;
    {
        CVector2D to = {from.x + 0.0f, from.y + -100000.0f};
        UnkSegment2D seg(from, to);
        fn_7100d693ac(&shape, &seg);
    }
    {
        UnkSegmentQuery_l108280 query(Position(self->owner));
        bool hit;
        {
            UnkMatrix44 identity;
            SetIdentity(&identity);
            hit = fn_7100d5b484(tree, 1, reinterpret_cast<UnkShape*>(&shape),
                                reinterpret_cast<const UnkMtx44*>(&identity), &query, 0);
        }

        UnkLeapActor_l108280* owner = self->owner;
        f32 y = owner->y;
        // Higher when the floor below is within reach of the owner's middle.
        f32 lift = 0.25f;
        if (hit) {
            f32 middle = y - owner->height * 0.5f;
            f32 floor = query.hit.point.y;
            lift = floor + -50.0f < middle && floor + 50.0f > middle ? 0.5f : 0.25f;
        }

        f32 dx = owner->x - target->x;
        f32 dy = y - target->y;
        CVector3D delta = {dx, dy, 0.0f};
        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z > 1.1920929e-07f) {
            CVector3D velocity;
            velocity.x = 0.0f;
            velocity.y = 0.0f;
            velocity.z = 0.0f;
            if (UnkLeapMovement_l108280* targetMovement = target->movement) {
                f32 speed = targetMovement->speed;
                if (dx * speed >= 0.0f) {
                    velocity.x += speed * 0.75f;
                    f32 up = velocity.y + lift * targetMovement->jump;
                    velocity.y = up > 0.0f ? up : 0.0f;
                }
            }
            movement->field_0x104 = false;
            CVector3D rotation = {0.0f, dx > 0.0f ? -1.5707964f : 1.5707964f, 0.0f};
            self->owner->vf_0xd8(&rotation, true);
            fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(movement),
                          reinterpret_cast<const UnkMovementVec_71010d08d4*>(&velocity), true);
            movement->field_0xf8 = false;
        }
    }
    fn_7100d69400(&shape);
}
