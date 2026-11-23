// Unit 2662 (tools/tu_map.py): fn_7101073c30 pushes one Lua value for each character of a parameter type
// string ('b', 'f', 'i', 's'), read from a parameter file under the names "Param1", "Param2"..., then calls
// into the game's Lua state with them. Names and layouts are placeholders.
#include <stdio.h>

#include <algorithm>

#include "../../../lib/lua/lua.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntString;
using base::global::CStrId;

struct UnkScriptParam_l073c30 {
    u64 offset;  // into the file
    u64 key;     // the name's key
    u32 type;    // 'b', 'f', 'i' or 's'
};

struct UnkScriptParams_l073c30 {
    UnkScriptParam_l073c30* entries;
    u32 count;
    u8 pad_0xc[0x28 - 0xc];
    CRntFile file;  // 0x28: its data holds the strings

    UnkScriptParam_l073c30* Find(const CStrId& name) {
        u64 key = name.data->Key();
        return std::find_if(entries, &entries[count],
                            [key](const UnkScriptParam_l073c30& param) { return param.key == key; });
    }
    void Read(const UnkScriptParam_l073c30* param, void* dest, u64 size) {
        CRntFile_Seek(&file, param->offset, 0);
        fn_710007c70c(&file, dest, size);
    }
    template <class T>
    T Read(const UnkScriptParam_l073c30* param) {
        T value;
        Read(param, &value, sizeof(value));
        return value;
    }
    UnkScriptParam_l073c30* End() { return &entries[count]; }
    // Missing or of another type: the result is left unset, as in the original.
    bool GetBool(const CStrId& name) {
        bool value;
        UnkScriptParam_l073c30* param = Find(name);
        if (param != End() && param->type == 'b')
            value = Read<bool>(param);
        return value;
    }
    s32 GetInt(const CStrId& name) {
        s32 value;
        UnkScriptParam_l073c30* param = Find(name);
        if (param != End() && param->type == 'i')
            value = Read<s32>(param);
        return value;
    }
    const char* GetString(const CStrId& name) {
        UnkScriptParam_l073c30* param = Find(name);
        if (param == End() || param->type != 's')
            return nullptr;
        return reinterpret_cast<const char*>(file.data) + param->offset;
    }
};

struct UnkScriptTarget_l073c30 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual bool vf_0x10();
};

struct UnkScriptCall_l073c30 {
    u8 pad_0x0[0x10];
    UnkScriptTarget_l073c30* target;  // 0x10
    CRntString types;                 // 0x18
    UnkScriptParams_l073c30* params;  // 0x48

    const char* Types() const { return types.data ? types.data : g_unk_710162e2ed; }
};

void fn_7101073c30(UnkScriptCall_l073c30* self) {
    if (!self->target || !self->target->vf_0x10())
        return;
    for (u64 i = 0; i < self->types.length; ++i) {
        char text[0x20];
        sprintf(text, "Param%d", (s32)(i + 1));
        CStrId name(text, true);
        switch (self->Types()[i]) {
        case 'b': {
            lua_pushboolean(g_unk_7172a01d18, self->params->GetBool(name));
            break;
        }
        case 'f': {
            // Integers are read as such and converted.
            f32 value;  // unset in the original when the parameter is missing
            UnkScriptParams_l073c30* params = self->params;
            UnkScriptParam_l073c30* param = params->Find(name);
            if (param != params->End()) {
                if (param->type == 'f')
                    params->Read(param, &value, 4);
                else if (param->type == 'i')
                    value = params->Read<s32>(param);
            }
            lua_pushnumber(g_unk_7172a01d18, value);
            break;
        }
        case 'i': {
            lua_pushinteger(g_unk_7172a01d18, self->params->GetInt(name));
            break;
        }
        case 's':
            lua_pushstring(g_unk_7172a01d18, self->params->GetString(name));
            break;
        }
    }
    char error[0x100];
    fn_71010c0d80(g_unk_7172a01d18, self->types.length, error, sizeof(error), nullptr, nullptr);
}
