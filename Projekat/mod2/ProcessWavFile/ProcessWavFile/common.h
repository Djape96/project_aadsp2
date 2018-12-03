#pragma once

#include "stdfix_emu.h"
#include "fixed_point_math.h"

#define BLOCK_SIZE 16
#define MAX_NUM_CHANNEL 8
#define NUM_OF_CHANNELS 5

enum output_mode {
	mode1, // (2_0_0) Samo LS i RS
	mode2, // (3_2_0) Svi, nemamo bass (to je treci broj)
};

typedef int DSPint;
typedef fract DSPfract;
typedef long_accum DSPaccum;
