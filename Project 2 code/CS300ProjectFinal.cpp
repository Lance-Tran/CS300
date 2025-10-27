#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>

    void clear_screen() {
        system("cls");
    }

    struct Course {
        std::string courseId;
        std::string courseName;
        std::vector<std::string> preList;
    };

    class BinarySearchTree {
    private:
        struct Node {
            Course course;
            Node* left;
            Node* right;

            Node(Course newCourse) : course(newCourse), left(nullptr), right(nullptr) {}
        };

        Node* root;
        int size = 0;

        void printInOrder(Node* node);
        void destroyTree(Node* node);
        Node* insertHelper(Node* node, Course newCourse);

    public:
        BinarySearchTree();
        ~BinarySearchTree();

        void InOrder();
        void Insert(Course newCourse);
        void Remove(std::string courseId);
        Course Search(std::string courseId);
        int Size();
    };

    // Constructor
    BinarySearchTree::BinarySearchTree() : root(nullptr), size(0) {}

    // Destructor
    BinarySearchTree::~BinarySearchTree() {
        destroyTree(root);
    }

    // Delete nodes post-order
    void BinarySearchTree::destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    // Public Insert method
    void BinarySearchTree::Insert(Course newCourse) {
        root = insertHelper(root, newCourse);
    }

    // Recursive helper for Insert
    BinarySearchTree::Node* BinarySearchTree::insertHelper(Node* node, Course newCourse) {
        if (node == nullptr) {
            size++;
            return new Node(newCourse);
        }

        if (newCourse.courseId < node->course.courseId) {
            node->left = insertHelper(node->left, newCourse);
        }
        else if (newCourse.courseId > node->course.courseId) {
            node->right = insertHelper(node->right, newCourse);
        }

        return node;
    }

    // Iterative remove function
    void BinarySearchTree::Remove(std::string courseId) {
        Node* parent = nullptr;
        Node* current = root;

        while (current != nullptr) {
            if (current->course.courseId == courseId) {
                break;
            }
            parent = current;
            if (courseId < current->course.courseId) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        if (current == nullptr) {
            std::cout << "\nCourse '" << courseId << "' not found for removal." << std::endl;
            return;
        }

        if (current->left == nullptr && current->right == nullptr) {
            if (parent == nullptr) { root = nullptr; }
            else if (parent->left == current) { parent->left = nullptr; }
            else { parent->right = nullptr; }
            delete current;
            size--;
        }
        else if (current->right == nullptr) {
            if (parent == nullptr) { root = current->left; }
            else if (parent->left == current) { parent->left = current->left; }
            else { parent->right = current->left; }
            delete current;
            size--;
        }
        else if (current->left == nullptr) {
            if (parent == nullptr) { root = current->right; }
            else if (parent->left == current) { parent->left = current->right; }
            else { parent->right = current->right; }
            delete current;
            size--;
        }
        else {
            Node* prev = current;
            Node* tempNode = current->right;
            while (tempNode->left != nullptr) {
                prev = tempNode;
                tempNode = tempNode->left;
            }
            current->course = tempNode->course;
            if (prev->left == tempNode) {
                prev->left = tempNode->right;
            }
            else {
                prev->right = tempNode->right;
            }
            delete tempNode;
            size--;
        }

        std::cout << "\nCourse '" << courseId << "' removed successfully." << std::endl;
    }

    // Public InOrder traversal
    void BinarySearchTree::InOrder() {
        printInOrder(root);
    }

    // Private helper for InOrder traversal
    void BinarySearchTree::printInOrder(Node* node) {
        if (node != nullptr) {
            printInOrder(node->left);
            std::cout << node->course.courseId << ", " << node->course.courseName << std::endl;
            printInOrder(node->right);
        }
    }

    // Search for a course
    Course BinarySearchTree::Search(std::string courseId) {
        Node* current = root;
        while (current != nullptr) {
            if (courseId == current->course.courseId) {
                return current->course;
            }
            if (courseId < current->course.courseId) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return Course();
    }

    int BinarySearchTree::Size() {
        return size;
    }

    //Split function
    std::vector<std::string> Split(const std::string& line) {
        std::string lineFeed = line;
        char delim = ',';
        lineFeed += delim;
        std::vector<std::string> lineTokens;
        std::string temp = "";
        for (size_t i = 0; i < lineFeed.length(); ++i) {
            if (lineFeed[i] == delim) {
                lineTokens.push_back(temp);
                temp = "";
            }
            else {
                temp += lineFeed[i];
            }
        }
        return lineTokens;
    }

    void convertCase(std::string& toConvert) {
        std::transform(toConvert.begin(), toConvert.end(), toConvert.begin(), ::toupper);
    }

    void loadCourses(std::string csvPath, BinarySearchTree* courseList) {
        std::ifstream inFS(csvPath);
        if (!inFS.is_open()) {
            std::cout << "Could not open file: " << csvPath << std::endl;
            return;
        }

        std::string line;
        while (getline(inFS, line)) {
            if (line.empty()) continue;

            std::vector<std::string> stringTokens = Split(line);
            if (stringTokens.size() < 2) {
                std::cout << "\nError: Skipping misformatted line." << std::endl;
            }
            else {
                Course newCourse;
                newCourse.courseId = stringTokens[0];
                newCourse.courseName = stringTokens[1];
                for (size_t i = 2; i < stringTokens.size(); ++i) {
                    newCourse.preList.push_back(stringTokens[i]);
                }
                courseList->Insert(newCourse);
            }
        }
        inFS.close();
    }

    void displayCourse(const Course& newCourse) {
        std::cout << newCourse.courseId << ", " << newCourse.courseName << std::endl;
        std::cout << "Prerequisites: ";
        if (newCourse.preList.empty()) {
            std::cout << "none" << std::endl;
        }
        else {
            for (size_t i = 0; i < newCourse.preList.size(); ++i) {
                std::cout << newCourse.preList[i] << (i == newCourse.preList.size() - 1 ? "" : ", ");
            }
            std::cout << std::endl;
        }
    }


int main(int argc, char* argv[]) {

    std::string csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
    if (argc > 1) {
        csvPath = argv[1];
    }

    BinarySearchTree* courseList = new BinarySearchTree();
    int choice = 0;

    std::cout << "Welcome to the course planner." << std::endl;

    while (choice != 9) {
        std::cout << "\nMenu:" << std::endl;
        std::cout << "  1. Load Courses" << std::endl;
        std::cout << "  2. Display All Courses" << std::endl;
        std::cout << "  3. Find Course" << std::endl;
        std::cout << "  4. Remove Course" << std::endl;
        std::cout << "  9. Exit" << std::endl;
        std::cout << "Enter choice: ";

        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clear_screen();
            continue;
        }

        std::string newCourseKey;

        switch (choice) {
        case 1: {
            loadCourses(csvPath, courseList);
            std::cout << courseList->Size() << " courses read." << std::endl;
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
        case 2: {
            std::cout << "\nAlphanumeric Course List:" << std::endl;
            courseList->InOrder();
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
        case 3: {
            std::cout << "\nWhat course do you want to know about? ";
            std::cin >> newCourseKey;
            convertCase(newCourseKey);
            Course course = courseList->Search(newCourseKey);
            if (!course.courseId.empty()) {
                displayCourse(course);
            }
            else {
                std::cout << "\nCourse ID '" << newCourseKey << "' not found." << std::endl;
            }
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
        case 4: {
            std::cout << "\nWhat course do you want to delete? ";
            std::cin >> newCourseKey;
            convertCase(newCourseKey);
            courseList->Remove(newCourseKey);
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
        case 9:
            std::cout << "Exiting program." << std::endl;
            break;
        default:
            std::cout << choice << " is not a valid option." << std::endl;
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
        clear_screen();
    }

    delete courseList;
    std::cout << "Good bye." << std::endl;
    return 0;
}