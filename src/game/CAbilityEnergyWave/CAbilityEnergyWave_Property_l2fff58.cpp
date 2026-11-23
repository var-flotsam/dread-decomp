// CAbilityEnergyWave unit (tools/tu_map.py): a property handler. For its one key it reads a name
// from the property's stream, looks up the entry of that name (fn_71003000cc) and passes the
// property on to it; other keys go to the base handler. Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "game/UnkComponent.h"
#include "types.h"

using namespace unk_prop;

class UnkEnergyWaveEntry_l2fff58 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual u32 vf_0x80(UnkProp* p);  // the entry's own property handler
};

struct UnkEnergyWave_l2fff58;

UnkEnergyWaveEntry_l2fff58* fn_71003000cc(UnkEnergyWave_l2fff58* self, const CStrId* name);

u32 fn_71002fff58(UnkEnergyWave_l2fff58* self, UnkProp* p) {
    if (Is(p, Key(0x678))) {
        const char* text;
        CRntFile_ReadCString(p->stream, &text);
        UnkEnergyWaveEntry_l2fff58* entry;
        {
            CStrId name(text, true);
            entry = fn_71003000cc(self, &name);
        }
        if (!entry)
            return 0;
        return entry->vf_0x80(p);
    }
    return fn_71010c4250(reinterpret_cast<UnkBaseComponent*>(self), reinterpret_cast<UnkComponentProp*>(p));
}
