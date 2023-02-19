# DiscountClassroomManager
A very bare bones classroom management application made by my group of 4 as a final project for Network Programming. 


Our implementation of a client-server application allows users to create and manage teachers, students, courses, and assignments. This project was made by Seth Robinson, Robert Bausch, Ian Anderson, and Gabriel Ruiz for our Network Programming class. 


## How To Compile

A makefile is already provided to you so simply just go into the correct directory and type
> ****make****

This will automatically compile all files for you.


## How to Run the Program for Testing

1. Unzip the files.

2. Open the terminal in the application directory.

3. Type "make"

4. Run the program "server"

5. Open another terminal in the same folder.
**Note**: this step can be skipped if you use "server&" in the previous command, but you will be unable to see server outputs for testing 

6. Then run the command "client 127.0.0.1"


## Using the Program

**Initially, you must add a folder named "Data Files" to the application directory and add Courses.txt, Logins.txt, Students.txt, and Teachers.txt to the folder. From here, the program will create the individual classroom folders when you create a classroom.**

To start off using the program after you connect you will be directed to a login menu.
The login Menu will have 2 choices:

1.)Access Account
	-To access your account you enter the User ID and your Password

2.)Exit
**Note:** If you fail to log in a certain amount of times you will be shut out of the program.
**Admin Login:** Username: 11111 Password: admin

Depending who you log in as you will be given different menus.
Below is the menu you will get if you log in as an admin.
As admin you will be directed to this menu that will give you the options of either:

1.)Create A Teacher
	-To create a teacher you will be prompted to give the teacher's first and last name, password, and teacher ID.
**Note:** When creating a teacher, you will be prompted to ensure that their teacher ID falls between 20,001 through 29,999. If not you will be given an error message and asked to repeat the process.

2.)Delete A Teacher
	-To delete a teacher you will enter the teachers ID and it will be deleted.

3.)Create A Student
	-To create a student you will be prompted to give the student's first and last name, password, and student ID.
**Note:** When creating a student, you will be prompted to ensure that their student ID falls between 30,001 through 39,999. If not you will be given an error message and asked to repeat the process.

4.)Delete A Student
	-To delete a student you will enter the students ID and it will be deleted.
or

5.)Exit to Main Menu

Now that a teacher has been created you may now log in as them and use the teachers menu.
The teachers menu will give you the options to:

1.)Access A Classroom
	-To be able to access a classroom, you first have to create one. 

2.)Create A Classroom
	-To create a classroom you will input a classroom number and classroom name.
**Note**: When a classroom is created, a folder and two files, that store the Assignements and Roster for the class will be created. 

3.)Delete A Classroom
	-To delete a classroom you just enter the classroom name.

4.)Exit to Login Menu

After creating a classroom you now can use the  "Access A Classroom"  menu choice.
This choice will bring you to the classroom menu where you will be given the options to:

1.)Add A Student To Classroom
	-To add a student to a classroom you just have to enter the student's id.

2.)Edit An Assignment
	-To edit an assignment you need the assignment name. Then you will need to input the student's id and grade.

3.)Create An Assignment
	-To create an assignment you just have to enter an assignment name.

4.)Delete An Assignment
	-To delete an assignment you just need to enter the assignment name.

5.)Print Roster
	-Once you choose this option it will automatically display all students that are in the classroom.

6.)Print Grades for an assignment
	-To print grades you just need to enter the assignment name that you want to print the grades of.

7.)Print Class Averages
	-Once you choose this option it will automatically print the average grades for the entire course.

8.)Exit to Teacher Menu
	-Returns you to the teachers menu.
