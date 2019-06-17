// Example program
#include <iostream>
#include <numeric>
#include <fstream>
#include <map>
#include <array>
#include <set>

using triple_t = std::array<int, 3>;

using namespace std;
const int ROUND_COUNT = 4;
static array<array<int, 8>, 3> permut_table;
static map<triple_t, set<triple_t>> baseL3S3;

void init_permut_table() {
	for (int i = 0; i < 8; i++) {
		permut_table[0][i] = (i + 2 * (i % 2) + 7) % 8;
		permut_table[1][i] = (i + 1 - 2 * (i % 2)) % 8;
		permut_table[2][i] = (5 * i + 6) % 8;
	}
}

triple_t s_blocks_increasing(array<int, 7> blocks) {
	triple_t result;
	result[0] = blocks[0];
	result[1] = blocks[1];
	result[2] = blocks[3];
	int c = 0;
	if (result[1] > 0)
	{
		c += 1;
		result[1] += blocks[2] + blocks[5] + blocks[6];
		blocks[2] = 0;
		blocks[5] = 0;
		blocks[6] = 0;

	}
	if (result[0] > 0)
	{
		c += 1;
		result[0] += blocks[2] + blocks[4] + blocks[6];
		blocks[2] = 0;
		blocks[4] = 0;
		blocks[6] = 0;

	}
	if (result[2] > 0)
	{
		c += 1;
		result[2] += blocks[5] + blocks[4] + blocks[6];
		blocks[5] = 0;
		blocks[4] = 0;
		blocks[6] = 0;

	}
	if (c == 0) {
		if (blocks[2] && blocks[4] && blocks[5] && blocks[6])
		{
			result[0] += blocks[2] + blocks[4];
			result[1] += blocks[5] + blocks[6];
		}
		else if (blocks[2] == 0) {
			result[2] += blocks[5] + blocks[4] + blocks[6];
		}
		else if (blocks[4] == 0) {
			result[0] += blocks[2] + blocks[5] + blocks[6];
		}
		else {
			result[1] += blocks[2] + blocks[4] + blocks[6];
		}
	}
	if (c == 1) {
		if (blocks[2] > 0) {
			result[0] += blocks[2];
			blocks[2] = 0;
		}
		if (blocks[4] > 0) {
			result[0] += blocks[4];
			blocks[4] = 0;
		}
		if (blocks[5] > 0) {
			result[2] += blocks[5];
			blocks[5] = 0;
		}
	}

	return result;
}

void initL3() {

	array<int, 3> tmp1;
	array<int, 7> tmp2;
	ifstream L3("C:\\Users\\Yarashenia_Y\\source\\repos\\bash_my\\bash_my\\in.txt", ios::in);
	if (!L3)
		cout << "bad";
	L3 >> tmp1[0];

	while (L3) {
		for (int i = 1; i < 3; i++) {
			L3 >> tmp1[i];
		//	cout << tmp1[i]<<" ";
		}
		if (tmp1[0] == 2 && tmp1[2] == 0) {
			int x = 1;
		}
	//	cout << tmp1[0];
		for (int i = 0; i < 7; i++) {
			L3 >> tmp2[i];
		//	cout << tmp2[i] << " ";
		}
	//	cout << endl;
		baseL3S3[tmp1].insert(baseL3S3[tmp1].begin(), s_blocks_increasing(tmp2));
		L3 >> tmp1[0];
	}
	L3.close();
}

void initBash() {
	initL3();
	init_permut_table();
}

array<triple_t, 8> permut(array<triple_t, 8> blocks) {
	array<triple_t,8> result;
	for (int i = 0; i < 8; i++) {
		result[i][1] = blocks[permut_table[0][i]][0];
		result[i][2] = blocks[permut_table[1][i]][1];
		result[i][0] = blocks[permut_table[2][i]][2];
	}
	return result;
}




int count_active(array<triple_t, 8> blocks) {
	int result = 0;
	for (triple_t block : blocks) {
		for (int active : block) {
			result += active;
			cout << active << " ";
		}
		cout << endl;
	}
	return result;
}

void iteration(array<triple_t, 8> test, int count_cur, int round_num);
void round_impl(array<triple_t, 8> & cur_state, size_t cur_index, int count, int round_num)
{
	if (cur_index == cur_state.size())
	{
		if ((round_num ) >= ROUND_COUNT && count < 34) {
			cout << "        " << count <<" "<<round_num<< endl;
		}
	/*	else {
			for (int i = 0; i < round_num; i++)
				cout << " ";
			cout << count << " "<< round_num<<endl;
		}*/
		iteration(cur_state, count, round_num);

	}
	else {
		triple_t original = cur_state[cur_index];
		if (baseL3S3.count(original)) {
			for (triple_t triple : baseL3S3[original]) {
				cur_state[cur_index] = triple;
				count += triple[0] + triple[1] + triple[2];
				round_impl(cur_state, cur_index + 1, count, round_num);
			}
		}
		else {
			count += 4;
		//	if(count<34)
		//		cout << "needed " << original[0] << " " << original[1] << " " << original[2] << endl;
			if (original[0] == 0 && original[1] == 0 && original[2] != 0)
				count += original[2] - 2;
			cur_state[cur_index] = { 0,0,0 };
			round_impl(cur_state, cur_index + 1, count, round_num);
		}
		cur_state[cur_index] = original;
	}
}



void round(array<triple_t, 8> init, int count,int round_num)
{
	round_impl(init, 0,count, round_num);
}
void print_blocks(array<triple_t, 8> test) {
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 8; i++) {
			cout << test[i][j] << " ";

		}
		cout << endl;
	}
}
void iteration(array<triple_t, 8> test, int count_cur, int round_num) {
	if (round_num <= ROUND_COUNT && count_cur<40) {
		//P
		//print_blocks(test);
		test = permut(test);
		//print_blocks(test);
		//L3+S3
		if(round_num < ROUND_COUNT)
			round(test, count_cur, round_num+1);
	}
}


int main() {
	initBash();
	array<triple_t, 8> test;
	for (int i = 0; i < 8; i++) {
		test[i].fill(0);
	}
	for (int i = 0; i < 24; i++) {
		test[i % 8][i / 8] = 1;
//		cout << i / 8 << " " << i % 8 << " "<<endl;
//		round(test,1,1) ;
//		cout << endl;
	test[i % 8][i / 8] = 0;
	}

	cout << "***************************2*********************" << endl;
	for (int i = 0; i < 24; i++) {
		for (int j = 0; j < 24; j++) {
//	int i = 20;
	//	int j = 23;
			test[i % 8][i / 8] += 1;
			test[j % 8][j / 8] += 1;
		//	cout << i / 8 << " " << i % 8 << " " << endl;
		//	cout << j / 8 << " " << j % 8 << " " << endl;
	//		if(i%8 == j%8 || int(i / 8) == int(j / 8))
	//			round(test, 2, 2);
		//	else
		//		round(test, 1, 2);
//			cout << endl;
			test[i % 8][i / 8] = 0;
			test[j % 8][j / 8] = 0;

		}
	}
	cout << "*************************3****************************" << endl;
	for (int i = 0; i < 24; i++) {
		for (int j = 0; j < 24; j++) {
			for (int k =0; k < 24; k++) {
				//	int i = 20;
					//	int j = 23;
				test[i % 8][i / 8] += 1;
				test[j % 8][j / 8] += 1;
				test[k % 8][k / 8] += 1;
		//		cout << i / 8 << " " << i % 8 << " " << endl;
		//		cout << j / 8 << " " << j % 8 << " " << endl;
			//	if (i % 8 == j % 8 || int(i / 8) == int(j / 8))
				//	round(test, 2, 2);
		//		else
		//			round(test, 1, 2);
				//			cout << endl;
				test[i % 8][i / 8] = 0;
				test[j % 8][j / 8] = 0;
				test[k % 8][k / 8] = 0;
			}
		}
	}
	cout << "***************************4*********************" << endl;

	for (int i = 0; i < 24; i++) {
		for (int j = i; j < 24; j++) {
			for (int k = j; k < 24; k++) {
				for (int m = k; m < 24; m++) {

					//	int i = 20;
						//	int j = 23;
					test[i % 8][i / 8] += 1;
					test[j % 8][j / 8] += 1;
					test[k % 8][k / 8] += 1;

					test[m % 8][m / 8] += 1;
					//		cout << i / 8 << " " << i % 8 << " " << endl;
					//		cout << j / 8 << " " << j % 8 << " " << endl;
						//	if (i % 8 == j % 8 || int(i / 8) == int(j / 8))
							//	round(test, 2, 2);
					//		else
			//		round(test, 1, 2);
					//			cout << endl;
					test[i % 8][i / 8] = 0;
					test[j % 8][j / 8] = 0;
					test[k % 8][k / 8] = 0;

					test[m % 8][m / 8] = 0;
				}
			}
		}
	}
	cout << "***************************5*********************" << endl;

	for (int i = 0; i < 24; i++) {
		for (int j = 0; j < 24; j++) {
			for (int k = j; k < 24; k++) {
				for (int k1 =k; k1 < 24; k1++)
				for (int m = k1; m < 24; m++) {


					//	int i = 20;
						//	int j = 23;
					test[i % 8][i / 8]+= 1;
					test[j % 8][j / 8]+= 1;
					test[k % 8][k / 8]+= 1;
					test[k1 % 8][k1 / 8] += 1;
					test[m % 8][m / 8] += 1;
					//		cout << i / 8 << " " << i % 8 << " " << endl;
					//		cout << j / 8 << " " << j % 8 << " " << endl;
						//	if (i % 8 == j % 8 || int(i / 8) == int(j / 8))
							//	round(test, 2, 2);
					//		else
				//	round(test, 1, 2);
					//			cout << endl;
					test[i % 8][i / 8] = 0;
					test[j % 8][j / 8] = 0;
					test[k % 8][k / 8] = 0;
					test[k1 % 8][k1 / 8] = 0;
					test[m % 8][m / 8] = 0;
				}
			}
		}
	}
	cout << "***************************6*********************" << endl;

	for (int i = 0; i < 24; i++) {
		for (int j = i; j < 24; j++) {
			for (int k =j; k < 24; k++) {
				for (int k1 = k; k1 < 24; k1++)
					for (int k2 = k1; k2 < 24; k2++)
					for (int m = k2; m < 24; m++) {


						//	int i = 20;
							//	int j = 23;

						test[i % 8][i / 8] += 1;
						test[j % 8][j / 8] += 1;
						test[k % 8][k / 8] += 1;
						test[k1 % 8][k1 / 8] += 1;
						test[k2 % 8][k2 / 8] += 1;
						test[m % 8][m / 8] += 1;
						//		cout << i / 8 << " " << i % 8 << " " << endl;
						//		cout << j / 8 << " " << j % 8 << " " << endl;
							//	if (i % 8 == j % 8 || int(i / 8) == int(j / 8))
								//	round(test, 2, 2);
						//		else
						round(test, 1, 2);
						//			cout << endl;
						test[i % 8][i / 8] = 0;
						test[j % 8][j / 8] = 0;
						test[k % 8][k / 8] = 0;
						test[k1 % 8][k1 / 8] = 0;
						test[k2 % 8][k2 / 8] = 0;
						test[m % 8][m / 8] = 0;
					}
			}
		}
	}
	system("pause");
	return 0;
}