#include "Student.h"

// student functions
void student::set_first_name( )
{
	std::string temp;
	std::cout << "Enter student's first name: ";
	std::cin >> temp;
	
	student::first_name = temp;
}
void student::set_last_name( )
{
	std::string temp;
	std::cout << "Enter student's last name: ";
	std::cin >> temp;
	
	student::last_name = temp;
}
void student::set_grade_level( )
{
	std::string temp;
	std::cout << "Enter student's grade level: ";
	std::cin >> temp;
	
	student::grade_level = temp;
}
void student::set_email( )
{
	std::string temp;
	std::cout << "Enter student's email: ";
	std::cin >> temp;
	
	student::email = temp;
}
void student::set_password( )
{
	std::string temp;
	std::cout << "Enter student's password: ";
	std::cin >> temp;
	
	student::password = temp;
}
void student::set_student_id( )
{
	int temp;
	std::cout << "Enter student's ID: ";
	std::cin >> temp;
	
	student::student_id = temp;
}
	
std::string student::get_first_name( )
{
	return this->first_name;
}
std::string student::get_last_name( )
{
	return this->last_name;
}	
std::string student::get_grade_level( )
{
	return this->grade_level;
}
std::string student::get_email( )
{
	return this->email;
}
std::string student::get_password( )
{
	return this->password;
}
int student::get_student_id( )
{
	return this->student_id;
}
