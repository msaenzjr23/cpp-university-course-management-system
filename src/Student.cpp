 #include "Student.h"
 
 Student::Student(std::string id, std::string name) : id(std::move(id)), name(std::move(name)) {}
 
 const std::string &Student::getId() const {
     return id;
 }
 
 const std::string &Student::getName() const {
     return name;
 }
 
 void Student::setName(const std::string &newName) {
     name = newName;
 }
 
 const std::set<std::string> &Student::getCourses() const {
     return enrolledCourseCodes;
 }
 
 const std::list<std::string> &Student::getCourseOrder() const {
     return enrollmentOrder;
 }
 
 bool Student::enrollInCourse(const std::string &courseCode) {
     auto inserted = enrolledCourseCodes.insert(courseCode);
     if (!inserted.second) {
         return false;
     }
     enrollmentOrder.push_back(courseCode);
     return true;
 }
 
 bool Student::dropCourse(const std::string &courseCode) {
     auto erased = enrolledCourseCodes.erase(courseCode);
     if (erased == 0) {
         return false;
     }
 
     for (auto it = enrollmentOrder.begin(); it != enrollmentOrder.end(); ++it) {
         if (*it == courseCode) {
             enrollmentOrder.erase(it);
             break;
         }
     }
     return true;
 }
