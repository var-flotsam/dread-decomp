// CWarLotusAIComponent: the bump state at +0x146d0. The reset clears it (value -1, factor 0, flag
// off); the event handler (unless the event's flag says otherwise) and the second step restart it
// from one of the stored values with factor 1 or -1. Each writes the factor into the property bag
// of the actor's character class (+0x80 of it) under "BumpToRelaxToImpactFactor".
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"
#include "../../game_s7/UnkPropBagS7.h"

using base::global::CStrId;

struct UnkBumpState {
    f32 field_0x0;
    f32 field_0x4;
    f32 current;  // 0x8
    f32 field_0xc;
    f32 field_0x10;
    f32 value;   // 0x14
    f32 factor;  // 0x18
    bool flag;   // 0x1c
};

struct UnkWarLotusAI {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

// The base reset (declared elsewhere without its argument; called with it).
void fn_7100369650();

static inline UnkBumpState* BumpOf(UnkWarLotusAI* self) {
    return reinterpret_cast<UnkBumpState*>(reinterpret_cast<char*>(self) + 0x146d0);
}

static inline unk_prop_bag::UnkPropBag* BagOf(UnkAnimationComponent* track) {
    return reinterpret_cast<unk_prop_bag::UnkPropBag*>(track->field_0x80);
}

void fn_71006e07b0(UnkWarLotusAI* self) {
    UnkBumpState* bump = BumpOf(self);
    reinterpret_cast<void (*)(UnkWarLotusAI*)>(fn_7100369650)(self);
    bump->value = -1.0f;
    bump->factor = 0.0f;
    bump->flag = false;
    UnkAnimationComponent* track = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (!track)
        return;
    unk_prop_bag::fn_710033187c(BagOf(track), CStrId("BumpToRelaxToImpactFactor", true), bump->factor);
}

static inline void WriteFactor(UnkWarLotusAI* self, UnkBumpState* bump) {
    UnkAnimationComponent* track = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (track)
        unk_prop_bag::fn_710033187c(BagOf(track), CStrId("BumpToRelaxToImpactFactor", true), bump->factor);
}

struct UnkSluggerAI;
struct UnkSluggerEvent;
u32 fn_7101105e44(UnkSluggerAI* self, UnkSluggerEvent* event);
void fn_7101105c94(UnkWarLotusAI* self);
void fn_71006e0acc(UnkWarLotusAI* self);

struct UnkWarLotusEvent {
    u8 pad_0x0[0xb4];
    u8 flags;  // 0xb4
};

u32 fn_71006e09e4(UnkWarLotusAI* self, UnkWarLotusEvent* event) {
    u32 result = fn_7101105e44(reinterpret_cast<UnkSluggerAI*>(self), reinterpret_cast<UnkSluggerEvent*>(event));
    if (!(event->flags & 2)) {
        UnkBumpState* bump = BumpOf(self);
        bump->value = bump->field_0xc;
        bump->factor = 1.0f;
        bump->current = bump->field_0x0;
        bump->flag = false;
        WriteFactor(self, bump);
        fn_71006e0acc(self);
    }
    return result;
}

void fn_71006e0ca0(UnkWarLotusAI* self) {
    UnkBumpState* bump = BumpOf(self);
    fn_7101105c94(self);
    bump->value = bump->field_0x10;
    bump->factor = -1.0f;
    bump->current = bump->field_0x4;
    bump->flag = false;
    WriteFactor(self, bump);
    fn_71006e0acc(self);
}
