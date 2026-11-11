#include "base/reflection/UnkReflStatics.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflTypeIdArg;

struct UnkReflZeroWord_bf20 {
    u32 value;
    UnkReflZeroWord_bf20() : value(0) {}
};

inline const UnkReflZeroWord_bf20& UnkMaskZero() {
    static UnkReflZeroWord_bf20 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkMaskZero().value & 0xff;
    return s_mask;
}

extern u64 g_unk_710164b958;
extern u64 g_unk_7172769740;

struct UnkHolder_bf20 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkHolderRef_bf20 {
    UnkHolder_bf20* holder;
};

// The object an animated node drives: its transform at +0x38.
struct UnkNode_bf20 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x98[0x220 - 0x98];
    void* props;  // 0x220

    void vf_0xd8(const UnkVec3f* angle, bool flag) {
        reinterpret_cast<void (*)(UnkNode_bf20*, const UnkVec3f*, bool)>(vtable[0xd8 / 8])(this, angle, flag);
    }
};

// The second node: its transform at +0xb8, mirrored at +0xf8.
struct UnkMirror_bf20 {
    u8 pad_0x0[0xb8];
    UnkXform xform;  // 0xb8
};

struct UnkGrappleState_bf20 {
    u8 pad_0x0[0x30];
    UnkNode_bf20* node;  // 0x30
    u8 pad_0x38[0xb88 - 0x38];
    UnkMirror_bf20* mirror;  // 0xb88
};

bool fn_710099bf20(UnkGrappleState_bf20* self, UnkHolderRef_bf20* ref, bool flag) {
    UnkVec3f angle;
    UnkVec3f flipped;
    UnkReflTypeIdArg arg;
    UnkReflTypeIdArg value;
    if (flag) {
        UnkNode_bf20* node = self->node;
        angle = node->xform.GetAngle();
        flipped.x = angle.x;
        flipped.y = -angle.y;
        flipped.z = angle.z;
        self->node->vf_0xd8(&flipped, true);
        if (UnkMirror_bf20* mirror = self->mirror)
            mirror->xform.SetAngle(flipped);
    }
    void* props = self->node->props;
    u64 hash = ref->holder->hash;
    value.id = 9;
    arg.id = UnkDefaultMask();
    return fn_7101176690(props, hash, 0xf4061ff25a2163d6, &value, &g_unk_710164b958, &g_unk_7172769740, &arg);
}
