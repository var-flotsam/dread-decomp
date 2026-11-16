// Unit 2666 (lua_bind): sets a property of a section of the blackboard of the entity named by
// argument 1, from a value of the type the letter in argument 4 names (b, f, i or s).
// Names and layouts are placeholders.
#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2676.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkEnumType;
UnkEnumType* fn_710009ffd4();  // base::global::TRntString256's type
s32 fn_71010475d0(CBlackboard* self, const CStrId& key, const CStrId& name);

// A string with 0x100 bytes of inline storage (0x148 bytes).
struct UnkInlineString_l085540 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_l085540() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    ~UnkInlineString_l085540() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

struct UnkBlackboardOwner_l085540 {
    u8 pad_0x0[0x1260];
    CBlackboard board;
};

struct UnkBlackboardEntity_l085540 {
    u8 pad_0x0[0x240];
    UnkBlackboardOwner_l085540* owner;
};

// The entity the game manager has under `id`.
inline UnkBlackboardEntity_l085540* FindEntity(UnkGameManager* manager, const CStrId& id) {
    u64 key = id.data->Key();
    alignas(8) u32 kind = g_unk_71727bebb8;
    return reinterpret_cast<UnkBlackboardEntity_l085540*>(fn_7100debbc8(manager, &key, &kind));
}

int fn_7101085540(lua_State* L) {
    const char* entity_name = luaL_checkstring(L, 1);
    UnkGameManager* manager = g_unk_7101d47310;
    UnkBlackboardEntity_l085540* entity;
    {
        CStrId id(entity_name, true);
        entity = FindEntity(manager, id);
    }
    if (!entity)
        return 0;
    UnkBlackboardOwner_l085540* owner = entity->owner;
    if (!owner)
        return 0;

    CStrId section_id(luaL_checkstring(L, 2), true);
    CStrId name(luaL_checkstring(L, 3), true);
    const char* type = luaL_checkstring(L, 4);
    if (__builtin_strlen(type) != 1)
        return 0;

    CBlackboard* board = &owner->board;
    if (fn_71010474f4(board, section_id, name)) {
        s32 current = fn_71010475d0(board, section_id, name);
        if (current != 'n' && type[0] != (char)current)
            return 0;
    }

    UnkPropertyName* property = reinterpret_cast<UnkPropertyName*>(&name);
    switch (type[0]) {
    case 'b': {
        bool value = fn_71010a4900(L, 5) != 0;
        CBlackboard::CSection* section = fn_71010472e0(board, section_id);
        if (section && !board->field_0xe8) {
            UnkTypedValue typed = {fn_710000d228(), &value};
            fn_7100332c80(section, property, &typed);
        }
        break;
    }
    case 'f': {
        f32 value = luaL_checknumber(L, 5);
        CBlackboard::CSection* section = fn_71010472e0(board, section_id);
        if (section && !board->field_0xe8) {
            UnkTypedValue typed = {fn_710000abbc(), &value};
            fn_7100332c80(section, property, &typed);
        }
        break;
    }
    case 'i': {
        s32 value = luaL_checknumber(L, 5);
        CBlackboard::CSection* section = fn_71010472e0(board, section_id);
        if (section && !board->field_0xe8) {
            UnkTypedValue typed = {fn_7100014768(), &value};
            fn_7100332c80(section, property, &typed);
        }
        break;
    }
    case 's': {
        const char* text = luaL_checkstring(L, 5);
        CBlackboard::CSection* section = fn_71010472e0(board, section_id);
        if (section && !board->field_0xe8) {
            UnkInlineString_l085540 value;
            fn_710007e77c(value.Str(), text);
            UnkTypedValue typed = {reinterpret_cast<const base::reflection::CType*>(fn_710009ffd4()), &value};
            fn_7100332c80(section, property, &typed);
        }
        break;
    }
    }
    return 0;
}
