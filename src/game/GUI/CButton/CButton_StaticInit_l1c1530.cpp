// GUI::CButton unit: the unit's static initialiser (game/UnkTUInit.h): the header data with two scales and a
// flag, the GUI::CButton::TextAlign enum type with its converters to and from the integer type, the class
// list entry and the name "Button". Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/GUI/CButton.h"
#include "game/UnkTUInit.h"
#include "base/reflection/TypeObject.h"

struct UnkButtonTUData_l1c1530 {
    UnkTUHeader header;
    u8 unk_0x18[0x10];
    f32 scale_a;  // 0x28
    f32 scale_b;  // 0x2c
    bool flag;    // 0x30
};

// The class name and the class type after it.
struct UnkButtonClassData_l1c1530 {
    CStrId name;
    u8 unk_0x8[0x10];
    UnkReflType type;  // 0x18
};

extern UnkButtonTUData_l1c1530 g_unk_7172a0d320 __attribute__((visibility("hidden")));
namespace GUI::CButton { enum class TextAlign : u32; }
template <> UnkReflType base::reflection::TypeObject<GUI::CButton::TextAlign, UnkReflType>::object asm("g_unk_7172a0d1b8");
extern char g_unk_7172a0d2d0;
extern UnkConverter g_unk_7172a0d2d8;
extern UnkConverter g_unk_7172a0d2f8;
extern char g_unk_7172a0d318;
void fn_71011be0d0(void* e);
void fn_71011be27c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71011c1358() __attribute__((visibility("hidden")));
void fn_71011c1364() __attribute__((visibility("hidden")));

void fn_71011c1530() {
    InitHeader(g_unk_7172a0d320.header);
    g_unk_7172a0d320.scale_a = -1.0f;
    g_unk_7172a0d320.scale_b = -1.0f;
    g_unk_7172a0d320.flag = true;
    InitEnum(&base::reflection::TypeObject<GUI::CButton::TextAlign, UnkReflType>::object, "GUI::CButton::TextAlign", fn_71011be27c, reinterpret_cast<void (*)()>(fn_71011be0d0));
    fn_710009ec00(&g_unk_7172a0d2d0, &base::reflection::TypeObject<GUI::CButton::TextAlign, UnkReflType>::object);
    InitEnumConverters(&base::reflection::TypeObject<GUI::CButton::TextAlign, UnkReflType>::object, g_unk_7172a0d2d8, fn_71011c1358, g_unk_7172a0d2f8, fn_71011c1364);
    fn_71011be400();
    fn_710009ec00(&g_unk_7172a0d318, &g_unk_7172a0d340.type);
    new (&g_unk_7172a0d340.name) CStrId("Button", true);
    fn_7100000250(CStrId_Destroy, &g_unk_7172a0d340.name, &g_unk_7101a19000);
}
