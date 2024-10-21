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

    int inputSubject() {
        cout << "Enter course name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter course code: ";
        getline(cin, code);
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
    int subjectCount,totalcredits;
    string semesterName;
    Subject subjects[10];  // Assuming a maximum of 10 subjects per semester

    Semester() : subjectCount(0) , totalcredits(0) {}

    void inputSemesterDetails(const string& semName) {
        semesterName = semName;
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
                cout << setw(15) << left << semesters[semIdx].subjects[0].type_of_marks[i] << "|";  // Theory, IT, Term Work
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
            cout << "|" << setw(30) << right << "Earned Credits: " << setw(10) << left << semesters[semIdx].totalcredits // Placeholder for earned credits logic
                << setw(30) << right << "SGPA: " << setw(10) << left << fixed << setprecision(2) << sgpa
                << setw(30) << right << "Status: " << setw(10) << left << "Completed" << setw(72) << "" <<right<< "|" << endl;
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

    // Save data to binary file
    void saveToFile() const {
        string filename = rollNo + ".bin";
        ofstream file(filename, ios::binary);
        if (!file) {
            cout << "Error opening file!" << endl;
            return;
        }
        // Write student details
        file.write(reinterpret_cast<const char*>(&studentName), sizeof(studentName));
        file.write(reinterpret_cast<const char*>(&rollNo), sizeof(rollNo));
        file.write(reinterpret_cast<const char*>(&branch), sizeof(branch));

        // Write semester details
        for (int i = 0; i < semesterCount; i++) {
            file.write(reinterpret_cast<const char*>(&semesters[i].semesterName), sizeof(semesters[i].semesterName));
            file.write(reinterpret_cast<const char*>(&semesters[i].subjectCount), sizeof(semesters[i].subjectCount));

            // Write subjects details
            for (int j = 0; j < semesters[i].subjectCount; j++) {
                file.write(reinterpret_cast<const char*>(&semesters[i].subjects[j]), sizeof(semesters[i].subjects[j]));
            }
        }
        file.close();
    }

    // Edit marks with random file access
    void editMarks(const string& rollNo) {
        string filename = rollNo + ".bin";
        fstream file(filename, ios::in | ios::out | ios::binary);
        if (!file) {
            cout << "Error opening file for editing!" << endl;
            return;
        }

        int semesterIndex, subjectIndex;
        cout << "Enter the semester index (1 or 2): ";
        cin >> semesterIndex;
        semesterIndex--;

        cout << "Enter the subject index to edit: ";
        cin >> subjectIndex;
        subjectIndex--;

        // Calculate the position to seek to (considering the size of student details and subjects)
        streampos subjectPosition = sizeof(studentName) + sizeof(rollNo) + sizeof(branch) +
                                    semesterIndex * sizeof(Semester) +
                                    subjectIndex * sizeof(Subject);

        // Move to the subject position
        file.seekp(subjectPosition);

        // Input new marks and write to the file
        semesters[semesterIndex].subjects[subjectIndex].input_marks();
        file.write(reinterpret_cast<const char*>(&semesters[semesterIndex].subjects[subjectIndex]), sizeof(Subject));

        cout << "Marks updated successfully!" << endl;

        file.close();
    }
    void searchAndDisplayResult( string& rollNo) {
        string filename = rollNo + ".bin";
        ifstream file(filename, ios::binary);
        if (!file) {
            cout << "Error: No record found for roll number " << rollNo << "!" << endl;
            return;
        }

        // Read student details from the file
        file.read(reinterpret_cast<char*>(&studentName), sizeof(studentName));
        file.read(reinterpret_cast<char*>(&rollNo), sizeof(rollNo));
        file.read(reinterpret_cast<char*>(&branch), sizeof(branch));

        // Read semester details
        for (int i = 0; i < semesterCount; i++) {
            file.read(reinterpret_cast<char*>(&semesters[i].semesterName), sizeof(semesters[i].semesterName));
            file.read(reinterpret_cast<char*>(&semesters[i].subjectCount), sizeof(semesters[i].subjectCount));

            // Read subjects details
            for (int j = 0; j < semesters[i].subjectCount; j++) {
                file.read(reinterpret_cast<char*>(&semesters[i].subjects[j]), sizeof(semesters[i].subjects[j]));
            }
        }

        // Display the result details
        displayStudentDetails();
        file.close();
    }
        // Append branch to the file
    void updateBranchInFile(string& rollNo) {
        string filename = rollNo + ".bin";

        // Open the file in read/write mode (in and out), binary mode
        fstream file(filename, ios::in | ios::out | ios::binary);

        // Check if file opens successfully
        if (!file.is_open()) {
            cout << "Error: File " << filename << " could not be opened!" << endl;
            return;
        }

        // Reading the current details
        string studentName, currentRollNo, branch;

        // Read the length of the student name and then the name
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        studentName.resize(nameLength);
        file.read(&studentName[0], nameLength); // Read the student name

        // Read the length of the roll number and then the roll number
        size_t rollLength;
        file.read(reinterpret_cast<char*>(&rollLength), sizeof(rollLength));
        currentRollNo.resize(rollLength);
        file.read(&currentRollNo[0], rollLength); // Read the roll number

        // Read the length of the branch and then the branch
        size_t branchLength;
        file.read(reinterpret_cast<char*>(&branchLength), sizeof(branchLength));
        branch.resize(branchLength);
        file.read(&branch[0], branchLength); // Read the branch

        // Display current branch
        cout << "Current branch: " << branch << endl;

        // Get new branch from user
        string newBranch;
        cout << "Enter the updated branch: ";
        cin.ignore();  // Clear the input buffer
        getline(cin, newBranch);

        // Move file pointer to the position of the branch to overwrite it
        streampos branchPosition = sizeof(size_t) + nameLength + sizeof(size_t) + rollLength + sizeof(size_t);
        file.seekp(branchPosition, ios::beg);  // Move to the branch position

        // Write the length of the new branch and the new branch to the file
        size_t newBranchLength = newBranch.length();
        file.write(reinterpret_cast<const char*>(&newBranchLength), sizeof(newBranchLength)); // Write the length of the new branch
        file.write(newBranch.c_str(), newBranchLength); // Write the new branch

        // Ensure write is successful
        if (!file) {
            cout << "Error writing to file!" << endl;
        } else {
            cout << "Branch updated successfully!" << endl;
        }

        file.close();  // Close the file
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
            case '1':
                resultSystem.inputStudentDetails();
                resultSystem.inputSemesterDetails();
                resultSystem.saveToFile();
                break;
            case '2':
                cout << "Enter roll number to edit marks: ";
                cin >> rollNo;
                resultSystem.editMarks(rollNo);
                break;
            case '3':
                cout << "Enter roll number to append branch: ";
                cin >> rollNo;
                resultSystem.updateBranchInFile(rollNo);
                break;
            case '4':
                cout << "Enter roll number to search: ";
                cin >> rollNo;
                resultSystem.searchAndDisplayResult(rollNo);
                break;
            case '0':
                cout << "Exiting..." << endl;
                exit(0);
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (1);

    return 0;
}
