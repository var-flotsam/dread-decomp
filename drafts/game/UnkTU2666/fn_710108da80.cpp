// Unit 2666 (Lua bindings): plays the sound file named by argument 6 on the entity named by
// argument 1 with six values set in a shared set of play parameters, and returns the file name.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// The parameters a sound is played with (0x70 bytes).
struct UnkSoundParams_l08b820 {
    u32 field_0x0;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    CStrId id;             // 0x18
    CFilePathStrId path;   // 0x20
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
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

    UnkSoundParams_l08b820()
        : field_0x0(0), field_0x4(0), field_0x8(0), field_0x10(0), id(g_unk_7101d470e0),
          path(g_unk_7101d470e8), field_0x28(0.0f), field_0x2c(0.0f), field_0x30(0.0f),
          field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f),
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkSoundParams_l08b820() asm("fn_710034e0b8");

    // Sets the two values at 0x38 and 0x44 and the fixed ones between them.
    void SetRange(f32 low, f32 high) {
        field_0x38 = low;
        field_0x3c = 2.0f;
        field_0x40 = 1.0f;
        field_0x44 = high;
    }
};
static_assert(sizeof(UnkSoundParams_l08b820) == 0x70, "UnkSoundParams_l08b820 layout");

// The entity with that name.
inline UnkWeakTarget* FindEntity(UnkGameManager* manager, const CStrId& name) {
    u64 key;
    alignas(8) u32 kind;
    u32 kind_value = g_unk_71727bebb8;
    key = name.data->Key();
    kind = kind_value;
    return fn_7100debbc8(manager, &key, &kind);
}

s32 fn_710108da80(lua_State* L) {
    const char* entity_name;
    const char* file;
    const char* event_name;
    f32 value_0x38;
    f32 value_0x44;
    f32 value_0x3c;
    f32 value_0x40;
    f32 value_0x4c;
    f32 value_0x50;
    bool flag;
    if (!fn_71010c0270(L, "sffffssbff", &entity_name, &value_0x38, &value_0x44, &value_0x3c, &value_0x40,
                       &file, &event_name, &flag, &value_0x4c, &value_0x50))
        return 0;
    UnkWeakTarget* entity = FindEntity(g_unk_7101d47310, CStrId(entity_name, true));
    if (!entity)
        return 0;
    static UnkSoundParams_l08b820 s_params;
    s_params.field_0x0 = 0;
    s_params.path = CFilePathStrId(file, true);
    s_params.field_0x6d = true;
    s_params.field_0x38 = value_0x38;
    s_params.field_0x3c = value_0x3c;
    s_params.field_0x40 = value_0x40;
    s_params.field_0x44 = value_0x44;
    s_params.field_0x4c = value_0x4c;
    s_params.field_0x50 = value_0x50;
    {
        CStrId event(event_name, true);
        fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(entity), &s_params, &event, flag, true, true);
    }
    return fn_71010c0b40(L, "s", file);
}
