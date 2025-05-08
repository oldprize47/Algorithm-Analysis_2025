/*
    (1) Lecture slide chapter 6. page 14
    (2) Blog: ** URL here **
    (3) Book: -
    (4) Hyperscale AI: ChatGPT
*/

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using namespace chrono;

struct Item {
    int weight;
    int value;
    double value_per_weight;
};

vector<Item> make_items(int n) {
    const int SEED = 100;
    std::mt19937 rng(SEED);

    vector<Item> items(n);
    for (int i = 0; i < n; ++i) {
        int v = rng() % 500 + 1;
        int w = rng() % 100 + 1;
        items[i] = {w, v, 1.0 * v / w};
    }
    return items;
}

bool cmp_value_per_weight(const Item& a, const Item& b) {
    return a.value_per_weight > b.value_per_weight;
}

// 완전탐색 함수
long long Brute_force(const vector<Item>& items, int capacity) {
    int n = (int)items.size();
    uint64_t total = 1ULL << n;  // 2^n
    long long best = 0;

    for (uint64_t mask = 0; mask < total; ++mask) {
        long long w_sum = 0, v_sum = 0;

        // mask 비트마다 포함 여부 체크
        for (int i = 0; i < n; i++) {
            if (mask & (1ULL << i)) {
                w_sum += items[i].weight;
                if (w_sum > capacity) {
                    // 용량 초과이면 더 볼 필요 없음
                    break;
                }
                v_sum += items[i].value;
            }
        }
        // 용량 안 넘고, 이익이 최대치면 갱신
        if (w_sum <= capacity && v_sum > best) {
            best = v_sum;
        }
    }

    return best;
}

long long Greedy(vector<Item> items, int capacity) {
    const int n = items.size();
    long long w_sum = 0, v_sum = 0;
    // 가성비 순으로 정렬
    sort(items.begin(), items.end(), cmp_value_per_weight);
    // 무게 초과하기 직전까지 담기
    for (int i = 0; i < n; i++) {
        if (w_sum + items[i].weight <= capacity) {
            w_sum += items[i].weight;
            v_sum += items[i].value;
        } else {
            v_sum += items[i].value_per_weight * (capacity - w_sum);  // 초과하면 가치 / 무게 * 남은 용량
            break;
        }
    }
    return v_sum;
}

long long DP(const vector<Item>& items, int capacity) {
    const int n = items.size();
    vector<vector<long long>> dp(n + 1, vector<long long>(capacity + 1));

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

long long BnB(vector<Item> items, int capacity) {
    const int n = items.size();
    long long w_sum = 0, v_sum = 0;
    // 가성비 순으로 정렬
    sort(items.begin(), items.end(), cmp_value_per_weight);
    // 무게 초과하기 직전까지 담기
    for (int i = 0; i < n; i++) {
        if (w_sum + items[i].weight <= capacity) {
            w_sum += items[i].weight;
            v_sum += items[i].value;
        } else {
            v_sum += items[i].value_per_weight * (capacity - w_sum);  // 초과하면 가치 / 무게 * 남은 용량
            break;
        }
    }
    return v_sum;
}

int main() {
    std::chrono::high_resolution_clock::time_point start, end;
    int n = 0;
    cout << "Enter the number of Items (N): ";
    cin >> n;
    vector<Item> items = make_items(n);
    int capacity = n * 25;
    // cout << endl
    //      << endl
    //      << "After sorting" << endl
    //      << endl;
    // for (int i = 0; i < n; i++) cout << "items[" << i << "]: Value = " << items[i].value << ", Weight = " << items[i].weight << ", Value_per_Weight = " << items[i].value_per_weight << endl;

    if (n < 31) {  // Bruth force
        start = high_resolution_clock::now();
        long long Brute_force_best = Brute_force(items, capacity);
        end = high_resolution_clock::now();
        auto Brute_force_duration = duration<double, milli>(end - start).count();

        cout << endl;
        cout << "-------------------------------------" << endl;
        cout << "Processing time in milliseconds(Brute_force): " << Brute_force_duration << " milliseconds" << endl
             << endl;
        cout << "Maximum benefit value (Brute_force): " << Brute_force_best << endl
             << endl;
    }

    start = high_resolution_clock::now();
    long long Greedy_best = Greedy(items, capacity);
    end = high_resolution_clock::now();
    auto Greedy_duration = duration<double, milli>(end - start).count();

    cout << endl;
    cout << "-------------------------------------" << endl;
    cout << "Processing time in milliseconds(Greedy): " << Greedy_duration << " milliseconds" << endl
         << endl;
    cout << "Maximum benefit value (Greedy): " << Greedy_best << endl
         << endl;

    start = high_resolution_clock::now();
    long long DP_best = DP(items, capacity);
    end = high_resolution_clock::now();
    auto DP_duration = duration<double, milli>(end - start).count();

    cout << endl;
    cout << "-------------------------------------" << endl;
    cout << "Processing time in milliseconds(DP): " << DP_duration << " milliseconds" << endl
         << endl;
    cout << "Maximum benefit value (DP): " << DP_best << endl
         << endl;

    return 0;
}