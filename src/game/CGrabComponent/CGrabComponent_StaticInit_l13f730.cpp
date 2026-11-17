// Static initialiser of the CGrabComponent unit: its string ids, the class registration, the
// CGrabComponent::ELinkMode enum and the registry of grabbing components (an empty list).
// Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/UnkSList.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CGrabComponent.h"
#include "base/reflection/TypeObject.h"

using namespace unk_static_init;

struct UnkStatics_710113f730 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x18];
    u8 field_0x40[8];
};
extern char g_unk_7172a08f50[];
namespace CGrabComponent { enum class ELinkMode : u32; }
template <> UnkEnumType base::reflection::TypeObject<CGrabComponent::ELinkMode, UnkEnumType>::object asm("g_unk_7172a08f58");
void fn_710113a9ec(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710113a840();
extern char g_unk_7172a09070[];
extern UnkConversion g_unk_7172a09078;
extern UnkConversion g_unk_7172a09098;
void fn_710113f520() __attribute__((visibility("hidden")));
void fn_710113f52c() __attribute__((visibility("hidden")));

extern char g_unk_7101cb33a8[];
void fn_710113ad50(void* list);

void fn_710113f730() {
    InitHeaderIds(&g_unk_7172a090e8.ids);
    InitId(&g_unk_7172a090e8.field_0x18, "grab");
    InitId(&g_unk_7172a090e8.field_0x20, "Grab");
    fn_710113a334();
    fn_710009ec00(g_unk_7172a08f50, g_unk_7172a090e8.field_0x40);
    InitEnum(&base::reflection::TypeObject<CGrabComponent::ELinkMode, UnkEnumType>::object, "CGrabComponent::ELinkMode", fn_710113a9ec, fn_710113a840,
             g_unk_7172a09070, &g_unk_7172a09078, fn_710113f520, &g_unk_7172a09098, fn_710113f52c);

    UnkGrabList_ovb0057& list = g_unk_7172a090b8;
    list.field_0x0 = reinterpret_cast<u64>(g_unk_7101cb33a8 + 0x10);
    list.count = 0;
    list.head = nullptr;
    list.tail = nullptr;
    list.allocator = fn_7100083990();
    list.flags = 0x13;
    fn_7100000250(fn_710113ad50, &list, g_unk_7101a19000);
}
