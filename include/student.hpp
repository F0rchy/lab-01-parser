#ifndef TEMPLATE_STUDENT_HPP
#define TEMPLATE_STUDENT_HPP

#endif // TEMPLATE_STUDENT_HPP

#include "header.hpp"

struct Student : public std::_Resetiosflags {
  std::string name;
  std::any group;
  std::any avg;
  std::any debt;
};

struct Lengths {
  int name;
  int group;
  int avg;
  int debt;
};