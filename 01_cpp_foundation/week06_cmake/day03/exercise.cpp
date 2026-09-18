// Week06 Day03：lambda 与 std::function 练习
// 说明：保留 TODO，请先独立完成，再上传 Ubuntu 构建。

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>

using std::cout;
using std::endl;
using std::function;
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

void test_score_algorithms()
{
    vector<int> scores{72, 45, 90, 61, 88, 59, 100};

    // TODO 1：使用 std::sort 和 lambda 将 scores 按降序排列。
    // 要求：不能手写冒泡排序；排序规则写在 lambda 中。
    std::sort(scores.begin(),scores.end(),[](int left,int right)
    {
        return left > right;
    });
    // TODO 2：定义 pass_line = 60，使用 std::copy_if 和 lambda
    // 将达到及格线的成绩复制到 passed_scores。
    vector<int> passed_scores;
    int pass_line = 60;
    std::copy_if(scores.begin(),scores.end(),std::back_inserter(passed_scores),[pass_line](int score)
    {
        return score >= pass_line;
    });

    // TODO 3：使用 std::count_if 和 lambda 统计及格人数。
    int passed_count = 
    static_cast<int>(std::count_if(scores.begin(),scores.end(),[pass_line](int score)
    {
        return score >= pass_line;
    }));
    print_scores(scores, "排序后的成绩");
    print_scores(passed_scores, "及格成绩");
    cout << "及格人数：" << passed_count << endl;
}

// TODO 4：实现 run_task。
// 要求：
// - 参数类型为 const std::function<void(int)>&。
// - progress 从 0 开始，每次增加 20，直到 100。
// - 每次循环调用 callback(progress)。
void run_task(const std::function<void(int)>& callback)
{
    // TODO：完成进度循环和回调调用。
    for(int progress = 0;progress <= 100;progress += 20)
    {
        callback(progress);
    }
}

// TODO 5：实现普通函数回调。
void print_progress(int progress)
{
    // TODO：打印 progress。
    std::cout << "普通函数回调：" << progress  << "%"<< std::endl;

}

void test_callbacks()
{
    // TODO 6：传入 lambda，打印“lambda 回调：xx%”。
    // run_task(...);
    run_task([](int progress){
        cout << "lambda回调:" << progress << "%" << endl;
    });
    // TODO 7：传入普通函数 print_progress。
    // run_task(print_progress);
    run_task(print_progress);
}

void test_capture()
{
    vector<int> scores{55, 60, 72, 88};
    int pass_line = 60;

    // TODO 8：使用 [pass_line] 按值捕获，筛选成绩。
    vector<int> result;
    std::copy_if(scores.begin(),scores.end(),std::back_inserter(result),[pass_line](int score){
        return score >= pass_line;
    });
    // TODO 9：修改 pass_line 为 70，再使用 [&pass_line] 按引用捕获统计人数。
    pass_line = 70;
    int count = static_cast<int>(std::count_if(scores.begin(),scores.end(),[&pass_line](int score){
        return score >= pass_line;
    }));
    // 要在注释中写清楚两种捕获方式的区别。
    //[pass_line]:按值捕获，Lambda 保存pass_line的副本
    //[&pass_line]:按引用捕获，Lambda使用外部的pass_line原变量
    print_scores(result,"按值捕获筛选结果");
    cout << "按引用捕获统计结果:" << count << endl;
}

int main()
{
    cout << "===== Week06 Day03 exercise =====" << endl;
    test_score_algorithms();
    test_callbacks();
    test_capture();
    return 0;
}
