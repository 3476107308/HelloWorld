#pragma once
#include"student.h"
#include<vector>
#include<unordered_map>

using std::vector;
using std::unordered_map;

class Strepository
{
private:
    vector<Student> student_;
    unordered_map<string,int> index_by_id;
    unordered_map<string,vector<int>> index_by_name;
    unordered_map<string,vector<int>> index_by_school;


public:
    bool add(Student s);
    bool remove(string id);
    bool update(string id,Student s);
    Student* find_by_id(string id);
    vector<Student> find_by_name(string name);
    vector<Student> find_by_school(string school);
    vector<Student> list_all()const;
    vector<Student> sort_by_total(bool issorted);
    int count()const;
};