#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
private:
    std::string name;
    std::string address;
    std::string email;

public:
    Person();
    Person(const std::string& name, const std::string& address, const std::string& email);

    std::string getName() const;
    std::string getAddress() const;
    std::string getEmail() const;

    void setName(const std::string& name);
    void setAddress(const std::string& address);
    void setEmail(const std::string& email);
};

#endif
