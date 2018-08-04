#include "tinyVM.h"
#include <cassert>
#include <cmath>
#include <cstring>
#include <ctime>
#include <exception>
#include <iostream>
#include <ctime>

TinyVM::TinyVM() {
  assert(sizeof(float) == 4);
  try {
    vmem = new int32_t[1024]();
    ax = new int32_t(0);
  } catch (std::bad_alloc &e) {
    std::cerr << "cannot alloc memory for VM" << std::endl;
    throw e;
  }
  varMap = std::make_shared<std::map<std::string, INNERPTR>>();
  sp = vmem;
}

void TinyVM::addSource(void *source, size_t sz) {
  if (sz % 4 != 0) {
    throw std::runtime_error("source code size invalid");
  }
  src = new int32_t[sz / 4];
  std::memcpy(src, source, sz);
  pc = src;
  //initVar();
}

void TinyVM::initVar() {
  int i = 0;
  while (*pc != START) {
    execute();
  }
  sp += i;
}

float TinyVM::varValue(const std::string &varname) const {
  return *(reinterpret_cast<float *>(vmem) + varMap->at(varname));
}

void TinyVM::run() {
  while (*pc != BACK) {
    execute();
  }
  execute();
  return;
}

TinyVM::~TinyVM() {
  delete[] vmem;
  delete ax;
}

void TinyVM::execute() {
  switch (*pc++) {
    case IMM:
      *ax = *pc++;
      break;
    case LF:
      *ax = vmem[static_cast<INNERPTR>(*ax)];
      break;
    case SF:
      vmem[static_cast<INNERPTR>(*sp--)] = *ax;
      break;
    case PUSH:
      *++sp = *ax;
      break;
    case JMP:
      pc = src + *pc;
      break;
    case JZ:
      if(*ax){
          pc = pc + 1;
      }else{
          pc = src + *pc;
      }
      break;
    case OR:
      *ax = static_cast<int32_t>(*reinterpret_cast<float *>(sp)) |
            static_cast<int32_t>(*reinterpret_cast<float *>(ax));
      sp--;
      break;
    case AND:
      *ax = static_cast<int32_t>(*reinterpret_cast<float *>(sp)) &
            static_cast<int32_t>(*reinterpret_cast<float *>(ax));
      sp--;
      break;
    case EQ:
      *ax = *reinterpret_cast<float *>(sp--) == *reinterpret_cast<float *>(ax);
      break;
    case BIG:
      *ax = *reinterpret_cast<float *>(sp--) > *reinterpret_cast<float *>(ax);
      break;
    case SMA:
      *ax = *reinterpret_cast<float *>(sp--) < *reinterpret_cast<float *>(ax);
      break;
    case ADD:
      *reinterpret_cast<float *>(ax) =
          *reinterpret_cast<float *>(sp--) + *reinterpret_cast<float *>(ax);
      break;
    case SUB:
      *reinterpret_cast<float *>(ax) =
          *reinterpret_cast<float *>(sp--) - *reinterpret_cast<float *>(ax);
      break;
    case DIV:
      *reinterpret_cast<float *>(ax) =
          *reinterpret_cast<float *>(sp--) / *reinterpret_cast<float *>(ax);
      break;
    case MUL:
      *reinterpret_cast<float *>(ax) =
          *reinterpret_cast<float *>(sp--) * *reinterpret_cast<float *>(ax);
      break;
    case POW:
      *reinterpret_cast<float *>(ax) = std::pow(*reinterpret_cast<float *>(sp--),
                                                *reinterpret_cast<float *>(ax));
      break;
    case NOT:
      *reinterpret_cast<float *>(ax) = !*reinterpret_cast<float *>(ax);
      break;
    case NEG:
      *reinterpret_cast<float *>(ax) = -(*reinterpret_cast<float *>(ax));
      break;
    case SIN:
      *reinterpret_cast<float *>(ax) = std::sin(*reinterpret_cast<float *>(ax));
      break;
    case COS:
      *reinterpret_cast<float *>(ax) = std::cos(*reinterpret_cast<float *>(ax));
      break;
    case TAN:
      *reinterpret_cast<float *>(ax) = std::tan(*reinterpret_cast<float *>(ax));
      break;
    case ATAN:
      *reinterpret_cast<float *>(ax) =
          std::atan(*reinterpret_cast<float *>(ax));
      break;
    case ACOS:
      *reinterpret_cast<float *>(ax) =
          std::acos(*reinterpret_cast<float *>(ax));
      break;
    case ASIN:
      *reinterpret_cast<float *>(ax) =
          std::asin(*reinterpret_cast<float *>(ax));
      break;
    case TIME:
      *reinterpret_cast<float *>(ax) =
          static_cast<float>(std::clock()) / 1000.0f;
      break;
    case START:
      st = pc;
      break;
    case BACK:
      pc = st;
      break;
    case BIND:
      if (st == nullptr) {
        sp++;
        (*varMap)[std::string("var") + std::to_string(varn)] = varn;
        varn++;
      } else
        throw std::runtime_error("cannot bind after start");
      break;
    case CLOCK:
      *reinterpret_cast<float *>(ax) = std::clock() / 1000.0f;
    default:
      break;
  }
}
