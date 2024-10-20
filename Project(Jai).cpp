#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

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
    string name;
    string code;
    int credits;

    Subject() : credits(0), name("undefined"), code("undefined") {}

    void inputSubject() {
        cout << "Enter course name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter course code: ";
        getline(cin, code);
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
    string semesterName;
    Subject subjects[10];  // Assuming a maximum of 10 subjects per semester

    Semester() : subjectCount(0) {}

    void inputSemesterDetails(const string& semName) {
        semesterName = semName;
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
    string studentName, rollNo, branch;
    int semesterCount;
    Semester semesters[2];  // Assuming two semesters per academic year

    StudentResult() : semesterCount(2) {}

    void inputStudentDetails() {
        cout << "Enter student name: ";
        cin.ignore();
        getline(cin, studentName);
        cout << "Enter roll number: ";
        getline(cin, rollNo);
        cout << "Enter branch: ";
        getline(cin, branch);
    }

    void inputSemesterDetails() {
        for (int i = 0; i < semesterCount; i++) {
            semesters[i].inputSemesterDetails("Semester " + to_string(i + 1));
        }
    }

    void displayStudentDetails() const {
        cout << setw(200) << setfill('_') << "" << endl; // line
        cout << setfill(' ') << "|" << setw(108) << "\e[1mGovernment of Goa\e[0m" << setw(99) << "|" << endl; //text middle
        cout << left << "|" << setw(199) << right << "|" << endl; //free
        cout << setfill(' ') << left << "|" << setw(113) << "\e[1mGoa College Of Engineering\e[0m" << setw(94) << right << "|" << endl; //text middle
        cout << setfill(' ') << "|" << setw(110) << "\e[1mFarmaguddi Ponda Goa\e[0m" << setw(97) << "|" << endl; //text middle
        cout << setw(200) << setfill('_') << "" << endl; //line

        // Student details
        cout << "|" << setw(6) << left << "\e[1mName: \e[0m" << setw(192) << left << studentName << "|" << endl; // free
        cout << "|" << setw(13) << left << "\e[1mRoll Number: \e[0m" << setw(185) << left << rollNo << "|" << endl; // free
        cout << "|" << setw(9) << left << "\e[1mProgram: \e[0m" << setw(189) << left << branch << right << "|" << endl; // free
        cout << setw(200) << setfill('_') << "" << endl; // line

        for (int semIdx = 0; semIdx < semesterCount; semIdx++) {
            cout << setfill(' ') << "|" << setw(118) << "\e[1mExamination: " + semesters[semIdx].semesterName + "\e[0m" << setw(89) << "|" << endl; //text middle
            cout << setw(200) << setfill('_') << "" << endl;

            // Header row for subjects and marks
            cout.fill(' ');
            cout << "|" << setw(17) << left << "Code" << "|" << setw(50) << left << "Course" << "|";
            for (int i = 0; i < 3; i++)
                cout << setw(15) << left <<semesters[semIdx].subjects[0].type_of_marks[i] << "|";  // Theory, IT, Term Work
            cout << setw(12) << left << "Total Marks" << "|";
            cout << setw(12) << left << "Credits" << "|";
            cout << endl;
            cout << setw(200) << setfill('_') << "" << endl; // line

            // Displaying each subject's marks
            for (int subIdx = 0; subIdx < semesters[semIdx].subjectCount; subIdx++) {
                const Subject& subject = semesters[semIdx].subjects[subIdx];
                cout.fill(' ');
                cout << "|" << setw(17) << left << subject.code << "|"
                    << setw(50) << left << subject.name << "|";

                // Display marks
                for (int markIdx = 0; markIdx < 3; markIdx++) {
                    cout << setw(7) << right << subject.course_mark[markIdx] << setw(8) << "" << "|";
                }

                // Display total marks obtained and credits
                float total_obtained = subject.getTotalMarksObtained();
                cout << setw(12) << right << total_obtained << "/" << subject.total_max_marks << "|";
                cout << setw(12) << right << subject.credits << "|";
                cout << endl;
                cout << setw(200) << setfill('_') << "" << endl; // line
            }

            // Display earned credits, SGPA, and status
            double sgpa = semesters[semIdx].calculateCGPA();
            cout.fill(' ');
            cout << "|" << setw(30) << right << "Earned Credits: " << setw(10) << left << "CalculateCredits" // Placeholder for earned credits logic
                << setw(30) << right << "SGPA: " << setw(10) << left << fixed << setprecision(2) << sgpa
                << setw(30) << right << "Status: " << setw(10) << left << "Completed" << setw(78) << "" << "|" << endl;
            cout << setw(200) << setfill('_') << "" << endl; // line
        }

        // Overall CGPA calculation
        double overallCGPA = 0;
        for (int i = 0; i < semesterCount; i++) {
            overallCGPA += semesters[i].calculateCGPA();
        }
        overallCGPA /= semesterCount;

        cout << "\nOverall CGPA: " << fixed << setprecision(2) << overallCGPA << endl;
    }

    void saveToFile() const {
        string filename = rollNo + ".txt";
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }
        file << studentName << endl;
        file << rollNo << endl;
        file << branch << endl;
        for (int i = 0; i < semesterCount; i++) {
            file << semesters[i].semesterName << endl;
            file << semesters[i].subjectCount << endl;
            for (int j = 0; j < semesters[i].subjectCount; j++) {
                file << semesters[i].subjects[j].name << " " << semesters[i].subjects[j].code << " ";
                for (int k = 0; k < 3; k++) {
                    file << semesters[i].subjects[j].course_mark[k] << " ";
                }
                file << semesters[i].subjects[j].total_max_marks << " " << semesters[i].subjects[j].credits << endl;
            }
        }
        file.close();
    }

    void editMarks(string rollNo) {
        cout << "Enter the semester (1 or 2): ";
        int semesterIndex;
        cin >> semesterIndex;
        semesterIndex--;

        cout << "Enter the subject name to edit: ";
        cin.ignore();
        string subjectName;
        getline(cin, subjectName);

        for (int i = 0; i < semesters[semesterIndex].subjectCount; i++) {
            if (semesters[semesterIndex].subjects[i].name == subjectName) {
                cout << "Enter new marks for " << subjectName << ":\n";
                semesters[semesterIndex].subjects[i].input_marks();
                cout << "Marks updated successfully!" << endl;
                saveToFile();  // Save updated marks to file
                return;
            }
        }
        cout << "Subject not found!" << endl;
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
                sr.editMarks(rollNo);
                break;
            case 0:
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
