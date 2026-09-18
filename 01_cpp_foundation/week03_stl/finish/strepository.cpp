#include"strepository.h"
#include<algorithm>
#include<iterator>

bool Strepository::add(Student s)
{
    if(index_by_id.count(s.id()) != 0) return false;
    student_.push_back(s);
    index_by_id[s.id()] = student_.size() - 1;
    index_by_school[s.school()].push_back(student_.size() - 1);
    index_by_name[s.name()].push_back(student_.size() - 1);
    return true;
}

bool Strepository::remove(string id)
{
    if(index_by_id.count(id) == 0) return false;
    int pos = index_by_id[id];
    student_.erase(student_.begin() + pos);
    index_by_id.clear();
    for(size_t i = 0;i < student_.size();i++)
    {
        index_by_id[student_[i].id()] = i;
    }
    index_by_school.clear();
    for(size_t i = 0;i < student_.size();i++)
    {
        index_by_school[student_[i].school()].push_back(i);
    }
    index_by_name.clear();
    for(size_t i = 0;i < student_.size();i++)
    {
        index_by_name[student_[i].name()].push_back(i);
    }
    return true;
}

bool Strepository::update(string id,Student s)
{
    if(index_by_id.count(id) == 0) return false;
    int pos  = index_by_id[id];
    student_[pos] = s;

    index_by_id.clear();
    for(size_t i = 0;i < student_.size();i++)
    {
        index_by_id[student_[i].id()] = i;
    }
    index_by_name.clear();
    for(size_t i = 0;i < student_.size();i++)
    {
        index_by_name[student_[i].name()].push_back(i);
    }
    index_by_school.clear();
    for (size_t i = 0; i < student_.size(); i++)
    {
        index_by_school[student_[i].school()].push_back(i);
    }
    return true;
}

Student* Strepository::find_by_id(string id)
{
    if(index_by_id.count(id) == 0) return nullptr;
    int pos = index_by_id[id];
    return &student_[pos];
}

vector<Student> Strepository::find_by_name(string name)
{
    if(index_by_name.count(name) == 0) return vector<Student>();
    vector<Student> result;
    for(int idx : index_by_name[name])
    {
        result.push_back(student_[idx]);
    }
    return result;
}

vector<Student> Strepository::find_by_school(string school)
{
    if(index_by_school.count(school) == 0) return vector<Student>();
    vector<Student> result;
    for(int idx : index_by_school[school])
    {
        result.push_back(student_[idx]);
    }
    return result;
}

vector<Student> Strepository::list_all()const
{
    vector<Student> v = student_;
    return v;
}

vector<Student> Strepository::sort_by_total(bool issorted)
{
    vector<Student> result = student_;
    if(issorted == true)
    {
        sort(result.begin(),result.end(),
        [](const Student& a,const Student& b)
        {
            if(a.total() == b.total())
                return a.id() < b.id();
            return a.total() < b.total();
        });
    }
    else
    {
        sort(result.begin(),result.end(),
        [](const Student& a,const Student& b)
        {
            if(a.total() == b.total())
                return a.id() < b.id();
            return a.total() > b.total();
        });
    }
    return result;
}

int Strepository::count()const
{
    return student_.size();
}