#include "HeaderInfo.hpp"
#include "MatioWrapper.hpp"
#include <iostream>
#include <matio.h>

int main() {
  std::cout << "____Matio project____" << std::endl;
  HeaderInfo &header = HeaderInfo::GetInstance();

  // 添加字段
  header.AddField({"Y", "single",
                   std::vector<float>{0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f,
                                      0.8f, 0.9f, 1.0f, 1.1f, 1.2f, 1.3f, 1.4f,
                                      1.5f, 1.6f, 1.7f, 1.8f, 1.9f, 2.0f}});
  header.AddField({"FreqValidMax", "double", 1.28});
  header.AddField({"XUnit", "char", std::string("Sec")});
  header.AddField({"InputZoom", "float", 1.7});

  MatioWrapper matio("output.mat");
  matio.WriteHeaderInfoToMatFile(header.GetHeaderInfo());

  return 0;
}