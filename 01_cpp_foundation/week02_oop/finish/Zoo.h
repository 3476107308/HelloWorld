#ifndef ZOO_H
#define ZOO_H

#include"Bird.h"
#include"Cat.h"
#include"Dog.h"
#include<vector>
using std::vector;
using std::endl;

class Zoo
{
    private:
    vector<Animal*> animal_;
    public:
    ~Zoo()
    {
        for(auto temp:animal_)
        {
            delete temp;
            temp = nullptr;
        }
        cout << "动物园已经关闭\n"; 
    }
    void add_animal(Animal* animal)
    {
        animal_.push_back(animal);
    }
    void show_all()const
    {
        for(auto temp:animal_)
        {
            cout << "名字:" << temp->get_name() << " 年龄：" << temp->get_age() << endl;
        }
    }
    void all_speak()const
    {
        for(auto temp:animal_)
        {
            temp->speak();
        }
    }
    void all_move()const
    {
        for(auto temp:animal_)
        {
            temp->move();
        }
    }
    Animal* find(string name)
    {
        for(auto temp:animal_)
        {
            if(temp->get_name() == name)
            return temp;
        }    
        return nullptr;
    }
};

#endif 