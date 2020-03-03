#include "bacnet_cov_reporting_object.h"

CBacnetCOVReportingObject::CBacnetCOVReportingObject(BACNET_OBJECT_TYPE paObjectType, uint32_t paObjectID, bool paCOVReportingEnabled, forte::core::io::IOConfigFBBase *paObjectConfigFB) : CBacnetObject(paObjectType, paObjectID, paObjectConfigFB), mCOVCondition(false), mCOVReportingEnabled(paCOVReportingEnabled)
{
}

CBacnetCOVReportingObject::~CBacnetCOVReportingObject()
{
}

bool CBacnetCOVReportingObject::COVCondition() {
    return mCOVCondition;
}

void CBacnetCOVReportingObject::setCOVCondition() {
    mCOVCondition = true;
}

void CBacnetCOVReportingObject::clearCOVCondition() {
    mCOVCondition = false;
}

bool CBacnetCOVReportingObject::COVReportingEnabled() {
    return mCOVReportingEnabled;
}