// lua_bind's unit (tools/tu_map.py): script bindings with the unit's argument reader (fn_71010c0270). An entity is
// found by name through the game manager (fn_7100deba8c with the name id's hash and the kind g_unk_71727bebb8) and
// its door-like logic object taken through fn_710055b104: clearing it (fn_7100fbbaa4: its 0x80-byte entries at +0x30
// destroyed by fn_7100ad04c4, then the ids at +0x16b0 released and the 0xd0-byte objects at +0x16a8 destroyed, which
// share the count at +0x16b8), setting a named state's flag (fn_7100acc8a8), linking a second entity (kind 2,
// fn_7100acc5ec); a name pushed onto the id list g_unk_7172a0768c; and the AI manager's group calls (the group named
// as a CAIManager::EAIGroup, fn_71000997b4) with or without a position. Names and layouts are placeholders.
#include <new>

#include "../../../lib/lua/lua.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU1629.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2704.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

struct UnkLuaDoorObj {
    virtual void vf_0x0();
    u8 pad_0x8[0xc8];
};
static_assert(sizeof(UnkLuaDoorObj) == 0xd0, "layout");

struct UnkLuaDoorTarget {
    u8 pad_0x0[0x30];
    u8 (*items)[0x80];  // 0x30
    u32 item_count;     // 0x38
    u8 pad_0x3c[0x16a8 - 0x3c];
    UnkLuaDoorObj* objects;  // 0x16a8
    CStrId* ids;             // 0x16b0
    u32 count;               // 0x16b8
};
static_assert(__builtin_offsetof(UnkLuaDoorTarget, count) == 0x16b8, "layout");

UnkLuaDoorTarget* fn_710055b104(void* object);

struct UnkLuaDoorState {
    u8 pad_0x0[0x10];
    bool enabled;  // 0x10
};
UnkLuaDoorState* fn_7100acc8a8(UnkLuaDoorTarget* target, const CStrId& name);
void* fn_7100acc5ec(UnkLuaDoorTarget* target, void* object);

struct UnkLuaStrData {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

void fn_7100fbbaa4(UnkLuaDoorTarget* self) {
    for (u64 i = 0; i < self->item_count; ++i)
        fn_7100ad04c4(self->items[i]);
    self->item_count = 0;
    if (self->ids && self->count) {
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].Reset();
    }
    if (self->objects && self->count) {
        for (u64 i = 0; i < self->count; ++i)
            self->objects[i].vf_0x0();
    }
    self->count = 0;
}

struct __attribute__((packed)) UnkLuaIdList {
    u32 count;
    CStrId items[1];
};
struct UnkStruct_7172a0768c;

int fn_7100fbb9e0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name) && *name) {
        CStrId id(name, true);
        UnkLuaIdList& list = reinterpret_cast<UnkLuaIdList&>(g_unk_7172a0768c);
        new (&list.items[list.count]) CStrId(id);
        ++list.count;
    }
    return 0;
}

int fn_7100fb9bc0(lua_State* L) {
    u64 hash;
    alignas(8) u32 kind;
    const char* name;
    if (fn_71010c0270(L, "s", &name) && *name) {
        UnkGameManager* manager = g_unk_7101d47310;
        void* object;
        {
            CStrId id(name, true);
            u32 k = g_unk_71727bebb8;
            u64 h = reinterpret_cast<UnkLuaStrData*>(id.data)->hash;
            hash = h;
            kind = k;
            object = fn_7100deba8c(manager, &hash, &kind);
        }
        if (object) {
            if (UnkLuaDoorTarget* target = fn_710055b104(object))
                fn_7100fbbaa4(target);
        }
    }
    return 0;
}

static inline void* FindObject(UnkGameManager* manager, const CStrId& id, u32 kind_value) {
    u64 hash;
    alignas(8) u32 kind;
    u32 k = kind_value;
    u64 h = reinterpret_cast<UnkLuaStrData*>(id.data)->hash;
    hash = h;
    kind = k;
    return fn_7100deba8c(manager, &hash, &kind);
}

int fn_7100fb9840(lua_State* L) {
    const char* name;
    const char* state;
    bool enabled;
    if (fn_71010c0270(L, "ssb", &name, &state, &enabled) && *name && *state) {
        UnkGameManager* manager = g_unk_7101d47310;
        void* object;
        {
            CStrId id(name, true);
            object = FindObject(manager, id, g_unk_71727bebb8);
        }
        if (!object)
            return 0;
        UnkLuaDoorTarget* target = fn_710055b104(object);
        if (!target)
            return 0;
        UnkLuaDoorState* entry;
        {
            CStrId id(state, true);
            entry = fn_7100acc8a8(target, id);
        }
        if (entry)
            entry->enabled = enabled;
    }
    return 0;
}

int fn_7100fb96c0(lua_State* L) {
    const char* name;
    const char* other;
    if (fn_71010c0270(L, "ss", &name, &other) && *name && *other) {
        void* object;
        {
            UnkGameManager* manager = g_unk_7101d47310;
            CStrId id(name, true);
            object = FindObject(manager, id, g_unk_71727bebb8);
        }
        void* object2;
        {
            UnkGameManager* manager = g_unk_7101d47310;
            CStrId id(other, true);
            object2 = FindObject(manager, id, 2);
        }
        if (object && object2) {
            if (UnkLuaDoorTarget* target = fn_710055b104(object))
                fn_7100acc5ec(target, object2);
        }
    }
    return 0;
}

struct UnkReflType;
u32 fn_71000997b4(UnkReflType* type, const char* name);

void fn_7100daa92c(UnkManager_71726af468* self, CStrId name, u32 value);
void fn_7100daa638(UnkManager_71726af468* self, CStrId name, u32 value, const CVector3D& pos);

static inline UnkManager_71726af468* AIManager() {
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return m;
}

int fn_7100fbb850(lua_State* L) {
    const char* name;
    const char* group;
    if (fn_71010c0270(L, "ss", &name, &group) && *name && *group) {
        CStrId id(name, true);
        u32 value = fn_71000997b4(&g_unk_71727bc290, group);
        fn_7100daa92c(AIManager(), id, value);
    }
    return 0;
}

int fn_7100fbb6e0(lua_State* L) {
    const char* name;
    const char* group;
    CVector3D pos;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    if (fn_71010c0270(L, "ssv", &name, &group, &pos) && *name && *group) {
        CStrId id(name, true);
        u32 value = fn_71000997b4(&g_unk_71727bc290, group);
        fn_7100daa638(AIManager(), id, value, pos);
    }
    return 0;
}
