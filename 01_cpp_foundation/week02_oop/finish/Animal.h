#ifndef ANIMAL_H
#define ANIMAL_H
#include<iostream>
#include<string>
using std::string;
class Animal
{
    protected:
    string name_;
    int age_;

    public:
    Animal(string name,int age):name_(name),age_(age){}
    virtual ~Animal() = 0;
    virtual void speak() = 0;
    virtual void move() = 0;
    string get_name()const {return name_;}
    int get_age()const {return age_;}
};
Animal::~Animal(){};
#endif 