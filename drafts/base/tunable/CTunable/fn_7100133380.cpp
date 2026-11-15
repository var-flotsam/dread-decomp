// base::tunable::CTunable's unit: finds the entry whose name is `id` in an array of 0x160-byte entries.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkTunableEntry {
    CStrId name;
    u8 pad_0x8[0x158];
};

struct UnkTunableList {
    UnkTunableEntry* data;
    u32 count;

    UnkTunableEntry* begin() { return data; }
    UnkTunableEntry* end() { return data + count; }

    // The first entry is tested before the loop, as the original does.
    UnkTunableEntry* FindByName(CStrId key) {
        UnkTunableEntry* it = begin();
        UnkTunableEntry* end = this->end();
        if (it != end) {
            if (it->name.data == key.data)
                return it;
            for (++it; it != end; ++it)
                if (it->name.data == key.data)
                    return it;
        }
        return end;
    }
};

UnkTunableEntry* fn_7100133380(UnkTunableList* self, const CStrId* id) {
    UnkTunableEntry* it = self->FindByName(*id);
    return it == self->end() ? nullptr : it;
}
