#include <bits/stdc++.h>
using namespace std;

class Course {
protected:
    string name, code;
public:
    void inputCourse() {
        cout << "Enter course name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter course code: ";
        getline(cin, code);
    }
};

class Marks {
protected:
    int totalMarksEarned = 0, totalMarks = 200;
    string markArr[8] = {"Credit", "Theory", "IT", "Termwork", "Theory total", "Practical", "Oral", "Total"};
    string DisplayMarkArr[10] = {"Credit", "Theory", "IT", "Termwork", "Theory total", "Practical", "Oral", "Total", "Grade", "Grade Point"};
    int markValue[8];
    string grades[2];
public:
    void inputMarks() {
        for (int i = 0; i < 8; i++) {
            cout << "Enter marks for " << markArr[i] << ": ";
            cin >> markValue[i];
        }
        for (int i = 1; i <= 6; i++) {
            if (markValue[i] != -1) {
                totalMarksEarned += markValue[i];
            } else totalMarks -= 25;
        }
        totalMarksEarned -= markValue[4];
    }

    void calcGrade() {
        if (totalMarksEarned >= 128) {
            grades[0] = "O"; grades[1] = "10";
        } else if (totalMarksEarned >= 107) {
            grades[0] = "A"; grades[1] = "9";
        } else if (totalMarksEarned >= 91) {
            grades[0] = "B"; grades[1] = "8";
        } else if (totalMarksEarned >= 70) {
            grades[0] = "C"; grades[1] = "7";
        } else if (totalMarksEarned >= 56) {
            grades[0] = "D"; grades[1] = "6";
        } else if (totalMarksEarned >= 40) {
            grades[0] = "E"; grades[1] = "5";
        } else {
            grades[0] = "F"; grades[1] = "0";
        }
    }
    int getTotalMarksEarned() const {
        return totalMarksEarned;
    }

    int getTotalMarks() const {
        return totalMarks;
    }

};

class Semester {
public:
    string semesterNo;
    int subjectCount;
    vector<Course*> subjects;
    Marks m;

    Semester() = default;

    Semester(string semName) : semesterNo(semName), subjectCount(0) {}

    void inputSemesterDetails() {
        cout << "Enter number of subjects in Semester " << semesterNo << ": ";
        cin >> subjectCount;
        subjects.resize(subjectCount);
        cin.ignore();
        for (int i = 0; i < subjectCount; i++) {
            cout << "\nEntering details for subject " << (i + 1) << ":\n";
            subjects[i] = new Course();
            subjects[i]->inputCourse();
            m.inputMarks();
        }
    }

    ~Semester() {
        for (Course* course : subjects) {
            delete course;
        }
    }
};

class Student : public virtual Marks, virtual public Course, public Semester {
private:
    string name, roll, seatno, program, status;
public:
    float sgpa;
    vector<Semester*> semesters;

    Student() = default;

    void inputStudentDetails() {
        cout << "Enter student name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter roll number: ";
        getline(cin, roll);
        cout << "Enter seat number: ";
        getline(cin, seatno);
        cout << "Enter program: ";
        getline(cin, program);
        cout << "Enter semester No: ";
        string semesterNo;
        getline(cin, semesterNo);
        semesters.push_back(new Semester(semesterNo));
        semesters.back()->inputSemesterDetails();
    }

    void calculateSGPA() {
        float totalSGPA = 0;
        int totalCredits = 0;
        for (Semester* sem : semesters) {
            int earnedCredits = 0;
            for (Course* course : sem->subjects) {
                earnedCredits += markValue[0]; // Assuming markValue[0] is credits
            }
            totalSGPA += (earnedCredits * getTotalMarksEarned()) / (float) getTotalMarks();
            totalCredits += earnedCredits;
        }
        sgpa = totalSGPA / totalCredits;
    }

    ~Student() {
        for (Semester* sem : semesters) {
            delete sem;
        }
    }
};

void showMenu() {
    cout << "\nExamination Result System Menu:";
    cout << "\n1. Enter student details, semester details, and marks";
    cout << "\n2. Calculate SGPA and display";
    cout << "\n0. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    Student s;
    int choice;
    bool studentDetailsEntered = false;

    while (true) {
        showMenu();
        cin >> choice;
        switch (choice) {
            case 1:
                s.inputStudentDetails();
                studentDetailsEntered = true;
                break;
            case 2:
                if (!studentDetailsEntered) {
                    cout << "Please enter student, semester, and marks details first.\n";
                    break;
                }
                s.calculateSGPA();
                cout << "SGPA: " << s.sgpa << endl;
                break;
            case 0:
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
