// behaviortree::CQuetzoaFollowPathAction's unit: updates the node. While the owner's navigation (+0x298)
// follows a path of kind 2 it works out how hard to steer along it (a share of the distance between the
// path's two points, a side from the owner's facing), remembers the owner's velocity (+0x3c) and hands a
// path request to the navigation; otherwise it runs the plain update (fn_710064755c). Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkKraidCreatureVec.h"
#include "game/UnkTU2523.h"
#include "game/behaviortree/CFlyingFollowPathAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern const CVector3D g_unk_710164a3b0 __attribute__((visibility("hidden")));
extern const CVector3D g_unk_710164a3bc __attribute__((visibility("hidden")));

struct UnkQuetzoaFollow_l647864;
void fn_710064755c(UnkQuetzoaFollow_l647864* self);

struct UnkQuetzoaAI_l647864 : base::reflection::UnkReflObject {};

struct UnkPathState_l647864 {
    u8 pad_0x0[0xc0];
    f32 from_x;  // 0xc0
    f32 from_y;  // 0xc4
    u8 pad_0xc8[0xc];
    f32 near_x;  // 0xd4
    f32 near_y;  // 0xd8
    f32 to_x;    // 0xdc
    f32 to_y;    // 0xe0
};

struct UnkPathKind_l647864 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkPathEntry_l647864 {
    u8 pad_0x0[0x10];
    UnkPathKind_l647864* kind;  // 0x10
};

struct UnkPathRecord_l647864 {
    UnkPathEntry_l647864* entry;  // 0
    u8 pad_0x8[8];
    UnkPathState_l647864* state;  // 0x10
};

struct UnkNavigation_l647864 {
    u8 pad_0x0[0xb0];
    UnkPathRecord_l647864* record;  // 0xb0
};

struct UnkBody_l647864 {
    u8 pad_0x0[0x40];
    CVector3D field_0x40;
};

struct UnkOwner_l647864 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    f32 pad_0x78[0];
    u8 pad_0x78b[0x1f0 - 0x78];
    UnkBody_l647864* body;  // 0x1f0
    u8 pad_0x1f8[0x240 - 0x1f8];
    UnkQuetzoaAI_l647864* ai;  // 0x240
    u8 pad_0x248[0x50];
    UnkNavigation_l647864* navigation;  // 0x298
};

struct UnkActorPos_l647864 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkQuetzoaFollow_l647864 {
    u8 pad_0x0[0x18];
    UnkOwner_l647864* owner;  // 0x18
    u8 pad_0x20[0xc];
    bool has_offset;  // 0x2c
    u8 pad_0x2d[3];
    f32 offset;  // 0x30
    bool has_limit;  // 0x34
    u8 pad_0x35[3];
    f32 limit;  // 0x38
    CVector3D velocity;  // 0x3c
};

// A path request: optional values with their flags, as fn_71006f4b0c takes it.
struct UnkPathRequest_l647864 {
    void** vtable;
    CVector3D direction;  // 0x8
    bool has_direction;   // 0x14
    f32 offset;           // 0x18
    bool has_offset;      // 0x1c
    u64 field_0x20;
    u64 field_0x28;
    bool field_0x30;
    u32 field_0x34;
    u8 pad_0x38[4];
    struct Entry {
        f32 value;
        bool set;
    } entries[7];  // 0x3c
    bool field_0x78;
    u8 pad_0x79[0x17];
    bool field_0x90;
    bool field_0x92;
    bool field_0x94;
    bool field_0x9c;
    bool field_0x9e;
    struct Flagged {
        bool flag;
        u64 field_0x8;
        u64 field_0x10;
    } flagged[3];  // 0xa0
    u32 field_0xe8;
    bool field_0xec;
    u32 field_0xf0;
    base::IAllocator* allocator;  // 0xf8
    bool field_0x100;
    bool field_0x102;
    bool field_0x104;
    bool field_0x106;

    UnkPathRequest_l647864() {
        has_direction = false;
        has_offset = false;
        field_0x20 = 0;
        field_0x28 = 0;
        field_0x30 = false;
        field_0x34 = 0;
        direction = {0, 0, 0};
        vtable = reinterpret_cast<void**>(g_unk_7101a264d0 + 0x10);
        for (u32 i = 0; i < 7; i++)
            entries[i].set = false;
        field_0x78 = false;
        field_0x90 = false;
        field_0x92 = false;
        field_0x94 = false;
        field_0x9c = false;
        field_0x9e = false;
        for (u32 i = 0; i < 3; i++) {
            flagged[i].flag = false;
            flagged[i].field_0x8 = 0;
            flagged[i].field_0x10 = 0;
        }
        field_0xe8 = 10;
        field_0xec = true;
        field_0xf0 = 15;
        allocator = GetDefaultAllocator();
        field_0x100 = false;
        field_0x102 = false;
        field_0x104 = false;
        field_0x106 = false;
        field_0x34 = 2;
    }
    ~UnkPathRequest_l647864() {
        vtable = reinterpret_cast<void**>(g_unk_7101a264d0 + 0x10);
        if (void* data = reinterpret_cast<void*>(flagged[2].field_0x8)) {
            reinterpret_cast<u32*>(&flagged[2].field_0x10)[0] = 0;
            allocator->vf_0x48(data);
            reinterpret_cast<u32*>(&flagged[2].field_0x10)[1] = 0;
        }
    }
};

static inline f32 Sign(f32 x) {
    return (f32)((x > 0.0f) - (x < 0.0f));
}

u32 fn_7100647864(UnkQuetzoaFollow_l647864* self) {
    UnkQuetzoaAI_l647864* ai = self->owner->ai;
    if (!ai)
        return 6;
    if (!fn_710009d50c(ai->vf_0x0(), fn_7100649630()))
        return 6;
    UnkNavigation_l647864* navigation = self->owner->navigation;
    if (!navigation)
        return 6;
    UnkPathRecord_l647864* record = navigation->record;
    if (!record->entry || !record->entry->kind || record->entry->kind->kind != 2 || !record->state) {
        fn_710064755c(self);
        return 2;
    }
    f32 fromX = record->state->from_x;
    f32 fromY = record->state->from_y;
    UnkBody_l647864* body = self->owner->body;
    const UnkMatrix44& xform = fn_710002614c(&self->owner->xform);
    CVector3D axis = {xform.m[2], xform.m[6], xform.m[10]};
    CVector2D facing;
    fn_710007b430(&facing, &axis);
    f32 toX = navigation->record->state->to_x;
    f32 toY = navigation->record->state->to_y;
    f32 dirY = toY - fromY;
    f32 dirX = toX - fromX;
    f32 len = __builtin_sqrtf(dirX * dirX + dirY * dirY);
    if (len != 0.0f) {
        f32 inv = 1.0f / len;
        dirX *= inv;
        dirY *= inv;
    }
    CVector2D bodyDir;
    fn_710007b430(&bodyDir, &body->field_0x40);
    UnkPathState_l647864* state = navigation->record->state;
    f32 gx = state->from_x - state->near_x;
    f32 gy = state->from_y - state->near_y;
    f32 share = __builtin_sqrtf(gx * gx + gy * gy) / 250.0f;
    f32 amount = share > 0.0f ? (!(share >= 1.0f) ? share : 1.0f) : 0.0f;

    UnkPathRequest_l647864 request;
    f32 speed = 1.0f;
    f32* offset = &self->offset;
    bool steer = false;
    if (amount < 1.0f && dirX * facing.x + dirY * facing.y < 0.0f) {
        CVector3D here = {self->owner->xform.m[0], 0, 0};
        UnkActorPos_l647864* actor = reinterpret_cast<UnkActorPos_l647864*>(self->owner);
        here = {actor->x, actor->y, actor->z};
        CVector2D here2;
        fn_710007b430(&here2, &here);
        f32 ey = toY - here2.y;
        f32 ex = toX - here2.x;
        f32 l = __builtin_sqrtf(ex * ex + ey * ey);
        f32 nx = l == 0.0f ? ex : ex * (1.0f / l);
        const CVector3D& side = nx >= 0.0f ? g_unk_710164a3b0 : g_unk_710164a3bc;
        request.direction = side;
        request.has_direction = true;
        self->has_offset = true;
        steer = true;
    } else if (Sign(facing.x) == Sign(bodyDir.x)) {
        self->has_offset = false;
    } else {
        steer = self->has_offset;
    }
    if (steer) {
        f32 limit;
        if (amount >= 1.0f)
            limit = 1.0f;
        else if (amount < 0.0f)
            limit = 0.2f;
        else
            limit = amount + (1.0f - amount) * 0.2f;
        *offset = *offset < limit ? *offset : limit;
        speed = 10.0f;
    } else {
        self->offset = 1.0f;
        if (self->has_limit)
            self->offset = self->limit;
        else
            speed = 0.5f;
    }
    UnkActorPos_l647864* actor = reinterpret_cast<UnkActorPos_l647864*>(self->owner);
    f32 dx = fromX - actor->x;
    f32 dy = fromY - actor->y;
    f32 dz = 0.0f - actor->z;
    if (dx * self->velocity.x + dy * self->velocity.y + dz * self->velocity.z < 0.0f) {
        speed = 100.0f;
        *offset = 0.0f;
    }
    UnkKraidCreatureVec velocity = fn_7100d7be08(self->owner);
    self->velocity.x = velocity.x;
    self->velocity.y = velocity.y;
    self->velocity.z = velocity.z;
    request.entries[4].value = speed;
    request.entries[4].set = true;
    request.offset = *offset;
    request.has_offset = true;
    fn_71006f4b0c(navigation, &request, 0);
    return 2;
}
