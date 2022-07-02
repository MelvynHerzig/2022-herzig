#ifndef XPERTREQUESTRESULTHTMLEXPORT_H
#define XPERTREQUESTRESULTHTMLEXPORT_H

#include "tuberxpert/exporter/abstractxpertrequestresultexport.h"
#include "tuberxpert/query/admindata.h"

#include "inja/inja.hpp"

namespace Tucuxi {
namespace Xpert {

/// \brief This class exports an XpertRequestResult into html.
/// \date 23/06/2022
/// \author Herzig Melvyn
class XpertRequestResultHtmlExport : public AbstractXpertRequestResultExport
{
public:

    /// \brief Constructor.
    XpertRequestResultHtmlExport();

    /// \brief Export the xpert request result to file. The export may fail. In that
    ///        case, the error message of the XpertRequestResult gets set.
    /// \param _xpertRequestResult Xpert request result to export.
    void exportToFile(XpertRequestResult& _xpertRequestResult) override;

protected:

    std::string makeHeaderString() const;

    std::string makeBodyString(XpertRequestResult& _xpertRequestResult) const;

    void getHeaderJson(XpertRequestResult& _xpertRequestResult, inja::json& _json) const;

    void getDrugIntroJson(XpertRequestResult& _xpertRequestResult, inja::json& _json) const;

    void getContactsJson(XpertRequestResult& _xpertRequestResult, inja::json& _json) const;

    void getPersonJson(const std::unique_ptr<FullPersonData>& _person, inja::json& _json) const;

    void getInstituteJson(const std::unique_ptr<InstituteData>& _institute, inja::json& _json) const;

    void getAddressJson(const std::unique_ptr<AddressData>& _address, inja::json& _json) const;

    void getPhoneJson(const std::unique_ptr<PhoneData>& _phone, inja::json& _json) const;

    void getEmailJson(const std::unique_ptr<EmailData>& _email, inja::json& _json) const;

    void getClinicalDataJson(XpertRequestResult& _xpertRequestResult, inja::json& _json) const;

};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTREQUESTRESULTHTMLEXPORT_H
