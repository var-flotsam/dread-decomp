// Describes an object's settings in a property bag and hands the bag to a manager-owned
// receiver. Layouts are placeholders.
#include "UnkPropBagS7.h"
#include "game/UnkGameManager.h"

using namespace unk_prop_bag;

struct UnkHandle_7100bda920 {
    void* ptr;
};

struct UnkObj_7100bda920 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void* vf_0x20(UnkHandle_7100bda920* handle);
    u8 unk_0x8[0x10];
    bool field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    UnkName* field_0x28;
    bool field_0x30;
    UnkName* field_0x38;
    UnkName* field_0x40;
    bool field_0x48;
    bool field_0x49;
    bool field_0x4a;
    bool field_0x4b;
    u32 field_0x4c;
    f32 field_0x50;
    bool field_0x54;
    bool field_0x55;
    f32 field_0x58;
    bool field_0x5c;
    UnkName* field_0x60;
    bool field_0x68;
    bool field_0x69;
    bool field_0x6a;
    bool field_0x6b;
};

struct UnkReceiver;
void fn_71010e560c(UnkReceiver* receiver, UnkPropBag* bag, void* target, void* info, const UnkPair& a,
                   const UnkPair& b);

void fn_7100bda920(UnkObj_7100bda920* self, UnkHandle_7100bda920* handle) {
    if (!handle->ptr)
        return;
    UnkReceiver* receiver = static_cast<UnkReceiver*>(fn_7100defcec(g_unk_7101d47310));
    if (!receiver)
        return;
    UnkPropBag bag;
    fn_710033187c(&bag, Key(0xeb0), self->field_0x58);
    fn_710050f4ac(&bag, Key(0xf58), self->field_0x30);
    fn_710033187c(&bag, Key(0xf60), self->field_0x20);
    fn_710050e4dc(&bag, Key(0xf70), UseName(self->field_0x28));
    fn_710050f4ac(&bag, Key(0xf78), self->field_0x54);
    fn_710050f4ac(&bag, Key(0x1370), self->field_0x48);
    fn_7100aa3b9c(&bag, Key(0x1448), self->field_0x4c);
    fn_710033187c(&bag, Key(0x1488), self->field_0x50);
    fn_710033187c(&bag, Key(0x1490), self->field_0x1c);
    fn_710050f4ac(&bag, Key(0x14e8), self->field_0x4b);
    fn_710050f4ac(&bag, Key(0x14f0), self->field_0x4a);
    fn_710050f4ac(&bag, Key(0x1d40), !self->field_0x49 || self->field_0x18);
    fn_710050f4ac(&bag, Key(0x1dd0), self->field_0x5c);
    fn_710050f4ac(&bag, Key(0x1de0), self->field_0x55);
    fn_710050e4dc(&bag, Key(0x21d0), UseName(self->field_0x38));
    fn_710050e4dc(&bag, Key(0xf10), UseName(self->field_0x40));
    fn_710050f4ac(&bag, Key(0x2458), self->field_0x68);
    fn_710050f4ac(&bag, Key(0x1680), self->field_0x69);
    fn_710050f4ac(&bag, Key(0x510), self->field_0x6a);
    fn_710050f4ac(&bag, Key(0x508), self->field_0x6b);
    fn_710050e4dc(&bag, Key(0x7d8), UseName(self->field_0x60));
    fn_71010e560c(receiver, &bag, handle->ptr, self->vf_0x20(handle), UnkPair(), UnkPair());
}
