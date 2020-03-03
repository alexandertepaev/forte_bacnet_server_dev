#ifndef _BACNET_COV_REPORTING_OBJECT_H_
#define _BACNET_COV_REPORTING_OBJECT_H_

#include "bacnet_object.h"

// class CBacnetCOVReportingObject {
class CBacnetCOVReportingObject: public CBacnetObject {
  private:
      /* data */
      bool mCOVCondition;
      bool mCOVReportingEnabled;
  public:
    // CBacnetCOVReportingObject(/* args */);
    CBacnetCOVReportingObject(BACNET_OBJECT_TYPE paObjectType, uint32_t paObjectID, bool paCOVReportingEnabled, forte::core::io::IOConfigFBBase *paObjectConfigFB); 
    ~CBacnetCOVReportingObject();
    bool COVCondition();
    void setCOVCondition();
    void clearCOVCondition();
    bool COVReportingEnabled();

    virtual void encodeValueList(BACNET_PROPERTY_VALUE* value_list) = 0;

    // virtual void foo() = 0;
};

#endif // _BACNET_COV_REPORTING_OBJECT_H_
