// CGoliathXBurstProjectionBombsAttack's unit (tools/tu_map.py unit 835): launches the bombs. With bombs
// already out it sets each one off and tells the owner's timeline; otherwise it spawns one bomb at the
// owner's node (flag +0x64) or a line of two or four bombs across the Goliath's area, and keeps their
// weak handles in the list at +0x38. Names and layouts are placeholders.
#include <new>
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharacterMovement.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTU54.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

extern "C" int rand();

struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;
struct UnkEntity;

struct UnkMatrix44Words {
    u32 m[16];

    UnkMatrix44Words() {}
    UnkMatrix44Words(const UnkMatrix44Words& other);
};

UnkMatrix44Words fn_7100076dc4(const UnkMatrix44Words* src);

struct UnkAIReach {
    f32 radius;
    f32 field_0x4;

    UnkAIReach() : radius(0.0f), field_0x4(0.0f) {}
    UnkAIReach(const UnkAIReach& other);
};

struct UnkTag_7100241b30;
template <class Tag>
struct UnkSlotArray;
void fn_7100241b30(UnkSlotArray<UnkTag_7100241b30>* self, u32 capacity);

struct UnkBurstBombMovement_b60;
void fn_710058b4ac(UnkBurstBombMovement_b60* self);

extern u64 g_unk_7101649fd0;  // timeline id: a single bomb
extern u64 g_unk_7101649fd8;  // the owner's launch node
extern UnkVec3f g_unk_7101649fe0;  // offset from that node
extern u64 g_unk_7101649ff0;  // timeline id: bombs set off (single)
extern u64 g_unk_7101649ff8;  // timeline id: bombs set off (line)
extern CStrId g_unk_71726e55b0 __attribute__((visibility("hidden")));  // the bomb's actor definition

struct UnkBombName_l584ab8 {
    u8 unk_0x0[0x10];
    char text[0x34];
    union {
        u32 use_count;  // 0x44
    };
};

struct alignas(8) UnkBombDefType_l584ab8 {
    u32 id;
    explicit UnkBombDefType_l584ab8(u32 id_) : id(id_) {}
};

// The bomb's movement (what fn_71005857e8 finds on the bomb actor).
struct UnkBombMovement_l584ab8 {
    void** vtable;
    u8 pad_0x8[0x74];
    f32 field_0x7c;
    u8 pad_0x80[0x480];
    f32 field_0x500;
    u32 field_0x504;
    u8 pad_0x508[0x894];
    f32 target_x;  // 0xd9c
    f32 target_y;  // 0xda0
    f32 target_z;  // 0xda4
    bool first;    // 0xda8

    void vf_0x290(UnkActor* owner) {
        reinterpret_cast<void (*)(UnkBombMovement_l584ab8*, UnkActor*)>(vtable[0x290 / 8])(this, owner);
    }
};
static_assert(__builtin_offsetof(UnkBombMovement_l584ab8, target_x) == 0xd9c, "layout");

UnkBombMovement_l584ab8* fn_71005857e8(UnkActor* actor);

struct UnkBombsData_l584ab8 {
    u8 pad_0x0[0xf4];
    u32 field_0xf4;
};

struct UnkBombList_l584ab8 {
    UnkWeakHandle** data;  // 0x38
    u32 count;             // 0x40
    u32 capacity;          // 0x44
    u32 step;              // 0x48
};

struct UnkBombsAttack_l584ab8 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkBombsData_l584ab8* data;  // 0x28
    u8 pad_0x30[0x8];
    UnkBombList_l584ab8 bombs;  // 0x38
    u8 pad_0x50[0x14];
    u8 single;  // 0x64
    u8 pad_0x65[3];
    u32 field_0x68;
};
static_assert(__builtin_offsetof(UnkBombsAttack_l584ab8, field_0x68) == 0x68, "layout");

// The area of the Goliath's arena the bombs land in.
struct UnkGoliathArea_l584ab8 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

struct UnkGoliathXAI_l584ab8 {
    u8 pad_0x0[0x14c70];
    UnkGoliathArea_l584ab8 area;  // 0x14c70
};

struct UnkBombStaticZero_l584ab8 {
    u32 value;
    UnkBombStaticZero_l584ab8() : value(0) {}
};

inline const UnkBombStaticZero_l584ab8& UnkBombStaticZero() {
    static UnkBombStaticZero_l584ab8 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkBombDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkBombStaticZero().value & 0xff;
    return s_mask;
}

// The address of a temporary, for the callees that take their matrix by pointer.
template <class T>
static inline const T* AddressOf(const T& value) {
    return &value;
}

static inline const UnkMatrix44* Transform(UnkActor* actor) {
    return reinterpret_cast<const UnkMatrix44*>(actor->field_0x38);
}

// A counted reference to a weak handle.
struct UnkBombRef_l584ab8 {
    UnkWeakHandle* handle;

    explicit UnkBombRef_l584ab8(UnkWeakHandle* h) : handle(h) {
        if (h)
            ++h->ref_count;
    }
    ~UnkBombRef_l584ab8() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
    UnkBombRef_l584ab8& operator=(UnkWeakHandle* h) {
        AssignRef(&handle, h);
        return *this;
    }
};

static inline UnkPowerUpLifeEntry_b52* LoadDef(UnkActorDefFactory* factory, const char* text, CStrId* id,
                                               const UnkBombDefType_l584ab8& type) {
    return fn_710119438c(factory, text, id, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr);
}

// Loads the bomb's actor definition and returns the new bomb's weak handle.
static inline UnkWeakHandle* SpawnBomb() {
    UnkActorDefFactory* factory = g_unk_71726a0220;
    CStrId* id = &g_unk_71726e55b0;
    ++reinterpret_cast<UnkBombName_l584ab8*>(id->data)->use_count;
    const char* text = reinterpret_cast<UnkBombName_l584ab8*>(id->data)->text;
    UnkPowerUpLifeEntry_b52* entry = LoadDef(factory, text, id, UnkBombDefType_l584ab8(g_unk_7172a0ba48));
    return GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(entry));
}

static inline void PushBack(UnkBombList_l584ab8* list, UnkWeakHandle* handle) {
    u32 n = list->count;
    u32 capacity = list->capacity;
    if (capacity <= n && list->step != 0)
        capacity = (n / list->step + 1) * list->step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_7100241b30(reinterpret_cast<UnkSlotArray<UnkTag_7100241b30>*>(list), capacity);
    UnkWeakHandle** slot = &list->data[list->count];
    *slot = nullptr;
    AssignRef(slot, handle);
    ++list->count;
}

// Adds a spawned bomb to the list and puts it at `pos`; returns its movement, or null.
static inline UnkBombMovement_l584ab8* Launch(UnkBombsAttack_l584ab8* self, UnkWeakHandle* handle,
                                              const UnkVec3f* pos) {
    PushBack(&self->bombs, handle);
    UnkActor* bomb = reinterpret_cast<UnkActor*>(handle->target);
    bomb->vf_0xd0(pos, true);
    return fn_71005857e8(reinterpret_cast<UnkActor*>(handle->target));
}

void fn_7100584ab8(UnkBombsAttack_l584ab8* self) {
    if (self->bombs.count != 0) {
        --self->field_0x68;
        for (UnkWeakHandle **it = self->bombs.data, **end = it + self->bombs.count; it != end; ++it) {
            UnkBombRef_l584ab8 ref(*it);
            if (!ref.handle)
                continue;
            UnkWeakTarget* target = ref.handle->target;
            if (!target)
                continue;
            UnkBombMovement_l584ab8* movement = fn_71005857e8(reinterpret_cast<UnkActor*>(target));
            if (!movement)
                continue;
            fn_710058b4ac(reinterpret_cast<UnkBurstBombMovement_b60*>(movement));
            UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
            if (timeline) {
                const u64* id = self->single ? &g_unk_7101649ff0 : &g_unk_7101649ff8;
                u32 mask = UnkBombDefaultMask();
                fn_7100a2cb7c(timeline, id, &mask, false, -1.0f);
            }
        }
        return;
    }

    if (UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        u32 mask = UnkBombDefaultMask();
        fn_7100a2cb7c(timeline, &g_unk_7101649fd0, &mask, false, -1.0f);
    }

    UnkActor* owner = self->owner;
    UnkVec3f pos(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    UnkMatrix44 node;
    for (s32 i = 0; i < 16; ++i)
        node.m[i] = 0.0f;
    node.m[15] = 1.0f;
    node.m[10] = 1.0f;
    node.m[5] = 1.0f;
    node.m[0] = 1.0f;
    if (fn_71011735e8(static_cast<UnkEntity*>(owner->field_0x228), &g_unk_7101649fd8, &node, 0)) {
        pos = UnkVec3f(node.m[3], node.m[7], node.m[11]);
        // The offset turned by the node's rotation.
        UnkVec3f offset = fn_7100078170(
            reinterpret_cast<const UnkMat4f*>(AddressOf(fn_7100076dc4(reinterpret_cast<const UnkMatrix44Words*>(&node)))),
            &g_unk_7101649fe0);
        pos.x = offset.x + pos.x;
        pos.y = offset.y + pos.y;
        pos.z = offset.z + pos.z;
    }

    if (self->single) {
        UnkBombRef_l584ab8 ref(SpawnBomb());
        if (!ref.handle || !ref.handle->target)
            return;
        if (UnkBombMovement_l584ab8* movement = Launch(self, ref.handle, &pos)) {
            movement->vf_0x290(self->owner);
            movement->field_0x7c = 1.0f;
            movement->field_0x504 = self->data->field_0xf4;
            movement->first = true;
        }
        return;
    }

    s32 r = rand();
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
    UnkGoliathXAI_l584ab8* goliath =
        ai && fn_710009d50c(ai->vf_0x0(), fn_7100587ee4()) ? reinterpret_cast<UnkGoliathXAI_l584ab8*>(ai) : nullptr;
    UnkAIReach reach = fn_7100977b90(fn_7100d7c990(self->owner)->field_0x1f8);
    (void)reach;
    if (!goliath)
        return;

    f32 chance = (f32)((r & 0x7fff) * (1.0 / 32767.0));
    UnkGoliathArea_l584ab8* area = &goliath->area;
    f32 start_x = self->owner->field_0x44;
    f32 z = self->owner->field_0x64;
    start_x = start_x + fn_710002614c(Transform(self->owner)).m[2] * -350.0f;
    f32 start_y = area->min_y + 100.0f;
    f32 max_x = area->max_x;
    f32 end_y = area->max_y + -150.0f;
    // Facing right: from the right of the arena towards the left; else the other way.
    f32 end_x = fn_710002614c(Transform(self->owner)).m[2] > 0.0f ? area->min_x + 150.0f : max_x + -150.0f;
    f32 count = chance < 0.5f ? 2.0f : 4.0f;
    f32 step_x = (end_x - start_x) / (count + -1.0f);
    f32 step_y = (end_y - start_y) / (count + -1.0f);

    bool first = true;
    if (chance < 0.5f) {
        // Two lines, from either side towards the middle.
        for (s32 i = 0; i < count; ++i) {
            f32 dx = step_x * i;
            f32 y = start_y + step_y * i;
            UnkBombRef_l584ab8 ref(SpawnBomb());
            if (ref.handle && ref.handle->target) {
                if (UnkBombMovement_l584ab8* movement = Launch(self, ref.handle, &pos)) {
                    movement->vf_0x290(self->owner);
                    movement->field_0x504 = 1;
                    movement->target_z = z;
                    movement->target_x = start_x + dx;
                    movement->target_y = y;
                    movement->field_0x7c = 0.0f;
                    movement->first = first;
                    first = false;
                }
            }
            ref = SpawnBomb();
            if (ref.handle && ref.handle->target) {
                if (UnkBombMovement_l584ab8* movement = Launch(self, ref.handle, &pos)) {
                    movement->vf_0x290(self->owner);
                    movement->target_x = end_x - dx;
                    movement->field_0x500 = 0.6f;
                    movement->field_0x504 = 1;
                    movement->target_y = y;
                    movement->target_z = 0.0f;
                    movement->field_0x7c = 0.0f;
                    movement->first = first;
                    first = false;
                }
            }
        }
    } else {
        for (s32 i = 0; i < count; ++i) {
            UnkBombRef_l584ab8 ref(SpawnBomb());
            if (!ref.handle || !ref.handle->target)
                continue;
            if (UnkBombMovement_l584ab8* movement = Launch(self, ref.handle, &pos)) {
                f32 x = start_x + step_x * i;
                f32 y = start_y + step_y * i;
                movement->vf_0x290(self->owner);
                movement->target_z = z;
                movement->field_0x504 = 1;
                movement->target_x = x;
                movement->target_y = y;
                movement->field_0x7c = 0.0f;
                movement->first = first;
                first = false;
            }
        }
    }
}
