#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

int recursive (int k, int n);
int DP(int k, int n);

int main(){
    int k = 0, n = 0;
    cout << "Enter the number of meters (N): ";
    cin >> n;
    cout << "Enter the number of meters (K): ";
    cin >> k;

    if( k <= 0 ){
        cout << "Error: Unable to check (K = 0)" << endl;
        return 0;
    }else if(n < 0){
        cout << "Error: Negative numbers are not allowed" << endl;
        return 0;
    }else if( n >= 25){
        cout << "Error: Too big data" << endl;
        return 0;
    }

    auto start = high_resolution_clock::now();
    int re_answer = recursive(k,n);
    auto end = high_resolution_clock::now();
    auto re_duration = duration_cast<microseconds>(end - start);

    start = high_resolution_clock::now();
    int dp_answer = DP(k,n);
    end = high_resolution_clock::now();
    auto dp_duration = duration_cast<microseconds>(end - start);

    cout << endl;
    cout << "-------------------------------------" << endl;
    cout << "Minimum Attempts (Recursive): " << re_answer << endl;
    cout << "Execution Time (Recursive): " << re_duration.count() << " microseconds" << endl << endl;

    cout << "Minimum Attempts (DP): " << dp_answer << endl;
    cout << "Execution Time (DP): " << dp_duration.count() << " microseconds" << endl;
    cout << "-------------------------------------" << endl;
    return 0;
}

int recursive (int k, int n) { 
    // Write base case code here
    if(k == 1){
        return n;
    }else if(n <= 1){
        return n;
    }
    int min_attempts = n;
    // Try dropping from every height X (1 to N) and take the worst case
    for (int x = 1; x <= n; x++) {
        // case1: Object breaks. Recursive call
        int case_1 = recursive(k-1, x-1);
        // case2: Object survives. Recursive call
        int case_2 = recursive(k, n-x);
        // worst_case = 1 + max(case1, case2)
        int worst_case = 1 + max(case_1,case_2);
        // min_attempts = min(min_attempts, worst_case);
        min_attempts = min(min_attempts, worst_case);
    }
    return min_attempts;
}

int DP(int k, int n) { // DP Table: dp[i][j] represents minimum attempts for i objects and j meters
    vector<vector<int>> dp(k+1, vector<int>(n+1, 0));
    int p = 0;
    // Fill base cases
    for(int i = 1; i <= n; i++){
        dp[1][i] = i;
        cout << "dp[1][" << i << "]: " << i << "    ";
    }
    cout << endl;
    // Fill the DP table
    for (int i = 2; i <= k; i++) { // Objects
        dp[i][1] = 1;
        cout << "dp[" << i << "][1]: 1" << "    ";
        for (int j = 2; j <= n; j++) { // Meters
            dp[i][j] = j;
            // Try dropping from every height X (1 to j)
            for (int x = 1; x <= j; x++) {
                // case 1: when object breaks, b = dp[.....][.....];
                int b = dp[i-1][x-1];
                // case 2: when object survives, s = dp[.....][.....];
                int s = dp[i][j-x];
                int worst = 1 + max(b, s);
                if (worst < dp[i][j]) {
                    dp[i][j] = worst;
                }
            } // end of for x
            cout << "dp[" << i << "][" << j << "]: " << dp[i][j] << "    ";
        } // end of for j
        cout << endl;
    } // end of for i
    cout << endl;

    return dp[k][n];
}