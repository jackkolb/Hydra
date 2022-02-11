#pragma once
#include <ostream>
#include <vector>

namespace config_parser {

// make sure vector operator is present in the right namespace
template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& values) {
  out << "[";
  auto iter = values.begin();
  while (iter != values.end()) {
    out << *iter;
    ++iter;
    if (iter != values.end()) {
      out << ", ";
    }
  }
  out << "]";
  return out;
}

class ConfigDisplay {
 public:
  explicit ConfigDisplay(std::ostream& out) : out_(out), prefix_("- ") {}

  ConfigDisplay operator[](const std::string& name) {
    const auto prev_pos = prefix_.find("-");
    const std::string spacing = prev_pos == std::string::npos ? "" : ("  " + prefix_.substr(prev_pos));
    const std::string new_prefix = spacing + "- " + name + ": ";
    return ConfigDisplay(out_, new_prefix);
  }

  template <typename T>
  void visit(T& value) {
    out_ << prefix_ <<  value << std::endl;
  }

  void visit(uint8_t& value) {
    out_ << prefix_ << static_cast<int>(value) << std::endl;
  }

  void visit(bool& value) {
    out_ << prefix_ << (value ? "true" : "false");
  }

 private:
  ConfigDisplay(std::ostream& out, const std::string& prefix) : out_(out), prefix_(prefix) {}

  std::ostream& out_;
  std::string prefix_;
};

}  // namespace config_parser