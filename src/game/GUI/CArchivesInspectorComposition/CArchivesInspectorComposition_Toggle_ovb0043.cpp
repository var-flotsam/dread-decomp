// GUI::CArchivesInspectorComposition's unit (tools/tu_map.py): the show/hide toggle (fn_7100af25dc) and the
// destructor (fn_7100af21a8). Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkArchivesInput_ovb0043 {
    u8 pad_0x0[0x1a];
    bool handled;  // 0x1a
};

struct UnkArchivesInspector_ovb0043 {
    struct VTable {
        void* slots[61];
        void (*PlayAnimation)(UnkArchivesInspector_ovb0043* self, const char* name, bool flag);  // 0x1e8
    };
    VTable* vtable;
    u8 pad_0x8[0xac];
    bool enabled;  // 0xb4
    u8 pad_0xb5[0x253];
    bool shown;  // 0x308
};

// Toggles the composition between shown and hidden ("UI_Show"/"UI_Hide") unless the input was
// already handled; marks the input handled.
void fn_7100af25dc(UnkArchivesInspector_ovb0043* self, UnkArchivesInput_ovb0043* input) {
    if (input && input->handled)
        return;
    if (!self->enabled)
        return;
    self->shown = !self->shown;
    self->vtable->PlayAnimation(self, self->shown ? "UI_Show" : "UI_Hide", false);
    input->handled = true;
}

extern void* g_unk_7101a97e00[];

// A callback registered with a deferred set: unregistered when destroyed.
struct UnkArchivesCallback_ovb0043 {
    base::UnkDeferredSet<void>* set;
    void* object;
    void (*fn)();

    ~UnkArchivesCallback_ovb0043() {
        if (set) {
            fn_71004f3ea4(set, &set);
            set = nullptr;
        }
        fn = nullptr;
        object = nullptr;
    }
};

struct UnkArchivesLinked_ovb0043 {
    void* back;
};

struct UnkArchivesInspectorFull_ovb0043 {
    void** vtable;
    u8 pad_0x8[0x228];
    UnkArchivesCallback_ovb0043 first;  // 0x230
    UnkArchivesCallback_ovb0043 second;  // 0x248
    base::UnkVector<UnkArchivesLinked_ovb0043*> linked;  // 0x260
    base::UnkVector<u64> field_0x288;
    base::UnkVector<u64> field_0x2b0;
};

// The destructor: the vtable; every linked object (+0x260) loses its back pointer; the three arrays are
// emptied and freed in reverse order and both callbacks unregistered; then the container's destructor
// (fn_71011d8e90).
void fn_7100af21a8(UnkArchivesInspectorFull_ovb0043* self) {
    self->vtable = &g_unk_7101a97e00[2];
    for (UnkArchivesLinked_ovb0043 **it = self->linked.data, **end = self->linked.data + self->linked.count; it != end;
         ++it)
        (*it)->back = nullptr;
    self->linked.count = 0;
    self->field_0x288.count = 0;
    self->field_0x2b0.count = 0;
    self->field_0x2b0.Free();
    self->field_0x288.Free();
    self->linked.Free();
    self->second.~UnkArchivesCallback_ovb0043();
    self->first.~UnkArchivesCallback_ovb0043();
    fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(self));
}
