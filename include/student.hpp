// Copyright 2021 Artem Artemov <theartemka6@mail.ru>
#ifndef INCLUDE_STUDENT_HPP_
#define INCLUDE_STUDENT_HPP_

#endif  // INCLUDE_STUDENT_HPP_

#include <string>
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
