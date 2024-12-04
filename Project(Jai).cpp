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

    int inputSubject() {
        cout << "Enter course name: ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "Enter course code: ";
        cin.getline(code, 10);
        cout << "Enter credits for the subject: ";
        cin >> credits;
        input_marks();
        return credits;
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
        strcpy(semesterName, semName.c_str());
        cout << "Enter number of subjects in " << semName << ": ";
        cin >> subjectCount;
        for (int i = 0; i < subjectCount; i++) {
            cout << "\nEntering details for subject " << (i + 1) << ":\n";
            totalcredits+=subjects[i].inputSubject();
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
    StudentResult resultSystem;
    char choice;
    do {
        cout << "\n1. Enter Result Details\n2. Edit Marks\n3. Append Branch\n4. Search Result\n0. Exit\nEnter your choice: ";
        cin >> choice;

        string rollNo;
        switch (choice) {
            case 1:
                sr.inputStudentDetails();
                sr.inputSemesterDetails();
                sr.saveToFile(); 
                sr.displayStudentDetails();
                break;
            case '2':
                cout << "Enter roll number to edit marks: ";
                cin >> rollNo;
                sr.editMarks(rollNo);
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (1);

    return 0;
}
