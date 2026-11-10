// GUI::CIntTunableButton's unit (tools/tu_map.py unit 1682): fn_7100b32404 steps the button's value
// (+0x2b0) down (0) or up (1) by its step (+0x2c0), clamped to its minimum (+0x2c4) or maximum
// (+0x2c8). The label (+0x2a8) shows the value's entry from the button's text table (+0x2b8), or the
// number itself when there is none, and is refreshed; a bound reflected object (+0x2d0) and field
// (+0x2d8) get the new value (fn_71000e7a80, like CBoolTunableButton's toggle), and a script name
// (+0x2e0) is called with it (fn_7100000790, "i"). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CRntString;

struct UnkGuiObj;
void fn_71011ec424(UnkGuiObj* label, const CRntString& text);
void fn_71011ecfbc(UnkGuiObj* label, const char32_t* text);
void fn_71011cd5a0(UnkGuiObj*);
void fn_7100000790(const char* name, const char* fmt, ...);
void fn_710007f634(void* self, u32 value);  // appends a number

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;
bool fn_71000e7a80(UnkReflField* field, const UnkReflFieldObject* obj, UnkReflValueArg* value);  // sets

struct UnkIntTunableObject_ovb0134 {
    virtual const void* vf_0x0();
};

struct UnkTypedPtr_ovb0134 {
    const void* type;
    void* value;
};

struct UnkScriptName_ovb0134 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 use_count;  // 0x44
    };
};

struct UnkTextEntry_ovb0134 {
    u8 data[0x10];
};

struct UnkTextTable_ovb0134 {
    u8 pad_0x0[0xf0];
    UnkTextEntry_ovb0134* entries;  // 0xf0
};

struct UnkIntTunableButton_ovb0134 {
    u8 pad_0x0[0x2a8];
    UnkGuiObj* label;                     // 0x2a8
    s32 value;                            // 0x2b0
    UnkTextTable_ovb0134* texts;          // 0x2b8
    s32 step;                             // 0x2c0
    s32 min;                              // 0x2c4
    s32 max;                              // 0x2c8
    UnkIntTunableObject_ovb0134* object;  // 0x2d0
    UnkReflField* field;                  // 0x2d8
    UnkScriptName_ovb0134* script;        // 0x2e0
};

void fn_7100b32404(UnkIntTunableButton_ovb0134* self, s32 direction) {
    switch (direction) {
    case 1: {
        s32 step = self->step;
        s32 v = self->value + step;
        self->value = v > self->max ? self->max : v;
        break;
    }
    case 0: {
        s32 step = self->step;
        s32 v = self->value - step;
        self->value = v < self->min ? self->min : v;
        break;
    }
    }
    if (UnkTextTable_ovb0134* texts = self->texts) {
        u32 index = self->value;
        fn_71011ecfbc(self->label, reinterpret_cast<const char32_t*>(&texts->entries[index]));
    } else {
        CRntString text("");
        fn_710007f634(&text, self->value);
        fn_71011ec424(self->label, text);
    }
    fn_71011cd5a0(self->label);
    if (UnkIntTunableObject_ovb0134* object = self->object) {
        if (self->field) {
            UnkTypedPtr_ovb0134 obj = {object->vf_0x0(), object};
            UnkReflField* field = self->field;
            s32* p = &self->value;
            UnkTypedPtr_ovb0134 value = {fn_7100014768(), p};
            fn_71000e7a80(field, reinterpret_cast<const UnkReflFieldObject*>(&obj),
                          reinterpret_cast<UnkReflValueArg*>(&value));
        }
    }
    UnkScriptName_ovb0134* script = self->script;
    if (script != reinterpret_cast<UnkScriptName_ovb0134*>(&g_unk_7101d47108->empty)) {
        ++script->use_count;
        const char* name = self->script->text;
        fn_7100000790(name ? name : g_unk_710162e2ed, "i", self->value);
    }
}
