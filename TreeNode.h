// TreeNode.h
﻿
// ===============================
// Ad, Soyad      : Hagar Al-gabri
// Ogrenci No     : B241200553
// BOOK MANAGER MODULE
// Handles operations related to book records in the library.
// ===============================

#include <iostream> // Basic input/output stream
#include <string> // String support for handling text
#include <ctime> // Time utilities for tracking book addition and borrowing dates
#include <vector> // STL vector for sorting and managing book lists
#include <algorithm> // Algorithms such as sort
#include <iomanip> // To format output (e.g., timestamps)
#include <sstream> // String stream for time formatting
#include <unordered_map> // Hash maps for ratings
#include <queue> // For priority queue logic

#pragma warning(disable : 4996) // Disable unsafe warning for localtime on Visual Studio

using namespace std;
//======================================================
//                   MODULE 3: RequestQueue
// Purpose: Manages book requests (normal and academic)
//======================================================
class RequestQueue {
private:
    // Structure to store each book request
    struct BookRequest {
        string title;
        string author;
        bool isAcademic;
        int priority; // 1 = academic, 0 = normal
        BookRequest* next;

        BookRequest(string t, string a, bool academic)
            : title(t), author(a), isAcademic(academic), priority(academic ? 1 : 0), next(nullptr) {}
    };

    BookRequest* front; // Points to the front of the queue
    BookRequest* rear; // Points to the rear of the queue

public:
    // Constructor: Initializes an empty queue
    RequestQueue() : front(nullptr), rear(nullptr) {}

    // Adds a new book request to the queue based on priority
    void enqueue(string title, string author, bool isAcademic) {
        BookRequest* newRequest = new BookRequest(title, author, isAcademic);
        // Insert with higher priority (academic) at the front
        if (!front || newRequest->priority > front->priority) {
            newRequest->next = front;
            front = newRequest;
            if (!rear) rear = newRequest;
        }
        else {
            // Traverse to insert based on priority order
            BookRequest* current = front;
            while (current->next && current->next->priority >= newRequest->priority) {
                current = current->next;
            }
            newRequest->next = current->next;
            current->next = newRequest;
            if (!newRequest->next) rear = newRequest;
        }
        cout << "Request added to queue!\n";
    }
    // Prints all current requests
    void printRequests() const {
        if (!front) {
            cout << "No requests in the queue.\n";
            return;
        }
        BookRequest* current = front;
        while (current) {
            cout << "- " << current->title << " by " << current->author;
            if (current->isAcademic) cout << " [Academic Priority]";
            cout << endl;
            current = current->next;
        }
    }
    // Processes (removes) the request at the front
    void fulfillRequest() {
        if (!front) {
            cout << "No requests to fulfill.\n";
            return;
        }
        BookRequest* temp = front;
        front = front->next;
        cout << "Fulfilling request: " << temp->title << " by " << temp->author << endl;
        delete temp;
        if (!front) rear = nullptr;
    }
};
// ===============================
// BOOK MANAGER MODULE
// Handles operations related to book records in the library.
// Uses a singly linked list to store and manage books.
// ===============================
class BookManager {
public:
    struct Book {
        int id;
        string title;
        string author;
        int pages;
        int year;
        string publisher;           // ("TR", "EN")
        string damageNote;
        time_t addDate;           // Date and time when the book was added
        Book* next;              // Pointer to the next book in the linked list

        // Constructor to initialize book attributes
        Book(int i, const string& t, const string& a, int p, int y, const string& pub)
            : id(i), title(t), author(a), pages(p), year(y), publisher(pub),
            damageNote(""), addDate(time(nullptr)), next(nullptr) {}
    };

private:
    Book* head;    // Pointer to the first book in the list
    int count;    // Total number of books

    // Constructor to initialize an empty list
public:
    BookManager() : head(nullptr), count(0) {}

    void addBook(int id, const string& title, const string& author, int pages, int year) {
        if (findBook(id) != nullptr) {
            cout << "Error: Book ID already exists!\n";
            return;
        }
        // Auto assign publisher based on even/odd ID
        string publisher = (id % 2 == 0) ? "TR" : "EN";
        // Create and insert the new book at the beginning of the list
        Book* newBook = new Book(id, title, author, pages, year, publisher);
        newBook->next = head;
        head = newBook;
        count++;
    }
    // Find a book by ID
    Book* findBook(int id) const {
        Book* current = head;
        while (current != nullptr) {
            if (current->id == id) return current;
            current = current->next;
        }
        return nullptr;
    }
    // Display all books sorted by ID in ascending order
    void listBooks() const {
        vector<Book*> books;
        Book* current = head;

        while (current != nullptr) {
            books.push_back(current);
            current = current->next;
        }

        sort(books.begin(), books.end(), [](Book* a, Book* b) {
            return a->id < b->id;
            });

        for (Book* book : books) {
            tm* timeinfo = localtime(&(book->addDate));
            cout << book->id << " | " << book->title
                << " by " << book->author
                << " (" << book->year << ")"
                << " | Pages: " << book->pages
                << " | Added: " << put_time(timeinfo, "%Y-%m-%d %H:%M:%S") << endl;
        }
    }
    void listBooksByPageCount() const {
        vector<Book*> books;
        Book* current = head;
        while (current != nullptr) {
            books.push_back(current);
            current = current->next;
        }

        sort(books.begin(), books.end(), [](Book* a, Book* b) {
            return a->pages < b->pages;
            });

        cout << "\n--- Books Sorted by Pages ---\n";
        for (Book* book : books) {
            cout << book->pages << " pages | " << book->title << endl;
        }
    }

    void listOldestFiveBooks() const {
        vector<Book*> books;
        Book* current = head;
        while (current != nullptr) {
            books.push_back(current);
            current = current->next;
        }

        if (books.empty()) {
            cout << "No books available.\n";
            return;
        }
        // Sort books by ID
        sort(books.begin(), books.end(), [](Book* a, Book* b) {
            return a->year < b->year;
            });

        cout << "\n--- Oldest 5 Books ---\n";
        int limit = min(5, (int)books.size());
        for (int i = 0; i < limit; ++i) {
            cout << books[i]->id << " | " << books[i]->title
                << " (" << books[i]->year << ")\n";
        }
    }

    bool deleteBook(int id) {
        if (head == nullptr) return false;

        if (head->id == id) {
            Book* temp = head;
            head = head->next;
            delete temp;
            count--;
            return true;
        }
        // Search for the book to delete
        Book* current = head;
        while (current->next != nullptr && current->next->id != id) {
            current = current->next;
        }

        if (current->next != nullptr) {
            Book* temp = current->next;
            current->next = current->next->next;
            delete temp;
            count--;
            return true;
        }

        return false;
    }

    void editBookByTitle(const string& title) {
        Book* current = head;
        while (current != nullptr) {
            if (current->title == title) {
                cout << "Editing Book: " << current->title << endl;
                cout << "Enter new title (or press enter to keep current): ";
                string newTitle;
                getline(cin, newTitle);
                if (!newTitle.empty()) current->title = newTitle;

                cout << "Enter new author (or press enter to keep current): ";
                string newAuthor;
                getline(cin, newAuthor);
                if (!newAuthor.empty()) current->author = newAuthor;

                cout << "Enter new pages (or -1 to keep current): ";
                int newPages;
                cin >> newPages;
                cin.ignore();
                if (newPages != -1) current->pages = newPages;

                cout << "Enter new year (or -1 to keep current): ";
                int newYear;
                cin >> newYear;
                cin.ignore();
                if (newYear != -1) current->year = newYear;

                cout << "Book updated successfully!\n";
                return;
            }
            current = current->next;
        }
        cout << "Book with title \"" << title << "\" not found.\n";
    }

    void addDamageNote(int id, const string& note) {
        Book* book = findBook(id);
        if (book != nullptr) {
            book->damageNote = note;
            cout << "Damage note added successfully!\n";
        }
        else {
            cout << "Book with ID " << id << " not found.\n";
        }
    }

    void listDamagedBooks() const {
        Book* current = head;
        bool found = false;

        cout << "\n--- Damaged Books ---\n";
        while (current != nullptr) {
            if (!current->damageNote.empty()) {
                cout << "ID: " << current->id << " | "
                    << "Title: " << current->title << " | "
                    << "Damage: " << current->damageNote << endl;
                found = true;
            }
            current = current->next;
        }

        if (!found) {
            cout << "No damaged books found.\n";
        }
    }
    // Return the total number of books
    int getBookCount() const {
        return count;
    }
};
// ====== Comment System ======
// This class allows users to add comments to books and view them.
// It uses a simple singly linked list to store comments for each book.
class CommentSystem {
private:
    struct Comment {
        int bookID;
        string text;
        Comment* next; // Pointer to the next comment
    };

    Comment* head;
public:
    CommentSystem() {
        head = nullptr;
    }
    // Adds a new comment to a book by pushing it to the front of the list
    void addComment(int bookID, const string& commentText) {
        Comment* newComment = new Comment{ bookID, commentText, head };
        head = newComment;
    }
    // Displays all comments for a specific book ID
    void showComments(int bookID) const {
        Comment* temp = head;
        bool found = false;
        cout << "\n--- Comments for Book ID " << bookID << " ---\n";
        while (temp) {
            if (temp->bookID == bookID) {
                cout << "- " << temp->text << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "No comments for this book.\n";
        }
    }
};
// ====== User Book Collection ======
// This module represents the user's personal book collection.
// It is implemented as a circular doubly linked list.
class UserCollection {
private:
    struct UserBook {
        int id;
        string title;
        string author;
        int pages;
        int year;
        bool isRead;      // Read status (true = read)
        UserBook* next;
        UserBook* prev;

        UserBook(int i, const string& t, const string& a, int p, int y)
            : id(i), title(t), author(a), pages(p), year(y),
            isRead(false), next(nullptr), prev(nullptr) {}
    };

    UserBook* head; // Head of the list
    bool collectionCreated; // Whether the collection is initialized

public:
    UserCollection() : head(nullptr), collectionCreated(false) {}

    // Creates the user's collection if not already created
    void createCollection() {
        if (collectionCreated) {
            cout << "You already have a collection.\n";
            return;
        }
        collectionCreated = true;
        cout << "Your personal book collection has been created!\n";
    }
    // Adds a book to the user's collection (at the end of the circular list)
    void addBook(int id, const string& title, const string& author, int pages, int year) {
        if (!collectionCreated) {
            cout << "Please create a collection first.\n";
            return;
        }

        UserBook* newBook = new UserBook(id, title, author, pages, year);

        if (head == nullptr) {
            head = newBook;
            head->next = head;
            head->prev = head;
        }
        else {
            newBook->next = head;
            newBook->prev = head->prev;
            head->prev->next = newBook;
            head->prev = newBook;
        }
        cout << "Book added to your collection!\n";
    }
    // Displays all books in the user's collection
    void listBooks() const {
        if (!collectionCreated || head == nullptr) {
            cout << "No books in your collection.\n";
            return;
        }

        cout << "\n--- Your Book Collection ---\n";
        UserBook* current = head;
        do {
            cout << current->id << " | " << current->title << " by " << current->author
                << " (" << current->year << ")"
                << " | Pages: " << current->pages
                << " | Status: " << (current->isRead ? "Read" : "Not Read") << endl;
            current = current->next;
        } while (current != head);
    }
    // Edits the read/unread status of a book
    void editBookStatus(int id) {
        if (!collectionCreated || head == nullptr) {
            cout << "No books in your collection.\n";
            return;
        }

        UserBook* current = head;
        do {
            if (current->id == id) {
                cout << "Current status: " << (current->isRead ? "Read" : "Not Read") << endl;
                cout << "Enter new status (1 = Read, 0 = Not Read): ";
                int status;
                cin >> status;
                cin.ignore();
                current->isRead = (status == 1);
                cout << "Status updated!\n";
                return;
            }
            current = current->next;
        } while (current != head);

        cout << "Book with ID " << id << " not found.\n";
    }
    // Deletes a book from the user's collection by ID
    void deleteBook(int id) {
        if (!collectionCreated || head == nullptr) {
            cout << "No books in your collection.\n";
            return;
        }

        UserBook* current = head;
        do {
            if (current->id == id) {
                if (current->next == current) {
                    delete current;
                    head = nullptr;
                }
                else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    if (current == head) {
                        head = current->next;
                    }
                    delete current;
                }
                cout << "Book deleted from your collection!\n";
                return;
            }
            current = current->next;
        } while (current != head);

        cout << "Book with ID " << id << " not found.\n";
    }
};
// ====== CircularList: Single Circular Linked List ======
// This class is used to manage circular lists such as borrowed, returned, or available books.
// ===============================
class CircularList {
private:
    // Node represents a single book in the circular list.
    struct Node {
        int id;
        string title;
        string author;
        int pages;
        int year;
        string borrowDate;
        string returnDate;
        Node* next;
        // Constructor initializes node attributes
        Node(int i, const string& t, const string& a, int p, int y)
            : id(i), title(t), author(a), pages(p), year(y),
            borrowDate("N/A"), returnDate("N/A"), next(nullptr) {}
    };

    Node* head; // Head pointer of the circular list

public:
    void removeBook(int id);
    // Returns the borrow date for a given book ID
    string getBorrowDate(int id) const {
        if (!head) return "N/A";

        Node* current = head;
        do {
            if (current->id == id)
                return current->borrowDate;
            current = current->next;
        } while (current != head);

        return "N/A";
    }


    CircularList() : head(nullptr) {}
    // Adds a borrowed book to the list and records the current date/time
    void addBorrowedBook(int id, const string& title, const string& author, int pages, int year) {
        Node* newNode = new Node(id, title, author, pages, year);


        if (!head) {
            head = newNode;
            head->next = head;
        }
        else {
            Node* temp = head;
            while (temp->next != head) temp = temp->next;
            temp->next = newNode;
            newNode->next = head;
        }
        // Set current time as borrow date
        time_t now = time(0);
        tm* nowTime = localtime(&now);
        ostringstream oss;
        oss << put_time(nowTime, "%Y-%m-%d %H:%M:%S");
        newNode->borrowDate = oss.str();
    }
    // Adds a returned book to the list and records the return time
    void addReturnedBook(int id, const string& title, const string& author, int pages, int year, const string& borrowDate) {
        Node* newNode = new Node(id, title, author, pages, year);

        if (!head) {
            head = newNode;
            head->next = head;
        }
        else {
            Node* temp = head;
            while (temp->next != head) temp = temp->next;
            temp->next = newNode;
            newNode->next = head;
        }
        newNode->borrowDate = borrowDate;
        // Set current time as return date
        time_t now = time(0);
        tm* nowTime = localtime(&now);
        ostringstream oss;
        oss << put_time(nowTime, "%Y-%m-%d %H:%M:%S");
        newNode->returnDate = oss.str();
    }
    // Adds an available book to the list
    void addAvailableBook(int id, const string& title, const string& author, int pages, int year) {
        Node* newNode = new Node(id, title, author, pages, year);

        if (!head) {
            head = newNode;
            head->next = head;
        }
        else {
            Node* temp = head;
            while (temp->next != head) temp = temp->next;
            temp->next = newNode;
            newNode->next = head;
        }
    }

    void displayBorrowedBooks() const {
        if (!head) {
            cout << "No borrowed books.\n";
            return;
        }

        cout << "\n--- Borrowed Books ---\n";
        Node* temp = head;
        do {
            cout << temp->id << " | " << temp->title
                << " | Borrowed On: " << temp->borrowDate << endl;
            temp = temp->next;
        } while (temp != head);
    }

    void displayReturnedBooks() const {
        if (!head) {
            cout << "No returned books.\n";
            return;
        }

        cout << "\n--- Returned Books ---\n";
        Node* temp = head;
        do {
            cout << temp->id << " | " << temp->title
                << " | Borrowed On: " << temp->borrowDate
                << " | Returned On: " << temp->returnDate << endl;
            temp = temp->next;
        } while (temp != head);
    }

    void displayAvailableBooks() const {
        if (!head) {
            cout << "No available books.\n";
            return;
        }

        cout << "\n--- Available Books ---\n";
        Node* temp = head;
        do {
            cout << temp->id << " | " << temp->title
                << " by " << temp->author << endl;
            temp = temp->next;
        } while (temp != head);
    }
};
//====== Stack for User Borrowed/Returned Books ======
class UserStack {
public:
    // Structure representing a node in the stack
    struct StackNode {
        int id;
        string title;
        string author;
        StackNode* next;

        // Constructor to initialize a book node
        StackNode(int i, const string& t, const string& a)
            : id(i), title(t), author(a), next(nullptr) {}
    };

    StackNode* top;

public:
    // Constructor initializes the top to null
    UserStack() : top(nullptr) {}
    // Add a book to the top of the stack
    void push(int id, const string& title, const string& author) {
        StackNode* newNode = new StackNode(id, title, author);
        newNode->next = top;
        top = newNode;
    }
    // Remove the top book from the stack
    void pop() {
        if (top == nullptr) {
            cout << "Stack is empty.\n";
            return;
        }
        StackNode* temp = top;
        top = top->next;
        delete temp;
    }
    // Return the top book without removing it
    StackNode* peek() const {
        return top;
    }
    // Check if the stack is empty
    bool isEmpty() const {
        return top == nullptr;
    }
    // Display all books in the stack
    void display() const {
        if (top == nullptr) {
            cout << "Stack is empty.\n";
            return;
        }
        StackNode* temp = top;
        cout << "\n--- Stack Books ---\n";
        while (temp) {
            cout << temp->id << " | " << temp->title
                << " by " << temp->author << endl;
            temp = temp->next;
        }
    }
};

// ====== Global Tree Node Structure ======
// Structure used in the Binary Search Tree for reporting
struct TreeNode {
    int id;
    string title;
    string author;
    int pages;
    int year;
    string publisher;
    string language;
    time_t borrowDate;

    TreeNode* left;
    TreeNode* right;
    // Constructor to initialize a tree node
    TreeNode(int i, string t, string a, int p, int y, string pub, string lang, time_t date)
        : id(i), title(t), author(a), pages(p), year(y),
        publisher(pub), language(lang), borrowDate(date),
        left(nullptr), right(nullptr) {}
};
// ====== Hash Table for Book Ratings ======
class RatingHashTable {
private:
    // Structure for a single rating entry in the hash table
    struct RatingEntry {
        int bookID;
        string title;
        int rating; // 1 to 5
        RatingEntry* next;
        // Constructor to initialize a rating entry
        RatingEntry(int id, const string& t, int r)
            : bookID(id), title(t), rating(r), next(nullptr) {}
    };

    static const int SIZE = 10;  // Size of the hash table
    RatingEntry* table[SIZE];   // Array of pointers to linked list buckets
    // Hash function based on book ID
    int hashFunction(int id) const {
        return id % SIZE;
    }

public:
    // Constructor initializes all buckets to null
    RatingHashTable() {
        for (int i = 0; i < SIZE; ++i)
            table[i] = nullptr;
    }
    // Insert a new rating into the hash table
    void insert(int id, const string& title, int rating) {
        if (rating < 1 || rating > 5) {
            cout << " Invalid rating. Must be between 1 and 5.\n";
            return;
        }

        int index = hashFunction(id);
        RatingEntry* newEntry = new RatingEntry(id, title, rating);
        newEntry->next = table[index];
        table[index] = newEntry;
    }
    // Display all books with rating 4 or higher
    void displayHighRatedBooks() const {
        cout << "\n--- Books with Rating >= 4 ---\n";
        for (int i = 0; i < SIZE; ++i) {
            RatingEntry* temp = table[i];
            while (temp) {
                if (temp->rating >= 4) {
                    cout << temp->bookID << " | " << temp->title << " | Rating: " << temp->rating << endl;
                }
                temp = temp->next;
            }
        }
    }
    // Display all books with their ratings
    void displayAllRatings() const {
        cout << "\n--- All Rated Books ---\n";
        for (int i = 0; i < SIZE; ++i) {
            RatingEntry* temp = table[i];
            while (temp) {
                cout << temp->bookID << " | " << temp->title << " | Rating: " << temp->rating << endl;
                temp = temp->next;
            }
        }
    }
};
// ====== Library System Controller ======
class LibrarySystem {

private:
    BookManager bookManager;
    CommentSystem commentSystem;
    UserCollection userCollection;
    CircularList borrowedList;
    CircularList returnedList;
    CircularList availableList;
    UserStack userBorrowStack;
    UserStack userReturnStack;
    RequestQueue requestQueue;
    bool userCollectionCreated;
    RatingHashTable staffRatings;
    RatingHashTable userRatings;
    RatingHashTable ratingTable;
    RatingHashTable staffRatingsHashTable;
    TreeNode* borrowTreeRoot = nullptr;

    void insertToTree(TreeNode*& root, int id, const string& title, const string& author,
        int pages, int year, const string& publisher, const string& language, time_t borrowDate) {
        if (!root) {
            root = new TreeNode(id, title, author, pages, year, publisher, language, borrowDate);
            return;
        }
        if (id < root->id) {
            insertToTree(root->left, id, title, author, pages, year, publisher, language, borrowDate);
        }
        else {
            insertToTree(root->right, id, title, author, pages, year, publisher, language, borrowDate);
        }
    }
    void addBorrowedBooksToTree() {
        UserStack::StackNode* temp = userBorrowStack.peek();

        while (temp) {

            BookManager::Book* book = bookManager.findBook(temp->id);
            if (book) {
                string publisher = (book->id % 2 == 0) ? "TR" : "EN";
                string language = (book->id % 2 == 0) ? "tr" : "en";
                time_t now = time(0);

                insertToTree(borrowTreeRoot, book->id, book->title, book->author,
                    book->pages, book->year, publisher, language, now);
            }
            temp = temp->next;
        }

        cout << "Books transferred from stack to tree successfully.\n";
    }

    void reportBooksByPublicationYear(TreeNode* node) {
        if (!node) return;

        reportBooksByPublicationYear(node->left);

        if (node->year < 1950)
            cout << "[Before 1950] ";
        else
            cout << "[1950 or After] ";

        cout << node->id << " | " << node->title << " | Year: " << node->year << endl;

        reportBooksByPublicationYear(node->right);
    }
    void reportBooksByBorrowDuration(TreeNode* node) {
        if (!node) return;

        time_t now = time(0);
        double days = difftime(now, node->borrowDate) / (60 * 60 * 24);

        cout << " Book ID: " << node->id << " | Title: " << node->title;

        if (days <= 30)
            cout << "  Borrowed < 30 days";
        else
            cout << " Borrowed ≥ 30 days";

        cout << endl;

        reportBooksByBorrowDuration(node->left);
        reportBooksByBorrowDuration(node->right);
    }

    void reportBooksByPublicationBeforeOrAfter1950(TreeNode* node) {
        if (!node) return;

        if (node->year < 1950)
            cout << " Book ID: " << node->id << " | Title: " << node->title << " | Year: " << node->year << " (Before 1950)\n";
        else
            cout << " Book ID: " << node->id << " | Title: " << node->title << " | Year: " << node->year << " (1950 or After)\n";

        reportBooksByPublicationBeforeOrAfter1950(node->left);
        reportBooksByPublicationBeforeOrAfter1950(node->right);
    }
    void reportBooksByYearAndDuration(TreeNode* node) {
        if (!node) return;

        reportBooksByYearAndDuration(node->left);

        time_t now = time(0);
        double days = difftime(now, node->borrowDate) / (60 * 60 * 24);

        if (node->year < 1950 && days > 30) {
            cout << "Book ID: " << node->id
                << " | Title: " << node->title
                << " | Year: " << node->year
                << " | Borrowed Days: " << days << endl;
        }

        reportBooksByYearAndDuration(node->right);
    }
    void transferUserBorrowedToTree() {
        UserStack::StackNode* temp = userBorrowStack.peek();
        if (!temp) {
            cout << "No borrowed books in stack.\n";
            return;
        }

        while (temp) {
            BookManager::Book* book = bookManager.findBook(temp->id);
            if (!book) {
                cout << "Book ID " << temp->id << " not found in library.\n";
                temp = temp->next;
                continue;
            }

            string publisher = (book->id % 2 == 0) ? "TR" : "EN";
            string language = (book->id % 2 == 0) ? "tr" : "en";
            time_t now = time(0);

            insertToTree(borrowTreeRoot, book->id, book->title, book->author,
                book->pages, book->year, publisher, language, now);

            temp = temp->next;
        }

        cout << " All borrowed books transferred to tree using stored data.\n";
    }

    void reportBooksByLanguage(TreeNode* node, const string& lang) {
        if (!node) return;

        reportBooksByLanguage(node->left, lang);

        if (lang == "both" || node->language == lang) {
            cout << "Book ID: " << node->id
                << " | Title: " << node->title
                << " | Language: " << node->language << endl;
        }

        reportBooksByLanguage(node->right, lang);
    }

    void reportBooksByPageCount(TreeNode* node, int threshold) {
        if (!node) return;

        reportBooksByPageCount(node->left, threshold);

        if (node->pages < threshold) {
            cout << "[< " << threshold << " pages] ";
        }
        else {
            cout << "[>= " << threshold << " pages] ";
        }
        cout << "Book ID: " << node->id
            << " | Title: " << node->title
            << " | Pages: " << node->pages << endl;

        reportBooksByPageCount(node->right, threshold);
    }
    void reportBooksByPublisher(TreeNode* node, const string& pub) {
        if (!node) return;

        reportBooksByPublisher(node->left, pub);

        if (node->publisher == pub) {
            cout << "Book ID: " << node->id
                << " | Title: " << node->title
                << " | Publisher: " << node->publisher << endl;
        }

        reportBooksByPublisher(node->right, pub);
    }


public:
    // Constructor to initialize system state
    LibrarySystem() : userCollectionCreated(false) {}

    // This function initializes the library with some predefined books
    void initializeAvailableBooks() {
        // Adding books to the main book manager
        bookManager.addBook(1, "Veri Yapilari ve Algoritmalar", "Rifat Colkesen", 480, 2021);
        bookManager.addBook(2, "Object-Oriented Programming in C++", "Robert Lafore", 925, 1999);
        bookManager.addBook(3, "C++ Pocket Reference", "Kyle Loudon", 140, 2002);
        bookManager.addBook(4, "Clean Code", "Robert C. Martin", 464, 2008);
        bookManager.addBook(5, "Design Patterns", "Erich Gamma", 395, 1994);
        bookManager.addBook(6, "Effective Modern C++", "Scott Meyers", 334, 2014);

        // Adding the same books to the available circular list
        availableList.addAvailableBook(1, "Veri Yapilari ve Algoritmalar", "Rifat Colkesen", 480, 2021);
        availableList.addAvailableBook(2, "Object-Oriented Programming in C++", "Robert Lafore", 925, 1999);
        availableList.addAvailableBook(3, "C++ Pocket Reference", "Kyle Loudon", 140, 2002);
        availableList.addAvailableBook(4, "Clean Code", "Robert C. Martin", 464, 2008);
        availableList.addAvailableBook(5, "Design Patterns", "Erich Gamma", 395, 1994);
        availableList.addAvailableBook(6, "Effective Modern C++", "Scott Meyers", 334, 2014);
    }

    // Main loop of the program where user chooses between staff/user or exits
    void run() {
        initializeAvailableBooks();  // Prepare initial library state
        while (true) {
            // Main menu interface
            cout << "\n=== LIBRARY SYSTEM ===\n"
                << "1. Staff Login\n"
                << "2. User Login\n"
                << "0. Exit\n"
                << "Choice: ";

            string input;
            getline(cin, input);

            // Validate input is numeri
            if (input.empty() || !all_of(input.begin(), input.end(), ::isdigit)) {
                cout << "Invalid choice. Please enter a number.\n";
                continue;
            }

            int choice = stoi(input);   // Convert input string to integer
            switch (choice) {
            case 0:
                cout << "Goodbye!\n";
                return;
            case 1:
                staffMenu();
                break;
            case 2:
                userMenu();
                break;
            default:
                cout << "Invalid choice.\n";
            }
        }
    }

    // ====== Staff Menu and Reports (Part of LibrarySystem class) ======
    void staffMenu() {
        // Loop until the staff decides to return to the main menu
        while (true) {
            // Display staff menu options
            cout << "\n----- STAFF MENU -----\n"
                << "1. Add Book\n"
                << "2. Delete Book\n"
                << "3. Edit Book by Title\n"
                << "4. List All Books\n"
                << "5. Sort Books by Page Count\n"
                << "6. List Oldest 5 Books\n"
                << "7. Search Book by ID\n"
                << "8. Show Total Number of Books\n"
                << "9. Add Damage Note\n"
                << "10. List Damaged Books\n"
                << "11. Report (Borrowed / Returned / Available)\n"
                << "12. Process Book Requests\n"
                << "13. Show Current Book Requests\n"
                << "14. Tree Report Menu\n"
                << "15. Show High-Rated Books (>=4)\n"
                << "0. Back to Main Menu\n"
                << "Choice: ";

            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 0) break;

            switch (choice) {
            case 1: {    // Add a new book to the system
                int id, pages, year;
                string title, author;
                cout << "Enter ID: "; cin >> id;
                cin.ignore();
                cout << "Enter Title: "; getline(cin, title);
                cout << "Enter Author: "; getline(cin, author);
                cout << "Enter Pages: "; cin >> pages;
                cout << "Enter Year: "; cin >> year;
                if (bookManager.findBook(id) != nullptr) {
                    cout << "Error: Book ID already exists!\n";
                }
                else {
                    bookManager.addBook(id, title, author, pages, year);
                    availableList.addAvailableBook(id, title, author, pages, year);
                    cout << "Book added successfully!\n";
                }

            }
            case 2: {    // Delete book by ID
                int id;
                cout << "Enter ID to delete: ";
                cin >> id;
                if (bookManager.deleteBook(id)) {
                    cout << "Book deleted successfully.\n";
                }
                else {
                    cout << "Book not found.\n";
                }
                break;
            }
            case 3: {    // Edit book by its title
                cout << "Enter title to edit: ";
                string title;
                getline(cin, title);
                bookManager.editBookByTitle(title);
                break;
            }
            case 4:    // Show all books
                bookManager.listBooks();
                break;
            case 5:    // Sort books by number of pages
                bookManager.listBooksByPageCount();
                break;
            case 6:    // Display the 5 oldest books
                bookManager.listOldestFiveBooks();
                break;
            case 7: {     // Search for a book using ID
                int id;
                cout << "Enter Book ID to search: ";
                cin >> id;
                BookManager::Book* book = bookManager.findBook(id);
                if (book) {
                    cout << "ID: " << book->id << " | Title: " << book->title
                        << " | Author: " << book->author
                        << " | Pages: " << book->pages
                        << " | Year: " << book->year << endl;
                }
                else {
                    cout << "Book not found!\n";
                }
                break;
            }
            case 8:    // Display number of books in the library
                cout << "Total books in library: " << bookManager.getBookCount() << endl;
                break;
            case 9: {    // Add a damage note to a book
                int id;
                string note;
                cout << "Enter Book ID for damage note: ";
                cin >> id;
                cin.ignore();
                cout << "Enter damage note: ";
                getline(cin, note);
                bookManager.addDamageNote(id, note);
                break;
            }
            case 10:     // Show all damaged books
                bookManager.listDamagedBooks();
                break;
            case 11:     // Open report menu
                reportMenu();
                break;
            default:
                cout << "Invalid choice.\n";
            case 12:    // Fulfill a book request
                requestQueue.fulfillRequest();
                break;
            case 13:     // Show all current book requests
                requestQueue.printRequests();
                break;
            case 14:     // Open tree-based reports
                reportTreeMenu();
                break;
            case 15:    // Display highly-rated books (rating 4 or more)
                staffRatings.displayHighRatedBooks();
                break;
            }
        }
    }
    // Report menu for staff to display different categories of books
    void reportMenu() {
        while (true) {
            cout << "\n=== REPORT MENU ===\n"
                << "1. Show Borrowed Books\n"
                << "2. Show Returned Books\n"
                << "3. Show Available Books\n"
                << "0. Back to Staff Menu\n"
                << "Choice: ";

            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 0) break;

            switch (choice) {
            case 1:
                borrowedList.displayBorrowedBooks();
                break;
            case 2:
                returnedList.displayReturnedBooks();
                break;
            case 3:
                availableList.displayAvailableBooks();
                break;
            default:
                cout << "Invalid choice.\n";
            }
        }
    }
    // Tree-based report filters for borrowed books
    void reportTreeMenu() {
        while (true) {
            cout << "\n----- TREE REPORT MENU -----\n"
                << "1. Transfer Borrowed Books to Tree\n"
                << "2. Report: Borrow Duration < or >= 30 days\n"
                << "3. Report: Published Before or After 1950\n"
                << "4. Report: Published Before 1950 AND Borrowed > 30 Days\n"
                << "0. Back\n"
                << "Choice: ";

            int choice;
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1:
                addBorrowedBooksToTree();
                break;
            case 2:
                cout << "\n--- Borrow Duration Report ---\n";
                reportBooksByBorrowDuration(borrowTreeRoot);
                break;
            case 3:
                cout << "\n--- Report: Published Before or After 1950 ---\n";
                reportBooksByPublicationYear(borrowTreeRoot);
                break;
            case 4:
                cout << "\n--- Combined Report: Before 1950 AND Borrowed > 30 Days ---\n";
                reportBooksByYearAndDuration(borrowTreeRoot);
                break;
            case 0:
                return;
            default:
                cout << "Invalid choice.\n";
            }
        }
    }
    // ====== User Tree Filter Menu ======
    void reportUserTreeMenu() {
        // Transfer user's borrowed books from the stack into a binary tree
        transferUserBorrowedToTree();

        while (true) {
            // Display tree filtering options to the user
            cout << "\n=== USER TREE FILTER MENU ===\n"
                << "1. Filter by Publisher (TR / EN)\n"
                << "2. Filter by Language (tr / en / both)\n"
                << "3. Filter by Page Count (> or < 1000)\n"
                << "0. Back\n"
                << "Choice: ";

            int choice;
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1: {
                string publisher;
                cout << "Enter publisher to filter (TR / EN): ";
                cin >> publisher;
                cin.ignore();
                cout << "\n--- Filtered by Publisher ---\n";
                reportBooksByPublisher(borrowTreeRoot, publisher);
                break;
            }
            case 2: {
                string lang;
                cout << "Enter language to filter (tr / en / both): ";
                cin >> lang;
                cin.ignore();
                cout << "\n--- Filtered by Language ---\n";
                reportBooksByLanguage(borrowTreeRoot, lang);
                break;
            }
            case 3: {
                int threshold;
                cout << "Enter page count threshold: ";
                cin >> threshold;
                cin.ignore();
                cout << "\n--- Filtered by Page Count ---\n";
                reportBooksByPageCount(borrowTreeRoot, threshold);
                break;
            }
            case 0:
                return;
            default:
                cout << "Invalid choice.\n";
            }
        }
    }
    // ====== Recommendation Based on Borrow History ======
    void recommendSimilarBooks() {
        cout << "\n--- Recommended Books Based on Your Borrowed Books ---\n";

        // Maps to count how many books of each publisher/language the user borrowed
        unordered_map<string, int> publisherCount;
        unordered_map<string, int> languageCount;

        // Traverse the borrowed stack and record stats
        UserStack::StackNode* temp = userBorrowStack.peek();
        while (temp) {
            BookManager::Book* book = bookManager.findBook(temp->id);
            if (book) {
                string publisher = (book->id % 2 == 0) ? "TR" : "EN";
                string language = (book->id % 2 == 0) ? "tr" : "en";
                publisherCount[publisher]++;
                languageCount[language]++;
            }
            temp = temp->next;
        }
        // If user hasn't borrowed anything, show a message
        if (publisherCount.empty()) {
            cout << "You haven't borrowed any books yet.\n";
            return;
        }

        // Determine most borrowed publisher and language
        string topPublisher, topLanguage;
        int maxPub = 0, maxLang = 0;
        for (auto& pair : publisherCount) {
            if (pair.second > maxPub) {
                topPublisher = pair.first;
                maxPub = pair.second;
            }
        }
        for (auto& pair : languageCount) {
            if (pair.second > maxLang) {
                topLanguage = pair.first;
                maxLang = pair.second;
            }
        }

        // Show recommendations based on top publisher or language
        cout << "Based on your borrowing pattern, we recommend books with:\n";
        cout << "- Publisher: " << topPublisher << "\n";
        cout << "- Language: " << topLanguage << "\n\n";

        // Traverse all books and suggest similar ones
        BookManager::Book* current = bookManager.findBook(1); // Start from head
        while (current) {
            string publisher = (current->id % 2 == 0) ? "TR" : "EN";
            string language = (current->id % 2 == 0) ? "tr" : "en";

            if (publisher == topPublisher || language == topLanguage) {
                cout << current->id << " | " << current->title
                    << " by " << current->author
                    << " | Publisher: " << publisher
                    << " | Language: " << language << endl;
            }
            current = current->next;
        }
    }

    // ====== User Main Menu ======
    void userMenu() {
        while (true) {
            // Display available user options
            cout << "\n=== USER MENU ===\n"
                << "1. Add Comment to Book\n"
                << "2. View Comments of Book\n"
                << "3. Create Personal Collection\n"
                << "4. Add Book to Collection\n"
                << "5. Edit Read/Unread Status\n"
                << "6. Delete Book from Collection\n"
                << "7. List My Collection\n"
                << "8. Borrow Book\n"
                << "9. List Borrowed Books\n"
                << "10. Return Book\n"
                << "11. List Returned Books\n"
                << "12. Request Book (Normal)\n"
                << "13. Request Book (Academic)\n"
                << "14. View My Requests\n"
                << "15. User Tree Report Menu\n"
                << "16. Rate Borrowed Book\n"
                << "17. Show My Ratings\n"
                << "18. Recommend Similar Books\n"
                << "0. Back to Main Menu\n"
                << "Choice: ";

            int choice;
            cin >> choice;
            cin.ignore();    // Clear newline after integer input

            if (choice == 0) break;

            switch (choice) {
            case 1: {     // Add a comment to a book
                int id;
                string text;
                cout << "Enter Book ID to comment: ";
                cin >> id;
                cin.ignore();
                cout << "Enter your comment: ";
                getline(cin, text);
                commentSystem.addComment(id, text);
                break;
            }
            case 2: {    // View comments of a specific book
                int id;
                cout << "Enter Book ID to view comments: ";
                cin >> id;
                commentSystem.showComments(id);
                break;
            }
            case 3:    // Create user’s personal collection
                userCollection.createCollection();
                break;
            case 4: {    // Add book to personal collection by ID
                int id;
                cout << "Enter Book ID to add to collection: ";
                cin >> id;
                BookManager::Book* book = bookManager.findBook(id);
                if (book) {
                    userCollection.addBook(book->id, book->title, book->author, book->pages, book->year);
                }
                else {
                    cout << "This book is not available in the library.\n";
                }
                break;
            }
            case 5: {    // Edit read/unread status for a book in collection
                int id;
                cout << "Enter Book ID to edit status: ";
                cin >> id;
                userCollection.editBookStatus(id);
                break;
            }
            case 6: {    // Delete book from collection
                int id;
                cout << "Enter Book ID to delete: ";
                cin >> id;
                userCollection.deleteBook(id);
                break;
            }
            case 7:    // Show user's personal collection
                userCollection.listBooks();
                break;
            case 8: {
                cout << "\n--- Available Books ---\n";
                availableList.displayAvailableBooks();

                int id;
                cout << "Enter Book ID to borrow from the list above: ";
                cin >> id;

                BookManager::Book* book = bookManager.findBook(id);
                if (book) {
                    userBorrowStack.push(book->id, book->title, book->author);
                    borrowedList.addBorrowedBook(book->id, book->title, book->author, book->pages, book->year);
                    availableList.removeBook(book->id);
                    cout << "Book borrowed successfully!\n";
                }
                else {
                    cout << "Book not found!\n";
                }
                break;
            }

            case 9:    // Display borrowed books from the stack
                userBorrowStack.display();
                break;
            case 10: {    // Return a borrowed book
                if (userBorrowStack.isEmpty()) {
                    cout << "You have no borrowed books to return.\n";
                    break;
                }

                cout << "\n--- Your Borrowed Books ---\n";
                userBorrowStack.display();

                int id;
                cout << "Enter Book ID to return: ";
                cin >> id;

                // Find the book in the borrow stack
                UserStack::StackNode* prev = nullptr;
                UserStack::StackNode* current = userBorrowStack.top;
                while (current && current->id != id) {
                    prev = current;
                    current = current->next;
                }

                if (!current) {
                    cout << "You have not borrowed a book with this ID.\n";
                    break;
                }
                // Add book to return stack and return list
                userReturnStack.push(current->id, current->title, current->author);

                string borrowDate = borrowedList.getBorrowDate(current->id);
                returnedList.addReturnedBook(current->id, current->title, current->author, 0, 0, borrowDate);
                availableList.addBorrowedBook(current->id, current->title, current->author, 0, 0);
                borrowedList.removeBook(current->id);

                // Remove book from borrow stack
                if (current == userBorrowStack.top) {
                    userBorrowStack.pop();
                }
                else if (prev) {
                    prev->next = current->next;
                    delete current;
                }

                cout << "Book returned successfully!\n";
                break;
            }
            case 11:    // Display books user has returned
                userReturnStack.display();
                break;
            default:
                cout << "Invalid choice.\n";
            case 12: {    // Request a book (normal)
                string title, author;
                cout << "Enter Book Title: ";
                getline(cin, title);
                cout << "Enter Book Author: ";
                getline(cin, author);
                requestQueue.enqueue(title, author, false);
                break;
            }
            case 13: {    // Request a book (academic)
                string title, author;
                cout << "Enter Book Title: ";
                getline(cin, title);
                cout << "Enter Book Author: ";
                getline(cin, author);
                requestQueue.enqueue(title, author, true);
                break;
            }
            case 14:    // Show all book requests
                requestQueue.printRequests();
                break;
            case 15:    // User filters tree reports
                reportUserTreeMenu();
                break;
            case 16: {    // Rate a borrowed book
                int id, rating;
                cout << "Enter Book ID to rate: ";
                cin >> id;
                cin.ignore();

                bool found = false;
                UserStack::StackNode* temp = userBorrowStack.top;
                while (temp) {
                    if (temp->id == id) {
                        found = true;
                        break;
                    }
                    temp = temp->next;
                }

                if (!found) {
                    cout << " You can only rate books you have borrowed.\n";
                    break;
                }

                BookManager::Book* book = bookManager.findBook(id);
                if (!book) {
                    cout << " Book not found!\n";
                    break;
                }

                cout << "Enter your rating (1-5): ";
                cin >> rating;
                cin.ignore();

                if (rating < 1 || rating > 5) {
                    cout << " Invalid rating. Must be between 1 and 5.\n";
                    break;
                }
                // Store rating in both user and staff tables
                userRatings.insert(book->id, book->title, rating);
                staffRatings.insert(book->id, book->title, rating);

                cout << " Rating submitted!\n";
                break;
            }

            case 17:     // Display user-rated books
                userRatings.displayAllRatings();
                break;
            case 18:    // Show similar book recommendations
                recommendSimilarBooks();
                break;
            }
        }
    }
    // ======  User Request Menu ======
    void requestUserMenu() {
        int choice;
        do {
            cout << "\n--- USER REQUEST MENU ---\n"
                << "1. Request book (normal)\n"
                << "2. Request book (academic)\n"
                << "3. View requests\n"
                << "4. Exit\n"
                << "Choice: ";
            cin >> choice;
            cin.ignore();

            if (choice == 1 || choice == 2) {
                string title, author;
                cout << "Enter Book Title: ";
                getline(cin, title);
                cout << "Enter Book Author: ";
                getline(cin, author);
                bool isAcademic = (choice == 2);
                requestQueue.enqueue(title, author, isAcademic);
            }
            else if (choice == 3) {
                requestQueue.printRequests();
            }
            else if (choice == 4) {
                cout << "Exiting user request menu.\n";
            }
            else {
                cout << "Invalid choice.\n";
            }
        } while (choice != 4);
    }
    // This menu allows staff to fulfill or view book requests.
    void requestStaffMenu() {
        int choice;
        do {
            cout << "\n--- STAFF REQUEST MENU ---\n"
                << "1. Fulfill next request\n"
                << "2. View all requests\n"
                << "3. Exit\n"
                << "Choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1:
                // Process the highest priority request (academic first)
                requestQueue.fulfillRequest();
                break;
            case 2:
                // Show all current book requests
                requestQueue.printRequests();
                break;
            case 3:
                // Exit this staff request menu
                cout << "Exiting staff request menu.\n";
                break;
            default:
                cout << "Invalid choice.\n";
            }
        } while (choice != 3);
    }

};

// ====== Main Function ======
// Entry point of the program. Creates LibrarySystem object and starts the program.
int main() {
    LibrarySystem library;
    library.run(); // Launch the main menu (staff/user)
    return 0;
};
// ====== Remove Book from Circular List ======
// This function removes a book (by ID) from a circular singly linked list.
void CircularList::removeBook(int id) {
    if (!head) return; // Empty list

    Node* current = head;
    Node* prev = nullptr;

    do {
        if (current->id == id) {
            if (current == head) {
                if (head->next == head) {
                    // Only one node in the list
                    head = nullptr;
                }
                else {
                    // Update tail to point to new head
                    Node* temp = head;
                    while (temp->next != head)
                        temp = temp->next;
                    temp->next = head->next;
                    head = head->next;
                }
            }
            else {
                // Removing a node that's not the head
                prev->next = current->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    } while (current != head);
}

