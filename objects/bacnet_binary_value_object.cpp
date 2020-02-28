#include "bacnet_binary_value_object.h"
#include "../BACnetBinaryValue.h"

CBacnetBinaryValueObject::CBacnetBinaryValueObject(uint32_t paObjectID, bool paPresentValue, forte::core::io::IOConfigFBBase *paConfigFB) : CBacnetObject(OBJECT_BINARY_VALUE, paObjectID, paConfigFB), mPresentValue(paPresentValue) {
    DEVLOG_DEBUG("[CBacnetBinaryValueObject] CBacnetBinaryValueObject(): Initialized Binary Value Object: ObjID=%d, PresentValue=%d\n", mObjectID, mPresentValue);
}

CBacnetBinaryValueObject::~CBacnetBinaryValueObject() {
    
}

int CBacnetBinaryValueObject::encodeApplicationData(uint8_t *buffer,  BACNET_PROPERTY_ID property) {
    int len = 0;
    switch (property)
    {   
    case PROP_PRESENT_VALUE:
        len = encode_application_enumerated(buffer, mPresentValue);
        break;
    
    default:
        break;
    }
    return len;
}

int CBacnetBinaryValueObject::writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property) {
    switch (property)
    {
    case PROP_PRESENT_VALUE:
        mPresentValue = paData->type.Enumerated;
        //FORTE_BACnetBinaryValue *obj = static_cast<FORTE_BACnetBinaryValue *>(mConfigFB);
        // startNewEventChain(mConfigFB);
        break;
    
    default:
        break;
    }
}

bool CBacnetBinaryValueObject::getPresentValue() {
    return mPresentValue;
}

void CBacnetBinaryValueObject::setPresentValue(bool paValue) {
    mPresentValue = paValue;
}