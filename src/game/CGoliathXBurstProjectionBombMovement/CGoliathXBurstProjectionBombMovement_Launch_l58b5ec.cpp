// CGoliathXBurstProjectionBombMovement's unit (tools/tu_map.py unit 840): starts the bomb at its
// owner's position: records the point in its trail, resets the four bounce directions, signals the
// owner's timeline, shows the next effect model there and hands back the spent one. Names and
// layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CRntVector.h"
#include "game/CGoliathXBurstProjectionBombMovement.h"
#include "game/UnkActor.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU285.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntVector;

void fn_7100146e0c(UnkSwitchObject* self, bool enable);

struct UnkBurstBombKey_l58b5ec;
extern UnkBurstBombKey_l58b5ec g_unk_710164a020;  // reached through the GOT

// As fn_710058beec's own file declares them (CGoliathXBurstProjectionBombMovement_Vector_b49.cpp).
struct UnkBombValue_b49 {
    u32 values[3];

    UnkBombValue_b49(const UnkBombValue_b49& other) {
        values[0] = other.values[0];
        values[1] = other.values[1];
        values[2] = other.values[2];
    }
};

struct UnkBombVector_b49 {
    UnkBombValue_b49* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool grows;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710058beec(UnkBombVector_b49* self, const UnkBombValue_b49* values, u64 count, u32 step,
                   base::UnkAllocator* allocator, u32 flags);

// Takes over other's array, leaving it empty (CRntVector's move assignment).
static inline void MoveAssign(UnkBombVector_b49* self, UnkBombVector_b49* other) {
    if (self != other) {
        self->allocator = other->allocator;
        self->flags = other->flags;
        self->step = other->step;
        self->grows = other->grows;
        self->count = other->count;
        self->capacity = other->capacity;
        self->data = other->data;
        other->data = nullptr;
        other->count = 0;
        other->capacity = 0;
    }
}

// Frees the array (CRntVector::Free).
static inline void Free(UnkBombVector_b49* self) {
    if (self->data) {
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
}

// A point of the bomb's trail.
struct UnkBombTrailPoint_l58b5ec {
    UnkVec3f position;
    u32 field_0xc;
    u32 field_0x10;

    explicit UnkBombTrailPoint_l58b5ec(const UnkVec3f& p) : field_0xc(0), field_0x10(0) { position = p; }
};

struct UnkGoliathBombMovement_l58b5ec {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x44];
    f32 field_0x7c;
    u8 pad_0x80[0x408];
    bool field_0x488;
    u8 pad_0x489[0xf];
    u32 field_0x498;
    u8 pad_0x49c[0x58];
    u16 field_0x4f4;
    u8 pad_0x4f6[0xe];
    u32 bounces;  // 0x504
    u8 pad_0x508[0x8];
    UnkSwitchObject* field_0x510;
    u32 next_model;  // 0x518
    u8 pad_0x51c[0x4];
    UnkSwitchObject* models_a[128];  // 0x520
    UnkSwitchObject* models_b[128];  // 0x920
    CRntVector<UnkBombTrailPoint_l58b5ec> trail;  // 0xd20
    UnkBombVector_b49 directions;  // 0xd48
    u8 pad_0xd70[0xc];
    UnkVec3f field_0xd7c;
    u8 pad_0xd88[0x20];
    bool field_0xda8;
};
static_assert(__builtin_offsetof(UnkGoliathBombMovement_l58b5ec, field_0xda8) == 0xda8,
              "UnkGoliathBombMovement_l58b5ec layout");

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

void fn_710058b5ec(UnkGoliathBombMovement_l58b5ec* self) {
    UnkActor* owner = self->owner;
    self->field_0xd7c = UnkVec3f(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    self->field_0x4f4 = 0;
    self->field_0x488 = true;
    self->field_0x498 = 0;
    UnkBombTrailPoint_l58b5ec point(self->field_0xd7c);
    self->trail.PushBackCopy(point);

    {
        UnkBombVector_b49 directions;
        const UnkVec3f values[4] = {UnkVec3f(1.0f, 0.0f, 0.0f), UnkVec3f(-1.0f, 0.0f, 0.0f), UnkVec3f(0.0f, 1.0f, 0.0f),
                                    UnkVec3f(0.0f, -1.0f, 0.0f)};
        fn_710058beec(&directions, reinterpret_cast<const UnkBombValue_b49*>(values), 4, 10, nullptr, 0xf);
        MoveAssign(&self->directions, &directions);
        Free(&directions);
    }

    if (self->field_0xda8) {
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a020), &mask.id, false, -1.0f);
        }
    }

    UnkVec3f angle(0.0f, 1.57079637f, 0.0f);
    UnkSwitchObject* model = self->field_0x7c > 0.0f ? self->models_b[self->next_model] : self->models_a[self->next_model];
    bool shown = model->field_0x121;
    fn_7100146dac(model, true);
    fn_7100146d84(reinterpret_cast<UnkFxObject_ovb0041*>(model), &point.position);
    fn_7100146d98(reinterpret_cast<UnkFxObject_ovb0041*>(model), &angle);
    fn_7100146e60(model, true);
    if (!shown)
        fn_7100208a40(model->field_0x28);
    fn_7100146e0c(model, true);
    u32 next = self->next_model + 1;
    self->next_model = next < 128 ? next : 0;

    if (self->bounces == 0 && self->field_0x510) {
        fn_7100146dac(self->field_0x510, false);
        fn_71011a91c4(ModelManager(), self->field_0x510, 0);
        self->field_0x510 = nullptr;
    }
}
