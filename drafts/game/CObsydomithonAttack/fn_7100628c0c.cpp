// CObsydomithonAttack unit (tools/tu_map.py unit 945): whether the attack may start. Past the base check it
// needs a target, a target not too close (its life component's +0x68 against fn_7100629368) and, with the
// Obsydomithon AI, the angle to the target accepted by fn_7100629600. Then, with a collision tree, each of the
// AI's candidate offsets (+0x14770, up to five) is tried: a box placed by the pose for its angle (one of two
// debug-grown pose tables, by the character class's flag 2), turned half a turn and moved to the owner, must
// hit the tree; the first one that does is kept at +0x1479c. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CAttack.h"
#include "game/CLifeComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

UnkMatrix44 fn_7100078e54(f32 a, f32 b, f32 c);

struct UnkObsydomithonAttack;
f32 fn_7100629368(UnkObsydomithonAttack* self);

struct UnkObsydomithonAI_b69a4;
struct UnkVec2_b69a4;
bool fn_710062f6e4(UnkObsydomithonAI_b69a4* self, f32* angle);
bool fn_710062f7d4(UnkObsydomithonAI_b69a4* self, const UnkVec2_b69a4* offset, f32* angle);

// A pose: a key and its matrix.
struct UnkPose_l628c0c {
    u32 key;
    UnkMatrix44 matrix;

    UnkPose_l628c0c() : key(0) {
        for (u32 i = 0; i < 16; ++i)
            matrix.m[i] = 0.0f;
        matrix.m[15] = 1.0f;
        matrix.m[10] = 1.0f;
        matrix.m[5] = 1.0f;
        matrix.m[0] = 1.0f;
    }
};
static_assert(sizeof(UnkPose_l628c0c) == 0x44, "layout");

// A pose array that grows on access when `auto_grow` is set (by `grow` at a time past its capacity).
struct UnkPoseArray_l628c0c;
void fn_7100467cc4(UnkPoseArray_l628c0c* self, u32 capacity);

struct UnkPoseArray_l628c0c {
    UnkPose_l628c0c* data;
    u32 size;
    u32 capacity;
    u32 grow;
    bool auto_grow;

    void Reserve(u32 n) { fn_7100467cc4(this, n); }

    void Resize(u32 n) {
        Reserve(n);
        for (u32 i = size; i < n; ++i)
            new (&data[i]) UnkPose_l628c0c();
        size = n;
    }

    UnkPose_l628c0c& At(u32 index) {
        if (auto_grow && index >= size) {
            u32 n = capacity;
            if (index >= n && grow)
                n = (index / grow + 1) * grow;
            Reserve(n);
            Resize(index + 1);
        }
        return data[index];
    }
};

__attribute__((visibility("hidden"))) extern UnkPoseArray_l628c0c g_unk_71726f8e38;
__attribute__((visibility("hidden"))) extern UnkPoseArray_l628c0c g_unk_71726f8e60;

struct UnkOffset_l628c0c {
    f32 x, y;
};

// Up to five offsets and their count.
struct UnkOffsetList_l628c0c {
    u32 count;
    UnkOffset_l628c0c items[5];

    UnkOffsetList_l628c0c(const UnkOffsetList_l628c0c& other) {
        for (u32 i = 0; i < other.count; ++i) {
            items[i].x = other.items[i].x;
            items[i].y = other.items[i].y;
        }
        count = other.count;
    }
    UnkOffset_l628c0c* begin() { return items; }
    UnkOffset_l628c0c* end() { return &items[count]; }
};

struct UnkObsydomithonAI_l628c0c : base::reflection::UnkReflObject {
    u8 pad_0x8[0x14768 - 0x8];
    f32 width;                      // 0x14768
    f32 height;                     // 0x1476c
    UnkOffsetList_l628c0c offsets;  // 0x14770
    UnkOffset_l628c0c offset;       // 0x1479c: the one that hit
};
static_assert(__builtin_offsetof(UnkObsydomithonAI_l628c0c, offset) == 0x1479c, "layout");

f32 fn_710062c8c8(UnkObsydomithonAI_l628c0c* ai);

struct UnkObsydomithonAttack_l628c0c;
bool fn_7100629600(UnkObsydomithonAttack_l628c0c* self, f32 angle);

struct UnkObsydomithonAttack_l628c0c {
    void* vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x50 - 0x10];
    u32 stage;  // 0x50
};

struct UnkTargetLife_l628c0c {
    u8 pad_0x0[0x68];
    f32 min_distance;  // 0x68
};

struct UnkTargetView_l628c0c {
    u8 pad_0x0[0x1c8];
    UnkTargetLife_l628c0c* life;  // 0x1c8
};

// A box: its centre, its two axes and its size.
struct UnkBoxDesc_l628c0c {
    CVector2D center;
    CVector2D axis_x;
    CVector2D axis_y;
    CVector2D size;
    u64 field_0x20;
};

struct UnkHitBox_l628c0c;
void fn_7100d683c0(UnkHitBox_l628c0c* self, const UnkMatrix44* pose, const UnkBoxDesc_l628c0c* desc);
void fn_7100d6845c(UnkHitBox_l628c0c* self);

struct UnkHitBox_l628c0c {
    u8 body[0xe8];

    UnkHitBox_l628c0c(const UnkMatrix44* pose, const UnkBoxDesc_l628c0c* desc) { fn_7100d683c0(this, pose, desc); }
    ~UnkHitBox_l628c0c() { fn_7100d6845c(this); }
};

static inline CVector3D Position_l628c0c(const UnkActor* actor) {
    CVector3D p;
    p.x = actor->field_0x44;
    p.y = actor->field_0x54;
    p.z = actor->field_0x64;
    return p;
}

bool fn_7100628c0c(UnkObsydomithonAttack_l628c0c* self, void* arg, u32 mode) {
    bool base = fn_7100360b84(reinterpret_cast<CAttack*>(self), arg, mode);
    auto* ai = static_cast<UnkObsydomithonAI_l628c0c*>(self->owner->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_710062c51c()))
        ai = nullptr;
    UnkActor* target = fn_7100d7c990(self->owner);
    bool result = target != nullptr;
    if (target) {
        UnkTargetLife_l628c0c* life;
        f32 angle;
        if (!target | !base)
            result = false;
        else if (self->stage != 0)
            result = true;
        else if ((life = reinterpret_cast<UnkTargetView_l628c0c*>(target)->life) && fn_7101161088(life) &&
                 life->min_distance > fn_7100629368(reinterpret_cast<UnkObsydomithonAttack*>(self)))
            result = false;
        else if (!ai)
            result = true;
        else if (angle = -1.0f, fn_710062f6e4(reinterpret_cast<UnkObsydomithonAI_b69a4*>(ai), &angle))
            result = fn_7100629600(self, angle);
        else
            result = false;
    }

    UnkGameManagerSub_0x608* world = g_unk_7101d47310->field_0x608;
    if (!world || !target || !((ai != nullptr) & result))
        return result;
    UnkTree* tree = static_cast<UnkTree*>(world->field_0x820);
    if (!tree)
        return result;

    UnkOffsetList_l628c0c offsets = ai->offsets;
    CVector3D from3 = Position_l628c0c(self->owner);
    CVector2D from;
    fn_710007b430(&from, &from3);
    CVector3D to3 = Position_l628c0c(target);
    CVector2D to;
    fn_710007b430(&to, &to3);
    for (UnkOffset_l628c0c* it = offsets.begin(); it != offsets.end(); ++it) {
        UnkOffset_l628c0c offset = *it;
        f32 angle = -1.0f;
        if (!fn_710062f7d4(reinterpret_cast<UnkObsydomithonAI_b69a4*>(ai),
                           reinterpret_cast<const UnkVec2_b69a4*>(&offset), &angle))
            continue;
        UnkMatrix44 pose = g_unk_7101a1a680;
        u32 index = static_cast<s32>(__builtin_roundf(angle));
        auto* char_class = static_cast<UnkAnimFlagsHolder_b47a3*>(self->owner->field_0x220);
        const UnkPose_l628c0c& entry =
            fn_7101178524(char_class, 2) ? g_unk_71726f8e38.At(index) : g_unk_71726f8e60.At(index);
        UnkCopyMatrix(&pose, entry.matrix);
        pose = fn_71000127c8(fn_7100078e54(0.0f, 3.14159274f, 0.0f), pose);
        const UnkActor* owner = self->owner;
        CVector3D center;
        center.x = 0.0f;
        center.y = 0.0f;
        center.z = 0.0f;
        pose.m[3] = owner->field_0x44 + pose.m[3];
        pose.m[7] = owner->field_0x54 + pose.m[7];
        pose.m[11] = owner->field_0x64 + pose.m[11];
        f32 reach = ai->width * 0.5f + fn_710062c8c8(ai);
        center.x = pose.m[0] * reach + center.x;
        center.y = pose.m[4] * reach + center.y;
        center.z = pose.m[8] * reach + center.z;
        UnkBoxDesc_l628c0c desc;
        fn_710007b430(&desc.center, &center);
        desc.axis_x.x = 1.0f;
        desc.axis_x.y = 0.0f;
        desc.axis_y.x = -desc.axis_x.y;
        desc.axis_y.y = desc.axis_x.x;
        desc.size.x = ai->width;
        desc.size.y = ai->height;
        desc.field_0x20 = 0;
        UnkHitBox_l628c0c box(&pose, &desc);
        if (fn_7100d5b484(tree, 0x20, reinterpret_cast<UnkShape*>(&box),
                          reinterpret_cast<const UnkMtx44*>(&g_unk_7101a1a680), nullptr, 0)) {
            ai->offset = offset;
            return true;
        }
    }
    return false;
}
