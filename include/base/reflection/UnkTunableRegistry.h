#pragma once

// The hand-written sources' older name for base::tunable::CTunableManager (base/tunable/CTunableManager.h, which
// declares its global pointer and its constructor): base::reflection::UnkTunableRegistry is that class. A source
// that is next worked on names the class.

#include "base/reflection/ReflectedAttributes.h"
#include "base/tunable/CTunableManager.h"

namespace base::reflection {
using UnkTunableRegistry = base::tunable::CTunableManager;
}  // namespace base::reflection
