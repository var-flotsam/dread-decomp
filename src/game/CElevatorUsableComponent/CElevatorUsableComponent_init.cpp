// The static initialiser of src/game/CElevatorUsableComponent/CElevatorUsableComponent_init.cpp7's unit: the TU header with a third id ("CElevatorUsableComponent"), the
// EElevatorDirection enum type with its converters to and from int, then the component's class.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/CEnumType.h"
#include "game/UnkTUInit.h"

// The TU header with one more id after the usual two.
struct UnkTUHeader_71727508f8 {
    UnkTUHeader header;
    CStrId field_0x18;
};

extern UnkTUHeader_71727508f8 g_unk_71727508f8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172750790;
void fn_7100a80f20();
void fn_7100a8106c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727508a8;
void fn_7100a827ac() __attribute__((visibility("hidden")));
void fn_7100a827b8() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727508b0;
extern UnkConverter g_unk_71727508d0;
extern char g_unk_71727508f0;
extern UnkReflType g_unk_7172750920 __attribute__((visibility("hidden")));

void fn_7100a82930() {
    InitHeader(g_unk_71727508f8.header);
    new (&g_unk_71727508f8.field_0x18) CStrId("CElevatorUsableComponent", true);
    fn_7100000250(fn_710000051c, &g_unk_71727508f8.field_0x18, &g_unk_7101a19000);
    InitEnum(&g_unk_7172750790, "EElevatorDirection", fn_7100a8106c, fn_7100a80f20);
    fn_710009ec00(&g_unk_71727508a8, &g_unk_7172750790);
    InitEnumConverters(&g_unk_7172750790, g_unk_71727508b0, fn_7100a827ac, g_unk_71727508d0, fn_7100a827b8);
    fn_7100a811f0();
    fn_710009ec00(&g_unk_71727508f0, &g_unk_7172750920);
}
