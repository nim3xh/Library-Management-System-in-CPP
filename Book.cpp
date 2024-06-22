#include "Book.h"
#include "Member.h"

Book::Book(int bookID, const std::string& bookName, const std::string& authorFirstName, const std::string& authorLastName)
        : bookID(bookID), bookName(bookName), authorFirstName(authorFirstName), authorLastName(authorLastName), dueDate(0, 0, 0), borrower(nullptr) {}

int Book::getBookID() const {
    return bookID;
}

const std::string& Book::getBookName() const {
    return bookName;
}

const std::string& Book::getAuthorFirstName() const {
    return authorFirstName;
}

const std::string& Book::getAuthorLastName() const {
    return authorLastName;
}

const std::string& Book::getBookType() const {
    return bookType;
}

const Date& Book::getDueDate() const {
    return dueDate;
}

void Book::setDueDate(const Date& newDueDate) {
    dueDate = newDueDate;
}

void Book::returnBook() {
    borrower = nullptr;
}

void Book::borrowBook(Member* newBorrower, const Date& newDueDate) {
    borrower = newBorrower;
    dueDate = newDueDate;
}
