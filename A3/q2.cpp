#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int dot (vector<int>& a, vector<int>& b, int s, int e) {
	int temp = 0;
	#pragma omp parallel for reduction(+:temp)
	for (int i = s; i < e; i++) {
		temp += a[i] * b[i];
	}
	return temp;
}
int main() {
	vector<int> a(1000), b(1000);
	int ans = 0;
	random_device r1;
	for (int i = 0; i < 1000; i++) {
		a[i] = (r1() % 100 + 1);
		b[i] = (r1() % 100 + 1);
	}
	for (int i = 0; i < 1000; i++) {
		ans += a[i] * b[i];
	}
	int p_ans = 0;
	omp_set_nested(1);
	#pragma omp parallel reduction(+:p_ans)
	{
		int t_id = omp_get_thread_num();
		int n_t = omp_get_num_threads();
		int s = t_id * (1000 / n_t);
		int e = (t_id == n_t - 1) ? 1000 : s + (1000 / n_t);
		int temp_ans = dot(a, b, s, e);
		p_ans += temp_ans;
	}
	
	cout << "Dot product using parallelism: " << p_ans << endl;
	cout << "Dot product without parallelism: " << ans << endl;
	return 0;
}