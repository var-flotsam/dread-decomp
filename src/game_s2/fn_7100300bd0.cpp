// Looks up the object with hash 0x6923d83cbb01adc0 in the owner's world (+0x280 of the owner at
// +0x30) and, when it exists and its id byte (+0xb) is live there, claims it (fn_710114fe8c), calls
// its slot 0x90 with 0 and its slot 0xd0 for each of the (slot 0xf0) count of its parts with the
// vector (0, 0, 0, 1.0). Returns the object, or null when it isn't live. With bit 0 of the argument
// set, first calls fn_71011e49fc on the object at +0x2d0 of the global manager's service.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkVec4 {
    u64 xy;
    u64 zw;
};

struct UnkPart {
    void** vtable;
    u8 pad_0x8[3];
    u8 field_0xb;
    void vf_0x90(u32 a) { reinterpret_cast<void (*)(UnkPart*, u32)>(vtable[18])(this, a); }
    void vf_0xd0(u32 a, u32 b, UnkVec4* v, u32 c) {
        reinterpret_cast<void (*)(UnkPart*, u32, u32, UnkVec4*, u32)>(vtable[26])(this, a, b, v, c);
    }
    s32 vf_0xf0() { return reinterpret_cast<s32 (*)(UnkPart*)>(vtable[30])(this); }
};

struct UnkWorld;

struct UnkOwner {
    u8 pad_0x0[0x280];
    UnkWorld* field_0x280;
};

struct UnkService {
    u8 pad_0x0[0x2d0];
    void* field_0x2d0;
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkOwner* field_0x30;
};

extern char g_unk_7101648d40[];
UnkService* fn_7100deac84(UnkGameManager* manager);
void fn_71011e49fc(void* object);
void fn_7100d7c828(UnkOwner* owner, const char* name, u32 flag);
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);
bool fn_710114e668(UnkWorld* world, u8 id);
void fn_710114fe8c(UnkWorld* world, u64 hash, u32 a, u32 b);

UnkPart* fn_7100300bd0(UnkSelf* self, u32 flags) {
    if (flags & 1) {
        if (void* object = fn_7100deac84(g_unk_7101d47310)->field_0x2d0)
            fn_71011e49fc(object);
    }
    fn_7100d7c828(self->field_0x30, g_unk_7101648d40, 0);
    UnkVec4 vector;
    vector.xy = 0;
    vector.zw = 0x3f80000000000000ull;
    UnkWorld* world = self->field_0x30->field_0x280;
    UnkPart* part = fn_7101152b90(world, 0x6923d83cbb01adc0ull);
    if (part) {
        if (fn_710114e668(world, part->field_0xb)) {
            fn_710114fe8c(world, 0x6923d83cbb01adc0ull, 1, 1);
            part->vf_0x90(0);
            s32 count = part->vf_0xf0();
            for (s32 i = 0; i < count; ++i)
                part->vf_0xd0(i, 0, &vector, 1);
        } else {
            part = nullptr;
        }
    }
    return part;
}
