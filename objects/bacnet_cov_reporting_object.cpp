
/*******************************************************************************
 * Copyright (c) 2020 Alexander Tepaev github.com/alexandertepaev
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alexander Tepaev
 *******************************************************************************/
#include "bacnet_cov_reporting_object.h"

CBacnetCOVReportingObject::CBacnetCOVReportingObject(BACNET_OBJECT_TYPE paObjectType, uint32_t paObjectID, bool paCOVReportingEnabled, CBacnetObjectConfigFB *paObjectConfigFB) : CBacnetObject(paObjectType, paObjectID, paObjectConfigFB), mCOVCondition(false), mCOVReportingEnabled(paCOVReportingEnabled)
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