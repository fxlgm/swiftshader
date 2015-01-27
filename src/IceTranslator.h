//===- subzero/src/IceTranslator.h - ICE to machine code --------*- C++ -*-===//
//
//                        The Subzero Code Generator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the general driver class for translating ICE to
// machine code.
//
//===----------------------------------------------------------------------===//

#ifndef SUBZERO_SRC_ICETRANSLATOR_H
#define SUBZERO_SRC_ICETRANSLATOR_H

namespace llvm {
class Module;
}

namespace Ice {

class ClFlags;
class Cfg;
class VariableDeclaration;
class GlobalContext;

// Base class for translating ICE to machine code.  Derived classes convert
// other intermediate representations down to ICE, and then call the appropriate
// (inherited) methods to convert ICE into machine instructions.
class Translator {
  Translator(const Translator &) = delete;
  Translator &operator=(const Translator &) = delete;

public:
  typedef std::vector<VariableDeclaration *> VariableDeclarationListType;

  Translator(GlobalContext *Ctx, const ClFlags &Flags);
  ~Translator();
  const ErrorCode &getErrorStatus() const { return ErrorStatus; }

  GlobalContext *getContext() const { return Ctx; }

  const ClFlags &getFlags() const { return Flags; }

  /// Translates the constructed ICE function Fcn to machine code.
  /// Takes ownership of Func.
  void translateFcn(Cfg *Func);

  /// Emits the constant pool.
  void emitConstants();

  /// If there was an error during bitcode reading/parsing, copy the
  /// error code into the GlobalContext.
  void transferErrorCode() const;

  /// Lowers the given list of global addresses to target. Generates
  /// list of corresponding variable declarations.
  void lowerGlobals(const VariableDeclarationListType &VariableDeclarations);

  /// Creates a name using the given prefix and corresponding index.
  std::string createUnnamedName(const IceString &Prefix, SizeT Index);

  /// Reports if there is a (potential) conflict between Name, and using
  /// Prefix to name unnamed names. Errors are put on Ostream.
  /// Returns true if there isn't a potential conflict.
  bool checkIfUnnamedNameSafe(const IceString &Name, const char *Kind,
                              const IceString &Prefix);

protected:
  GlobalContext *Ctx;
  const ClFlags &Flags;
  std::unique_ptr<TargetGlobalLowering> GlobalLowering;
  // Exit status of the translation. False is successful. True otherwise.
  ErrorCode ErrorStatus;
};

} // end of namespace Ice

#endif // SUBZERO_SRC_ICETRANSLATOR_H
