#include "ResourceHelpers.h"

namespace ResourceHelpers {
  std::string getProjectDirectory() {
    auto currentDirectory = std::filesystem::absolute(".").lexically_normal();
    std::string projectName = Constants::PROJECT_NAME;
    std::string result = "";

    for (auto it : currentDirectory) {
      result = result + it.string() + "/";
      if (it.string().compare(projectName) == 0) {
        return result;
      }
    }

    return "";
  }

  std::filesystem::path getShaderPath(std::string filename) {
    auto projectDirectory = getProjectDirectory();
    auto shaderPath = std::filesystem::path(projectDirectory + "Source/views/shaders/" + filename);
    return shaderPath.lexically_normal();
  }

  std::filesystem::path getSVGPath(std::string filename) {
    auto projectDirectory = getProjectDirectory();
    auto svgPath = std::filesystem::path(projectDirectory + "Source/res/" + filename);
    return svgPath.lexically_normal();
  }
}
