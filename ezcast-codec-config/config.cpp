#include "config.h"

std::shared_ptr<INI::Parser>    config::config_parser;
std::string                     config::format_cfg;
std::string                     config::video_enc;
std::string                     config::video_fmt;
std::string                     config::video_cfg;
std::string                     config::audio_enc;
std::string                     config::audio_cfg;
std::string                     config::audio_fmt;
LogLevel                        config::log_level;