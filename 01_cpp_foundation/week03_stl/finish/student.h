#pragma once
#include<string>
using std::string;

class Student
{
private:
    string id_;
    string name_;
    string school_;
    string major_;
    int chinese_;
    int math_;
    int english_;
public:
    Student(string id,string name,string school,string major,int chinese,int math,int english):id_(id),name_(name),school_(school),major_(major),chinese_(chinese),math_(math),english_(english){}
    int total()const{return chinese_ + math_ + english_;}
    double average()const{return total() / 3.0;}
    const string& id()const {return id_;}
    const string& name()const {return name_;}
    const string& school()const {return school_;}
    const string& major()const {return major_;}
    int chinese()const{return chinese_;}
    int math()const {return math_;}
    int english()const {return english_;}
};