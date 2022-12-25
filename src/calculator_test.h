#ifndef CALCULATOR_TEST_H
#define CALCULATOR_TEST_H
/*==============================================================================
  Copyright (c) 2012-2014, 2020 Qualcomm Technologies, Inc.
  All rights reserved. Qualcomm Proprietary and Confidential.
==============================================================================*/

#include "AEEStdDef.h"
#include <stdbool.h>
#include "remote.h"

#ifdef __cplusplus
extern "C" {
#endif

int calculator_test(int runMode, int domain, int num, bool isUnsignedPD_Enabled);

#ifdef __cplusplus
}
#endif

#endif // CALCULATOR_TEST_H

