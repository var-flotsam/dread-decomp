// CSamusMovementState's unit (tools/tu_map.py unit 1468): entering the state. Stops the owner's body, takes the
// animation's root motion into world space, moves the owner's shape bounds there and switches on the first
// enabled CForcedMovementAreaComponent they overlap (keeping a weak reference to it at +0x160); then hooks the
// callback fn_71009b3030 (link at +0x168, handle at +0x180) up to the animation. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CActorComponent.h"
#include "game/CAnimationComponent.h"
#include "game/CCharacterMovement.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU437.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::reflection::UnkReflObject;

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

// As src/game/CForcedMovementAreaComponent/CForcedMovementAreaComponent_Area_ovb0124.cpp declares them.
struct UnkAreaBox_ovb0124 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

// A collider whose bounds (+0x40) are refreshed through slot 0x98 while its flag at +0x60 is set.
struct UnkAreaCollider_ovb0124 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkAreaBox_ovb0124 bounds;  // 0x40
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60

    const UnkAreaBox_ovb0124& Bounds() {
        if (dirty)
            reinterpret_cast<void (*)(UnkAreaCollider_ovb0124*)>(vtable[0x98 / 8])(this);
        return bounds;
    }
};

struct UnkForcedAreaBounds_ovb0124;
bool fn_71007ecc08(UnkForcedAreaBounds_ovb0124* self, const UnkAreaBox_ovb0124* box);
CRntVector<UnkWeakHandle*>* fn_71007ecbf0();
void fn_71007e9a18(UnkWeakTarget* self, bool enable);
void fn_71009b3030();

struct UnkSamusStateOwner_l9b1ae8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x188];
    UnkFlagged* movement;  // 0x200
    u8 pad_0x208[0x18];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x88];
    void* body;  // 0x2b0
};

struct UnkSamusStateLink_l9b1ae8 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
    UnkWeakHandle* handle;  // what the link is hooked up to
};

struct UnkSamusState_l9b1ae8 {
    u8 pad_0x0[0x30];
    UnkSamusStateOwner_l9b1ae8* owner;  // 0x30
    u8 pad_0x38[0x128];
    UnkWeakHandle* area;  // 0x160
    UnkSamusStateLink_l9b1ae8 link;  // 0x168
};
static_assert(__builtin_offsetof(UnkSamusState_l9b1ae8, link.handle) == 0x180, "UnkSamusState_l9b1ae8 layout");

static inline UnkWeakTarget* Target(UnkWeakHandle* handle) { return handle ? handle->target : nullptr; }

void fn_71009b1ae8(UnkSamusState_l9b1ae8* self, const u64* event) {
    if (void* body = self->owner->body)
        fn_7100352b14(body, nullptr, 1);

    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    fn_7101179d08(self->owner->animation, &motion, &g_unk_717270ec40);
    UnkMatrix44 world = fn_71000127c8(fn_710002614c(&self->owner->xform),
                                      fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&motion)));
    f32 x = world.m[3];
    f32 y = world.m[7];

    auto* shape = reinterpret_cast<UnkAreaCollider_ovb0124*>(fn_7100976588(self->owner->movement));
    if (!shape)
        return;
    // The shape's bounds, centred on the moved position.
    const UnkAreaBox_ovb0124& bounds = shape->Bounds();
    f32 dx = x - (bounds.min_x + bounds.max_x) * 0.5f;
    f32 dy = y - (bounds.min_y + bounds.max_y) * 0.5f;
    UnkAreaBox_ovb0124 box;
    box.min_x = dx + bounds.min_x;
    box.min_y = dy + bounds.min_y;
    box.max_x = dx + bounds.max_x;
    box.max_y = dy + bounds.max_y;

    CRntVector<UnkWeakHandle*>* areas = fn_71007ecbf0();
    UnkWeakHandle** end = areas->data + areas->count;
    for (UnkWeakHandle** it = areas->data; it != end; ++it) {
        UnkWeakTarget* area = Target(*it);
        if (!area || !fn_710009d50c(reinterpret_cast<UnkReflObject*>(area)->vf_0x0(), fn_71007e8fb0()))
            continue;
        if (!fn_710024fbc8(Target(*it)) ||
            !fn_71007ecc08(reinterpret_cast<UnkForcedAreaBounds_ovb0124*>(area), &box))
            continue;

        fn_71007e9a18(area, true);
        AssignRef(&self->area, GetWeakHandle(area));

        if (UnkWeakTarget* track = reinterpret_cast<UnkWeakTarget*>(self->owner->animation)) {
            UnkSamusStateLink_l9b1ae8* link = &self->link;
            link->context = self;
            link->callback = fn_71009b3030;
            if (UnkWeakHandle* handle = link->handle) {
                if (UnkWeakTarget* target = handle->target)
                    fn_710117b3f0(target, link);
                if (UnkWeakHandle* p = link->handle) {
                    if (--p->ref_count == 0)
                        p->vf_0x10();
                    link->handle = nullptr;
                }
            }
            fn_7100316018(track, &link->handle);
            fn_710117b3e8(Target(link->handle), link);
        }
        return;
    }
}
