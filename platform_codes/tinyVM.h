#ifndef TINYVM_H
#define TINYVM_H
#include <cinttypes>
#include <map>
#include <memory>
#include <string>

class TinyVM {
 public:
  TinyVM();
  void addSource(void *source, size_t sz);
  void run();
  float varValue(const std::string &varName) const;
  ~TinyVM();

 private:
  void initVar();
  enum command : int32_t {
    IMM,
    LF,
    SF,
    PUSH,
    JMP,
    JZ,
    OR,
    AND,
    EQ,
    BIG,
    SMA,
    ADD,
    SUB,
    DIV,
    MUL,
    POW,
    NOT,
    NEG,
    SIN,
    COS,
    TAN,
    ATAN,
    ASIN,
    ACOS,
    TIME,
    START,
    BACK,
    BIND,
    CLOCK
  };
  void execute();
  using INNERPTR = int32_t;
  std::shared_ptr<std::map<std::string, INNERPTR>> varMap;
  int32_t *src = nullptr;
  int32_t *vmem = nullptr;
  int32_t *pc = nullptr;
  int32_t *ax = 0;
  int32_t *sp = nullptr;
  int32_t *st = nullptr;
  int32_t varn = 0;
};
#endif