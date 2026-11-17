// GUI::CArchivesInspectorComposition's unit (tools/tu_map.py unit 1651): fn_7100af232c binds four input
// events (0x12, 0x17, 0x14, 0x13) of the composition to its handlers and resets the timer at +0x30c.
// Names and layouts are placeholders.
#include <functional>

#include "types.h"

struct UnkArchivesInput_ovb0043;

struct UnkArchivesInspector_laf232c {
    u8 pad_0x0[0x30c];
    f32 timer;  // 0x30c

    void fn_7100af2508(UnkArchivesInput_ovb0043* input);
    void fn_7100af25dc(UnkArchivesInput_ovb0043* input);
    void fn_7100af2648(UnkArchivesInput_ovb0043* input);
    void fn_7100af26fc(UnkArchivesInput_ovb0043* input);
};

void fn_7100af30a0(UnkArchivesInspector_laf232c* composition, const s32* id,
                   const std::function<void(UnkArchivesInput_ovb0043*)>& f, void* unk, u32 flags);

void fn_7100af232c(UnkArchivesInspector_laf232c* self) {
    using std::placeholders::_1;
    s32 id = 0x12;
    if (self)
        fn_7100af30a0(self, &id, std::bind(&UnkArchivesInspector_laf232c::fn_7100af2508, self, _1), nullptr, 2);
    id = 0x17;
    if (self)
        fn_7100af30a0(self, &id, std::bind(&UnkArchivesInspector_laf232c::fn_7100af25dc, self, _1), nullptr, 2);
    id = 0x14;
    if (self)
        fn_7100af30a0(self, &id, std::bind(&UnkArchivesInspector_laf232c::fn_7100af2648, self, _1), nullptr, 2);
    id = 0x13;
    if (self)
        fn_7100af30a0(self, &id, std::bind(&UnkArchivesInspector_laf232c::fn_7100af26fc, self, _1), nullptr, 2);
    self->timer = -1.0f;
}
