#ifndef Student_h
#define Student_h

#include "header_stack.h"

class student{
private:
    std::string first_name{}, last_name{};
    std::string grade_level{};    // ie: junior, senior, ect
    std::string email{};
    int student_id{};        //00000
    std::string password{};
    //TODO: give classroom vector for easy access to finding grades

public:
    student(){
        first_name = "empty";
    }
    student(std::string f_name, std::string l_name, int id){
        first_name = f_name; last_name = l_name;
        this->student_id = id;
    }
    void set_first_name();
    void set_last_name();
    void set_grade_level();
    void set_email();
    void set_password();
    void set_student_id();
    
    std::string get_first_name();
    std::string get_last_name();
    std::string get_grade_level();
    std::string get_email();
    std::string get_password();
    int get_student_id();
};

#endif /* Student_h */
