#ifndef _BACNET_BI_OBJCET_H_
#define _BACNET_BI_OBJCET_H_

#include "bacnet_cov_reporting_object.h"

class CBacnetBinaryInputObject : public CBacnetCOVReportingObject
{
  public:
    CBacnetBinaryInputObject(uint32_t paObjectID, bool paPresentValue, bool paReversePolarity, bool paCOVReportingEnabled, forte::core::io::IOConfigFBBase *paConfigFB);
    ~CBacnetBinaryInputObject();
    int readProperty(uint8_t *buffer,  BACNET_PROPERTY_ID property);
    bool writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property);
    void encodeValueList(BACNET_PROPERTY_VALUE* value_list);
  
    bool getPresentValue();
    void setPresentValue(bool paValue);

    bool isPolarityReversed();

    void setOutOfService(bool paValue); 
  private:
    bool mPresentValue; // current state of the input
    bool mReversePolarity; // descriptive property which tells if the input is low-active (logical 0 on the input means that the input is active) TODO: should we also change the behaviour of PresentValueOut() ?
    bool mOutOfService; // tells if the input is out of service. If so, present value property can be changed using WriteProperty service requests, if not all WriteProperty requests to the present value should be ignored // TODO: change mOutOfService when triggering WRITE_PR_VAL event



};

#endif