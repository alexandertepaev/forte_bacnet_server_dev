#include <devlog.h>
#include "objects/bacnet_object.h"
#include "bacnet_server_controller.h"

CBacnetServerController::CBacnetServerController() : mObjectTable(NULL) {
  DEVLOG_DEBUG("[CBacnetServerController] CBacnetServerController(): created new server controller instance\n");
}

CBacnetServerController::~CBacnetServerController() {

}

bool CBacnetServerController::init(uint16_t paPort) {
  DEVLOG_DEBUG("[CBacnetServerController] init(): Initializing controller on Port=%04X\n", paPort);

  mObjectTable = new TObjectTable();
  
  return true;
}

bool CBacnetServerController::addObjectTableEntry(CBacnetObject *paObject) {
  if(paObject == NULL)
    return false;

  //TODO check for duplicates
  mObjectTable->pushBack(paObject);

  DEVLOG_DEBUG("[CBacnetServerController] addObjectTableEntry(): updated object table with Object=%d, 0bjectID=%d\n", paObject->mObjectType, paObject->mObjectID);
  return true;
}