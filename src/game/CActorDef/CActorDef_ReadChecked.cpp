// From unit 345 (CActorDef): a read step that first takes a string from the stream
// (fn_710007cee4, the string unused), then asks the object's slot 0x38 whether to go on and, if
// so, returns its slot 0x40. Names are placeholders.
#include "game/UnkTU56.h"
#include "types.h"

struct UnkActorDefReader {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual bool vf_0x38(void* stream);
    virtual bool vf_0x40(void* stream);
};

bool fn_71002561fc(UnkActorDefReader* self, void* stream) {
    const char* name;
    fn_710007cee4(stream, &name);
    return self->vf_0x38(stream) && self->vf_0x40(stream);
}
