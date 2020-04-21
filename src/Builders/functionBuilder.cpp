#include "Builders/functionBuilder.hpp"

namespace Builders {

/**
* Transforms a IR::Function to a PybindProxy::Function
*/
PybindProxy::Function buildFunction(IR::Function const& fun);
}    // namespace Builders
