// CCharClassShowRandomNodeEvent unit: hands the event to the actor's component at +0x280
// (fn_710115635c) when its id at +0x10 is set, else to the one at +0x228 (fn_7101173a04).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"
struct UnkShowNodeEvent {
    u8 pad_0x0[0x10];
    base::global::CStrId id;  // 0x10
};
struct UnkShowNodeComponentA;
struct UnkShowNodeComponentB;
void fn_710115635c(UnkShowNodeComponentA* component, UnkShowNodeEvent* event);
void fn_7101173a04(UnkShowNodeComponentB* component, UnkShowNodeEvent* event);
struct UnkShowNodeActor {
    u8 pad_0x0[0x228];
    UnkShowNodeComponentB* field_0x228;
    u8 pad_0x230[0x50];
    UnkShowNodeComponentA* field_0x280;
};
void fn_7100cdb7bc(UnkShowNodeEvent* self, UnkShowNodeActor** actor_ref) {
    UnkShowNodeActor* actor = *actor_ref;
    if (!actor)
        return;
    if (self->id.data != &g_unk_7101d47108->empty) {
        if (UnkShowNodeComponentA* component = actor->field_0x280)
            fn_710115635c(component, self);
    } else {
        if (UnkShowNodeComponentB* component = actor->field_0x228)
            fn_7101173a04(component, self);
    }
}
