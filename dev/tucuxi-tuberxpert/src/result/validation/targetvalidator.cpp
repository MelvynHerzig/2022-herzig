#include "targetvalidator.h"

#include <memory>
#include <map>
#include <vector>
#include <algorithm>

#include "tucucore/drugtreatment/target.h"
#include "tucucore/drugmodel/targetdefinition.h"
#include "tucucore/drugdefinitions.h"
#include "tucucore/drugmodel/activemoiety.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

TargetValidator::TargetValidator()
{}

void TargetValidator::getTargetValidations(XpertRequestResult& _xpertRequestResult) const
{

    // Checks treatment
    if (_xpertRequestResult.getTreatment() == nullptr) {
        _xpertRequestResult.setErrorMessage("No treatment set.");
        return;
    }

    // Checks drug model
    if (_xpertRequestResult.getDrugModel() == nullptr) {
        _xpertRequestResult.setErrorMessage("No drug model set.");
        return;
    }

    // This map keeps tracks of active moiety seen and for each of them save the related target type.
    // This allows us to check, when iterating over the target, if a similar target has already
    // been seen.
    map<Core::ActiveMoietyId, vector<Core::TargetType>> seenActiveMoietyToTargetType;

    // For each patient's target, check that two don't overlap and that the active moiety is the drug model.
    for (const unique_ptr<Core::Target>& target : _xpertRequestResult.getTreatment()->getTargets()) {

        // Checks if already existing in map of previous targets.
        auto itMapEntry = seenActiveMoietyToTargetType.find(target->getActiveMoietyId());

        // If another target with same active has already been seen
        // and if the target type has already been seen.
        if (itMapEntry != seenActiveMoietyToTargetType.end() &&
                count(itMapEntry->second.begin(), itMapEntry->second.end(), target->getTargetType()) > 0){
            _xpertRequestResult.setErrorMessage("Two patient's targets with the same active moiety and the same target type detected.");
            return;
        }

        // Adding the target to the map.
        if (itMapEntry != seenActiveMoietyToTargetType.end()) {
            itMapEntry->second.emplace_back(target->getTargetType());
        } else {
            seenActiveMoietyToTargetType.emplace(make_pair(target->getActiveMoietyId(), vector<Core::TargetType>{target->getTargetType()}));
        }

        // Checking if belonging to the drug model.
        const Core::ActiveMoieties& activeMoieties = _xpertRequestResult.getDrugModel()->getActiveMoieties();

        auto itActiveMoieties = find_if(activeMoieties.begin(), activeMoieties.end(), [&target](const unique_ptr<Core::ActiveMoiety>& am){
            return am->getActiveMoietyId() == target->getActiveMoietyId();
        });

        if (itActiveMoieties == activeMoieties.end()) {
            _xpertRequestResult.setErrorMessage("A target is related to an active moiety that does not belong to the drug model: " + _xpertRequestResult.getDrugModel()->getDrugModelId());
            return;
        }
    }
}

} // namespace XpertResult
} // namespace Tucuxi
