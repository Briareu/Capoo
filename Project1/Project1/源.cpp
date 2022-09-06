#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<stack>
#include<algorithm>

using namespace std;

class Solution {
public:
	int cuttingRope(int n) {//dp[n] = max{dp[i] * (n - i)}
		int *dp = new int[n + 1];
		if (n <= 1) return n;
		int max = 1;
		for (int k = 1; k < n; k++)
		{
			dp[k] = 1;
			for (int i = 1; i <= k; i++)
			{
				if (i == k)
				{
					if (dp[k] >= k) continue;
					else
					{
						dp[k] = k;
					}
				}
				dp[k] = dp[k] >= dp[i] * (k - i) ? dp[k] : dp[i] * (k - i);
			}
			max = max >= dp[k] ? max : dp[k];
		}
		return max;
	}
};

int main()
{
	Solution sol;
	int result = sol.cuttingRope(10);
	cout << result;
}