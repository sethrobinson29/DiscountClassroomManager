/*
    Container class to hold name of assignments
*/

#ifndef Assignment_h
#define Assignment_h
#include "header_stack.h"

class assignment{
private:
    std::string name;

public:
    assignment(){
        name = "quiz1";
    }
    void set_name(std::string s);
    std::string get_name();

};

#endif 
