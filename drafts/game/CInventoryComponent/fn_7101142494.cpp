// CInventoryComponent's unit (tools/tu_map.py unit 2710): the property handler. Two properties read an item
// name from the stream and write back what the component's map holds for it: whether the amount is above
// zero (flag) or the amount itself (float). Other properties go to the base handler (fn_71010c4250).
// Names and layouts are placeholders.
#include "../../../src/game_s7/UnkPropS7.h"
#include "base/global/CRntDictionary.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "game/UnkComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkStrIdHandle.h"
#include "types.h"

using namespace unk_prop;

void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

// The key at `offset` in the id table.
inline const CStrId& PropKey(u32 offset) {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<const u8*>(&s_table) + offset);
}

struct UnkInventoryProps_l142494 {
    u8 pad_0x0[0x40];
    base::global::CRntDictionary<CStrId, f32*> amounts;  // 0x40
};

// A handle built in place from an id by fn_7100ae986c, released when it goes out of scope.
class UnkItemHandle_l142494 {
public:
    explicit UnkItemHandle_l142494(const CStrId& id) { fn_7100ae986c(get(), id); }
    ~UnkItemHandle_l142494() { get()->id.~CStrId(); }
    UnkStrIdHandle* get() { return reinterpret_cast<UnkStrIdHandle*>(storage); }

private:
    alignas(UnkStrIdHandle) char storage[sizeof(UnkStrIdHandle)];
};

// The stream's item name, answered with the amount the map holds for it (null when it has none).
static inline const f32* AmountOf(UnkInventoryProps_l142494* self, UnkProp* p, UnkItemHandle_l142494& handle) {
    auto* node = self->amounts.FindNode(handle.get()->id);
    return node ? node->value : nullptr;
}

u32 fn_7101142494(UnkInventoryProps_l142494* self, UnkComponentProp* prop) {
    UnkProp* p = reinterpret_cast<UnkProp*>(prop);
    if (Is(p, PropKey(0x1338)) && !p->set) {
        char name[0x80];
        CRntFile_ReadFormat(p->stream, "s", name);
        CStrId id(name, true);
        UnkItemHandle_l142494 handle(id);
        const f32* amount = AmountOf(self, p, handle);
        bool present = amount && *amount > 0.0f;
        CRntFile_Seek(reinterpret_cast<base::global::CRntFile*>(p->stream), 0, 0);
        CRntFile_WriteFormat(p->stream, "b", present);
        return true;
    }
    if (Is(p, PropKey(0x11a0)) && !p->set) {
        char name[0x80];
        CRntFile_ReadFormat(p->stream, "s", name);
        CStrId id(name, true);
        UnkItemHandle_l142494 handle(id);
        const f32* amount = AmountOf(self, p, handle);
        f64 value = 0.0;
        if (amount)
            value = *amount;
        CRntFile_Seek(reinterpret_cast<base::global::CRntFile*>(p->stream), 0, 0);
        CRntFile_WriteFormat(p->stream, "f", value);
        return true;
    }
    return fn_71010c4250(reinterpret_cast<UnkBaseComponent*>(self), prop);
}
