// Unit 2666 (lua_bind): plays a sound file on the entity named by argument 2, with the four volume and
// distance values and an optional sound name (the id table's default otherwise).
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU102.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct lua_State;

// The vtable of the allocator that follows a string with 0x80 bytes of inline storage.
extern void* g_unk_7101a1b750[];

UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// A string with 0x80 bytes of inline storage (0xc8 bytes).
struct UnkInlineString_l08b410 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x80];
    u64 field_0xb8;
    u32 field_0xc0;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    explicit UnkInlineString_l08b410(const char* text) {
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a1b750 + 2);
        field_0xb8 = 0;
        field_0xc0 = 0;
        CRntString_AssignCString(Str(), text);
    }
    ~UnkInlineString_l08b410() {
        CRntString_Clear(Str());
        CRntString_Destroy(Str());
    }
};

// A string id made from a string's text (fn_7100000850).
struct UnkTextStrId_l08b410 {
    UnkStringData* data;

    explicit UnkTextStrId_l08b410(const CRntString& text) {
        CStrId_ConstructFromRntString(reinterpret_cast<base::global::UnkStrIdBase*>(this), text, true);
    }
    ~UnkTextStrId_l08b410() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// The id table (its slot at +0xd90 is the default sound name).
inline const UnkGameManagerIdTable& SoundIds_l08b410() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table;
}

// The sound name argument as an id, the id table's default when there is none.
inline CStrId SoundName_l08b410(const char* name) {
    if (!name)
        return SoundIds_l08b410().field_0xd90;
    return CStrId(name, true);
}

// The entity the game manager has under `id`.
inline UnkEntityWithSound* FindEntity_l08b410(UnkGameManager* manager, const UnkTextStrId_l08b410& id) {
    u32 kind_value = g_unk_71727bebb8;
    u64 key = id.data->Key();
    alignas(8) u32 kind = kind_value;
    return reinterpret_cast<UnkEntityWithSound*>(fn_7100debbc8(manager, &key, &kind));
}

int fn_710108b410(lua_State* L) {
    const char* path = nullptr;
    const char* entity_name = nullptr;
    const char* sound_name = nullptr;
    f32 value_0x38;
    f32 value_0x3c;
    f32 value_0x40;
    f32 value_0x44;
    if (!fn_71010c0270(L, "ssffff|s", &path, &entity_name, &value_0x38, &value_0x3c, &value_0x40, &value_0x44,
                       &sound_name))
        return 0;

    UnkInlineString_l08b410 text(entity_name);
    UnkGameManager* manager = g_unk_7101d47310;
    UnkEntityWithSound* entity;
    {
        UnkTextStrId_l08b410 id(*text.Str());
        entity = FindEntity_l08b410(manager, id);
    }
    if (entity) {
        static UnkSoundDesc s_desc asm("g_unk_7172a01a18");
        s_desc.kind.field_0x0 = 0;
        s_desc.path = CFilePathStrId(path, true);
        {
            CStrId name = SoundName_l08b410(sound_name);
            s_desc.kind.field_0x4 = fn_7100106820(name.Text());
        }
        s_desc.field_0x6d = true;
        s_desc.field_0x38 = value_0x38;
        s_desc.field_0x3c = value_0x3c;
        s_desc.field_0x40 = value_0x40;
        s_desc.field_0x44 = value_0x44;
        fn_7100d7c374(entity, &s_desc, &g_unk_7101d470e0, false, false, true);
    }
    return 0;
}
