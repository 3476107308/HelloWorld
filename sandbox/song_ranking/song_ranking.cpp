#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <unordered_map>
#include<windows.h>

using namespace std;

// 歌曲结构体
struct Song {
    string name;        // 歌名（唯一主键）
    string singer;      // 演唱者
    string creator;     // 创作者
    string genre;       // 类别
    int year;           // 发行年份
    int loveLevel;      // 喜爱程度1-10
    string style;       // 曲风
    int searchCnt;      // 查找次数，统计热门

    // 重载==用于去重判断
    bool operator==(const Song& other) const {
        return name == other.name;
    }
};

// 全局歌曲库、热门歌单、用户浏览记录
vector<Song> songLib;
unordered_map<string, vector<Song>> userPlayList; // 用户->歌单
unordered_map<string, vector<string>> browseLog;   // 用户浏览日志
const string DATA_FILE = "song_data.dat";
const int PAGE_SIZE = 5; // 每页展示条数

// 哈希散列：按歌名哈希查找
size_t hashSongName(const string& name) {
    return hash<string>()(name);
}

// 从文件加载数据
void loadFromFile() {
    ifstream fin(DATA_FILE, ios::binary);
    if (!fin.is_open()) return;
    Song s;
    while (fin.read((char*)&s, sizeof(Song))) {
        songLib.push_back(s);
    }
    fin.close();
}

// 保存数据到文件
void saveToFile() {
    ofstream fout(DATA_FILE, ios::binary);
    for (auto& s : songLib) {
        fout.write((char*)&s, sizeof(Song));
    }
    fout.close();
}

// 添加歌曲（查重）
void addSong(const Song& newSong) {
    for (auto& s : songLib) {
        if (s == newSong) {
            cout << "⚠️ 歌曲已存在，禁止重复添加！\n";
            return;
        }
    }
    songLib.push_back(newSong);
    saveToFile();
    cout << "✅ 歌曲添加成功！\n";
}

// 删除歌曲（按歌名）
void delSong(const string& name) {
    for (auto it = songLib.begin(); it != songLib.end(); ++it) {
        if (it->name == name) {
            songLib.erase(it);
            saveToFile();
            cout << "✅ 删除成功！\n";
            return;
        }
    }
    cout << "❌ 未找到该歌曲！\n";
}

// 修改歌曲信息（不能改歌名）
void modifySong(const string& name) {
    for (auto& s : songLib) {
        if (s.name == name) {
            cout << "=== 修改歌曲信息（歌名不可改）===\n";
            cout << "输入新演唱者："; cin >> s.singer;
            cout << "输入新创作者："; cin >> s.creator;
            cout << "输入新类别："; cin >> s.genre;
            cout << "输入新年份："; cin >> s.year;
            cout << "输入喜爱程度(1-10)："; cin >> s.loveLevel;
            cout << "输入新风曲："; cin >> s.style;
            saveToFile();
            cout << "✅ 修改完成！\n";
            return;
        }
    }
    cout << "❌ 未找到目标歌曲！\n";
}

// 按歌名排序播放列表
void sortByName(vector<Song>& list) {
    sort(list.begin(), list.end(), [](const Song& a, const Song& b) {
        return a.name < b.name;
        });
}

// 分页展示歌曲列表
void showPage(const vector<Song>& list) {
    if (list.empty()) {
        cout << "📭 暂无数据\n";
        return;
    }
    int total = list.size();
    int page = 0;
    while (true) {
        int start = page * PAGE_SIZE;
        int end = min(start + PAGE_SIZE, total);
        cout << "\n===== 第" << page + 1 << "页 / 共" << (total + PAGE_SIZE - 1) / PAGE_SIZE << "页 =====\n";
        cout << left << setw(15) << "歌名" << setw(12) << "歌手" << setw(10) << "类别" << setw(6) << "年份" << setw(8) << "喜爱度\n";
        for (int i = start; i < end; ++i) {
            auto& s = list[i];
            cout << left << setw(15) << s.name << setw(12) << s.singer << setw(10) << s.genre
                << setw(6) << s.year << setw(8) << s.loveLevel << "\n";
        }
        cout << "输入0退出翻页，输入页码跳转：";
        int op; cin >> op;
        if (op == 0) break;
        page = op - 1;
        if (page < 0 || page * PAGE_SIZE >= total) page = 0;
    }
}

// 多条件查找
vector<Song> searchSong() {
    vector<Song> res;
    cout << "\n===== 查找方式 ====\n";
    cout << "1.按歌名 2.按歌手 3.按类别 4.多条件组合\n";
    int op; cin >> op;
    if (op == 1) {
        string name; cout << "输入歌名："; cin >> name;
        for (auto& s : songLib) {
            if (s.name.find(name) != string::npos) {
                s.searchCnt++;
                res.push_back(s);
            }
        }
    }
    else if (op == 2) {
        string singer; cout << "输入歌手："; cin >> singer;
        for (auto& s : songLib) {
            if (s.singer.find(singer) != string::npos) {
                s.searchCnt++;
                res.push_back(s);
            }
        }
    }
    else if (op == 3) {
        string genre; cout << "输入类别："; cin >> genre;
        for (auto& s : songLib) {
            if (s.genre == genre) {
                s.searchCnt++;
                res.push_back(s);
            }
        }
    }
    else if (op == 4) {
        string n, si, g; int y;
        cout << "歌名(空跳过)："; cin >> n;
        cout << "歌手(空跳过)："; cin >> si;
        cout << "类别(空跳过)："; cin >> g;
        cout << "年份(0跳过)："; cin >> y;
        for (auto& s : songLib) {
            bool ok = true;
            if (!n.empty() && s.name.find(n) == string::npos) ok = false;
            if (!si.empty() && s.singer.find(si) == string::npos) ok = false;
            if (!g.empty() && s.genre != g) ok = false;
            if (y != 0 && s.year != y) ok = false;
            if (ok) {
                s.searchCnt++;
                res.push_back(s);
            }
        }
    }
    sortByName(res); // 结果按歌名排序
    return res;
}

// 生成热门歌单（按查找次数降序）
vector<Song> getHotList() {
    vector<Song> tmp = songLib;
    sort(tmp.begin(), tmp.end(), [](const Song& a, const Song& b) {
        return a.searchCnt > b.searchCnt;
        });
    return tmp;
}

// 歌单管理
void playlistManage() {
    string userName;
    cout << "输入你的用户名："; cin >> userName;
    cout << "\n1.添加歌曲到歌单 2.删除歌单歌曲 3.查看我的歌单\n";
    int op; cin >> op;
    if (op == 1) {
        string name; cout << "要加入的歌曲名："; cin >> name;
        for (auto& s : songLib) {
            if (s.name == name) {
                userPlayList[userName].push_back(s);
                cout << "✅ 加入歌单成功\n";
                return;
            }
        }
        cout << "❌ 歌曲不存在\n";
    }
    else if (op == 2) {
        string name; cout << "要移除的歌曲名："; cin >> name;
        auto& pl = userPlayList[userName];
        for (auto it = pl.begin(); it != pl.end(); ++it) {
            if (it->name == name) {
                pl.erase(it);
                cout << "✅ 移除成功\n";
                return;
            }
        }
        cout << "❌ 歌单内无此歌曲\n";
    }
    else if (op == 3) {
        auto pl = userPlayList[userName];
        sortByName(pl);
        showPage(pl);
    }
}

// 主菜单
void menu() {
    loadFromFile(); // 启动加载历史数据
    int choice;
    while (true) {
        system("cls");
        cout << "==================== 个人乐库管理系统 ====================\n";
        cout << "1.新增歌曲  2.删除歌曲  3.修改歌曲信息\n";
        cout << "4.多条件查找歌曲  5.浏览全部歌曲  6.查看热门歌单\n";
        cout << "7.个人歌单管理  0.退出系统\n";
        cout << "==========================================================\n";
        cout << "请输入操作序号：";
        cin >> choice;
        if (choice == 0) {
            saveToFile();
            cout << "数据已保存，系统退出！\n";
            break;
        }
        if (choice == 1) {
            Song s;
            cout << "请输入歌曲信息：\n";
            cout << "歌名："; cin >> s.name;
            cout << "演唱者："; cin >> s.singer;
            cout << "创作者："; cin >> s.creator;
            cout << "类别："; cin >> s.genre;
            cout << "发行年份："; cin >> s.year;
            cout << "喜爱程度(1-10)："; cin >> s.loveLevel;
            cout << "曲风："; cin >> s.style;
            s.searchCnt = 0;
            addSong(s);
        }
        else if (choice == 2) {
            string n; cout << "输入要删除的歌名："; cin >> n;
            delSong(n);
        }
        else if (choice == 3) {
            string n; cout << "输入要修改的歌名："; cin >> n;
            modifySong(n);
        }
        else if (choice == 4) {
            auto res = searchSong();
            showPage(res);
        }
        else if (choice == 5) {
            vector<Song> all = songLib;
            sortByName(all);
            showPage(all);
        }
        else if (choice == 6) {
            auto hot = getHotList();
            showPage(hot);
        }
        else if (choice == 7) {
            playlistManage();
        }
        cout << "\n按回车键继续...";
        cin.get(); cin.get();
    }
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    menu();
    return 0;
}



/*
第 1 步：装 MSYS2

  去 https://www.msys2.org/ 下载安装包，一路默认安装。

  第 2 步：打开 MSYS2 UCRT64 终端

  开始菜单搜 "MSYS2 UCRT64"，打开一个蓝色终端窗口。

  第 3 步：更新包管理器

  pacman -Syu

  第 4 步：装编译工具

  pacman -S mingw-w64-ucrt-x86_64-gcc
  pacman -S mingw-w64-ucrt-x86_64-gdb
  pacman -S mingw-w64-ucrt-x86_64-cmake

  验证：

  g++ --version
  gdb --version
  cmake --version

  第 5 步：装 VS Code

  去 https://code.visualstudio.com/ 下载安装。然后装两个插件：
  - C/C++ (Microsoft)
  - 可选：简体中文语言包

  第 6 步：让 VS Code 找到 MSYS2

  - C/C++ (Microsoft)
  - 可选：简体中文语言包

  第 6 步：让 VS Code 找到 MSYS2

  VS Code → 设置 → 搜索 terminal.integrated.profiles.windows，或者在 VS Code 中按 F1 输入 Terminal: Select Default
  Profile，选 Git Bash 或配置 MSYS2 的 bash 路径。

  第 7 步：拉代码开始写

  git clone https://github.com/3476107308/cpp-study.git
  cd cpp-study
  g++ -std=c++17 -g -Wall -O0 hello.cpp -o hello.exe

  ▎ 注意：MSYS2 UCRT64 终端和 VS Code 自带的终端是独立的。建议在 MSYS2 终端里编译调试，VS Code 只当编辑器用来写代码。
*/