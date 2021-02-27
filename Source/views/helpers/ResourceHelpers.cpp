#include <regex>
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

  std::filesystem::path getJSPath() {
    auto projectDirectory = getProjectDirectory();
    auto jsPath = std::filesystem::path(projectDirectory + "Source/jsui/build/js/main.js");
    return jsPath.lexically_normal();
  }

  // mostly internal helper - to parse other string sizes use parseColorString
  juce::Colour hexStringToColor(std::string colorString) {
    jassert(colorString.length() == 8);

    juce::uint32 result = std::stoul(colorString, nullptr, 16);
    return juce::Colour(result);
  }

  // TODO: add rgb(), hsl()
  juce::Colour parseColorString(juce::String color) {
    jassert(!color.isEmpty());

    if (color.startsWith("#")) {
      color = color.substring(1);
    }

    const struct {
      std::regex HEX_3 = std::regex("^[a-fA-F0-9]{3}$");
      std::regex HEX_4 = std::regex("^[a-fA-F0-9]{4}$");
      std::regex HEX_6 = std::regex("^[a-fA-F0-9]{6}$");
      std::regex HEX_8 = std::regex("^[a-fA-F0-9]{8}$");
    } regexes;

    auto s = color.toStdString();
    if (std::regex_match(s, regexes.HEX_3)) {
      return hexStringToColor(std::string("ff") + s[0] + s[0] + s[1] + s[1] + s[2] + s[2]);
    }
    else if (std::regex_match(s, regexes.HEX_4)) {
      return hexStringToColor(std::string("") + s[0] + s[0] + s[1] + s[1] + s[2] + s[2] + s[3] + s[3]);
    }
    else if (std::regex_match(s, regexes.HEX_6)) {
      return hexStringToColor(std::string("ff") + s);
    }
    else if (std::regex_match(s, regexes.HEX_8)) {
      return hexStringToColor(s);
    }

    // no match, throw error
    jassert(false);
  }
}
