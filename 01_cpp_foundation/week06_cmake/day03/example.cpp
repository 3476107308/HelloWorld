// Week06 Day03：lambda 表达式与 std::function 回调
// 目标：把“做什么”作为参数传给算法或业务函数，减少模块之间的耦合。

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using std::copy_if;
using std::count_if;
using std::cout;
using std::endl;
using std::function;
using std::sort;
using std::string;
using std::vector;

void print_scores(const vector<int>& scores, const string& title)
{
    cout << title << ": ";
    for (int score : scores)
    {
        cout << score << ' ';
    }
    cout << endl;
}

void demonstrate_sort_lambda()
{
    vector<int> scores{72, 45, 90, 61, 88, 59};

    // sort 只负责遍历和交换，具体的“谁应该排在前面”由 lambda 提供。
    sort(scores.begin(), scores.end(), [](int left, int right) {
        return left > right;
    });

    print_scores(scores, "降序成绩");
}

void demonstrate_capture()
{
    vector<int> scores{72, 45, 90, 61, 88, 59};
    int pass_line = 60;
    vector<int> passed_scores;

    // [pass_line] 表示按值捕获；lambda 内部使用的是 pass_line 的副本。
    copy_if(scores.begin(), scores.end(), std::back_inserter(passed_scores),
        [pass_line](int score) {
            return score >= pass_line;
        });

    print_scores(passed_scores, "及格成绩");

    // [&pass_line] 表示按引用捕获；lambda 访问的是外部原变量。
    pass_line = 70;
    int high_score_count = static_cast<int>(count_if(
        scores.begin(), scores.end(), [&pass_line](int score) {
            return score >= pass_line;
        }));

    cout << "达到新分数线 " << pass_line << " 的人数："
         << high_score_count << endl;
}

// std::function<void(int)> 表示：可以保存“接收 int、返回 void”的可调用对象。
void run_task(const function<void(int)>& callback)
{
    for (int progress = 0; progress <= 100; progress += 20)
    {
        callback(progress);
    }
}

void print_progress(int progress)
{
    cout << "普通函数回调：" << progress << "%" << endl;
}

void demonstrate_callback()
{
    // lambda 可以直接作为回调传入，不需要额外声明一个函数名。
    run_task([](int progress) {
        cout << "lambda 回调：" << progress << "%" << endl;
    });

    // 普通函数也符合 std::function<void(int)> 的调用签名。
    run_task(print_progress);

    int base = 10;
    // 按值捕获 base，回调保存自己的副本。
    function<void(int)> add_base = [base](int value) {
        cout << "value + base = " << value + base << endl;
    };
    add_base(5);
}

int main()
{
    cout << "===== Week06 Day03 example =====" << endl;
    demonstrate_sort_lambda();
    demonstrate_capture();
    demonstrate_callback();
    return 0;
}
