#pragma once

// The description a sound is played from (0x70 bytes): its kind, its event name and file path, where it
// plays, and its volume, pitch and distance settings. Entities pass one to fn_7100d7c374; most players
// keep one in a function-local static, set a few members and play it; fn_7101037bc0 fills one from a
// sound definition. The constructor is inlined into each player with the defaults below; the destructor,
// fn_710034e0b8, is out of line (src/game/CAimComponent/UnkSoundDesc.cpp, in unit 437) and releases the
// path, then the name. Not reflected. Names and layout are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

// The sound's kind as a pair of indices (fn_7101037bc0 keeps the first below 7 and the second below 10,
// 0x7fffffff otherwise).
struct UnkSoundKind {
    u32 field_0x0;
    u32 field_0x4;
};

struct UnkSoundDesc {
    UnkSoundKind kind;
    UnkSoundKind field_0x8;  // the kind as the definition gives it, unchecked
    u32 field_0x10;
    u32 field_0x14;  // left unset by the constructor, as in the original
    base::global::CStrId name;          // 0x18
    base::global::CFilePathStrId path;  // 0x20
    f32 position[3];                    // 0x28
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    bool field_0x6c;
    bool field_0x6d;
    bool field_0x6e;

    UnkSoundDesc()
        : kind{0, 0}, field_0x8{0, 0}, field_0x10(0), name(g_unk_7101d470e0), path(g_unk_7101d470e8),
          position{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f),
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkSoundDesc();  // fn_710034e0b8
};
static_assert(sizeof(UnkSoundDesc) == 0x70, "UnkSoundDesc placeholder layout");
