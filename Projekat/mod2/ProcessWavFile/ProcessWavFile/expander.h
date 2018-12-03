#ifndef EXPANDER_H
#define EXPANDER_H

#include "stdfix_emu.h"
#include "fixed_point_math.h"

typedef struct __AudioExpander {
	DSPfract threshold;
	DSPfract ratio;
} AudioExpander_t;


void gst_audio_dynamic_transform_expander_double(AudioExpander_t * expander,
	DSPfract * data);

void audio_expander_init(AudioExpander_t * expander);

#endif