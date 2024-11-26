#ifndef HEADER_INFO_HPP
#define HEADER_INFO_HPP

#include <stdint.h>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

struct FieldInfo {
  std::string name;
  std::string type;
  std::variant<double, int16_t, uint8_t, std::string, std::vector<float>,
               std::vector<double>>
      values;
};

class HeaderInfo {
public:
  using HeaderInfoMap = std::unordered_map<std::string, FieldInfo>;
  static HeaderInfo &GetInstance() {
    static HeaderInfo instance;
    return instance;
  }

  void AddField(const FieldInfo &field) { fields_[field.name] = field; }
  const FieldInfo &GetField(const std::string &name) const {
    return fields_.at(name);
  }

  const HeaderInfoMap &GetHeaderInfo() const { return fields_; }

private:
  HeaderInfo() {}
  ~HeaderInfo() {}
  HeaderInfo(const HeaderInfo &) = delete;
  HeaderInfo &operator=(const HeaderInfo &) = delete;

  HeaderInfoMap fields_;
};

#endif