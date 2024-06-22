#include "Member.h"

Member::Member(int memberID, const std::string& name, const std::string& address, const std::string& email)
        : Person(name, address, email), memberID(memberID) {}

int Member::getMemberID() const {
    return memberID;
}

const std::vector<Book>& Member::getBooksBorrowed() const {
    return booksLoaned;
}

void Member::setBooksBorrowed(const Book& book) {
    booksLoaned.push_back(book);
}
