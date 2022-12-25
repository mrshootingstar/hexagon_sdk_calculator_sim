/*==============================================================================
  Copyright (c) 2012-2014, 2020 Qualcomm Technologies, Inc.
  All rights reserved. Qualcomm Proprietary and Confidential.
==============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "HAP_farf.h"
#include "calculator.h"

int calculator_open(const char*uri, remote_handle64* handle) {
   void *tptr = NULL;
  /* can be any value or ignored, rpc layer doesn't care
   * also ok
   * *handle = 0;
   * *handle = 0xdeadc0de;
   */
   tptr = (void *)malloc(1);
   *handle = (remote_handle64)tptr;
   assert(*handle);
   return 0;
}

/**
 * @param handle, the value returned by open
 * @retval, 0 for success, should always succeed
 */
int calculator_close(remote_handle64 handle) {
   if (handle)
      free((void*)handle);
   return 0;
}

int calculator_sum(remote_handle64 h, const int* vec, int vecLen, int64* res)
{
  int ii = 0;
  *res = 0;
  for (ii = 0; ii < vecLen; ++ii)
    *res = *res + vec[ii];

  FARF(RUNTIME_HIGH, "\n===============     DSP: sum result %lld ===============\n", *res);
  return 0;
}

int calculator_max(remote_handle64 h, const int* vec, int vecLen, int* res) {
  int ii = 0;
  int max = 0;

  for (ii = 0; ii < vecLen; ++ii)
    max = (vec[ii] > max) ? vec[ii] : max;

  *res = max;
  FARF(HIGH, "\n===============     DSP: maximum result %d ==============\n", *res);
  return 0;
}
