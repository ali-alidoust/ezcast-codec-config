#pragma once

#ifndef _MY_CONFIG_H_
#define _MY_CONFIG_H_

#include <ini.hpp>
#include <string>
#include <algorithm>
#include <sstream>
#include <ShlObj.h>
#include <regex>
#include "logger.h"

#define CFG_FORMAT_SECTION "FORMAT"
#define CFG_EXPORT_FORMAT "format"
#define CFG_FORMAT_CFG "options"

#define CFG_LOG_LEVEL "log_level"

#define CFG_VIDEO_SECTION "VIDEO"
#define CFG_VIDEO_ENC "encoder"
#define CFG_VIDEO_FMT "pixel_format"
#define CFG_VIDEO_CFG "options"

#define CFG_AUDIO_SECTION "AUDIO"
#define CFG_AUDIO_ENC "encoder"
#define CFG_AUDIO_FMT "sample_format"
#define CFG_AUDIO_CFG "options"

//#define CFG_AUDIO_CODEC "audio_codec"
#define INI_FILE_NAME TARGET_NAME ".ini"

class config {
public:
	static std::string                     format_cfg;
	static std::string                     video_enc;
	static std::string                     video_fmt;
	static std::string                     video_cfg;
	static std::string                     audio_enc;
	static std::string                     audio_cfg;
	static std::string                     audio_fmt;
	static LogLevel                        log_level;

	static void reload() {
		config_parser.reset(new INI::Parser(INI_FILE_NAME));

		format_cfg = parse_format_cfg();
		video_enc = parse_video_enc();
		video_fmt = parse_video_fmt();
		video_cfg = parse_video_cfg();
		audio_enc = parse_audio_enc();
		audio_cfg = parse_audio_cfg();
		audio_fmt = parse_audio_fmt();
		log_level = parse_log_level();
	}

private:
	static std::shared_ptr<INI::Parser> config_parser;
	//static std::shared_ptr<INI::Parser> preset_parser;

	static std::string getTrimmed(std::shared_ptr<INI::Parser> parser, std::string config_name) {
		std::string orig_str = parser->top()[config_name];
		return std::regex_replace(orig_str, std::regex("(^\\s*)|(\\s*$)"), "");
	}

	static std::string getTrimmed(std::shared_ptr<INI::Parser> parser, std::string config_name, std::string section) {
		std::string orig_str = parser->top()(section)[config_name];
		return std::regex_replace(orig_str, std::regex("(^\\s*)|(\\s*$)"), "");
	}


	static HRESULT GetVideosDirectory(LPSTR output)
	{
		PWSTR vidPath = NULL;

		RET_IF_FAILED((SHGetKnownFolderPath(FOLDERID_Videos, 0, NULL, &vidPath) != S_OK), "Failed to get Videos directory for the current user.", E_FAIL);

		int pathlen = lstrlenW(vidPath);

		int buflen = WideCharToMultiByte(CP_UTF8, 0, vidPath, pathlen, NULL, 0, NULL, NULL);
		if (buflen <= 0)
		{
			return E_FAIL;
		}

		buflen = WideCharToMultiByte(CP_UTF8, 0, vidPath, pathlen, output, buflen, NULL, NULL);

		output[buflen] = 0;

		CoTaskMemFree(vidPath);

		return S_OK;
	}

	static std::string toLower(std::string input) {
		std::string result = input;
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}

	static bool stringToBoolean(std::string booleanString) {
		std::transform(booleanString.begin(), booleanString.end(), booleanString.begin(), ::tolower);

		bool value;
		std::istringstream(booleanString) >> std::boolalpha >> value;
		return value;
	}

	template <typename T>
	static T failed(std::string key, std::string value, T def) {
		LOG(LL_NON, "Failed to parse value for \"", key, "\": ", value);
		LOG(LL_NON, "Using default value of \"", def, "\" for \"", key, "\"");
		return def;
	}

	template <typename T1, typename T2>
	static std::pair<T1, T2> failed(std::string key, std::string value, std::pair<T1, T2> def) {
		LOG(LL_NON, "Failed to parse value for \"", key, "\": ", value);
		LOG(LL_NON, "Using default value of <", def.first, ", ", def.second, "> for \"", key, "\"");
		return def;
	}

	template <typename T>
	static T succeeded(std::string key, T value) {
		LOG(LL_NON, "Loaded value for \"", key, "\": ", value);
		return value;
	}

	template <typename T1, typename T2>
	static std::pair<T1, T2> succeeded(std::string key, std::pair<T1, T2> value) {
		LOG(LL_NON, "Loaded value for \"", key, "\": <", value.first, ", ", value.second, ">");
		return value;
	}

	static std::string parse_format_cfg() {
		std::string string = getTrimmed(config_parser, CFG_FORMAT_CFG, CFG_FORMAT_SECTION);
		try {
			return succeeded(CFG_FORMAT_CFG, string);
		} catch (std::exception& ex) {
			LOG(LL_ERR, ex.what());
		}

		return failed(CFG_FORMAT_CFG, string, "");
	}

	static std::string parse_video_enc() {
		std::string string = getTrimmed(config_parser, CFG_VIDEO_ENC, CFG_VIDEO_SECTION);
		try {
			if (!string.empty()) {
				return succeeded(CFG_VIDEO_ENC, string);
			}
		} catch (std::exception& ex) {
			LOG(LL_ERR, ex.what());
		}

		LOG(LL_NFO, "No video encoder specified. Video encoding will be disabled.");
		return "";
		//return failed(CFG_VIDEO_ENC, string, "");
	}

	static std::string parse_video_fmt() {
		std::string string = getTrimmed(config_parser, CFG_VIDEO_FMT, CFG_VIDEO_SECTION);
		try {
			if (!string.empty()) {
				return succeeded(CFG_VIDEO_FMT, string);
			}
		} catch (std::exception& ex) {
			LOG(LL_ERR, ex.what());
		}

		return failed(CFG_VIDEO_FMT, string, "yuv420p");
	}

	static std::string parse_video_cfg() {
		std::string string = getTrimmed(config_parser, CFG_VIDEO_CFG, CFG_VIDEO_SECTION);
		try {
			if (!string.empty()) {
				return succeeded(CFG_VIDEO_CFG, string);
			}
		} catch (std::exception& ex) {
			LOG(LL_ERR, ex.what());
		}

		return failed(CFG_VIDEO_CFG, string, "");
	}

	static std::string parse_audio_enc() {
		std::string string = getTrimmed(config_parser, CFG_AUDIO_ENC, CFG_AUDIO_SECTION);
		try {
			if (!string.empty()) {
				return succeeded(CFG_AUDIO_ENC, string);
			}
		} catch (std::exception& ex) {
			LOG(LL_ERR, ex.what());
		}

		LOG(LL_NFO, "No audio encoder specified. Audio encoding will be disabled.");
		return "";
		//return failed(CFG_AUDIO_ENC, string, "ac3");
	}

	static std::string parse_audio_cfg() {
		std::string string = getTrimmed(config_parser, CFG_AUDIO_CFG, CFG_AUDIO_SECTION);
		try {
			if (!string.empty()) {
				return succeeded(CFG_AUDIO_CFG, string);
			}
		} catch (std::exception& ex) {
			LOG(LL_ERR, ex.what());
		}

		return failed(CFG_AUDIO_CFG, string, "");
	}

	static std::string parse_audio_fmt() {
		std::string string = getTrimmed(config_parser, CFG_AUDIO_FMT, CFG_AUDIO_SECTION);
		try {
			if (!string.empty()) {
				return succeeded(CFG_AUDIO_FMT, string);
			}
		} catch (std::exception& ex) {
			LOG(LL_ERR, ex.what());
		}

		return failed(CFG_AUDIO_FMT, string, "fltp");
	}

	static LogLevel parse_log_level() {
		std::string string = toLower(getTrimmed(config_parser, CFG_LOG_LEVEL));
		try {
			if (string == "error") {
				return succeeded(CFG_LOG_LEVEL, LL_ERR);
			} else if (string == "warn") {
				return succeeded(CFG_LOG_LEVEL, LL_WRN);
			} else if (string == "info") {
				return succeeded(CFG_LOG_LEVEL, LL_NFO);
			} else if (string == "debug") {
				return succeeded(CFG_LOG_LEVEL, LL_DBG);
			} else if (string == "trace") {
				return succeeded(CFG_LOG_LEVEL, LL_TRC);
			}
		} catch (std::exception& ex) {
			LOG(LL_ERR, ex.what());
		}

		return failed(CFG_LOG_LEVEL, string, LL_ERR);
	}
};

#endif _MY_CONFIG_H_