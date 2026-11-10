// GUI::CFloatTunableButton's unit (tools/tu_map.py, unit 1676). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CRntString;

// As src/game/GUI/CBoolTunableButton/CBoolTunableButton_Toggle_ovb0119.cpp declares them.
struct UnkGuiObj;
void fn_71011ec424(UnkGuiObj* label, const CRntString& text);
void fn_71011cd5a0(UnkGuiObj*);
void fn_7100000790(const char* name, const char* fmt, ...);
struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;
bool fn_71000e7a80(UnkReflField* field, const UnkReflFieldObject* obj, UnkReflValueArg* value);  // sets
// As src/game/behaviortree/CFloatCondition/CFloatCondition_Eval_ovb0121.cpp declares it.
void fn_710007f5e0(void* self, f32 value);  // appends a number

struct UnkFloatTunableObject_ovb0130 {
    virtual const void* vf_0x0();
};

struct UnkFloatTunableLabel_ovb0130 {
    void** vtable;
    void vf_0x88() { reinterpret_cast<void (*)(UnkFloatTunableLabel_ovb0130*)>(vtable[0x88 / 8])(this); }
};

// The field call's arguments: a type and where its value is.
struct UnkTypedPtr_ovb0130 {
    const void* type;
    void* value;
};

// The script name: a pooled string whose use count is at +0x44 and text at +0x10.
struct UnkScriptName_ovb0130 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 use_count;  // 0x44
    };
};

struct UnkFloatTunableButton_ovb0130 {
    u8 pad_0x0[0x2a8];
    UnkGuiObj* label;  // 0x2a8
    f32 value;         // 0x2b0
    f32 step;          // 0x2b4
    f32 min;           // 0x2b8
    f32 max;           // 0x2bc
    UnkFloatTunableObject_ovb0130* object;  // 0x2c0
    UnkReflField* field;                    // 0x2c8
    UnkScriptName_ovb0130* script;          // 0x2d0
};

// fn_7100b1f80c: steps the button's value down (0) or up (1) within its range, then shows it on the label,
// writes it to the bound field and calls the script function with it, as CBoolTunableButton does.
void fn_7100b1f80c(UnkFloatTunableButton_ovb0130* self, s32 direction) {
    switch (direction) {
    case 1: {
        f32 v = self->step + self->value;
        self->value = v;
        if (v > self->max)
            self->value = self->max;
        break;
    }
    case 0: {
        f32 step = self->step;
        f32 v = self->value - step;
        self->value = v;
        if (v < self->min)
            self->value = self->min;
        break;
    }
    }
    CRntString text(g_unk_710162e2ed);
    fn_710007f5e0(&text, self->value);
    fn_71011ec424(self->label, text);
    reinterpret_cast<UnkFloatTunableLabel_ovb0130*>(self->label)->vf_0x88();
    fn_71011cd5a0(self->label);
    if (UnkFloatTunableObject_ovb0130* object = self->object) {
        if (self->field) {
            UnkTypedPtr_ovb0130 obj = {object->vf_0x0(), object};
            UnkReflField* field = self->field;
            UnkTypedPtr_ovb0130 value = {fn_710000abbc(), &self->value};
            fn_71000e7a80(field, reinterpret_cast<const UnkReflFieldObject*>(&obj),
                          reinterpret_cast<UnkReflValueArg*>(&value));
        }
    }
    UnkScriptName_ovb0130* script = self->script;
    if (script != reinterpret_cast<UnkScriptName_ovb0130*>(&g_unk_7101d47108->empty)) {
        ++script->use_count;
        const char* name = self->script->text;
        fn_7100000790(name ? name : g_unk_710162e2ed, "f", (f64)self->value);
    }
}
