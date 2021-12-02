#include <any>
#include <vector>

#include "../include/header.hpp"

using json = nlohmann::json;

std::string get_name(json* stt) {
  std::string name;
  if (stt->at("name").is_string()) {
    name = stt->at("name").get<std::string>();
  }

  return name;
  }

  std::any get_group(json* stt) {
    std::any group;
    if (stt->at("group").is_string()) {
      group = stt->at("group").get<std::string>();
    }

    if (stt->at("group").is_number_integer()) {
      group = stt->at("group").get<int>();
    }

    return group;
  }

  std::any get_avg(json* stt) {
    std::any avg;
    if(stt->at("avg").is_number_integer()) {
      avg = stt->at("avg").get<int>();
    }

    if (stt->at("avg").is_string()) {
      avg = stt->at("avg").get<std::string>();
    }

    if (stt->at("avg").is_number_float()) {
      avg = stt->at("avg").get<float>();
    }

    return avg;
  }

  std::any get_debt(json* stt) {
    std::any debt;
    if (stt->at("debt").is_string()) {
      debt = stt->at("debt").get<std::string>();
    }

    if (stt->at("debt").is_null()) {
      debt = (std::string)"null";
    }

    if (stt->at("debt").is_array()) {
      debt = (int)stt->at("debt").size();
    }

    return debt;
  }

  std::pair<std::vector<Student>,std::string> parse_json(std::string path) {
    std::pair<std::vector<Student>,std::string> answer;
    std::ifstream ifs(path);
    if (!ifs) {
      throw std::runtime_error{"Can't find json file"};
    }
    json jf = json::parse(ifs);
    if(!jf.at("items").is_array()) {
      answer.first = std::vector<Student>{};
      answer.second = "Items is not an array!";
      return answer;
    }
    if (jf.at("items").size() != jf.at("_meta").at("count")) {
      answer.first = std::vector<Student>{};
      answer.second = "Invalid number of students. The counter doesn't match";
      return answer;
    }
    std::vector<Student> students;
    for (auto& e1 : jf.at("items").items()) {
      json stt = e1.value();
      Student student;

      // NAME
      student.name = get_name(&stt);
      if (student.name == "") {
        std::string error ="Incorrect name of Student:  " + to_string(stt.at("name"));
        answer.first = std::vector<Student>{};
        answer.second = error;
        return answer;
      }

      // GROUP
      student.group = get_group(&stt);
      if (!student.group.has_value()) {
        answer.first = std::vector<Student>{};
        answer.second = "Incorrect group of Student " + to_string(stt.at("group"));
        return answer;
      }

      // AVG
      student.avg = get_avg(&stt);
      if (!student.avg.has_value()) {
        answer.first = std::vector<Student>{};
        answer.second = "Incorrect avg of Student " + to_string(stt.at("avg")) ;
        return answer;
      }

      // DEBT
      student.debt = get_debt(&stt);
      if (!student.debt.has_value()) {
        answer.first = std::vector<Student>{};
        answer.second = "Incorrect debt of Student: " + to_string(stt.at("debt"));
        return answer;
      }

      students.push_back(student);
    }
    answer.first = students;
    answer.second = "";
    return answer;
  }

  Lengths get_lengths(std::vector<Student> students) {
    Lengths ls{0, 0, 0, 0};
    for (Student student1 : students) {
      if ((int)student1.name.length() > ls.name) {
        ls.name = (int)student1.name.length();
      }
      if (student1.group.type() == typeid(int)) {
        std::string strgroup =
            std::to_string(std::any_cast<int>(student1.group));
        if ((int)strgroup.length() > ls.group) {
          ls.group = (int)strgroup.length();
        }
      } else if (student1.group.type() == typeid(std::string)) {
        if ((int)std::any_cast<std::string>(student1.group).length() >
            ls.group) {
          ls.group = (int)std::any_cast<std::string>(student1.group).length();
        }
      } else
        return Lengths{};

      if (student1.avg.type() == typeid(int)) {
        std::string stravg = std::to_string(std::any_cast<int>(student1.avg));
        if ((int)stravg.length() > ls.avg) {
          ls.avg = (int)stravg.length();
        }
      } else if (student1.avg.type() == typeid(std::string)) {
        if ((int)std::any_cast<std::string>(student1.avg).length() > ls.avg) {
          ls.avg = (int)std::any_cast<std::string>(student1.avg).length();
        }
      } else if (student1.avg.type() == typeid(float)) {
        std::string stravg = std::to_string(std::any_cast<float>(student1.avg));
        if ((int)stravg.length() > ls.avg) {
          ls.avg = (int)stravg.length();
        }
      } else
        return Lengths{};
      if (student1.debt.type() == typeid(std::string)) {
        if ((int)std::any_cast<std::string>(student1.debt).length() > ls.debt) {
          ls.debt = (int)std::any_cast<std::string>(student1.debt).length();
        }
      } else if (student1.debt.type() == typeid(int)) {
        std::string strdebt = std::to_string(std::any_cast<int>(student1.debt));
        if ((int)strdebt.length() + 6 > ls.debt) {
          ls.debt = (int)strdebt.length() + 6;
        }
      } else
        return Lengths{};
    }
    return ls;
  }

  bool output(const std::vector<Student>& students, std::ostream& os) {
    std::string s = "-";
    char dash;
    dash = s[0];
    std::string s2 = " ";
    char space;
    space = s2[0];

    os.fill(dash);
    os.setf(std::ios::left);
    Lengths ls = get_lengths(students);
    os << std::setw(ls.name + 5) << "|" << std::setw(ls.group + 7) << "|"
       << std::setw(ls.avg + 5) << "|" << std::setw(ls.debt + 5) << "|";
    os << "|" << std::endl;
    os.fill(space);
    os << std::setw(ls.name + 5) << "| name" << std::setw(ls.group + 7)
       << "| group" << std::setw(ls.avg + 5) << "| avg" << std::setw(ls.debt + 5)
       << "| debt";
    os << "|" << std::endl;

    for (Student student : students) {
      os.fill(dash);
      os << std::setw(ls.name + 5) << "|" << std::setw(ls.group + 7) << "|"
         << std::setw(ls.avg + 5) << "|" << std::setw(ls.debt + 5) << "|";
      os << "|" << std::endl;
      os.fill(space);
      std::string nametable = "| " + student.name;
      os << std::setw(ls.name + 5) << nametable;
      if (student.group.type() == typeid(int)) {
        std::string grouptable =
            "| " + std::to_string(std::any_cast<int>(student.group));
        os << std::setw(ls.group + 7) << grouptable;
      } else if (student.group.type() == typeid(std::string)) {
        std::string grouptable = "| " + std::any_cast<std::string>(student.group);
        os << std::setw(ls.group + 7) << grouptable;
      } else
        return false;

      if (student.avg.type() == typeid(int)) {
        std::string avgtable =
            "| " + std::to_string(std::any_cast<int>(student.avg));
        os << std::setw(ls.avg + 5) << avgtable;
      } else if (student.avg.type() == typeid(std::string)) {
        std::string avgtable = "| " + std::any_cast<std::string>(student.avg);
        os << std::setw(ls.avg + 5) << avgtable;
      } else if (student.avg.type() == typeid(float)) {
        std::string avgtable =
            "| " + std::to_string(std::any_cast<float>(student.avg));
        os << std::setw(ls.avg + 5) << avgtable;
      } else
        return false;

      if (student.debt.type() == typeid(std::string)) {
        std::string debttable = "| " + std::any_cast<std::string>(student.debt);
        os << std::setw(ls.debt + 5) << debttable;
      } else if (student.debt.type() == typeid(int)) {
        std::string debttable =
            "| " + std::to_string(std::any_cast<int>(student.debt)) + " items";
        os << std::setw(ls.debt + 5) << debttable;
      } else
        return false;
      os << "|" << std::endl;
    }
    os.fill(dash);
    os << std::setw(ls.name + 5) << "|" << std::setw(ls.group + 7) << "|"
       << std::setw(ls.avg + 5) << "|" << std::setw(ls.debt + 5) << "|";
    os << "|" << std::endl;
    return true;
  }