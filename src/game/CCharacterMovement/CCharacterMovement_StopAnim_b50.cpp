// CCharacterMovement unit (tools/tu_map.py, the name from reflection): fetches the current animation
// request (virtual slot 0x308); when it is set (id not -1) marks it (bit 0 of +0x24), stops it on the
// owner's animation (+0x220 of the entity at +0x30; fn_7101176ea8 with key 0xf4061ff25a2163d6, as in
// src/game/CSamusMovementState/CSamusMovementState_b34.cpp), and runs virtual slot 0x318.
// Names are placeholders.
#include "game/CAnimationComponent.h"
#include "types.h"

struct UnkStruct_7101176ea8;
struct UnkRequest_7101176ea8;

struct UnkAnimRequest8438 {
    s64 id;  // -1 when unset
    u8 pad_0x8[0x1c];
    u32 flags;  // 0x24
    u8 pad_0x28[0x8];
};

void fn_7100978438(char* self) {
    using GetFn = UnkAnimRequest8438 (*)(char*);
    using RunFn = void (*)(char*);
    UnkAnimRequest8438 req = (*reinterpret_cast<GetFn**>(self))[0x308 / 8](self);
    if (req.id != -1) {
        req.flags |= 1;
        UnkStruct_7101176ea8* anim =
            *reinterpret_cast<UnkStruct_7101176ea8**>(*reinterpret_cast<char**>(self + 0x30) + 0x220);
        fn_7101176ea8(anim, reinterpret_cast<const UnkRequest_7101176ea8*>(&req), 0xf4061ff25a2163d6);
        (*reinterpret_cast<RunFn**>(self))[0x318 / 8](self);
    }
}
