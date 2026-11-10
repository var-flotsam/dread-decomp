// Unit 2702 (hint CAIComponent): two AI component handlers working on the owner's attached objects.
// fn_71011051fc lets its handler (+0x1610) update the owner, then, when the owner's +0x1e0 object has a
// name, builds "<name><suffix>" (the suffix is a global string whose use counter is bumped) and calls
// the script function of that name with the owner. fn_710110fec0 turns the owner's effect object
// (+0x218) to the owner's up axis (matrix column 1), sends it a parameter block whose two vectors are
// the object's own direction (vtable slot 0x370), and clears the object's flag at +0x1a08. Names and
// layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntString;

// A string with 0x100 bytes of inline storage (0x148 bytes), as in src/base/UnkTunableRegistry.cpp.
struct UnkInlineString_b0269 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    const char* Data() const { return reinterpret_cast<const CRntString*>(str)->data; }
    const char* CStr() const { return Data() ? Data() : g_unk_710162e2ed; }
    UnkInlineString_b0269() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    ~UnkInlineString_b0269() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

struct UnkSuffixHolder_b0269 {
    u8 pad_0x0[0x10];
    u8 suffix[0x30];  // 0x10
    u8 pad_0x40[4];
    u32 uses;  // 0x44
};

struct UnkNamedObject_b0269 {
    u8 pad_0x0[0x98];
    u8 name[0x30];  // 0x98
};

struct UnkScriptOwner_b0269 {
    u8 pad_0x0[0x1e0];
    UnkNamedObject_b0269* named;  // 0x1e0
};

struct UnkHandler_b0269 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(UnkScriptOwner_b0269* owner);
};

struct UnkScriptAI_b0269 {
    u8 pad_0x0[0x30];
    UnkScriptOwner_b0269* owner;  // 0x30
    u8 pad_0x38[0x15d8];
    UnkHandler_b0269* handler;  // 0x1610
};

struct lua_State;
extern lua_State* g_unk_7172a01d18;
extern UnkSuffixHolder_b0269* g_unk_7172a06300 __attribute__((visibility("hidden")));

s32 fn_71010c0bb0(lua_State* L, const char* name);
void fn_7100fbc4e0(lua_State* L, void* arg);
s32 fn_71010c0d80(lua_State* L, s32 count, char* out, u32 out_size, void* a, void* b);

void fn_71011051fc(UnkScriptAI_b0269* self) {
    if (UnkHandler_b0269* handler = self->handler)
        handler->vf_0x40(self->owner);
    UnkNamedObject_b0269* named = self->owner->named;
    if (!named)
        return;
    const CRntString* name = reinterpret_cast<const CRntString*>(named->name);
    UnkInlineString_b0269 text;
    fn_710007eaa0(text.Str(), name);
    ++g_unk_7172a06300->uses;
    fn_710007f3b4(text.Str(), reinterpret_cast<CRntString*>(g_unk_7172a06300->suffix));
    if (fn_71010c0bb0(g_unk_7172a01d18, text.CStr())) {
        char out[0x100];
        fn_7100fbc4e0(g_unk_7172a01d18, self->owner);
        fn_71010c0d80(g_unk_7172a01d18, 1, out, 0x100, nullptr, nullptr);
    }
}

extern void* g_unk_7101a24db8[];

struct UnkEffectSink;
void fn_71010c7e6c(UnkEffectSink* sink, void* params, u32 flags);

struct UnkEffectObject_b0269 {
    void** vtable;
    u8 pad_0x8[0x1a00];
    u8 field_0x1a08;

    UnkVec3f vf_0x370() {
        return reinterpret_cast<UnkVec3f (*)(UnkEffectObject_b0269*)>(vtable[0x370 / 8])(this);
    }
};

struct UnkDirected_b0269 {
    UnkVec3f dir;
    u8 flag;
};

struct UnkEffectParams_b0269 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    UnkDirected_b0269 field_0x18;
    UnkDirected_b0269 field_0x28;
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
    u8 field_0x3e;
    u8 field_0x3f;
    u8 pad_0x40;
    u8 field_0x41;
    u8 pad_0x42[2];
    u8 field_0x44[13];
    u8 field_0x51;
    u8 field_0x52;
};

struct UnkEffectOwner_b0269 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x1a0];
    UnkEffectObject_b0269* effect;  // 0x218
};

struct UnkEffectAI_b0269 {
    u8 pad_0x0[0x30];
    UnkEffectOwner_b0269* owner;  // 0x30
};

void fn_71010cb518(UnkEffectObject_b0269* effect, const UnkVec3f* dir);

void fn_710110fec0(UnkEffectAI_b0269* self) {
    UnkEffectObject_b0269* effect = self->owner->effect;
    if (!effect)
        return;
    const UnkMatrix44& m = fn_710002614c(&self->owner->xform);
    UnkVec3f up(m.m[1], m.m[5], m.m[9]);
    fn_71010cb518(effect, &up);
    UnkEffectParams_b0269 params;
    params.field_0xc = 0;
    params.field_0x14 = 0;
    __builtin_memset(&params.field_0x18, 0, 13);
    __builtin_memset(&params.field_0x28, 0, 13);
    params.vtable = g_unk_7101a24db8 + 2;
    params.field_0x3d = 0;
    params.field_0x41 = 0;
    params.field_0x52 = 0;
    __builtin_memset(params.field_0x44, 0, 13);
    params.field_0x38 = 1;
    params.field_0x3e = 1;
    params.field_0x3f = 1;
    UnkVec3f dir = self->owner->effect->vf_0x370();
    params.field_0x28.dir = dir;
    params.field_0x28.flag = 1;
    params.field_0x18.dir = dir;
    params.field_0x18.flag = 1;
    u16 both = 0x101;
    __builtin_memcpy(&params.field_0x51, &both, 2);
    fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(self->owner->effect), &params, 1);
    self->owner->effect->field_0x1a08 = 0;
}
