/*
	This field was automatically created with CMake please don't modify it
*/
#pragma once

#include <filesystem>

namespace Grafica
{
	
static const char * const source_directory = "C:/Users/autofact/Documents/qdu-engine/";

static std::filesystem::path getPath(const std::string &relative_path){
	return source_directory + relative_path;
}
	
} // Grafica
