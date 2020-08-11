#ifndef _BACNET_BO_OBJCET_H_
#define _BACNET_BO_OBJCET_H_

#include "bacnet_cov_reporting_object.h"

class CBacnetBinaryOutputObject : public CBacnetCOVReportingObject
{
  public:
    CBacnetBinaryOutputObject(uint32_t paObjectID, bool paPresentValue, bool paCOVReportingEnabled, forte::core::io::IOConfigFBBase *paConfigFB);
    ~CBacnetBinaryOutputObject();
    int readProperty(uint8_t *buffer,  BACNET_PROPERTY_ID property);
    bool writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property);
    void encodeValueList(BACNET_PROPERTY_VALUE* value_list);
  
    bool getPresentValue();
    void setPresentValue(bool paValue);
  private:
    bool mPresentValue;

};

#endif