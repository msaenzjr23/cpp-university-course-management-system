 #ifndef STUDENT_H
 #define STUDENT_H
 
 #include <list>
 #include <set>
 #include <string>
 
 class Student {
 public:
     Student() = default;
     Student(std::string id, std::string name);
 
     const std::string &getId() const;
     const std::string &getName() const;
     void setName(const std::string &newName);
 
     const std::set<std::string> &getCourses() const;
     const std::list<std::string> &getCourseOrder() const;
 
     bool enrollInCourse(const std::string &courseCode);
     bool dropCourse(const std::string &courseCode);
 
 private:
     std::string id;
     std::string name;
     std::set<std::string> enrolledCourseCodes;
     std::list<std::string> enrollmentOrder;
 };
 
 #endif
