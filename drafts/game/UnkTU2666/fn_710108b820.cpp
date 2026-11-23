// Unit 2666 (Lua bindings): plays the sound file named by argument 1 on the entity named by
// argument 2, with two values (arguments 3 and 4) set in a shared set of play parameters.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"
#include "../../../src/game_aca57c/UnkBufferString.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::global::UnkStrIdBase;

extern void* g_unk_7101a1b750[];

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// A string id built from a string's text (fn_7100000850), released when it goes.
struct UnkTextId_l08b820 {
    UnkStringData* data;

    explicit UnkTextId_l08b820(const CRntString& text) {
        CStrId_ConstructFromRntString(reinterpret_cast<UnkStrIdBase*>(this), text, true);
    }
    ~UnkTextId_l08b820() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

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

// The entity with that name's hash.
inline UnkWeakTarget* FindEntity(UnkGameManager* manager, const UnkTextId_l08b820& name) {
    u64 key;
    alignas(8) u32 kind;
    u32 kind_value = g_unk_71727bebb8;
    key = name.data->Key();
    kind = kind_value;
    return fn_7100debbc8(manager, &key, &kind);
}

s32 fn_710108b820(lua_State* L) {
    const char* file;
    const char* entity_name;
    f32 value_0x38;
    f32 value_0x44;
    if (!fn_71010c0270(L, "ssff", &file, &entity_name, &value_0x38, &value_0x44))
        return 0;
    UnkBufferString<0x80, g_unk_7101a1b750> name(entity_name);
    UnkWeakTarget* entity = FindEntity(g_unk_7101d47310, UnkTextId_l08b820(*name.Str()));
    if (entity) {
        static UnkSoundParams_l08b820 s_params;
        s_params.field_0x0 = 4;
        s_params.path = CFilePathStrId(file, true);
        s_params.field_0x6d = true;
        s_params.SetRange(value_0x38, value_0x44);
        fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(entity), &s_params, &g_unk_7101d470e0, false,
                      false, true);
    }
    return 0;
}
