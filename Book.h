#ifndef BOOK_H
#define BOOK_H

#include <string>
#include "Date.h"
class Member;

class Book {
private:
    int bookID;
    std::string bookName;
    std::string authorFirstName;
    std::string authorLastName;
    std::string bookType;
    Date dueDate;
    Member* borrower;

public:
    Book(int bookID, const std::string& bookName, const std::string& authorFirstName, const std::string& authorLastName);
    int getBookID() const;
    const std::string& getBookName() const;
    const std::string& getAuthorFirstName() const;
    const std::string& getAuthorLastName() const;
    const std::string& getBookType() const;
    const Date& getDueDate() const;
    void setDueDate(const Date& dueDate);
    void returnBook();
    void borrowBook(Member* borrower, const Date& dueDate);
};

#endif
