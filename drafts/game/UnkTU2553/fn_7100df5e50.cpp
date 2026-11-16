// CGameManager::CTunableGameManager's unit (unit 2553, hint only): creates the entity `name` from the
// character class of the same name unless one by that name exists, and when it is an entity sets its camera
// up above the player. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "game/UnkTU344.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkActorDefFactory;

// A counted reference to a resource whose destructor reads the pointer once (as in
// src/game/CSpawnPointComponent/CSpawnPointComponent_b52.cpp).
struct UnkSpawnDefRef_b52 {
    UnkCountedResource* ptr;

    UnkSpawnDefRef_b52& operator=(const UnkSpawnDefRef_b52& other) {
        UnkCountedResource* p = other.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkSpawnDefRef_b52() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

UnkSpawnDefRef_b52 fn_7100e051b4(UnkActorDefFactory* factory, const CStrId* name);
void* fn_7100dea8d0(UnkGameManager* manager, const char* name, void* char_class, bool flag);
void fn_71002324f0(void* entity);
struct UnkActor_71002325f8;
void fn_71002325f8(UnkActor_71002325f8* self);
void fn_7100df60c0(UnkCountedResource* char_class);
void fn_7100df63a4(UnkCountedResource* char_class);

struct UnkVec3_df5e50 {
    f32 x, y, z;
};

struct UnkCamera_df5e50;
struct UnkOwner_71010e2278;
struct UnkArg1_71010e2278;
struct UnkArg2_71010e2278;
void fn_71010e2278(UnkOwner_71010e2278* self, UnkArg1_71010e2278* a1, UnkArg2_71010e2278* a2,
                   f32 x, f32 y, f32 z);
void fn_71010e530c(UnkCamera_df5e50* self, UnkVec3_df5e50* position);
void fn_71010e5344(UnkCamera_df5e50* self, UnkVec3_df5e50* direction);

struct UnkReflObject_df5e50 {
    virtual const base::reflection::CClass* GetClass();
};

struct UnkEntity_df5e50 {
    u8 pad_0x0[0x248];
    UnkCamera_df5e50* camera;  // 0x248
    u8 pad_0x250[0x154];
    bool field_0x3a4;
};

// The player: its transform's translation column.
struct UnkPlayer_df5e50 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
};
struct UnkPlayerHandle_df5e50 {
    u8 pad_0x0[0x10];
    UnkPlayer_df5e50* player;
};
struct UnkManager_df5e50 {
    u8 pad_0x0[0x498];
    char* mode;  // 0x498: starts with a pointer-sized field, then the mode's name
    u8 pad_0x4a0[0x29b8 - 0x4a0];
    UnkPlayerHandle_df5e50* field_0x29b8;
};

static inline bool IsEditor(UnkManager_df5e50* self) {
    if (self->mode)
        return fn_710007f140(reinterpret_cast<const CRntString*>(self->mode + 8), "EDITOR");
    return false;
}

// The character class with this name.
static inline UnkSpawnDefRef_b52 CharClass(const char* name) {
    UnkActorDefFactory* factory = g_unk_71726a0220;
    CStrId id(name, true);
    return fn_7100e051b4(factory, &id);
}

void fn_7100df5e50(UnkGameManager* manager, const char* name) {
    UnkManager_df5e50* self = reinterpret_cast<UnkManager_df5e50*>(manager);
    void* existing;
    {
        u32 kind;
        u64 key;
        CStrId id(name, true);
        key = reinterpret_cast<UnkGameManagerKey*>(id.data)->id;
        kind = g_unk_71727bebb8;
        existing = fn_7100deba8c(manager, &key, &kind);
    }
    if (existing)
        return;
    UnkSpawnDefRef_b52 def = CharClass(name);
    fn_7100df60c0(def.ptr);
    fn_7100df63a4(def.ptr);
    void* object = fn_7100dea8d0(manager, name, def.ptr, true);
    if (!object)
        return;
    UnkReflObject_df5e50* refl = static_cast<UnkReflObject_df5e50*>(object);
    if (!fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(refl->GetClass()),
                       reinterpret_cast<const base::reflection::CType*>(fn_7100d70c20())))
        return;
    UnkEntity_df5e50* entity = static_cast<UnkEntity_df5e50*>(object);
    entity->field_0x3a4 = true;
    fn_71002324f0(entity);
    fn_71002325f8(reinterpret_cast<UnkActor_71002325f8*>(entity));
    fn_710023272c(entity);
    UnkCamera_df5e50* camera = entity->camera;
    if (!camera)
        return;
    IsEditor(self);
    {
        UnkVec3_df5e50 position = {0.0f, 0.0f, 15000.0f};
        UnkVec3_df5e50 direction = {0.0f, 0.0f, -1.0f};
        fn_71010e2278(reinterpret_cast<UnkOwner_71010e2278*>(camera), reinterpret_cast<UnkArg1_71010e2278*>(&position),
                      reinterpret_cast<UnkArg2_71010e2278*>(&direction), 45.0f, 100.0f, 100000.0f);
    }
    UnkPlayerHandle_df5e50* handle = self->field_0x29b8;
    if (!handle || !handle->player)
        return;
    UnkPlayer_df5e50* player = handle->player;
    {
        UnkVec3_df5e50 at;
        at.x = player->transform[0][3] + 0.0f;
        at.y = player->transform[1][3] + 0.0f;
        at.z = player->transform[2][3] + 2000.0f;
        fn_71010e530c(camera, &at);
    }
    {
        UnkVec3_df5e50 down = {0.0f, 0.0f, -1.0f};
        fn_71010e5344(camera, &down);
    }
}
