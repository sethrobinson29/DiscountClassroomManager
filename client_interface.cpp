#include "header_stack.h"
using namespace std;

std::vector<int> cur_roster;													//keep track of current roster of current course
int cur_course = 0, cur_user = 0;												//keep track of current course and current user

void loginMenu(package &package);	
void failedLogin(int socket);
void adminMenu(package &package);
void teacherMenu(package &package);
void classroomMenu(int sockfd, package &package);
bool isInt(const string& s);
bool isFloat(string& s);
void grade_loop(package&);

/*
	Client-side logic of Classroom Management Application
	fp should point to stdin, sockfd is socket descriptor
*/
void client_interface(FILE *fp, int sockfd){
	ssize_t n, err;
	package package;															//structure to pass data across socket
	
	//validate user credentials
	loginMenu(package);
	
	do{
		write(sockfd, &package, sizeof(package));
		// Clear the package.message buffer 
		memset(package.message, '\0', MAXLINE);
		if( n = read(sockfd, &package, sizeof(package)) < 0){
			cerr << "server_interface: read error\n";
			exit(1);
		} else if (n < 0 && err == EINTR) {
			continue;
		}

		fputs(package.message, stdout);
		// Clear the package.message buffer 
		memset(package.message, '\0', MAXLINE);

		switch(package.code){
			case 91:{	//repeat user validation
				loginMenu(package);
				} break;
			case 92:{ 	//display admin menu
				adminMenu(package);
				} break;
			case 93:{	//display teacher menu
				teacherMenu(package);
			}	break;

			//PACKAGE CODE 94 WILL BREAK THE DO-WHILE LOOP AND END THE PROGRAM
			case 94: {
				std::cout << "Session Terminated.\n";
			} break;

			case 95: {	//display classroom menu
				classroomMenu(sockfd, package);
			} break;
			case 322: {	//special case for entering grades of students
				grade_loop(package);
			} break;
			case 555:{	//Failed Login Attempt Exit
				failedLogin(sockfd);
			} break;
			default: {
				std::cout << "BASE CASE\n";
				package.code = 94;
			} break;
		}
	}while(package.code != 94);
	
	shutdown(sockfd, SHUT_RDWR);		//close connection
	exit(0);
}

/* Present the user with the Login Menu. Validate the user input with the */
/* isInt and isFloat functions. From this menu, the user is able to      */
/* access an existing account, whether that be a teacher or an          */
/* admin account, or exit the program                                  */
void loginMenu(package &package){
	string selection;
  
	start_main:
	cout << "\n*****  LOGIN MENU  *****\n\n";
	cout << "1. Access Account\n";
	cout << "2. Exit\n\n";
	cout << "Make Selection: ";
	cin >> selection;
 
	if(!isInt(selection))
		selection = "99"; // Always select the default case
	
	switch(atoi(selection.c_str())){
	
		case 1:{
			package.code = 11;
			int userID;
			char *a1;
			
			cout << "\nEnter your user ID: ";
			cin >> userID;

			string pass(a1 = getpass("\nEnter your password: "));
			package.id = userID; 
			cur_user = userID;
			
			memset(package.message, '\0', MAXLINE);
			pass.copy(package.message, pass.length());
			
			} break;
		
		// Exit
		case 2:{
			package.code = 66;
			} break;
		
		default:{
			cout << "\nInvalid Selection, please choose again.\n";
			goto start_main;
			}
		}
}

//closes connection if login fails too many times
void failedLogin(int socket){
	cout << "You have failed the login attempt many times. \n";
	cout << "Please try again!" << endl;
	shutdown(socket, SHUT_RDWR);
	exit(0);
}

/*
	Admin Menu allows administrator users to add and remove teachers from the global variables
		that hold the teacher information. It also allows them to add students to the global
		variables that hold the student information.
*/
void adminMenu(package &package){
	string selection;
	int tempID;
	std::string answer, temp;

	start_admin_account:
	cout << "\n*****  ADMIN ACCESS MENU  *****\n\n";
	cout << "1. Create A Teacher\n";
	cout << "2. Delete A Teacher\n";
	cout << "3. Create A Student\n";
	cout << "4. Delete A Student\n";
	cout << "5. Exit to Main Menu\n\n";
	cout << "Make Selection: ";
	cin >> selection;

	if(!isInt(selection))
		selection = "99"; // Always select the default case
	
	switch(atoi(selection.c_str())){
		
		// Create A Teacher
		case 1:{
			cout << "\nEnter Teacher's first name: ";
			cin >> package.firstName;
			cout << "\nEnter Teacher's last name: ";
			cin >> package.lastName;
			cout << "\nEnter Teacher's password: ";
			cin >> package.password;

			do{
				std::cout << "\nEnter Teacher's ID: \n";
				std::cin >> tempID;
					if(tempID < 30000 && tempID > 20000){
						package.id = tempID;
					}else{
						std::cout << "Error: Teacher's ID isn't in the correct field range.\n";
						std::cout << "Note: Teacher ID's must be greater than 20000 and less than 30000\n";
						std::cout << "Do you want to try again?\n";
						std::cin >> answer;
					}
				}while(answer == "Y"|| answer == "y" || answer == "Yes" || answer == "yes");

			package.code = 101;
		} break;

		// Delete A Teacher	
		case 2:{
			cout << "\nEnter ID of Teacher you want to delete: ";
			cin >> package.id;
			package.code = 111;
		} break;

		// Create A Student
		case 3:{
      cout << "\nEnter first name of student: ";
      cin >> temp;
      temp.copy(package.firstName, temp.length());

      cout << "\nEnter last name of student; ";
      cin >> temp;
      temp.copy(package.lastName, temp.length());

      cout << "\nEnter the password of the student: ";
      cin >> temp;
      temp.copy(package.password, temp.length());

      do{
			std::cout << "\nEnter student's ID: \n";
			std::cin >> tempID;
			if(tempID < 40000 && tempID > 30000){
				package.id = tempID;
			}else{
				std::cout << "Error: Student's ID isn't in the correct field range.\n";
				std::cout << "Note: Student ID's must be greater than 30000 and less than 40000\n";
				std::cout << "Do you want to try again?\n";
				std::cin >> answer;
			}
		}while(answer == "Y"|| answer == "y" || answer == "Yes" || answer == "yes");

      package.code = 121;
			} break;

		// Delete A Student
		case 4:{
      cout << "\nEnter ID of Student you want to delete: ";
			cin >> package.id;
			package.code = 131;
			} break;
		
		// Exit to Login Menu
		case 5:{
			package.code = 141;
			} break;
		
		// Invalid input will goto the Account Menu
		default:{
			cout << "\nInvalid menu selection! Please choose again.\n";
			goto start_admin_account;
			}
		}
	return;
}

/*
	Teacher Menu allows teacher users to add and delete classes to the courses they teach. They can also
		access existing courses to add assignments and grades, as well as add and remove students from the course
*/
void teacherMenu(package &package){
	string selection;
	int course_num;

	start_teacher:  
	cout << "\n***** TEACHER MENU *****\n\n";
	cout << "1. Access A Classroom\n";
	cout << "2. Create A Classroom\n";
	cout << "3. Delete A Classroom\n";
	cout << "4. Exit to Login Menu\n\n";
	cout << "Make Selection: ";
	cin >> selection;

	if(!isInt(selection))
		selection = "99"; // Always select the default case
	
	switch(atoi(selection.c_str())){
		
		// Access A Classroom
		case 1:{
			cout << "\nEnter the classroom number: ";
			cin >> course_num;
			package.courseNumber = course_num;
			cur_course = course_num;
			package.code = 201;
		} break;
		
		// Create A Classroom
		case 2:{
			cout << "\nEnter the classroom number: ";
			cin >> course_num;
			package.courseNumber = course_num;
			cout << "\nEnter the classroom name: ";
			std::string tmp;
			cin >> tmp;
			tmp.copy(package.message, tmp.length());
    
			package.code = 211;
		} break;

		// Delete A Classroom
		case 3:{
			cout << "\nEnter the number of the classroom you want to delete: ";
			cin >> package.id;
			package.code = 221;
		} break;
    
		case 4:{
			package.code = 141;
			} break;
    
		default:{
			cout << "\nInvalid menu selection! Please choose again.\n";
			goto start_teacher;
		}
	}
}


void classroomMenu(int sockfd, package &package) {

  string selection, assignmentName;
  char answer = 'y';
  int studentID = 0, grade = 0, studentCounter = 0;

  // Try and save the course number from package into this menu
  
  start_classroom:  
	cout << "\n***** CLASSROOM MENU *****\n\n";
  	cout << "1. Add A Student To Classroom\n"; // code 301
	cout << "2. Edit An Assignment\n"; // code 311
	cout << "3. Create An Assignment\n"; // code 321
	cout << "4. Delete An Assignment\n"; // code 331
	cout << "5. Print Roster\n"; // code 341
  	cout << "6. Print Grades for an assignment\n"; // code 351
	cout << "7. Print Class Averages\n"; //code 371
  	cout << "8. Exit to Teacher Menu\n\n"; // code 361
	cout << "Make Selection: ";
	cin >> selection;

	if(!isInt(selection))
		selection = "99"; // Always select the default case
	
	switch(atoi(selection.c_str())){

    // Add A Student To Classroom
    case 1:{
      	cout << "\nEnter the ID of the student you want to add: ";
      	cin >> studentID;
      	cur_roster.push_back(studentID);
		package.id = studentID;
      
      	package.code = 301;
    } break;
    
		// Edit An Assignment
		case 2:{
			cout << "\nEnter the assignment name: ";
			cin.ignore(1);
			std::getline(cin, assignmentName);	
      		assignmentName.copy(package.message, assignmentName.length());
      		cout << "\nEnter the student's ID: ";
      		cin >> studentID;
			package.id = studentID;
			cout << "\nEnter grade of " << studentID << ": ";
			cin >> grade;
			package.courseNumber = grade; 			//temp container for student grade
			package.code = 311;
		} break;
		
		// Create An Assignment
		case 3:{
			cout << "\nEnter the assignment name: ";
			cin.ignore(1);
			std::getline(cin, assignmentName);								//TODO: change other name gathering statements to get entire line
			assignmentName.copy(package.message, assignmentName.length());
			package.code = 321;
			} break;

		// Delete An Assignment
		case 4:{
			cout << "\nEnter the name of the assignment you want to delete: ";
			cin.ignore(1);
			std::getline(cin, assignmentName);	
			memset(package.message, '\0', MAXLINE);
      		assignmentName.copy(package.message, assignmentName.length());
			package.code = 331;
		} break;
    
    // Print Roster
		case 5:{
			package.code = 341;
		} break;
    
    // Print Grades for an assignment
		case 6:{
			std::cout << "Enter assignment to print grades of: ";
			cin.ignore(1);
			std::getline(cin, assignmentName);
			assignmentName.copy(package.message, assignmentName.length());

			package.code = 351;
		} break;

		//print average grades for entire course
		case 7: {
			package.code = 371;
		} break;
    
		case 8:{  // Exit To Teacher Menu
      		cur_roster.erase(cur_roster.begin(), cur_roster.end());  // cleans the current classroom roster when leaving
			package.code = 361;
			} break;
		default:{
			cout << "\nInvalid menu selection! Please choose again.\n";
			goto start_classroom;
			}
		}
}

//repeating function to get id of student and pass back to server
void grade_loop(package& package) {
	int x = 0;
	std::cout << "\nEnter grade for " << std::to_string(package.id) << " on " << package.message << ": ";

	cin >> x;
	package.courseNumber = x;	//temp use to pass grade value
	package.code = 322;
}

// Test if string correlates to an integer
bool isInt(const std::string& s){
	for(int i = 0; i < s.length(); i++){
		if(!isdigit(s[i]))
			return false;
	}
	return true;
}

// Test if a string correlates to a float
bool isFloat(std::string& s){
	bool dec = false;
	int count = -1;
	std::string temp;

	for(int i = 0; i < s.length(); i++){
		if(s[i] == '.' && dec == false){
			dec = true;
			count++;
		}
		else if(s[i] == '.' && dec == true)
			return false;
		else if(isdigit(s[i]) && dec == true)
			count++;
		else if(!isdigit(s[i]))
			return false;
	}

	if(count > 2){
		temp = s.substr(0, s.length() - count+2);
		s.assign(temp);
	}
	return true;
}