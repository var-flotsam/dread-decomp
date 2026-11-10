// CSamusStandMovementState::CParams' unit (tools/tu_map.py): whether standing turns into the morph ball
// (fn_71009b45a8: the inventory holds ITEM_MORPH_BALL and fn_71009b49dc or fn_71009b4b50 allows it; the kind
// from fn_71009b4de0) and the transition it drives (fn_71009b4490), plus the reposition step (fn_71009b3bfc)
// that moves the target by the remaining offset, spread over the reposition track when one plays. Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/UnkMatrix44.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

struct UnkInventory;
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);
struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;
void fn_71010d08d4(UnkMovement_71010d08d4* self, const UnkMovementVec_71010d08d4* v, bool start);
extern const u8 g_unk_710164b428[] __attribute__((visibility("hidden")));  // a zero vector

struct UnkPrimaryState_ovb0103 {
    u8 pad_0x0[0x10];
    s32 id;
};

struct UnkStandAiState_ovb0103 {
    u8 pad_0x0[0x360];
    bool field_0x360;
};

struct UnkStandOwner_ovb0103 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x220 - 0x78];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x270 - 0x228];
    UnkInventory* inventory;  // 0x270
    u8 pad_0x278[0x2b0 - 0x278];
    UnkStandAiState_ovb0103* field_0x2b0;
};

struct UnkStandMovement_ovb0103 {
    u8 pad_0x0[0x40];
    f32 dir_x;  // 0x40
    f32 dir_y;
    f32 dir_z;
    u8 pad_0x4c[0x7c - 0x4c];
    f32 speed;  // 0x7c
    u8 pad_0x80[0x208 - 0x80];
    u64 field_0x208;
    u32 field_0x210;
};

struct UnkStandState_ovb0103 {
    u8 pad_0x0[0x30];
    UnkStandOwner_ovb0103* owner;  // 0x30
    UnkStandMovement_ovb0103* movement;  // 0x38
    u8 pad_0x40[0x194 - 0x40];
    f32 field_0x194;
    u8 pad_0x198[0x1a0 - 0x198];
    f32 field_0x1a0;
};

struct UnkStandTarget_ovb0103 {
    u8 pad_0x0[0x10];
    f32 field_0x10;
    u8 pad_0x14[0x60 - 0x14];
    f32 field_0x60;
    u8 pad_0x64[0x6c - 0x64];
    f32 field_0x6c;
};

bool fn_71009b49dc(UnkStandState_ovb0103* self);
bool fn_71009b4b50(UnkStandState_ovb0103* self, bool* out);
u32 fn_71009b4de0(UnkStandState_ovb0103* self, bool* out);
void fn_71009b170c(UnkStandState_ovb0103* self);
void fn_71009b4688(UnkStandState_ovb0103* self, u32 kind);
void fn_710099d8f4(UnkStandState_ovb0103* self);

static inline const UnkQualifiedId& GetQualifiedId_ovb0103(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// Whether the state can turn into the morph ball: the kind (fn_71009b4de0) when the inventory holds
// ITEM_MORPH_BALL and fn_71009b49dc or fn_71009b4b50 allows it.
__attribute__((always_inline)) bool fn_71009b45a8(UnkStandState_ovb0103* self, u32* kind, bool* a, bool* b) {
    *kind = 0;
    *a = false;
    *b = false;
    UnkInventory* inventory = self->owner->inventory;
    if (fn_71011428bc(inventory, GetQualifiedId_ovb0103(g_unk_717275a8d8))) {
        if (fn_71009b49dc(self) || fn_71009b4b50(self, a))
            *kind = fn_71009b4de0(self, b);
    }
    return *kind != 0;
}

void fn_71009b4490(UnkStandState_ovb0103* self) {
    u32 kind;
    bool a;
    bool b;
    if (fn_71009b45a8(self, &kind, &a, &b)) {
        if (a)
            fn_71009b170c(self);
        if (b) {
            fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(self->movement),
                          reinterpret_cast<const UnkMovementVec_71010d08d4*>(g_unk_710164b428), true);
            self->movement->field_0x208 = 0;
            self->movement->field_0x210 = 0;
        }
        fn_71009b4688(self, kind);
        return;
    }
    fn_710099d8f4(self);
}

void fn_71009b3bfc(UnkStandState_ovb0103* self, UnkStandTarget_ovb0103* target) {
    if (self->field_0x1a0 == 0.0f)
        return;
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation && fn_710117b348(animation, fn_7100c467d8())) {
        UnkTrackQuery query = {fn_7100c467d8(), nullptr, -1.0f};
        fn_710117b08c(self->owner->animation, &query);
        f32 length = query.result->field_0x14 - query.result->field_0x10;
        f32 t = fn_7101177ea0(self->owner->animation, 0xf4061ff25a2163d6) - query.result->field_0x10;
        f32 value = 0.0f;
        if (!(t <= 0.0f))
            value = t >= length ? length : t;
        f32 ratio = value / length;
        f32 delta = self->field_0x194 + self->field_0x1a0 * ratio - target->field_0x10;
        target->field_0x60 += delta;
        target->field_0x6c += delta;
        return;
    }
    f32 delta = self->field_0x194 + self->field_0x1a0 - target->field_0x10;
    target->field_0x60 += delta;
    target->field_0x6c += delta;
    self->field_0x1a0 = 0.0f;
}

