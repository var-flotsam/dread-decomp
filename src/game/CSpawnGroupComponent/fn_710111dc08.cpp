// Reads or writes one of an object's named settings (mostly strings and flags). Unknown keys go
// to the base handler (fn_71010c4250). Layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "game/UnkComponent.h"

using namespace unk_prop;
using base::global::CRntString;

struct UnkObj_710111dc08 {
    u8 unk_0x0[0x13a];
    bool bIsGenerator;
    bool bIsInfinite;
    bool bAutomanaged;
    u8 unk_0x13d[0x1];
    bool bAutoenabled;
    u8 unk_0x13f[0x9];
    CRntString field_0x148;
    u8 unk_0x178[0x8];
    s32 field_0x180;
    u8 unk_0x184[0x8];
    s32 iMaxToGenerate;
    s32 iMaxSimultaneous;
    s32 field_0x194;
    u8 unk_0x198[0x30];
    CRntString sOnEnable;
    CRntString sOnDisable;
    CRntString sOnEntityGenerated;
    CRntString sOnBeforeGenerateEntity;
    CRntString sOnMaxSimultaneous;
    CRntString sOnMaxGenerated;
    CRntString sOnEntityDead;
    CRntString sOnEntityDamaged;
    CRntString sOnAllEntitiesDead;
};


u32 CSpawnGroupComponent_ReadOrWriteProperty(UnkObj_710111dc08* self, UnkProp* p) {
    if (Is(p, Key(0xef0)) && p->set)
        return true;
    if (Is(p, Key(0xdf8)) && p->set)
        return true;
    if (Is(p, Key(0x1dd8)) && p->set)
        return true;
    if (Is(p, Key(0x1a88)) && p->set) {
        char buf[0x80];
        CRntFile_ReadFormat(p->stream, "s", buf);
        CRntString_AssignCString(&self->sOnBeforeGenerateEntity, buf);
        return true;
    }
    if (Is(p, Key(0x1ad8)) && p->set) {
        char buf[0x80];
        CRntFile_ReadFormat(p->stream, "s", buf);
        CRntString_AssignCString(&self->sOnEntityGenerated, buf);
        return true;
    }
    if (Is(p, Key(0x1aa8)) && p->set) {
        char buf[0x80];
        CRntFile_ReadFormat(p->stream, "s", buf);
        CRntString_AssignCString(&self->sOnEnable, buf);
        return true;
    }
    if (Is(p, Key(0x1a98)) && p->set) {
        char buf[0x80];
        CRntFile_ReadFormat(p->stream, "s", buf);
        CRntString_AssignCString(&self->sOnDisable, buf);
        return true;
    }
    if (Is(p, Key(0x1af8)) && p->set) {
        char buf[0x80];
        CRntFile_ReadFormat(p->stream, "s", buf);
        CRntString_AssignCString(&self->sOnMaxSimultaneous, buf);
        return true;
    }
    if (Is(p, Key(0x1af0)) && p->set) {
        char buf[0x80];
        CRntFile_ReadFormat(p->stream, "s", buf);
        CRntString_AssignCString(&self->sOnMaxGenerated, buf);
        return true;
    }
    if (Is(p, Key(0x1ad0)) && p->set) {
        char buf[0x80];
        CRntFile_ReadFormat(p->stream, "s", buf);
        CRntString_AssignCString(&self->sOnEntityDead, buf);
        return true;
    }
    if (Is(p, Key(0x1ac8)) && p->set) {
        char buf[0x80];
        CRntFile_ReadFormat(p->stream, "s", buf);
        CRntString_AssignCString(&self->sOnEntityDamaged, buf);
        return true;
    }
    if (Is(p, Key(0x1a68)) && p->set) {
        char buf[0x80];
        CRntFile_ReadFormat(p->stream, "s", buf);
        CRntString_AssignCString(&self->sOnAllEntitiesDead, buf);
        return true;
    }
    if (!p->set && Is(p, Key(0x1500)))
        return CRntFile_WriteFormat(p->stream, "b", self->bAutoenabled) != 0;
    if (!p->set && Is(p, Key(0x1508)))
        return CRntFile_WriteFormat(p->stream, "b", self->bAutomanaged) != 0;
    if (!p->set && Is(p, Key(0x1518)))
        return CRntFile_WriteFormat(p->stream, "b", self->bIsGenerator) != 0;
    if (p->set && Is(p, Key(0x1518)))
        return CRntFile_ReadFormat(p->stream, "b", &self->bIsGenerator) != 0;
    if (p->set && Is(p, Key(0x1520)))
        return CRntFile_ReadFormat(p->stream, "b", &self->bIsInfinite) != 0;
    if (!p->set && Is(p, Key(0x1758)))
        return CRntFile_WriteFormat(p->stream, "i", self->iMaxToGenerate) != 0;
    if (p->set && Is(p, Key(0x1758)))
        return CRntFile_ReadFormat(p->stream, "i", &self->iMaxToGenerate) != 0;
    if (!p->set && Is(p, Key(0x19c0)))
        return CRntFile_WriteFormat(p->stream, "i", self->field_0x194) != 0;
    if (p->set && Is(p, Key(0x19c0)))
        return CRntFile_ReadFormat(p->stream, "i", &self->field_0x194) != 0;
    if (p->set && Is(p, Key(0x1738)))
        return CRntFile_ReadFormat(p->stream, "i", &self->iMaxSimultaneous) != 0;
    if (p->set && Is(p, Key(0x1180)))
        return CRntFile_ReadFormat(p->stream, "i", &self->field_0x180) != 0;
    if (Is(p, Key(0x1f88))) {
        const char* value;
        const char* unused;
        CRntFile_ReadCString(p->stream, &value);
        CRntFile_ReadCString(p->stream, &unused);
        CRntString_AssignCString(&self->field_0x148, value);
        return true;
    }
    // The base handler takes the component and the property record: the object's first 0x40
    // bytes are the component, UnkProp is the record.
    return fn_71010c4250(reinterpret_cast<UnkBaseComponent*>(self), reinterpret_cast<UnkComponentProp*>(p));
}
