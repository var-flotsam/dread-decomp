// Constructor of a component: runs the base constructor, then constructs seventeen members in place
// (each by its own constructor) and sets a few defaults (four pairs of tuning floats and two flags).
//
// The fields are addressed by offset.
#include "types.h"

extern void* g_unk_7101a369a8[];

// Only the vtable at +0x0 is named; the rest is addressed by offset.
struct UnkComponent {
    u8 unk_0x0[1];
};

void fn_710041a198(UnkComponent* self);
void fn_7100422d7c(void* member);
void fn_7100420fa4(void* member);
void fn_71004228f8(void* member);
void fn_7100422200(void* member);
void fn_710041d07c(void* member);
void fn_71004202c8(void* member);
void fn_7100421740(void* member);
void fn_710041c440(void* member);
void fn_7100423b2c(void* member);
void fn_710041e310(void* member);
void fn_710041fb50(void* member);
void fn_710041e990(void* member);
void fn_7100424690(void* member);
void fn_710041f538(void* member);
void fn_710042315c(void* member);
void fn_71004206fc(void* member);
void fn_71008c2da8(void* member);

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_710044e048(UnkComponent* self) {
    fn_710041a198(self);
    At<void**>(self, 0) = g_unk_7101a369a8 + 2;
    fn_7100422d7c(reinterpret_cast<char*>(self) + 0x528);
    fn_7100420fa4(reinterpret_cast<char*>(self) + 0x628);
    fn_71004228f8(reinterpret_cast<char*>(self) + 0x770);
    fn_7100422200(reinterpret_cast<char*>(self) + 0x870);
    fn_710041d07c(reinterpret_cast<char*>(self) + 0x988);
    fn_71004202c8(reinterpret_cast<char*>(self) + 0xa88);
    fn_7100421740(reinterpret_cast<char*>(self) + 0xb98);
    fn_710041c440(reinterpret_cast<char*>(self) + 0xca0);
    fn_7100423b2c(reinterpret_cast<char*>(self) + 0xd90);
    fn_710041e310(reinterpret_cast<char*>(self) + 0xea8);
    fn_710041fb50(reinterpret_cast<char*>(self) + 0xf98);
    fn_710041e990(reinterpret_cast<char*>(self) + 0x10a0);
    fn_7100424690(reinterpret_cast<char*>(self) + 0x11a8);
    fn_710041f538(reinterpret_cast<char*>(self) + 0x12b8);
    fn_710042315c(reinterpret_cast<char*>(self) + 0x13c0);
    fn_71004206fc(reinterpret_cast<char*>(self) + 0x14b0);
    At<f32>(self, 0x15a0) = 1000.0f;
    At<f32>(self, 0x15a4) = 500.0f;
    At<f32>(self, 0x15a8) = 1000.0f;
    At<f32>(self, 0x15ac) = 100.0f;
    At<f32>(self, 0x15b0) = 20.0f;
    At<f32>(self, 0x15b4) = 0.6f;
    At<f32>(self, 0x15b8) = 250.0f;
    At<f32>(self, 0x15bc) = 200.0f;
    fn_71008c2da8(reinterpret_cast<char*>(self) + 0x15c0);
    fn_71008c2da8(reinterpret_cast<char*>(self) + 0x1630);
    At<u8>(self, 0x409) = 0;
    At<u8>(self, 0x4fc) = 1;
    At<u8>(self, 0x4fd) = 1;
}
