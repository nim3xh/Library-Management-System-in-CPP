#include <iostream>
#include <chrono>
#include <algorithm>
#include "Librarian.h"

Librarian::Librarian(int staffID, const std::string& name, const std::string& address, const std::string& email, int salary)
        : Person(name, address, email), staffID(staffID), salary(salary) {}

void Librarian::addMember() {
    int memberID;
    std::string name, address, email;

    std::cout << "Enter Member ID: ";
    std::cin >> memberID;
    std::cin.ignore();
    std::cout << "Enter Name: ";
    std::getline(std::cin, name);
    std::cout << "Enter Address: ";
    std::getline(std::cin, address);
    std::cout << "Enter Email: ";
    std::getline(std::cin, email);

    Member newMember(memberID, name, address, email);
    members.push_back(newMember);

    std::cout << "Member added successfully!\n";
    std::cout << "New Member Details:\n";
    std::cout << "Member ID: " << newMember.getMemberID() << "\n";
    std::cout << "Name: " << newMember.getName() << "\n";
    std::cout << "Address: " << newMember.getAddress() << "\n";
    std::cout << "Email: " << newMember.getEmail() << "\n";
}

void Librarian::issueBook(int memberID, int bookID) {
    Member* member = nullptr;
    Book* book = nullptr;

    for (Member& m : members) {
        if (m.getMemberID() == memberID) {
            member = &m;
            //std::cout << "getMemberID!\n";
            break;
        }
    }

    for (Book& b : books) {
        if (b.getBookID() == bookID) {
            book = &b;
            //std::cout << "getBookID!\n";
            break;
        }
    }

    if (member == nullptr || book == nullptr) {
        std::cout << "Member or book not found." << std::endl;
        return;
    }

    if (book->getDueDate().getYear() != 0) {
        std::cout << "Book is not available for borrowing." << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    struct tm* dateInfo = localtime(&currentTime);

    Date dueDate(dateInfo->tm_mday + 3, dateInfo->tm_mon + 1, dateInfo->tm_year + 1900);

    book->setDueDate(dueDate);
    book->borrowBook(member, dueDate);
    member->setBooksBorrowed(*book);

    std::cout << "Book issued successfully! Due Date: " << dueDate.getDay() << "/"
              << dueDate.getMonth() << "/" << dueDate.getYear() << std::endl;
}

void Librarian::returnBook(int memberID, int bookID) {
    Member* member = nullptr;
    Book* book = nullptr;

    for (Member& m : members) {
        if (m.getMemberID() == memberID) {
            member = &m;
            break;
        }
    }

    for (Book& b : books) {
        if (b.getBookID() == bookID) {
            book = &b;
            break;
        }
    }

    if (member == nullptr || book == nullptr) {
        std::cout << "Member or book not found." << std::endl;
        return;
    }

    bool bookBorrowed = false;
    for (const Book& borrowedBook : member->getBooksBorrowed()) {
        if (borrowedBook.getBookID() == bookID) {
            bookBorrowed = true;
            break;
        }
    }

    if (!bookBorrowed) {
        std::cout << "This member did not borrow this book." << std::endl;
        return;
    }

//    auto now = std::chrono::system_clock::now();
//    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
//    struct std::tm* timeInfo = std::localtime(&currentTime);
//
//    Date currentDate(timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);
//
//    if (currentDate.isPast(book->getDueDate())) {
//        int fine;
//        int daysOverdue = currentDate.getDay() - book->getDueDate().getDay();
//        if(daysOverdue>0){
//            fine = daysOverdue;  // £1 per day
//        }else{
//            fine = 0;
//        }
//        std::cout << "Fine for Book ID " << bookID << ": pound" << fine << std::endl;
//    }
    calcFine(memberID);

    std::vector<Book>& borrowedBooks = const_cast<std::vector<Book> &>(member->getBooksBorrowed());
    borrowedBooks.erase(std::remove_if(borrowedBooks.begin(), borrowedBooks.end(),
                                       [bookID](const Book& b) { return b.getBookID() == bookID; }),
                        borrowedBooks.end());

    book->returnBook();

    std::cout << "Book returned successfully!" << std::endl;
}

void Librarian::displayBorrowedBooks(int memberID) {
    Member* member = nullptr;

    for (Member& m : members) {
        if (m.getMemberID() == memberID) {
            member = &m;
            break;
        }
    }


    if (member == nullptr) {
        std::cout << "Member not found." << std::endl;
        return;
    }

    const std::vector<Book>& borrowedBooks = member->getBooksBorrowed();

    if (borrowedBooks.empty()) {
        std::cout << "No books borrowed by Member " << memberID << ".\n";
    } else {
        std::cout << "Books borrowed by Member " << memberID << ": \n";
        for (const Book& book : borrowedBooks) {
            std::cout << "Book ID: " << book.getBookID() << " - " << book.getBookName() << "\n";
            std::cout << "   Due Date: " << book.getDueDate().getDay() << "/"
                      << book.getDueDate().getMonth() << "/" << book.getDueDate().getYear() << "\n";
        }
    }
}

void Librarian::calcFine(int memberID) {
    Member* member = nullptr;

    for (Member& m : members) {
        if (m.getMemberID() == memberID) {
            member = &m;
            break;
        }
    }

    if (member == nullptr) {
        std::cout << "Member not found." << std::endl;
        return;
    }

    const std::vector<Book>& borrowedBooks = member->getBooksBorrowed();

    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    struct tm* dateInfo = localtime(&currentTime);

    Date currentDate(dateInfo->tm_mday, dateInfo->tm_mon + 1, dateInfo->tm_year + 1900);

    for (const Book& book : borrowedBooks) {
        if (currentDate.isPast(book.getDueDate())) {
            int daysOverdue = currentDate.getDay() - book.getDueDate().getDay();
            int fine;
            if(daysOverdue>0){
                fine = daysOverdue;  // £1 per day
            }else{
                fine = 0;
            }
            std::cout << "Fine for Book ID " << book.getBookID() << ": pound " << fine << std::endl;
        }
    }
}


int Librarian::getStaffID() const {
    return staffID;
}

int Librarian::getSalary() const {
    return salary;
}

void Librarian::setStaffID(int newStaffID) {
    staffID = newStaffID;
}

void Librarian::setSalary(int newSalary) {
    salary = newSalary;
}
