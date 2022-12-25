/*==============================================================================
  Copyright (c) 2022 Qualcomm Technologies, Inc.
  All rights reserved. Qualcomm Proprietary and Confidential.
==============================================================================*/

typedef struct note_type{
  int size_name; // Size of the NOTE section
  int size_desc; // Size of the descriptor(unused)
  int type; // Type of section(unused)
  char name[100]; // Name of NOTE section(version of shared object)
} note_type;

/* Library version needs to be added in the name member of note_type structure in below format
 * "lib.ver.1.0.0." + "<library_name>" + ":" + "<version>"
 */
const note_type so_ver __attribute__ ((section (".note.lib.ver")))
      __attribute__ ((visibility ("default"))) = {
  100,
  0,
  0,
  "lib.ver.1.0.0.libcalculator_skel.so:4.5.0",
};
