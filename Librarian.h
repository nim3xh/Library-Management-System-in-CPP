#ifndef LIBRARIAN_H
#define LIBRARIAN_H

#include "Person.h"
#include "Member.h"
#include "Book.h"
#include "Date.h"
#include <vector>

class Librarian : public Person {
private:
    int salary;
    int staffID;


public:
    std::vector<Book> books;
    std::vector<Member> members;
    Librarian(int staffID, const std::string& name, const std::string& address, const std::string& email, int salary);

    void addMember();
    void issueBook(int memberID, int bookID);
    void returnBook(int memberID, int bookID);
    void displayBorrowedBooks(int memberID);
    void calcFine(int memberID);

    int getStaffID() const;
    int getSalary() const;
    void setStaffID(int newStaffID);
    void setSalary(int newSalary);
};

#endif
