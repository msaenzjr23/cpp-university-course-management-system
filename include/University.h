 #ifndef UNIVERSITY_H
 #define UNIVERSITY_H
 
 #include "Course.h"
 #include "Student.h"
 
 #include <deque>
 #include <map>
 #include <string>
 #include <utility>
 #include <vector>
 
 class University {
 public:
     bool addStudent(const std::string &id, const std::string &name);
     bool updateStudentName(const std::string &id, const std::string &newName);
     bool removeStudent(const std::string &id);
 
     bool addCourse(const std::string &code, const std::string &title, const std::string &instructor, std::size_t capacity);
     bool updateCourse(const std::string &code, const std::string &title, const std::string &instructor, std::size_t capacity);
     bool removeCourse(const std::string &code);
 
     bool enrollStudentInCourse(const std::string &studentId, const std::string &courseCode);
     bool dropStudentFromCourse(const std::string &studentId, const std::string &courseCode);
 
     std::vector<std::string> listCoursesForStudent(const std::string &studentId) const;
     std::vector<std::string> listStudentsInCourse(const std::string &courseCode) const;
     std::vector<std::string> listWaitlistForCourse(const std::string &courseCode) const;
 
     std::pair<bool, Student> getStudent(const std::string &id) const;
     std::pair<bool, Course> getCourse(const std::string &code) const;
     std::vector<std::pair<std::string, std::string>> searchCoursesByInstructor(const std::string &instructorQuery) const;
 
     std::deque<std::string> getActivityLog() const;
 
 private:
     std::map<std::string, Student> students;
     std::map<std::string, Course> courses;
     std::deque<std::string> activityLog;
     std::size_t logLimit = 100;
 
     void logActivity(const std::string &entry);
     static std::string currentTimestamp();
     bool studentExists(const std::string &id) const;
     bool courseExists(const std::string &code) const;
     void cleanStudentFromWaitlists(const std::string &studentId);
 };
 
 #endif
