#ifndef Teacher_h
#define Teacher_h

#include "header_stack.h"
#include "Classroom.h"

class teacher{
private:
    std::string first_name{}, last_name{};
    std::string department_name{};
    std::string email{};
    int teacher_id{};        //00000
    std::string password{};

    std::unordered_map<std::string, classroom> classes;
public:
    teacher(){
        this->first_name = "test";
    };

    teacher(std::string fname, std::string lname, int id, std::string pass){
        this->first_name = fname; this->last_name = lname;
        this->teacher_id = id;
        this->password = pass;
    };
    
    void set_first_name();
    void set_last_name();
    void set_department_name();
    void set_email();
    void set_password();
    void set_teacher_id();
    void add_class(classroom);
    
    std::string get_first_name();
    std::string get_last_name();
    std::string get_department_name();
    std::string get_email();
    std::string get_password();
    int get_teacher_id();
    classroom get_class(std::string);
};

#endif /* Teacher_h */
