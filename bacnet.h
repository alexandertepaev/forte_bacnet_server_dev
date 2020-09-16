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
#ifndef BACNET_H
#define BACNET_H

#define IP_ADDR_LEN 4
#define PORT_LEN 2

#define BVLC_HEADER_LEN 4
#define BVLC_TYPE_BYTE 0
#define BVLC_FUNCTION_BYTE 1
#define BVLC_LEN_BYTE 2

#define NPDU_OFFSET BVLC_HEADER_LEN

#define APDU_TYPE_MASK 0xF0
#define CONFIRMED_REQ_APP_TAGS_OFFSET 4
#define CONFIRMED_REQ_SERVICE_CHOICE_OFFSET 3
#define CONFIRMED_REQ_INVOKE_ID_OFFSET 2

#define UNCONFIRMED_REQ_APP_TAGS_OFFSET 2
#define UNCONFIRMED_REQ_SERVICE_CHOICE_OFFSET 1

/* core files */
#include "bacdcode.h"

#include "bacdef.h"
#include "bacenum.h"
#include "datalink.h"
#include "npdu.h"

#include "rp.h"
#include "wp.h"
// #include "bip.h"
// #include "bvlc.h"
#include "whois.h"
#include "iam.h"
#include "cov.h"

#endif