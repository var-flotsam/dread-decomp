// A lookup in the CCameraComponent unit (unit 2691 of tools/tu_map.py): the block at +0x38 of the
// object the holder's reference at +0x60 points to, or a constant default block when there is none.
#include "game/UnkCameraCtrl.h"

extern const u8 g_unk_710164cf20[] __attribute__((visibility("hidden")));

struct UnkCameraHolder {
    u8 pad_0x0[0x60];
    u8** field_0x60;
};

// Defined twice (fn_71010e537c and its copy next door, fn_71010e53ac, which returns the block at
// +0x44); an inline helper would merge the two default returns.
#define UNK_CAMERA_OBJECT(name, at)                                         \
    const void* name(void* holder) {                                        \
        if (u8** ref = static_cast<UnkCameraHolder*>(holder)->field_0x60) { \
            if (u8* object = *ref)                                          \
                return object + at;                                         \
            return g_unk_710164cf20;                                        \
        }                                                                   \
        return g_unk_710164cf20;                                            \
    }

UNK_CAMERA_OBJECT(fn_71010e537c, 0x38)
UNK_CAMERA_OBJECT(fn_71010e53ac, 0x44)
