/*
    (1) Lecture slide chapter 6. page 14,27
    (2) Blog:   https://min-zero.tistory.com/entry/C-%EA%B8%B0%EB%B3%B8-%EA%B3%B5%EB%B6%80%EC%A0%95%EB%A6%AC-9-%EA%B5%AC%EC%A1%B0%EC%B2%B4struct (구조체 생성자)
                https://jungeu1509.github.io/algorithm/use-priorityqueue/#11-%ED%97%A4%EB%8D%94 (우선 순위 큐)
                https://hwan-shell.tistory.com/119 (vector)
                https://blog.naver.com/thebaleuncoding/221922917364 (문자열 간격)
    (3) Book: -
    (4) Hyperscale AI: ChatGPT
*/

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

using namespace std;
using namespace chrono;
int col1 = 18, col2 = 80;

struct Item {
    int value;
    int weight;
    double value_per_weight;
    double bound;
    int item_idx;
    Item(int v, int w, double vpw) : value(v), weight(w), value_per_weight(vpw) {}
    Item() {
        value = 0;
        weight = 0;
        value_per_weight = 0;
        bound = 0;
        item_idx = 0;
    }
};

// Generate items (value, weight, value/weight)
vector<Item> make_items(int n) {
    const int SEED = 100;
    mt19937 rng(SEED);

    vector<Item> items(n);
    for (int i = 0; i < n; ++i) {
        int v = rng() % 500 + 1;
        int w = rng() % 100 + 1;
        items[i] = {v, w, 1.0 * v / w};
    }
    return items;
}

// Compare by value-to-weight ratio (descending)
bool cmp_value_per_weight(const Item& a, const Item& b) {
    return a.value_per_weight > b.value_per_weight;
}

struct cmp {
    // Priority Queue: max-heap by bound
    bool operator()(const Item& a, const Item& b) {
        return a.bound < b.bound;
    }
};

// Brute-force (2^n subsets)
int Brute_force(const vector<Item>& items, int capacity) {
    int n = (int)items.size();
    uint64_t total = 1ULL << n;  // 2^n
    int best = 0;

    for (uint64_t mask = 0; mask < total; ++mask) {
        int w_sum = 0, v_sum = 0;

        // mask 비트마다 포함 여부 체크
        for (int i = 0; i < n; i++) {
            if (mask & (1ULL << i)) {
                w_sum += items[i].weight;
                if (w_sum > capacity) break;  // Over capacity → skip
                v_sum += items[i].value;
            }
        }
        // update if better & within capacity
        if (w_sum <= capacity && v_sum > best) {
            best = v_sum;
        }
    }

    return best;
}

// Greedy: pick items by value/weight until full
int Greedy(const vector<Item>& items, int capacity, int start = 0, int curr_weight = 0) {
    const int n = items.size();
    int v_sum = 0;
    // fill until weight limit
    for (int i = start; i < n; i++) {
        if (curr_weight + items[i].weight <= capacity) {
            curr_weight += items[i].weight;
            v_sum += items[i].value;
        } else {
            v_sum += items[i].value_per_weight * (capacity - curr_weight);  // add fractional value
            break;
        }
    }
    return v_sum;
}

// Dynamic Programming (0/1 Knapsack)
int DP(const vector<Item>& items, int capacity) {
    const int n = items.size();
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1));

    for (int w = 0; w <= capacity; w++) dp[0][w] = 0;
    for (int i = 1; i <= n; i++) {
        dp[i][0] = 0;
        int w_i = items[i - 1].weight;
        int b_i = items[i - 1].value;
        for (int w = 1; w <= capacity; w++) {
            if (w_i <= w)
                if (b_i + dp[i - 1][w - w_i] > dp[i - 1][w])
                    dp[i][w] = b_i + dp[i - 1][w - w_i];
                else
                    dp[i][w] = dp[i - 1][w];
            else
                dp[i][w] = dp[i - 1][w];
        }
    }

    return dp[n][capacity];
}

// Branch and Bound
int BnB(const vector<Item>& items, int capacity) {
    const int n = items.size();
    int max_benefit = 0;
    priority_queue<Item, vector<Item>, cmp> heap;
    Item cur_node, left_node, right_node;  // current, left (include), right (exclude)
    cur_node.bound = Greedy(items, capacity);
    heap.push(cur_node);
    while (!heap.empty()) {
        cur_node = heap.top();
        heap.pop();
        if (cur_node.bound <= max_benefit) continue;  // skip if not promising

        if (cur_node.weight + items[cur_node.item_idx].weight <= capacity) {  // if includable
            left_node.value = cur_node.value + items[cur_node.item_idx].value;
            left_node.weight = cur_node.weight + items[cur_node.item_idx].weight;
            left_node.item_idx = cur_node.item_idx + 1;
            left_node.bound = left_node.value + Greedy(items, capacity, cur_node.item_idx + 1, left_node.weight);
            if (left_node.item_idx < n)
                heap.push(left_node);
            if (left_node.value > max_benefit) max_benefit = left_node.value;
        }
        right_node.value = cur_node.value;
        right_node.weight = cur_node.weight;
        right_node.item_idx = cur_node.item_idx + 1;
        right_node.bound = right_node.value + Greedy(items, capacity, cur_node.item_idx + 1, right_node.weight);
        if (right_node.item_idx < n)
            heap.push(right_node);
    }

    return max_benefit;
}
void print_line() {
    cout << left << setw(col1 + col2 + 3) << "---------------------------------------------------------------------------------------------------------" << endl;
}
void print_title1() {
    cout << "\n 1) Brute Force" << endl;
    print_line();
    cout << left << setw(col1) << " Number of Items" << " | " << setw(col2) << "                   Processing time in milliseconds / Max benefit                   |" << endl;
    print_line();
}

void print_title2() {
    cout << " 2) Others" << endl;
    print_line();
    cout << left << setw(col1) << "    Number of" << " | " << setw(col2) << "                   Processing time in milliseconds / Max benefit                   |" << endl;
    cout << left << setw(col1) << " " << " | " << setw(col2) << "------------------------------------------------------------------------------------" << endl;
    cout << left << setw(col1) << "      Items" << " | " << setw(col2) << "            Greedy         |           D. P.           |           B. & B.         |" << endl;
    print_line();
}
void print_time_benefit(double processing_time, int max_benefit) {
    cout << right << setw(13) << processing_time << setw(3) << " / " << left << setw(11) << max_benefit << "|";
}
int main() {
    std::chrono::high_resolution_clock::time_point start, end;

    vector<vector<Item>> items(7);
    vector<int> capacity(7);
    vector<int> size_num = {11, 21, 31, 10, 100, 1000, 10000};
    int max_benefit = 0;
    double processing_time = 0;
    double total_minute = 0;
    print_title1();
    for (int i = 0; i < 3; i++) {  // Did not implement brute force for (data size > 31)
        items[i] = make_items(size_num[i]);
        capacity[i] = size_num[i] * 25;

        // Brute Force
        start = high_resolution_clock::now();
        max_benefit = Brute_force(items[i], capacity[i]);
        end = high_resolution_clock::now();
        processing_time = duration<double, milli>(end - start).count();
        total_minute += processing_time;
        cout << right << setw(col1 - 5) << size_num[i] << "      | " << setw(35) << processing_time << setw(5) << "  /  " << left << setw(43) << max_benefit << "|" << endl;
        print_line();
    }
    cout << endl;
    print_title2();
    for (int i = 3; i < 7; i++) {
        items[i] = make_items(size_num[i]);
        capacity[i] = size_num[i] * 25;
        cout << right << setw(col1 - 5) << size_num[i] << "      | ";
        // 가성비 순으로 정렬
        sort(items[i].begin(), items[i].end(), cmp_value_per_weight);

        // Greedy
        start = high_resolution_clock::now();
        max_benefit = Greedy(items[i], capacity[i]);
        end = high_resolution_clock::now();
        processing_time = duration<double, milli>(end - start).count();
        print_time_benefit(processing_time, max_benefit);
        total_minute += processing_time;
        // Dynamic programming
        start = high_resolution_clock::now();
        max_benefit = DP(items[i], capacity[i]);
        end = high_resolution_clock::now();
        processing_time = duration<double, milli>(end - start).count();
        print_time_benefit(processing_time, max_benefit);
        total_minute += processing_time;
        // Branch & Bound
        start = high_resolution_clock::now();
        max_benefit = BnB(items[i], capacity[i]);
        end = high_resolution_clock::now();
        processing_time = duration<double, milli>(end - start).count();
        print_time_benefit(processing_time, max_benefit);
        total_minute += processing_time;
        // next
        cout << endl;
        print_line();
    }
    cout << "\nTotal minute : " << total_minute / 60000.0 << " min" << endl
         << endl;
    return 0;
}