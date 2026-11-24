// CFreezeRoomComponent's unit static initialiser (tools/tu_map.py unit 1537): the unit header and a
// pair of -1 limits with a flag, the "FREEZE_ROOMS" property, the room type enum with its converters,
// the two config classes, the component's class registration and its id. Names and layouts are
// placeholders.
#include "base/UnkStrIdRegistry.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/UnkTUInit.h"
#include "base/reflection/TypeObject.h"

// The unit's data that follows its header (its statics, merged into one block).
struct UnkFreezeRoomTU_la24610 {
    UnkTUHeader header;
    u8 pad_0x18[0x30 - 0x18];
    f32 field_0x30;
    f32 field_0x34;
    bool field_0x38;
};

// The component's id and class object (another merged block).
struct UnkFreezeRoomClassTU_la24610 {
    CStrId id;
    u8 pad_0x8[0x90 - 0x8];
    u8 type[8];  // 0x90
};

extern UnkFreezeRoomTU_la24610 g_unk_7172745fe0 __attribute__((visibility("hidden")));
extern UnkFreezeRoomClassTU_la24610 g_unk_7172746000 __attribute__((visibility("hidden")));
extern UnkPropertyEx<> g_unk_7172745bc0;
enum class EThermalFreezeRoomType : u32;
template <> UnkReflType base::reflection::TypeObject<EThermalFreezeRoomType, UnkReflType>::object asm("g_unk_7172745c00");
extern char g_unk_7172745d18;
extern UnkConverter g_unk_7172745d20;
extern UnkConverter g_unk_7172745d40;
extern UnkReflType g_unk_7172745d60;  // CFreezeRoomConfig
extern char g_unk_7172745e98;
extern UnkReflType g_unk_7172745ea0;  // CFreezeRoomCoolConfig
extern char g_unk_7172745fd8;
extern char g_unk_7172745fd9;

void fn_7100a21a4c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_7100a21900();
void fn_7100a2439c() __attribute__((visibility("hidden")));
void fn_7100a243a8() __attribute__((visibility("hidden")));
void fn_7100a21fc8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100a21bcc();
void fn_7100a2253c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100a2214c();

void CFreezeRoomComponent_InitStatics() {
    UnkFreezeRoomTU_la24610& tu = g_unk_7172745fe0;
    InitHeader(tu.header);
    tu.field_0x30 = -1.0f;
    tu.field_0x34 = -1.0f;
    tu.field_0x38 = true;
    InitPropertyEx(&g_unk_7172745bc0, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101a45f90, fn_71002ef5bc,
                   "FREEZE_ROOMS", &g_unk_7101d5c998, fn_71004fd2cc, fn_71004fa3b8);
    InitEnum(&base::reflection::TypeObject<EThermalFreezeRoomType, UnkReflType>::object, "EThermalFreezeRoomType", fn_7100a21a4c, fn_7100a21900);
    fn_710009ec00(&g_unk_7172745d18, &base::reflection::TypeObject<EThermalFreezeRoomType, UnkReflType>::object);
    InitEnumConverters(&base::reflection::TypeObject<EThermalFreezeRoomType, UnkReflType>::object, g_unk_7172745d20, fn_7100a2439c, g_unk_7172745d40, fn_7100a243a8);
    InitClass(&g_unk_7172745d60, "CFreezeRoomConfig", fn_7100a21fc8, fn_7100a21bcc);
    fn_710009ec00(&g_unk_7172745e98, &g_unk_7172745d60);
    InitClass(&g_unk_7172745ea0, "CFreezeRoomCoolConfig", fn_7100a2253c, fn_7100a2214c);
    fn_710009ec00(&g_unk_7172745fd8, &g_unk_7172745ea0);
    fn_7100a226c4();
    UnkFreezeRoomClassTU_la24610& cls = g_unk_7172746000;
    fn_710009ec00(&g_unk_7172745fd9, cls.type);
    {
        CStrId name("CFreezeRoomComponent", true);
        new (&cls.id) CStrId(fn_7100de8f78(name));
    }
    fn_7100000250(CStrId_Destroy, &cls.id, &g_unk_7101a19000);
}
