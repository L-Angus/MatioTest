#ifndef MATIO_WRAPPER_HPP
#define MATIO_WRAPPER_HPP

#include <iostream>
#include <matio.h>
#include <stdexcept>
#include <string>

#include "HeaderInfo.hpp"

class MatioWrapper {
public:
  MatioWrapper(const std::string &filename) : filename_(filename) {}

  void WriteHeaderInfoToMatFile(const HeaderInfo::HeaderInfoMap &headerInfo) {
    mat_t *matfp = Mat_CreateVer(filename_.c_str(), nullptr, MAT_FT_MAT5);
    if (!matfp) {
      throw std::runtime_error("无法创建 MAT 文件: " + filename_);
    }
    for (const auto &[name, field] : headerInfo) {
      matvar_t *matvar = CreateMatVar(field);
      if (!matvar) {
        std::cerr << "字段创建失败: " << name << std::endl;
        continue;
      }
      Mat_VarWrite(matfp, matvar, MAT_COMPRESSION_NONE);
      Mat_VarFree(matvar);
    }

    Mat_Close(matfp);
  }

  const std::string &GetFilename() const { return filename_; }

private:
  std::string filename_;

  matvar_t *CreateMatVar(const FieldInfo &field) const {
    return std::visit(
        [&](const auto &value) -> matvar_t * {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_same_v<T, double>) {
            // 标量值 double
            size_t dims[2] = {1, 1};
            return Mat_VarCreate(field.name.c_str(), MAT_C_DOUBLE, MAT_T_DOUBLE,
                                 2, dims, const_cast<double *>(&value), 0);
          } else if constexpr (std::is_same_v<T, uint8_t>) {
            // 标量值 uint8_t
            size_t dims[2] = {1, 1};
            return Mat_VarCreate(field.name.c_str(), MAT_C_UINT8, MAT_T_UINT8,
                                 2, dims, const_cast<uint8_t *>(&value), 0);
          } else if constexpr (std::is_same_v<T, std::string>) {
            // 字符串类型
            size_t dims[2] = {1, value.size()};
            return Mat_VarCreate(field.name.c_str(), MAT_C_CHAR, MAT_T_UINT8, 2,
                                 dims, const_cast<char *>(value.c_str()), 0);
          } else if constexpr (std::is_same_v<T, std::vector<float>>) {
            // 向量 float
            size_t dims[2] = {1, value.size()};
            return Mat_VarCreate(field.name.c_str(), MAT_C_SINGLE, MAT_T_SINGLE,
                                 2, dims, const_cast<float *>(value.data()), 0);
          } else if constexpr (std::is_same_v<T, std::vector<double>>) {
            // 向量 double
            size_t dims[2] = {1, value.size()};
            return Mat_VarCreate(field.name.c_str(), MAT_C_DOUBLE, MAT_T_DOUBLE,
                                 2, dims, const_cast<double *>(value.data()),
                                 0);
          } else {
            std::cerr << "Unsupported type for field: " << field.name
                      << std::endl;
            return nullptr;
          }
        },
        field.values);
  }
};

#endif