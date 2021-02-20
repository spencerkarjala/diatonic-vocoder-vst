#pragma once
#include <string>
#include <filesystem>
#include "../../models/Constants.h"

namespace ResourceHelpers {
  std::string getProjectDirectory();
  std::filesystem::path getShaderPath(std::string filename);
  std::filesystem::path getSVGPath(std::string filename);
}
