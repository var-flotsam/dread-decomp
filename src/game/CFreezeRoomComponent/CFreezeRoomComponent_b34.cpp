#include "game/CActorComponent.h"
#include "game/CFreezeRoomComponent.h"
#include "types.h"

struct UnkStruct_710024fa0c;

struct CFreezeRoomComponent;

// Once `value` reaches 1, enables the component, runs fn_7100a238f4 and returns fn_7100a232d4;
// below 1 returns `value`.
f32 fn_7100a241c4(CFreezeRoomComponent* self, f32 value) {
    if (!(value >= 1.0f))
        return value;
    fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self), true);
    fn_7100a238f4(self);
    return fn_7100a232d4(self);
}
