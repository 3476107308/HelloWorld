#include"Zoo.h"
#include<windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    Zoo zoo1;
    Cat* cat1 = new Cat("cat1",1);
    Cat* cat2 = new Cat("cat2",2);
    Dog* dog1 = new Dog("dog1",3);
    Dog* dog2 = new Dog("dog2",4);
    Bird* bird1 = new Bird("bird1",5);
    Bird* bird2 = new Bird("bird2",6);

    zoo1.add_animal(cat1);
    zoo1.add_animal(cat2);
    zoo1.add_animal(dog1);
    zoo1.add_animal(dog2);
    zoo1.add_animal(bird1);
    zoo1.add_animal(bird2);

    zoo1.show_all();
    zoo1.all_speak();
    zoo1.all_move();



    if(zoo1.find("cat2") != nullptr)
    {
        cout << "能够找到" << "cat2" << endl;
    }
    else
    {
        cout << "不能找到" << "cat2" << endl;
    }
    if(zoo1.find("111") != nullptr)
    {
        cout << "能够找到" << "111" << endl;
    }
     else
    {
        cout << "不能找到" << "111" << endl;
    }

    
    return 0;
}