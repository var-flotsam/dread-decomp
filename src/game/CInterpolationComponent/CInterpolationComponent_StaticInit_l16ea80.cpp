// CInterpolationComponent's unit (tools/tu_map.py unit 2718): its static initialiser. Builds the unit's
// header, notes the class, and registers the EInterpDirection and EInterpolationType enums with their
// integer converters. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"
#include "types.h"

struct UnkInterpolationTUData_l16ea80 {
    UnkTUHeader header;
    u8 pad_0x18[0x8];
    u8 field_0x20;
};
extern UnkInterpolationTUData_l16ea80 g_unk_7172a0ab88 __attribute__((visibility("hidden")));

extern char g_unk_7172a0a8c0;
extern base::reflection::CType g_unk_7172a0a8c8;
extern char g_unk_7172a0a9e0;
extern UnkConverter g_unk_7172a0a9e8;
extern UnkConverter g_unk_7172a0aa08;
extern base::reflection::CType g_unk_7172a0aa28;
extern char g_unk_7172a0ab40;
extern UnkConverter g_unk_7172a0ab48;
extern UnkConverter g_unk_7172a0ab68;

void fn_710116d760(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
void fn_710116daec(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
void fn_710116d5b4(void* e);
void fn_710116d8e0(void* e);
void fn_710116e89c() __attribute__((visibility("hidden")));
void fn_710116e8a8() __attribute__((visibility("hidden")));
void fn_710116e8b4() __attribute__((visibility("hidden")));
void fn_710116e8c0() __attribute__((visibility("hidden")));

void fn_710116ea80() {
    InitHeader(g_unk_7172a0ab88.header);
    fn_710116d338();
    fn_710009ec00(&g_unk_7172a0a8c0, &g_unk_7172a0ab88.field_0x20);

    UnkReflType* direction = reinterpret_cast<UnkReflType*>(&g_unk_7172a0a8c8);
    InitEnum(direction, "CInterpolationComponent::EInterpDirection", fn_710116d760,
             reinterpret_cast<void (*)()>(fn_710116d5b4));
    fn_710009ec00(&g_unk_7172a0a9e0, direction);
    InitEnumConverters(direction, g_unk_7172a0a9e8, fn_710116e89c, g_unk_7172a0aa08, fn_710116e8a8);

    UnkReflType* type = reinterpret_cast<UnkReflType*>(&g_unk_7172a0aa28);
    InitEnum(type, "CInterpolationComponent::EInterpolationType", fn_710116daec,
             reinterpret_cast<void (*)()>(fn_710116d8e0));
    fn_710009ec00(&g_unk_7172a0ab40, type);
    InitEnumConverters(type, g_unk_7172a0ab48, fn_710116e8b4, g_unk_7172a0ab68, fn_710116e8c0);
}
