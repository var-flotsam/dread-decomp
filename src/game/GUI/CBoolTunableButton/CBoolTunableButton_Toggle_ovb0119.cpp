// GUI::CBoolTunableButton's unit (tools/tu_map.py unit 1652): fn_7100af3d60 toggles the button's
// value (+0x2b0): the label (+0x2a8) shows the new "true"/"false" text and is refreshed; when the
// button is bound to a reflected object (+0x2b8) and field (+0x2c0), the field is set to the new
// value (fn_71000e7a80, as src/base/UnkTunableRegistry.cpp's SetField); when it has a script name
// (+0x2c8), that script function is called with the value (fn_7100000790, "b"). Names and layouts
// are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CRntString;

struct UnkGuiObj;
void fn_71011ec424(UnkGuiObj* label, const CRntString& text);
void fn_71011cd5a0(UnkGuiObj*);
void fn_7100000790(const char* name, const char* fmt, ...);

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;
bool fn_71000e7a80(UnkReflField* field, const UnkReflFieldObject* obj, UnkReflValueArg* value);  // sets

struct UnkBoolTunableObject_b0119 {
    virtual const void* vf_0x0();
};

// The field call's arguments: a type and where its value is.
struct UnkTypedPtr_b0119 {
    const void* type;
    void* value;
};

// The script name: a pooled string whose use count is at +0x44 and text at +0x10.
struct UnkScriptName_b0119 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 use_count;  // 0x44
    };
};

struct UnkBoolTunableButton_b0119 {
    u8 pad_0x0[0x2a8];
    UnkGuiObj* label;                 // 0x2a8
    bool value;                       // 0x2b0
    UnkBoolTunableObject_b0119* object;  // 0x2b8
    UnkReflField* field;              // 0x2c0
    UnkScriptName_b0119* script;      // 0x2c8
};

void fn_7100af3d60(UnkBoolTunableButton_b0119* self) {
    UnkGuiObj* label = self->label;
    self->value = !self->value;
    if (self->value)
        fn_71011ec424(label, CRntString("true"));
    else
        fn_71011ec424(label, CRntString("false"));
    fn_71011cd5a0(self->label);
    if (UnkBoolTunableObject_b0119* object = self->object) {
        if (self->field) {
            UnkTypedPtr_b0119 obj = {object->vf_0x0(), object};
            UnkReflField* field = self->field;
            UnkTypedPtr_b0119 value = {fn_710000d228(), &self->value};
            fn_71000e7a80(field, reinterpret_cast<const UnkReflFieldObject*>(&obj),
                          reinterpret_cast<UnkReflValueArg*>(&value));
        }
    }
    UnkScriptName_b0119* script = self->script;
    if (script != reinterpret_cast<UnkScriptName_b0119*>(&g_unk_7101d47108->empty)) {
        ++script->use_count;
        const char* name = self->script->text;
        fn_7100000790(name ? name : g_unk_710162e2ed, "b", self->value);
    }
}
