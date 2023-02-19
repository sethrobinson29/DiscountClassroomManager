#include "Teacher.h"

// teacher functions
void teacher::set_first_name( )
{
	std::string temp;
	std::cout << "Enter teacher's first name: ";
	std::cin >> temp;
	
	teacher::first_name = temp;
}
void teacher::set_last_name( )
{
	std::string temp;
	std::cout << "Enter teacher's last name: ";
	std::cin >> temp;
	
	teacher::last_name = temp;
}
void teacher::set_department_name( )
{
	std::string temp;
	std::cout << "Enter teacher's department: ";
	std::cin.ignore(90000, '\n');
  std::getline(std::cin, temp);
	
	teacher::department_name = temp;
}
void teacher::set_email( )
{
	std::string temp;
	std::cout << "Enter teacher's email: ";
	std::cin >> temp;
	
	teacher::email = temp;
}
void teacher::set_password( )
{
	std::string temp;
	std::cout << "Enter teacher's password: ";
	std::cin >> temp;
	
	teacher::password = temp;
}
void teacher::set_teacher_id( )
{
	int temp;
	std::cout << "Enter teacher's ID: ";
	std::cin >> temp;
	
	teacher::teacher_id = temp;
}

void teacher::add_class(classroom course) {

  std::string temp;
	std::cout << "Enter name of class: ";
	std::cin.ignore(90000, '\n');
  std::getline(std::cin, temp);
  
  teacher::classes.insert(make_pair(temp, course));
}
	
std::string teacher::get_first_name( )
{
	return teacher::first_name;
}
std::string teacher::get_last_name( )
{
	return teacher::last_name;
}
std::string teacher::get_department_name( )
{
	return teacher::department_name;
}
std::string teacher::get_email( )
{
	return teacher::email;
}
std::string teacher::get_password( )
{
	return teacher::password;
}
int teacher::get_teacher_id( )
{
	return teacher::teacher_id;
}

classroom teacher::get_class(std::string course) {
  return teacher::classes[course];
}
