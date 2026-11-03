// CNailongAIComponent: the direction the component uses: +x while the actor's track is in the state
// with the id 0xed2efa31d6575244, else the base component's (fn_710110f1c8).
// Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

// A direction returned in memory (not a plain vector: the original builds it through x8).
struct UnkNailongDir {
    f32 x;
    f32 y;
    f32 z;
    UnkNailongDir(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    UnkNailongDir(const UnkNailongDir& other);
};

struct UnkNailongAI {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

UnkNailongDir fn_710110f1c8(UnkNailongAI* self);

UnkNailongDir fn_71006219f4(UnkNailongAI* self) {
    UnkAnimationComponent* track = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (track && fn_710117ae38(track, 0xed2efa31d6575244ull, false))
        return UnkNailongDir(1.0f, 0.0f, 0.0f);
    return fn_710110f1c8(self);
}
