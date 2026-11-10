// Unit 2678 (tools/tu_map.py, "lua_lib"): the Lua bindings of base::math::CVector3D (full userdata with the
// "base::math::CVector3D" metatable): arithmetic metamethods, __index/__newindex and the dot, cross, mod
// and norm methods. fn_71010a4740 is the userdata test (luaL_checkudata without the error). Names are
// placeholders.
#include <math.h>
#include <string.h>

#include <new>

#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "types.h"

// The userdata at `index` when its metatable is the registry's `name`, else null.
void* fn_71010a4740(lua_State* L, int index, const char* name);

struct UnkLuaVector3D_ovb0061 {
    f32 x, y, z;
    UnkLuaVector3D_ovb0061() : x(0.0f), y(0.0f), z(0.0f) {}
    UnkLuaVector3D_ovb0061(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

static inline UnkLuaVector3D_ovb0061* CheckVector3D(lua_State* L, int index) {
    luaL_checktype(L, index, LUA_TUSERDATA);
    UnkLuaVector3D_ovb0061* v = static_cast<UnkLuaVector3D_ovb0061*>(fn_71010a4740(L, index, "base::math::CVector3D"));
    if (!v)
        luaL_typerror(L, index, "base::math::CVector3D");
    return v;
}

static inline UnkLuaVector3D_ovb0061* NewVector3D(lua_State* L) {
    UnkLuaVector3D_ovb0061* v = new (lua_newuserdata(L, sizeof(UnkLuaVector3D_ovb0061))) UnkLuaVector3D_ovb0061;
    luaL_getmetatable(L, "base::math::CVector3D");
    lua_setmetatable(L, -2);
    return v;
}

// __add
int fn_71010c1290(lua_State* L) {
    UnkLuaVector3D_ovb0061* a = CheckVector3D(L, 1);
    UnkLuaVector3D_ovb0061* b = CheckVector3D(L, 2);
    UnkLuaVector3D_ovb0061* r = NewVector3D(L);
    *r = UnkLuaVector3D_ovb0061(a->x + b->x, a->y + b->y, a->z + b->z);
    return 1;
}

// __sub
int fn_71010c1390(lua_State* L) {
    UnkLuaVector3D_ovb0061* a = CheckVector3D(L, 1);
    UnkLuaVector3D_ovb0061* b = CheckVector3D(L, 2);
    UnkLuaVector3D_ovb0061* r = NewVector3D(L);
    *r = UnkLuaVector3D_ovb0061(a->x - b->x, a->y - b->y, a->z - b->z);
    return 1;
}

// __mul: by a vector (component-wise) or by a number.
int fn_71010c1490(lua_State* L) {
    UnkLuaVector3D_ovb0061* a = CheckVector3D(L, 1);
    UnkLuaVector3D_ovb0061* b = static_cast<UnkLuaVector3D_ovb0061*>(fn_71010a4740(L, 2, "base::math::CVector3D"));
    UnkLuaVector3D_ovb0061* r = NewVector3D(L);
    if (b) {
        *r = UnkLuaVector3D_ovb0061(a->x * b->x, a->y * b->y, a->z * b->z);
    } else {
        f32 s = luaL_checknumber(L, 2);
        *r = UnkLuaVector3D_ovb0061(s * a->x, s * a->y, s * a->z);
    }
    return 1;
}

// __div: by a vector (component-wise) or by a number.
int fn_71010c15a0(lua_State* L) {
    UnkLuaVector3D_ovb0061* a = CheckVector3D(L, 1);
    UnkLuaVector3D_ovb0061* b = static_cast<UnkLuaVector3D_ovb0061*>(fn_71010a4740(L, 2, "base::math::CVector3D"));
    UnkLuaVector3D_ovb0061* r = NewVector3D(L);
    if (b) {
        *r = UnkLuaVector3D_ovb0061(a->x / b->x, a->y / b->y, a->z / b->z);
    } else {
        f32 inv = 1.0f / luaL_checknumber(L, 2);
        *r = UnkLuaVector3D_ovb0061(inv * a->x, inv * a->y, inv * a->z);
    }
    return 1;
}

// __eq: every component within 0.001.
int fn_71010c1770(lua_State* L) {
    UnkLuaVector3D_ovb0061* a = CheckVector3D(L, 1);
    UnkLuaVector3D_ovb0061* b = CheckVector3D(L, 2);
    bool eq = a->x - 0.001f <= b->x && a->x + 0.001f >= b->x &&
              a->y - 0.001f <= b->y && a->y + 0.001f >= b->y &&
              a->z - 0.001f <= b->z && a->z + 0.001f >= b->z;
    lua_pushboolean(L, eq);
    return 1;
}

__attribute__((visibility("hidden"))) int fn_71010c1bb0(lua_State* L);
__attribute__((visibility("hidden"))) int fn_71010c1c80(lua_State* L);
__attribute__((visibility("hidden"))) int fn_71010c1da0(lua_State* L);
__attribute__((visibility("hidden"))) int fn_71010c1e40(lua_State* L);
__attribute__((visibility("hidden"))) int fn_71010c1ec0(lua_State* L);

// __index: a component by 0-based index or by "x"/"y"/"z", or one of the methods.
int fn_71010c18d0(lua_State* L) {
    UnkLuaVector3D_ovb0061* v = CheckVector3D(L, 1);
    switch (lua_type(L, 2)) {
    case LUA_TSTRING: {
        const char* key = luaL_checkstring(L, 2);
        if (strlen(key) == 1) {
            switch (*key) {
            case 'x':
                lua_pushnumber(L, v->x);
                return 1;
            case 'y':
                lua_pushnumber(L, v->y);
                return 1;
            case 'z':
                lua_pushnumber(L, v->z);
                return 1;
            }
        } else if (!strcmp(key, "dot")) {
            lua_pushcfunction(L, fn_71010c1bb0);
            return 1;
        } else if (!strcmp(key, "cross")) {
            lua_pushcfunction(L, fn_71010c1c80);
            return 1;
        } else if (!strcmp(key, "mod")) {
            lua_pushcfunction(L, fn_71010c1da0);
            return 1;
        } else if (!strcmp(key, "modSqr")) {
            lua_pushcfunction(L, fn_71010c1e40);
            return 1;
        } else if (!strcmp(key, "norm")) {
            lua_pushcfunction(L, fn_71010c1ec0);
            return 1;
        }
        lua_pushnil(L);
        break;
    }
    case LUA_TNUMBER:
        switch ((int)luaL_checkinteger(L, 2)) {
        case 0:
            lua_pushnumber(L, v->x);
            return 1;
        case 1:
            lua_pushnumber(L, v->y);
            return 1;
        case 2:
            lua_pushnumber(L, v->z);
            return 1;
        }
        lua_pushnil(L);
        break;
    }
    return 1;
}

// __newindex: sets a component by 0-based index or by "x"/"y"/"z".
int fn_71010c1a90(lua_State* L) {
    UnkLuaVector3D_ovb0061* v = CheckVector3D(L, 1);
    f32 value = luaL_checknumber(L, 3);
    switch (lua_type(L, 2)) {
    case LUA_TSTRING: {
        const char* key = luaL_checkstring(L, 2);
        if (strlen(key) != 1)
            break;
        switch (*key) {
        case 'x':
            v->x = value;
            return 1;
        case 'y':
            v->y = value;
            return 1;
        case 'z':
            v->z = value;
            return 1;
        }
        luaL_error(L, "Invalid access to base::math::CVector3D element");
        break;
    }
    case LUA_TNUMBER:
        switch ((int)luaL_checkinteger(L, 2)) {
        case 0:
            v->x = value;
            return 1;
        case 1:
            v->y = value;
            return 1;
        case 2:
            v->z = value;
            return 1;
        }
        luaL_error(L, "Invalid access to base::math::CVector3D element");
        break;
    }
    return 1;
}

// dot
int fn_71010c1bb0(lua_State* L) {
    UnkLuaVector3D_ovb0061* a = CheckVector3D(L, 1);
    UnkLuaVector3D_ovb0061* b = CheckVector3D(L, 2);
    lua_pushnumber(L, a->x * b->x + a->y * b->y + a->z * b->z);
    return 1;
}

// cross
int fn_71010c1c80(lua_State* L) {
    UnkLuaVector3D_ovb0061* a = CheckVector3D(L, 1);
    UnkLuaVector3D_ovb0061* b = CheckVector3D(L, 2);
    UnkLuaVector3D_ovb0061* r = NewVector3D(L);
    *r = UnkLuaVector3D_ovb0061(a->y * b->z - a->z * b->y, a->z * b->x - b->z * a->x, b->y * a->x - a->y * b->x);
    return 1;
}

// mod: the length.
int fn_71010c1da0(lua_State* L) {
    UnkLuaVector3D_ovb0061* v = CheckVector3D(L, 1);
    lua_pushnumber(L, sqrtf(v->x * v->x + v->y * v->y + v->z * v->z));
    return 1;
}

// norm: the vector over its length (plus a small epsilon).
int fn_71010c1ec0(lua_State* L) {
    UnkLuaVector3D_ovb0061* v = CheckVector3D(L, 1);
    UnkLuaVector3D_ovb0061* r = NewVector3D(L);
    f32 inv = 1.0f / (sqrtf(v->x * v->x + v->y * v->y + v->z * v->z) + 1e-4f);
    *r = UnkLuaVector3D_ovb0061(v->x * inv, v->y * inv, inv * v->z);
    return 1;
}
