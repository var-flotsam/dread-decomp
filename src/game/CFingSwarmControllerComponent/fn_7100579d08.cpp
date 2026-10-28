// Handler for a message: unless fn_710074ddd8 already handles it, accepts only the one whose type
// (the value at +0x30 of the message's first field) is the expected hash, then flips the sign of the
// float at +0x6c, clears the byte at +0x180, finds the billboard component under the owner's +0xe8
// and starts the effect keyed by the value at +0x30 of its +0xd0 (creating the global manager on first
// use), and sets the float at +0x64 to 1.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkKeyHolder {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkBillboard {
    void** vtable;
    u8 pad_0x8[0xc8];
    UnkKeyHolder* field_0xd0;
};

struct UnkChain2 {
    u8 pad_0x0[0x10];
    UnkBillboard* field_0x10;
};

struct UnkChain1 {
    u8 pad_0x0[0x10];
    UnkChain2* field_0x10;
};

struct UnkSelf {
    u8 pad_0x0[0xe8];
    UnkChain1* field_0xe8;
};

struct UnkTypeRef {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkMessage {
    UnkTypeRef* field_0x0;
};

struct UnkTarget {
    u8 pad_0x0[0x64];
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    u8 pad_0x70[0x110];
    u8 field_0x180;
};

extern void* g_unk_71726da048;
extern u8 g_unk_7101649f60[];
void fn_7100e1bca4(void* manager);
void* fn_7100e1cb50(void* manager, u64* key, void* arg);
void fn_7100745440(UnkTarget* target, void* effect, u32 flag);
bool fn_710074ddd8();

bool fn_7100579d08(UnkSelf* self, UnkTarget* target, UnkMessage* message) {
    if (fn_710074ddd8())
        return true;
    if (message->field_0x0->field_0x30 != 0xfab221522cf4e099ull)
        return false;
    target->field_0x6c = -target->field_0x6c;
    target->field_0x180 = 0;
    UnkBillboard* obj = self->field_0xe8->field_0x10->field_0x10;
    UnkBillboard* billboard =
        obj && fn_710009d50c(reinterpret_cast<const CClass* (*)(UnkBillboard*)>(obj->vtable[0])(obj), fn_710072be34()) ? obj
                                                                                                          : nullptr;
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(manager);
        g_unk_71726da048 = manager;
    }
    u64 key = billboard->field_0xd0->field_0x30;
    fn_7100745440(target, fn_7100e1cb50(manager, &key, g_unk_7101649f60), 0);
    target->field_0x64 = 1.0f;
    return true;
}
