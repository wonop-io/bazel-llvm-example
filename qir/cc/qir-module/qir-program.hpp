#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/constant-int.hpp"
#include "qir/cc/qir-module/function-declaration.hpp"
#include "qir/cc/qir-module/qir-scope.hpp"
#include "qir/cc/qir-module/typed-value.hpp"

#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>
namespace compiler {

class QirBuilder;

struct QirType
{
  using String = std::string;
  using Type   = llvm::Type;
  Type           *value{nullptr};
  std::type_index type_id{std::type_index(typeid(void))};
  int64_t         size{sizeof(int64_t)};
  String          name;
};

class QirProgram
{
public:
  using Type          = llvm::Type;
  using StructType    = llvm::StructType;
  using LlvmContext   = llvm::LLVMContext;
  using LlvmModule    = llvm::Module;
  using Function      = llvm::Function;
  using Constant      = llvm::Constant;
  using ConstantInt   = llvm::ConstantInt;
  using String        = std::string;
  using LlvmFunction  = llvm::Function;
  using LlvmBlock     = llvm::BasicBlock;
  using Arguments     = std::vector<QirType>;
  using QirBuilderPtr = std::shared_ptr<QirBuilder>;
  using Builders      = std::unordered_set<QirBuilder *>;
  using ArgTypeNames  = FunctionDeclaration::ArgTypeNames;
  using FunctionCache = std::unordered_map<String, FunctionDeclaration>;

  QirProgram();
  ~QirProgram();

  LlvmContext *context();
  LlvmModule  *module();

  QirType getType(String const &name);
  QirType getType(std::type_index const &type_id);

  Type *getLlvmType(String const &name);

  //  QirBuilderPtr newFunction(String const &name, QirType return_type = QirType(),
  //                            Arguments args = Arguments());
  QirBuilderPtr newFunction(String const &name, String const &return_type = "Void",
                            ArgTypeNames const &arguments = {});

  String getQir();

  void addBuilder(QirBuilder *builder);
  void removeBuilder(QirBuilder *builder);
  void finalise();

  Function *getOrDeclareFunction(String const &name, String const &return_type = "Void",
                                 ArgTypeNames const &arguments = {});

  FunctionDeclaration getFunctionByLlvmName(String const &name);

  ConstantIntegerPtr toInt8(llvm::IRBuilder<> &builder, int8_t const &value);
  ConstantIntegerPtr toInt16(llvm::IRBuilder<> &builder, int16_t const &value);
  ConstantIntegerPtr toInt32(llvm::IRBuilder<> &builder, int32_t const &value);
  ConstantIntegerPtr toInt64(llvm::IRBuilder<> &builder, int64_t const &value);

  QirScope &scope()
  {
    return *scope_;
  }

  std::unique_ptr<LlvmContext> context_{};
  std::unique_ptr<LlvmModule>  module_{};

private:
  void registerType(QirType const &type);

  std::unordered_map<String, QirType>          types_{};
  std::unordered_map<std::type_index, QirType> from_native_types_{};

  Builders    builders_{};
  QirScopePtr scope_{nullptr};

  FunctionCache function_declaration_cache_{};
};

}  // namespace compiler