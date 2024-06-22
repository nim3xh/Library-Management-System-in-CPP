#include <iostream>
#include <vector>
#include "Person.h"
#include "Member.h"
#include "Book.h"
#include "Librarian.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <algorithm>

std::vector<Book> parseCSV(const std::string& filename);
std::vector<Member> parseMembersCSV(const std::string& filename);
std::vector<Member> parseBorrowedBooksCSV(const std::string& filename, const std::vector<Member>& members, std::vector<Book>& books);
void saveBorrowedBooksCSV(const std::string& filename, const std::vector<Member>& members);

int main() {
    Librarian librarian(1, "Librarian Name", "Library Address", "librarian@email.com", 50000);
    librarian.books = parseCSV("library_books.csv");
    librarian.members = parseMembersCSV("library_members.csv");
    librarian.members = parseBorrowedBooksCSV("library_borrowed_books.csv", librarian.members,librarian.books);

    while (true) {
        std::cout << "\n========== Library System Menu ==========\n"
          << "1. Create Member Account\n"
          << "2. Add New Book to Collection\n"
          << "3. Borrow a Book\n"
          << "4. Return a Borrowed Book\n"
          << "5. Display Borrowed Books\n"
          << "6. Exit\n"
          << "=========================================\n"
          << "Enter your choice (1-6): ";


        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                int memberID;
                std::cout << "Enter Member ID: ";
                std::cin >> memberID;

                auto existingMember = std::find_if(librarian.members.begin(), librarian.members.end(),
                                                   [memberID](const Member& member) { return member.getMemberID() == memberID; });

                if (existingMember != librarian.members.end()) {
                    std::cout << "A member with the same ID already exists. Please choose a different Member ID.\n";
                } else {
                    std::string name, address, email;

                    std::cin.ignore();
                    std::cout << "Enter Name: ";
                    std::getline(std::cin, name);
                    std::cout << "Enter Address: ";
                    std::getline(std::cin, address);
                    std::cout << "Enter Email: ";
                    std::getline(std::cin, email);

                    Member newMember(memberID, name, address, email);
                    librarian.members.push_back(newMember);

                    std::ofstream file("library_members.csv", std::ios::app);
                    if (file.is_open()) {
                        file << newMember.getMemberID() << ","
                             << newMember.getName() << ","
                             << newMember.getAddress() << ","
                             << newMember.getEmail() << "\n";
                        file.close();
                    } else {
                        std::cerr << "Error opening file for writing: library_data.csv" << std::endl;
                    }

                    std::cout << "Member added successfully!\n";
                }
                break;
            }
            case 2: {
                int bookID;
                std::string bookName, authorFirstName, authorLastName, bookType;

                std::cout << "Enter Book ID: ";
                std::cin >> bookID;

                auto existingBook = std::find_if(librarian.books.begin(), librarian.books.end(),
                                                [bookID](const Book& book) { return book.getBookID() == bookID; });

                if (existingBook != librarian.books.end()) {
                    std::cout << "A book with the same ID already exists. Please choose a different Book ID.\n";
                } else {
                    std::cin.ignore();
                    std::cout << "Enter Book Name: ";
                    std::getline(std::cin, bookName);
                    std::cout << "Enter Author First Name: ";
                    std::getline(std::cin, authorFirstName);
                    std::cout << "Enter Author Last Name: ";
                    std::getline(std::cin, authorLastName);

                    Book newBook(bookID, bookName, authorFirstName, authorLastName);
                    librarian.books.push_back(newBook);

                    std::ofstream file("library_books.csv", std::ios::app);

                    if (file.is_open()) {
                        file << newBook.getBookID() << ","
                            << newBook.getBookName() << ","
                            << "N/A," 
                            << newBook.getAuthorFirstName() << ","
                            << newBook.getAuthorLastName() << ","
                            << "N/A\n"; 
                        file.close();
                    } else {
                        std::cerr << "Error opening file for writing: library_books.csv" << std::endl;
                    }

                    std::cout << "Book created successfully!\n";
                }
                break;
            }
            case 3: {
                int memberID, bookID;
                std::cout << "Enter Member ID: ";
                std::cin >> memberID;
                std::cout << "Enter Book ID: ";
                std::cin >> bookID;

                librarian.issueBook(memberID, bookID);
                break;
            }
            case 4: {
                int memberID, bookID;
                std::cout << "Enter Member ID: ";
                std::cin >> memberID;
                std::cout << "Enter Book ID: ";
                std::cin >> bookID;

                librarian.returnBook(memberID, bookID);
                break;
            }
            case 5: {
                int memberID;
                std::cout << "Enter Member ID: ";
                std::cin >> memberID;

                librarian.displayBorrowedBooks(memberID);
                break;
            }
            case 6:
                saveBorrowedBooksCSV("library_borrowed_books.csv", librarian.members);
                std::cout << "Exiting the program. Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 6.\n";
        }
    }
    return 0;
}



std::vector<Book> parseCSV(const std::string& filename) {
    std::vector<Book> books;

    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    std::cout << "Current working directory: " << cwd << std::endl;

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return books;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        int bookID;
        std::string bookName, authorFirstName, authorLastName;

        std::getline(iss, token, ',');
        bookID = std::stoi(token);

        std::getline(iss, token, ',');
        if (token.front() == '"' && token.back() != '"') {
            bookName = token.substr(1);
            while (std::getline(iss, token, ',')) {
                bookName += "," + token;
                if (token.back() == '"') {
                    bookName.pop_back();
                    break;
                }
            }
        } else {
            bookName = token;
        }

        std::getline(iss, token, ',');

        std::getline(iss, authorFirstName, ',');
        std::getline(iss, authorLastName, ',');

        books.emplace_back(bookID, bookName, authorFirstName, authorLastName);
    }

    file.close();
    return books;
}

std::vector<Member> parseMembersCSV(const std::string& filename) {
    std::vector<Member> members;

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return members;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        int memberID;
        std::string name, address, email;

        std::getline(iss, token, ',');
        memberID = std::stoi(token);

        std::getline(iss, name, ',');
        std::getline(iss, address, ',');
        std::getline(iss, email, ',');

        members.emplace_back(memberID, name, address, email);
    }

    file.close();
    return members;
}

std::vector<Member> parseBorrowedBooksCSV(const std::string& filename, const std::vector<Member>& members, std::vector<Book>& books) {
    std::vector<Member> updatedMembers = members;

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return updatedMembers;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        int memberID, bookID;
        std::getline(iss, token, ',');
        memberID = std::stoi(token);

        std::getline(iss, token, ',');
        bookID = std::stoi(token);

        auto it = std::find_if(updatedMembers.begin(), updatedMembers.end(),
                               [memberID](const Member& m) { return m.getMemberID() == memberID; });

        if (it != updatedMembers.end()) {
            Member& member = *it;

            auto bookIt = std::find_if(books.begin(), books.end(),
                                       [bookID](const Book& b) { return b.getBookID() == bookID; });

            if (bookIt != books.end()) {
                Book& book = *bookIt;

                std::getline(iss, token, ',');  
                std::getline(iss, token, ',');  
                std::getline(iss, token, ','); 

                int dueDay, dueMonth, dueYear;
                char delimiter;

                iss >> dueDay >> delimiter >> dueMonth >> delimiter >> dueYear;

                Date dueDate(dueDay, dueMonth, dueYear);

                book.setDueDate(dueDate);
                book.borrowBook(&member, dueDate);
                member.setBooksBorrowed(book);
            }
        }
    }

    file.close();
    return updatedMembers;
}


void saveBorrowedBooksCSV(const std::string& filename, const std::vector<Member>& members) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    file << "MemberID,BookID,BookName,AuthorFirstName,AuthorLastName,DueDate\n";

    for (const Member& member : members) {
        const std::vector<Book>& borrowedBooks = member.getBooksBorrowed();
        for (const Book& book : borrowedBooks) {
            file << member.getMemberID() << "," << book.getBookID() << ","
                 << book.getBookName() << "," << book.getAuthorFirstName() << ","
                 << book.getAuthorLastName() << "," << book.getDueDate().getDay() << "/"
                 << book.getDueDate().getMonth() << "/" << book.getDueDate().getYear() << "\n";
        }
    }

    file.close();
}