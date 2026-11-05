// CPlaythroughDef's unit (tools/tu_map.py unit 425): the invoker of "_strTags_GetterFunction"
// (src/reflection/CPlaythroughDef.cpp): builds the tags string of the object at +8 (fn_71002f2478, a string with
// 0x80 bytes of inline storage returned in memory) and returns it as a reflected value of fn_710009fb38's type.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::CType;
using base::reflection::UnkReflValue;

// A string with 0x80 bytes of inline storage (0xc8 bytes), as in
// src/game/behaviortree/CStringEqualsCondition/CStringEqualsCondition_Check_A2_b72.cpp.
struct UnkInlineString80_A2_b74 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x80];
    u64 field_0xb8;
    u32 field_0xc0;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    ~UnkInlineString80_A2_b74() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

struct UnkPlaythroughDef_A2_b74;
UnkInlineString80_A2_b74 fn_71002f2478(UnkPlaythroughDef_A2_b74* self);
struct UnkEnumType;
UnkEnumType* fn_710009fb38();  // as CGameManager.cpp declares it

UnkReflValue fn_71002f4f44(void*, char* obj) {
    UnkInlineString80_A2_b74 tags = fn_71002f2478(*reinterpret_cast<UnkPlaythroughDef_A2_b74**>(obj + 8));
    const CType* type = reinterpret_cast<const CType*>(fn_710009fb38());
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &tags, false);
    return r;
}
