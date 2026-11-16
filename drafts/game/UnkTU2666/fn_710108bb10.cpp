// Unit 2666 Lua binding: plays a sound (a file path) on the entity named by the second argument, with
// four float parameters and a flag (true by default), through one function-local set of sound
// parameters. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct lua_State;
struct UnkWeakTarget;
struct UnkEntityWithSound;

struct UnkNamedKey_b0163 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// The parameters a sound is played with: its id and file path at 0x18 and its tuning values.
struct UnkSoundParams_l08bb10 {
    u32 field_0x0;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    CStrId id;            // 0x18
    CFilePathStrId path;  // 0x20
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

    UnkSoundParams_l08bb10()
        : field_0x0(0), field_0x4(0), field_0x8(0), field_0x10(0), id(g_unk_7101d470e0),
          path(g_unk_7101d470e8), field_0x28(0.0f), field_0x2c(0.0f), field_0x30(0.0f),
          field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f),
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f),
          field_0x68(10.0f), field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkSoundParams_l08bb10() asm("fn_710034e0b8");
};
static_assert(sizeof(UnkSoundParams_l08bb10) == 0x70, "");

// The entity of the given name and kind in the game manager.
static inline UnkWeakTarget* FindEntity(UnkGameManager* manager, const CStrId& id, u32 kind_value) {
    u64 hash;
    alignas(8) u32 kind;
    u32 k = kind_value;
    u64 h = reinterpret_cast<UnkNamedKey_b0163*>(id.data)->hash;
    hash = h;
    kind = k;
    return fn_7100debbc8(manager, &hash, &kind);
}

s32 fn_710108bb10(lua_State* L) {
    const char* path_name;
    const char* entity_name;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    bool flag = true;
    if (!fn_71010c0270(L, "ssffffb", &path_name, &entity_name, &a, &b, &c, &d, &flag))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    UnkWeakTarget* entity;
    {
        CStrId id(entity_name, true);
        entity = FindEntity(manager, id, g_unk_71727bebb8);
    }
    if (!entity)
        return 0;
    static UnkSoundParams_l08bb10 s_params;
    s_params.field_0x0 = 0;
    s_params.path = CFilePathStrId(path_name, true);
    s_params.field_0x38 = a;
    s_params.field_0x3c = b;
    s_params.field_0x40 = c;
    s_params.field_0x44 = d;
    s_params.field_0x6c = true;
    s_params.field_0x6d = true;
    fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(entity), &s_params, &g_unk_7101d470e0, flag,
                  true, true);
    return 0;
}
