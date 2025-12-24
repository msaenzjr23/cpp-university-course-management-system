 #ifndef COURSE_H
 #define COURSE_H
 
 #include <queue>
 #include <set>
 #include <string>
 #include <utility>
 #include <vector>
 
 class Course {
 public:
     Course() = default;
     Course(std::string code, std::string title, std::string instructor, std::size_t capacity);
 
     const std::string &getCode() const;
     const std::string &getTitle() const;
     const std::string &getInstructor() const;
     std::size_t getCapacity() const;
     std::size_t enrolledCount() const;
 
     void setTitle(const std::string &newTitle);
     void setInstructor(const std::string &newInstructor);
     void setCapacity(std::size_t newCapacity);
 
     const std::set<std::string> &getEnrolledStudents() const;
     const std::queue<std::string> &getWaitlist() const;
 
     bool isEnrolled(const std::string &studentId) const;
     bool isInWaitlist(const std::string &studentId) const;
     bool removeFromWaitlist(const std::string &studentId);
 
     bool addStudent(const std::string &studentId);
     bool removeStudent(const std::string &studentId);
     void addToWaitlist(const std::string &studentId);
     std::pair<bool, std::string> popFromWaitlist();
 
 private:
     std::string code;
     std::string title;
     std::string instructor;
     std::size_t capacity{0};
     std::set<std::string> enrolledStudentIds;
     std::queue<std::string> waitlist;
 };
 
 #endif
