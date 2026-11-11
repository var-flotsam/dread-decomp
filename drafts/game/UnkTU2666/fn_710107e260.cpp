#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct lua_State;
s32 fn_71010c0270(lua_State* L, const char* fmt, ...);
struct UnkWeakTarget;
extern u32 g_unk_71727bebb8;
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);
void fn_7100d7babc(void* self, bool flag);
void fn_71010e46cc(void* self, u8 flag);

struct UnkLuaVec3_b0164 {
    f32 x, y, z;
    UnkLuaVec3_b0164() : x(0.0f), y(0.0f), z(0.0f) {}
};
struct UnkStrIdHash_b0164 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkTarget_b0164;
struct UnkTargetVTable_b0164 {
    void* pad_0x0[26];
    void (*vf_0xd0)(UnkTarget_b0164* self, const UnkLuaVec3_b0164* v, bool flag);
    void (*vf_0xd8)(UnkTarget_b0164* self, const UnkLuaVec3_b0164* v, bool flag);
};
struct UnkTarget_b0164 {
    UnkTargetVTable_b0164* vtable;
};

s32 fn_710107e260(lua_State* L) {
    f32 speed;
    u32 kind;
    const char* name;
    f32 yaw;
    u8 flag;
    UnkLuaVec3_b0164 v;
    if (fn_71010c0270(L, "sfbffff", &name, &speed, &flag, &v.x, &v.y, &v.z, &yaw)) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkTarget_b0164* target;
        {
            u64 key;
            CStrId id(name, true);
            u32 k = g_unk_71727bebb8;
            u64 h = reinterpret_cast<UnkStrIdHash_b0164*>(id.data)->hash;
            key = h;
            kind = k;
            target = reinterpret_cast<UnkTarget_b0164*>(fn_7100debbc8(manager, &key, &kind));
        }
        if (!target)
            return 1;
        target->vtable->vf_0xd0(target, &v, true);
        f32 turned = yaw * 1.57f;
        UnkLuaVec3_b0164 rot;
        rot.y = turned;
        target->vtable->vf_0xd8(target, &rot, true);
        if (speed > 0.0f)
            fn_7100d7babc(target, true);
        if (flag) {
            void* camera = fn_7100defcec(g_unk_7101d47310);
            if (!camera)
                return 0;
            fn_71010e46cc(camera, flag);
        }
        return 0;
    }
    return 0;
}
