// UnkTU811 (CEmmyAIComponent): whether the E.M.M.I.'s target can be reached. A target hanging on a magnet
// sliding block answers by the block's position; otherwise the navigation mesh is probed 400 units to each
// side of the target. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CBlackboard.h"
#include "game/CSamusMagnetGloveMovementState/CParams.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU811.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern char g_unk_71726debe0[];

struct UnkSource_7100286798;
u32 fn_7100413fd0(UnkSource_7100286798* self);
bool fn_71009e8524(void* movement);

struct Vec2;
struct Result;
bool fn_7100ebfa7c(UnkNavMeshManager* self, Vec2* pos, Result* out, f32 a, u32 flags, f32 b);

struct UnkMoveState_l549ed8 {
    void* vtable;
    u8 pad_0x8[0x8];
    s32 kind;  // 0x10
};

struct UnkMovementVtbl_l549ed8 {
    void* unk_0x0[0x3b8 / 8];
    bool (*vf_0x3b8)(struct UnkMovement_l549ed8* self);
};

#pragma pack(push, 4)
struct UnkMovement_l549ed8 {
    UnkMovementVtbl_l549ed8* vtbl;
    u8 pad_0x8[0x2d08 - 0x8];
    UnkMoveState_l549ed8* current;  // 0x2d08
    u8 pad_0x2d10[0x2d18 - 0x2d10];
    u32 state_count;
    UnkMoveState_l549ed8* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

struct UnkTargetInfo_l549ed8 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkTarget_l549ed8 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1f0 - 0x68];
    UnkTargetInfo_l549ed8* info;  // 0x1f0
    u8 pad_0x1f8[0x10];
    UnkMovement_l549ed8* movement;  // 0x208
};

struct UnkTargetHolder_l549ed8 {
    u8 pad_0x0[0x10];
    UnkTarget_l549ed8* target;  // 0x10
};

struct UnkSlide_l549ed8 {
    u8 pad_0x0[0x350];
    f32 position;  // 0x350
};

struct UnkNavPoint_l549ed8 {
    f32 x;
    f32 y;
    UnkNavPoint_l549ed8(f32 x_, f32 y_) : x(x_), y(y_) {}
};

// A segment with its direction (end - start).
struct UnkNavSegment_l549ed8 {
    UnkNavPoint_l549ed8 start;
    UnkNavPoint_l549ed8 end;
    UnkNavPoint_l549ed8 delta;
    UnkNavSegment_l549ed8(const UnkNavPoint_l549ed8& s, const UnkNavPoint_l549ed8& e)
        : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
    UnkNavSegment_l549ed8& operator=(const UnkNavSegment_l549ed8& other) asm("fn_71001259f4");
};

// A zero whose constructor is not constexpr, so the static default is guarded.
struct UnkNavDistance_l549ed8 {
    u32 value;
    UnkNavDistance_l549ed8() : value(0) {}
};

inline const UnkNavDistance_l549ed8& DefaultDistance_l549ed8() {
    static UnkNavDistance_l549ed8 s_value asm("g_unk_71726b5354");
    return s_value;
}

// fn_7100ebfa7c's result: no cell (0xffff), no side (0xff), points cleared.
struct UnkNavResult_l549ed8 {
    u16 cell;
    u8 side;
    UnkNavPoint_l549ed8 points[5];
    UnkNavDistance_l549ed8 distance;
    UnkNavResult_l549ed8()
        : cell(0xffff), side(0xff), points{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}} {}
};

struct UnkEmmyAI_l549ed8;
struct UnkEmmyAIVtbl_l549ed8 {
    void* unk_0x0[0x568 / 8];
    bool (*vf_0x568)(UnkEmmyAI_l549ed8* self, u16 cell, u8 side, void* a, void* b, void* c, u32 d, u32 e);
};

struct UnkEmmyAI_l549ed8 {
    UnkEmmyAIVtbl_l549ed8* vtbl;
    u8 pad_0x8[0x1260 - 0x8];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x14b40 - 0x1350];
    UnkTargetHolder_l549ed8* target_holder;  // 0x14b40
    u8 pad_0x14b48[0x27720 - 0x14b48];
    bool field_0x27720;
    u8 pad_0x27721[0x29ee0 - 0x27721];
    UnkSource_7100286798* source;  // 0x29ee0
    u8 pad_0x29ee8[0x8];
    s32 field_0x29ef0;
};
static_assert(__builtin_offsetof(UnkEmmyAI_l549ed8, target_holder) == 0x14b40, "layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l549ed8, field_0x29ef0) == 0x29ef0, "layout");

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

static inline UnkMoveState_l549ed8* FindState(UnkMovement_l549ed8* self, s32 kind) {
    for (UnkMoveState_l549ed8** it = self->states; it != self->states + self->state_count; ++it) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

// The source's parameters, as fn_7100286798 reads them.
struct UnkSourceParams_l549ed8 {
    u8 pad_0x0[0x218];
    u32 field_0x218;
    u8 pad_0x21c[0xc];
    u32 field_0x228;
};

static inline const UnkSourceParams_l549ed8* Params(UnkEmmyAI_l549ed8* self) {
    return reinterpret_cast<const UnkSourceParams_l549ed8*>(self->source);
}

inline bool Probe(UnkEmmyAI_l549ed8* self, UnkNavSegment_l549ed8& segment, UnkNavResult_l549ed8& result) {
    return fn_7100ebfa7c(NavMeshManager(), reinterpret_cast<Vec2*>(&segment), reinterpret_cast<Result*>(&result),
                         0.0f, fn_7100413fd0(self->source), -1.0f);
}

inline bool Reaches(UnkEmmyAI_l549ed8* self, const UnkNavResult_l549ed8& result) {
    return self->vtbl->vf_0x568(self, result.cell, result.side, nullptr, nullptr,
                                fn_71010fb398(reinterpret_cast<UnkComponent_7100286798*>(self)),
                                Params(self)->field_0x228, Params(self)->field_0x218);
}

bool fn_7100549ed8(UnkEmmyAI_l549ed8* self) {
    UnkTarget_l549ed8* target = self->target_holder ? self->target_holder->target : nullptr;
    UnkMovement_l549ed8* hanging = target ? target->movement : nullptr;
    if (hanging && (fn_71009e12d0(hanging) || target->movement->vtbl->vf_0x3b8(target->movement))) {
        UnkMovement_l549ed8* movement = target->movement;
        u32 kind = fn_71009a60fc();
        if (kind > 14)
            return false;
        UnkMoveState_l549ed8* state = FindState(movement, kind);
        if (!state || movement->current != state)
            return false;
        void* hold = fn_71009ad440(reinterpret_cast<UnkHangMoveState_A6_b70*>(state));
        if (!hold)
            return false;
        const UnkSlide_l549ed8* slide = reinterpret_cast<const UnkSlide_l549ed8*>(fn_710054a2cc(hold));
        if (!slide)
            return false;
        if (!fn_71009ae798(reinterpret_cast<UnkFootstepMode*>(state)))
            return false;
        return slide->position - 0.001f > 1.0f || slide->position + 0.001f < 1.0f;
    }

    if (!target->info->field_0xf9)
        return true;
    if (target->movement && fn_71009e8524(target->movement))
        return false;

    s32 value = 0;
    if (CBlackboard::CSection* section = fn_7101047270(&self->blackboard, g_unk_71726b7cb8))
        fn_7100395a30(section, g_unk_71726debe0, &value);
    if (!self->field_0x27720 && value == 1 && self->field_0x29ef0 == 1)
        return false;

    CVector2D pos;
    {
        CVector3D pos3 = {target->x, target->y, target->z};
        fn_710007b430(&pos, &pos3);
    }
    UnkNavSegment_l549ed8 segment(UnkNavPoint_l549ed8(pos.x, pos.y), UnkNavPoint_l549ed8(pos.x + -400.0f, pos.y));
    UnkNavResult_l549ed8 result;
    if (Probe(self, segment, result) && Reaches(self, result))
        return false;

    segment = UnkNavSegment_l549ed8(UnkNavPoint_l549ed8(pos.x, pos.y), UnkNavPoint_l549ed8(pos.x + 400.0f, pos.y));
    result.distance = DefaultDistance_l549ed8();
    if (Probe(self, segment, result))
        return !Reaches(self, result);
    return true;
}
