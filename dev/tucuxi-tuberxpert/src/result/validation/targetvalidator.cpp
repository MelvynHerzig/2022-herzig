#include "targetvalidator.h"

#include <memory>
#include <map>
#include <vector>

#include "tucucore/drugtreatment/target.h"
#include "tucucore/drugmodel/targetdefinition.h"
#include "tucucore/drugdefinitions.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

TargetValidator::TargetValidator()
{}

void TargetValidator::getTargetValidations(XpertRequestResult& _xpertRequestResult) const
{

    // This map keeps tracks of active moiety seen and for each of them save the related target type.
    // This allows us to check, when iterating over the target, if a similar target has already
    // been seen.
    map<Core::ActiveMoietyId, vector<Core::TargetType>> seenActiveMoietyToTargetType;

    // For each patient's target, check that two don't overlap and that the active moiety is the drug model.
    for (const unique_ptr<Core::Target>& target : _xpertRequestResult.getTreatment()->getTargets()) {

        // Checks if already existing in map of previous targets.

        // Adding the target to the map.

        // Checking if belonging to the drug model.
    }
}

} // namespace XpertResult
} // namespace Tucuxi
