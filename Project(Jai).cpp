#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

// Base class to handle marks
class Marks {
public:
    float course_mark[3];  // Array to hold marks for theory, IT, and term work
    string type_of_marks[3] = {"Theory", "IT", "Term Work"};
    float total_max_marks;  // Maximum total marks for the subject

    void input_marks() {
        cout << "Enter the maximum total marks for the subject: ";
        cin >> total_max_marks;

        float total_marks_obtained = 0;
        for (int i = 0; i < 3; i++) {
            cout << "Enter marks obtained in " << type_of_marks[i] << ": ";
            cin >> course_mark[i];
            total_marks_obtained += course_mark[i];
        }

        while (total_marks_obtained > total_max_marks) {
            cout << "Invalid! The total marks cannot exceed " << total_max_marks << ". Enter again:\n";
            total_marks_obtained = 0;
            for (int i = 0; i < 3; i++) {
                cout << "Enter marks obtained in " << type_of_marks[i] << ": ";
                cin >> course_mark[i];
                total_marks_obtained += course_mark[i];
            }
        }
    }

    void display_marks() const {
        cout << "Total Maximum Marks: " << total_max_marks << endl;
        float total_obtained = 0;
        for (int i = 0; i < 3; i++) {
            cout << "Marks for " << type_of_marks[i] << ": " << course_mark[i] << endl;
            total_obtained += course_mark[i];
        }
        cout << "Total Marks Obtained: " << total_obtained << " / " << total_max_marks << endl;
    }

    float getTotalMarksObtained() const {
        float total_obtained = 0;
        for (int i = 0; i < 3; i++) {
            total_obtained += course_mark[i];
        }
        return total_obtained;
    }
};

// Class for subjects, inherits from Marks
class Subject : public Marks {
public:
    char name[50];  // Changed to fixed-size char array for binary compatibility
    char code[10];  // Same here
    int credits;

    Subject() : credits(0) {
        strcpy(name, "undefined");
        strcpy(code, "undefined");
    }

    void inputSubject() {
        cout << "Enter course name: ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "Enter course code: ";
        cin.getline(code, 10);
        cout << "Enter credits for the subject: ";
        cin >> credits;
        input_marks();
    }

    void displaySubject() const {
        cout << "Subject: " << name << " (Code: " << code << ")" << endl;
        display_marks();
        cout << "Credits: " << credits << endl;
    }
};

// Class to represent a semester
class Semester {
public:
    int subjectCount;
    char semesterName[50];
    Subject subjects[10];  // Assuming a maximum of 10 subjects per semester

    Semester() : subjectCount(0) {
        strcpy(semesterName, "Undefined");
    }

    void inputSemesterDetails(const string& semName) {
        strcpy(semesterName, semName.c_str());
        cout << "Enter number of subjects in " << semName << ": ";
        cin >> subjectCount;
        for (int i = 0; i < subjectCount; i++) {
            cout << "\nEntering details for subject " << (i + 1) << ":\n";
            subjects[i].inputSubject();
        }
    }

    void displaySemester() const {
        cout << "Semester: " << semesterName << endl;
        for (int i = 0; i < subjectCount; i++) {
            subjects[i].displaySubject();
        }
    }

    double calculateCGPA() const {
        double totalPoints = 0.0;
        int totalCredits = 0;

        for (int i = 0; i < subjectCount; i++) {
            double gradePoint = subjects[i].getTotalMarksObtained() / subjects[i].total_max_marks * 10; // Simplified CGPA logic
            totalPoints += gradePoint * subjects[i].credits;
            totalCredits += subjects[i].credits;
        }

        return (totalCredits > 0) ? (totalPoints / totalCredits) : 0.0;
    }
};

// Class to handle student result
class StudentResult {
public:
    char studentName[50], rollNo[20], branch[50];
    int semesterCount;
    Semester semesters[2];  // Assuming two semesters per academic year

    StudentResult() : semesterCount(2) {}

    void inputStudentDetails() {
        cout << "Enter student name: ";
        cin.ignore();
        cin.getline(studentName, 50);
        cout << "Enter roll number: ";
        cin.getline(rollNo, 20);
        cout << "Enter branch: ";
        cin.getline(branch, 50);
    }

    void inputSemesterDetails() {
        for (int i = 0; i < semesterCount; i++) {
            semesters[i].inputSemesterDetails("Semester " + to_string(i + 1));
        }
    }

    void displayStudentDetails() const {
        cout << "Student: " << studentName << " (" << rollNo << "), Branch: " << branch << endl;
        for (int semIdx = 0; semIdx < semesterCount; semIdx++) {
            semesters[semIdx].displaySemester();
        }
    }

    // Saving to a unique file for each student
    void saveToFile() const {
        string fileName = string(rollNo) + "_result.dat";
        ofstream file(fileName, ios::binary);
        if (!file) {
            cout << "Error creating file!" << endl;
            return;
        }
        file.write(reinterpret_cast<const char*>(this), sizeof(StudentResult));
        file.close();
        cout << "Result saved to file: " << fileName << endl;
    }

    // Loading from file based on roll number
    bool loadFromFile(const string& rollNumber) {
        string fileName = rollNumber + "_result.dat";
        ifstream file(fileName, ios::binary);
        if (!file) {
            cout << "Error opening file: " << fileName << endl;
            return false;
        }
        file.read(reinterpret_cast<char*>(this), sizeof(StudentResult));
        file.close();
        return true;
    }

    // Edit marks in the student's specific file
    void editMarks() {
        string fileName = string(rollNo) + "_result.dat";
        fstream file(fileName, ios::binary | ios::in | ios::out);
        if (!file) {
            cout << "Error opening file!" << endl;
            return;
        }

        string subjectName;
        cout << "Enter the semester (1 or 2): ";
        int semesterIndex;
        cin >> semesterIndex;
        semesterIndex--;

        cout << "Enter the subject name to edit: ";
        cin.ignore();
        getline(cin, subjectName);

        for (int i = 0; i < semesters[semesterIndex].subjectCount; i++) {
            if (string(semesters[semesterIndex].subjects[i].name) == subjectName) {
                cout << "Enter new marks for " << subjectName << ":\n";
                semesters[semesterIndex].subjects[i].input_marks();

                file.seekp(0);
                file.write(reinterpret_cast<const char*>(this), sizeof(StudentResult));  // Overwrite the file
                cout << "Marks updated successfully!" << endl;
                file.close();
                return;
            }
        }
        cout << "Subject not found!" << endl;
        file.close();
    }
};

int main() {
    StudentResult sr;
    string rollNo;
    int choice;

    while (true) {
        cout << "\nExamination Result System Menu:";
        cout << "\n1. Enter result details and display";
        cout << "\n2. Edit marks of a student and display";
        cout << "\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                sr.inputStudentDetails();
                sr.inputSemesterDetails();
                sr.saveToFile();
                sr.displayStudentDetails();
                break;
            case 2:
                cout << "Enter roll number to edit marks: ";
                cin >> rollNo;
                if (sr.loadFromFile(rollNo)) {
                    sr.editMarks();
                    sr.displayStudentDetails();
                } else {
                    cout << "Student not found!" << endl;
                }
                break;
            case 0:
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
