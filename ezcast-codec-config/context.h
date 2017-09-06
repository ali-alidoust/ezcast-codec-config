#pragma once

#include <libavcodec\avcodec.h>

class Context {
public:
	AVCodecContext* avcodec_context; // 0
	AVCodec* avcodec;				 // 4
	AVFrame* frame_input;			 // 8
	AVFrame* frame_output;			 // C
	int32_t unknown1;				 // 10
	int32_t unknown2;				 // 14
	int32_t unknown3;				 // 18
	int8_t is_context_created;		 // 1C
};