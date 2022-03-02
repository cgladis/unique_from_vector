#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <random>
#include <unordered_set>

#define C_WHITE "\033[0m"
#define C_BLUE "\033[34m"
#define C_YELLOW "\033[33m"
#define C_MAGENTA "\033[35m"

std::vector<int> create_arr(size_t n, int range)
{
    std::vector<int> arr;

    std::random_device rd;   // non-deterministic generator
    std::mt19937 gen(rd());  // to seed mersenne twister.
    std::uniform_int_distribution<> dist(1,range);

    arr.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        arr.push_back(dist(gen));
    }
    return arr;
}

void unique_by_set(const std::vector<int>& src_arr){

    std::set<int> temp_set;
    for (int num: src_arr) {
        temp_set.insert(num);
    }
//    return {std::make_move_iterator(temp_set.begin()),std::make_move_iterator(temp_set.end())};
}

void unique_by_sort(const std::vector<int>& src_arr){

    std::vector<int> arr = src_arr;
    std::sort(arr.begin(), arr.end());
    arr.erase(std::unique(arr.begin(), arr.end()), arr.end());

//    return {std::make_move_iterator(arr.begin()),std::make_move_iterator(arr.end())};
}

void unique_by_unordered_set(const std::vector<int>& src_arr){

    std::unordered_set<int> temp_set;

    for (int num: src_arr) {
        if (temp_set.find(num) != temp_set.end())
            temp_set.insert(num);
    }
}

auto decorator(const std::function <void (std::vector<int>)>& func){

    return [func](std::vector<int> value)
    {
        auto start = std::chrono::steady_clock::now();
        func(std::move(value));
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << C_BLUE << " execution time: " << C_YELLOW << elapsed_ms.count() << C_BLUE << " ms" << C_WHITE << std::endl;
    };
}

void test (int count, const std::string& fill){
    static int num = 1;
    static auto fcn_unique1 = decorator(unique_by_set);
    static auto fcn_unique2 = decorator(unique_by_sort);
    static auto fcn_unique3 = decorator(unique_by_unordered_set);

    std::vector<int> src_arr;
    int range = 1;
    fill == "минимальное"  && (range = count);
    fill == "среднее"  && (range = count / 2);
    src_arr = create_arr(count, range);

    std::cout << C_MAGENTA << "Тест № " << C_YELLOW << num << C_WHITE << std::endl;
    std::cout << C_MAGENTA << "Количество элементов: " << C_YELLOW << count << C_WHITE << std::endl;
    std::cout << C_MAGENTA << "Количество повторяющихся элементов: " << C_YELLOW << fill << C_WHITE << std::endl;

    std::cout << "через методы vector:         ";
    fcn_unique2(src_arr);
    std::cout << "set через set:               ";
    fcn_unique1(src_arr);
    std::cout << "через unordered_set:         ";
    fcn_unique3(src_arr);
    std::cout << std::endl;

    num++;
}


int main() {

    test(100000, "минимальное");
    test(100000, "среднее");
    test(100000, "100%");

    test(1000000, "минимальное");
    test(1000000, "среднее");
    test(1000000, "100%");

    test(10000000, "минимальное");
    test(10000000, "среднее");
    test(10000000, "100%");
}
