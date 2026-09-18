#include"strepository.h"
#include<iostream>
#include<windows.h>
#include<algorithm>
#include<limits>
using std::cout;
using std::cin;
using std::endl;

void menu_add(Strepository& repo);
void menu_delete(Strepository& repo);
void menu_update(Strepository& repo);
void menu_find(Strepository& repo);
void menu_find_by_name(Strepository& repo);
void menu_find_by_school(Strepository& repo);
void menu_list(Strepository& repo);
void menu_sort(Strepository& repo);
void menu_stats(Strepository& repo);

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    Strepository repo;
    int choice;
    while(true)
    {
        cout << "===== 高校学生信息查询系统 =====\n"
        << "1. 添加学生\n"
        << "2. 删除学生\n"
        << "3. 修改学生\n"
        << "4. 按学号查询\n"
        << "5. 按姓名查询\n"
        << "6. 按院系查询\n"
        << "7. 查看全部学生\n"
        << "8. 按总分排序查看\n"
        << "9. 统计信息\n"
        << "10. 退出\n"
        << "请选择(1-10)\n";
        while(!(cin >> choice) || choice < 1 || choice > 10)
        {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(),'\n');
            cout << "输入无效，请重新输入（1-10）\n";
        }


    switch (choice)
    {
        case 1:  menu_add(repo);           break;
        case 2:  menu_delete(repo);        break;
        case 3:  menu_update(repo);        break;
        case 4:  menu_find(repo);          break;
        case 5:  menu_find_by_name(repo);  break;
        case 6:  menu_find_by_school(repo);break;
        case 7:  menu_list(repo);          break;
        case 8:  menu_sort(repo);          break;
        case 9:  menu_stats(repo);         break;
        case 10: cout << "再见\n"; return 0;
        default: cout << "输入错误，请重新选择\n";
        }
    }
    return 0;
}

void menu_add(Strepository& repo)
{
    cout << "请输入学号、姓名、院系、专业、语数英成绩\n";
    string id, name, school, major;
    int a, b, c;
    cin >> id >> name >> school >> major;
    while(true)
    {
        cin >> a >> b >> c;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            cout << "成绩输入不合法，请重新输入成绩:\n";
            continue;
        }
        if(a >= 0 && a <= 100
        && b >= 0 && b <= 100
        && c >= 0 && c <= 100
        ) break;
        else
        cout << "成绩输入不合法 请重新输入成绩\n";
    }
    if(repo.add(Student(id, name, school, major, a, b, c)))
    cout << "添加成功\n";
    else
    cout << "添加失败\n";
}

void menu_delete(Strepository& repo)
{
    cout << "请输入要删除学生的学号\n";
    string id;
    cin >> id;
    if(repo.remove(id))
    cout << "删除成功\n";
    else
    cout << "删除失败\n";
}

void menu_update(Strepository& repo)
{
    cout << "请输入要修改学生的学号\n";
    string id, name, school, major;
    cin >> id;
    cout << "请输入修改后的姓名、院系、专业和成绩\n";
    int a, b, c;
    cin >> name >> school >> major;
   while(true)
    {
        cin >> a >> b >> c;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            cout << "成绩输入不合法，请重新输入成绩:\n";
            continue;
        }
        if(a >= 0 && a <= 100
        && b >= 0 && b <= 100
        && c >= 0 && c <= 100
        ) break;
        else
        cout << "成绩输入不合法 请重新输入成绩\n";
    }
    if(repo.update(id, Student(id, name, school, major, a, b, c)))
    cout << "修改成功\n";
    else
    cout << "修改失败,该学号不存在\n";
}

void menu_find(Strepository& repo)
{
    cout << "请输入学生的学号\n";
    string id;
    cin >> id;
    Student* temp = repo.find_by_id(id);
    if(temp == nullptr)
    cout << "不存在该学生\n";
    else
    {
        cout << "学号：" << temp->id()
             << "  姓名：" << temp->name()
             << "  院系：" << temp->school()
             << "  专业：" << temp->major()
             << "  语文：" << temp->chinese()
             << "  数学：" << temp->math()
             << "  英语：" << temp->english();
    }
    cout << endl;
}

void menu_find_by_name(Strepository& repo)
{
    cout << "请输入学生姓名\n";
    string name;
    cin >> name;
    vector<Student> result = repo.find_by_name(name);
    if(result.empty())
    {
        cout << "不存在姓名为 " << name << " 的学生\n";
        return;
    }
    for(auto& s : result)
    {
        cout << "学号：" << s.id()
             << "  姓名：" << s.name()
             << "  院系：" << s.school()
             << "  专业：" << s.major()
             << "  语文：" << s.chinese()
             << "  数学：" << s.math()
             << "  英语：" << s.english()
             << endl;
    }
}

void menu_find_by_school(Strepository& repo)
{
    cout << "请输入院系名称\n";
    string school;
    cin >> school;
    vector<Student> result = repo.find_by_school(school);
    if(result.empty())
    {
        cout << "不存在院系为 " << school << " 的学生\n";
        return;
    }
    for(auto& s : result)
    {
        cout << "学号：" << s.id()
             << "  姓名：" << s.name()
             << "  院系：" << s.school()
             << "  专业：" << s.major()
             << "  语文：" << s.chinese()
             << "  数学：" << s.math()
             << "  英语：" << s.english()
             << endl;
    }
}

void menu_list(Strepository& repo)
{
    vector<Student> v = repo.list_all();
    for(auto temp:v)
    {
        cout << "学号：" << temp.id()
             << "  姓名：" << temp.name()
             << "  院系：" << temp.school()
             << "  专业：" << temp.major()
             << "  语文：" << temp.chinese()
             << "  数学：" << temp.math()
             << "  英语：" << temp.english()
             << endl;
    }
}

void menu_sort(Strepository& repo)
{
    cout << "升序还是降序\n";
    string a;
    cin >> a;
    vector<Student> result;
    if(a == "升序")
    result = repo.sort_by_total(true);
    else
    result = repo.sort_by_total(false);
    for(auto s:result)
    {
        cout << s.id() << " " << s.name() << " 总分：" << s.total() << endl;
    }
}

void menu_stats(Strepository& repo)
{
    auto all = repo.list_all();
    int n = repo.count();

    cout << "===== 统计信息 =====\n"
         << "学生总数：" << n << endl;
    if(n == 0)
    {
        cout << "不存在学生\n";
        return;
    }
    double sumChinese = 0, sumMath = 0, sumEnglish = 0;
    for_each(all.begin(), all.end(),
        [&](const Student& s) {
            sumChinese += s.chinese();
            sumMath    += s.math();
            sumEnglish += s.english();
        });

    double maxAvg = -1;
    for (auto& s : all)
        if (s.average() > maxAvg) maxAvg = s.average();

    auto best = find_if(all.begin(), all.end(),
        [maxAvg](const Student& s) { return s.average() == maxAvg; });

    cout << "平均分最高的学生：" << best->name()
         << "(" << best->id() << ")，平均分 " << best->average() << endl;
    cout << "语文平均分：" << sumChinese / n << endl;
    cout << "数学平均分：" << sumMath / n << endl;
    cout << "英语平均分：" << sumEnglish / n << endl;

    int excellent = count_if(all.begin(), all.end(),
        [](const Student& s) {
            return s.chinese() >= 90 && s.math() >= 90 && s.english() >= 90;
        });

    int failed = count_if(all.begin(), all.end(),
        [](const Student& s) {
            return s.chinese() < 60 || s.math() < 60 || s.english() < 60;
        });

    cout << "90分以上学生数:" << excellent << "人  (三科都 >= 90)\n";
    cout << "不及格学生数:" << failed << "人    (任一科 < 60)\n";
}
