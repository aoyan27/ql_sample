#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int xy2s(int x, int y, int x_size){
	int s;
	s = x + y *x_size;
	return s;
}

int move(int a, int &x, int &y, int x_size){
	if(a==0){
		y = y + 1;
	}
	else if(a==1){
		x = x + 1;
	}
	else if(a==2){
		y = y - 1;
	}
	else{
		x = x - 1;
	}

	int sd;
	sd = xy2s(x, y, x_size);

	return sd;
}

int select_action(int s, int num_a, float** Qtable){
	double max;
	int i = 0;
	int* i_max=new int[num_a];
	int num_i_max=1;
	int a;

	i_max[0]=0;
	max=Qtable[s][0];

	for(i=1;i<num_a;i++){
		if(Qtable[s][i]>max){
			max=Qtable[s][i];
			num_i_max=1;
			i_max[0]=i;
		}
		else if(Qtable[s][i]==max){
			num_i_max++;
			i_max[num_i_max-1]=i;
		}
	}

	for(i=0;i<num_i_max;i++){
		// printf("i_max[%d]=%d\n",i,i_max[i]);
	}

	a=i_max[rand()%num_i_max];
	return a;
}

float max_Qval(int s, int num_a, float** Qtable){
	double max;
	int i = 0;

	max=Qtable[s][0];
	for(i=1;i<num_a;i++){
		if(Qtable[s][i]>max){
		max=Qtable[s][i];
		}
	}
	return max;
}

int epsilon_greedy(int epsilon, int s, int num_a, float** Qtable){
	int a;
	// printf("rand() = %d\n", rand()%100);
	if(epsilon>rand()%100){
		a=rand()%num_a;
		// printf("無作為に選択 : %d\n", rand()%num_a);
	}
	else{
		a=select_action(s, num_a, Qtable);
		// printf("最大値をとる行動を選択\n");
	}

	return a;
}

int main(){
	int x_size;
	int y_size;

	float alpha, gamma;
	int x, y, x_init, y_init;
	int **maze;
	int num_step;
	int num_trial;
	int i, j;
	int a, s, sd;
	int num_a;
	int num_s;
	float **Qtable;
	int reward;
	float Qmax;
	int epsilon;

	int failure_num;
	int success_num;

	alpha = 0.5;
	gamma = 0.9;
	epsilon = 10;
	// x_size = 110*70/2;
	// y_size = 110*70/2;
	x_size = 6;
	y_size = 6;
	x_init = 1;
	y_init = 1;
	num_step = 1000;
	num_trial = 3000;
	num_a = 4;
	num_s = x_size * y_size;

	failure_num = 0;
	success_num = 0;

	srand((unsigned)time(NULL));

	Qtable = new float*[num_s];
	for(i=0;i<num_s;i++){
		Qtable[i] = new float[num_a];
	}

	for(i=0;i<num_s;i++){
		for(j=0;j<num_a;j++){
			Qtable[i][j] = 0;
		}
	}
	

	maze = new int*[x_size];
	for(i=0;i<x_size;i++){
		maze[i] = new int[y_size];
	}

	for(i=0;i<x_size;i++){
		for(j=0;j<y_size;j++){
			if(i==0 || j==0 || i==(x_size-1) || j==(y_size-1)){
				maze[i][j] = -1;
			}
			else{
				maze[i][j] = 0;
			}
		}
	}

	// maze[2][2] = -1;
	// maze[3][2] = -1;
	// maze[6][3] = -1;
	// maze[7][3] = -1;
	// maze[8][3] = -1;
	// maze[2][6] = -1;
	// maze[2][7] = -1;
	// maze[3][6] = -1;
	// maze[6][7] = -1;

	// maze[8][6] = 10;
	//
	maze[2][2] = -1;
	maze[2][3] = -1;
	maze[2][4] = -1;

	maze[4][4] = 10;

	for(i=0;i<x_size;i++){
		for(j=0;j<y_size;j++){
			printf("%3d", maze[i][j]);
		}
		printf("\n");
	}

	int x_temp;
	int y_temp;

	x_temp = rand()%x_size;
	y_temp = rand()%y_size;

	// if(maze[x_temp][y_temp]!=-1){
		// x_init = x_temp;
		// y_init = y_temp;
	// }
	// else{
		// x_init = 1;
		// y_init = 1;
	// }

	x = x_init;
	y = y_init;
	s = xy2s(x, y, x_size);

	for(i=0;i<num_trial;i++){
		printf("trial=%d\n",i);
		// x_temp = rand()%x_size;
		// y_temp = rand()%y_size;

		// if(maze[x_temp][y_temp]!=-1){
			// x_init = x_temp;
			// y_init = y_temp;
		// }
		// else{
			// x_init = 1;
			// y_init = 1;
		// }

		// x = x_init;
		// y = y_init;
		// s = xy2s(x, y, x_size);
		for(j=0;j<num_step;j++){
			a = epsilon_greedy(epsilon, s, num_a, Qtable);
			sd = move(a, x, y, x_size);
			reward = maze[x][y];
			Qmax = max_Qval(sd, num_a, Qtable);
			Qtable[s][a] = (1-alpha) * Qtable[s][a] + alpha * ((float)reward + gamma * Qmax);

			if(reward<0){
				x = x_init;
				y = y_init;
				s = xy2s(x, y, x_size);
				failure_num++;
				printf("失敗\n");
				break;
			}
			else if(reward>0){
				x = x_init;
				y = y_init;
				s = xy2s(x, y, x_size);
				success_num++;
				printf("成功\n");

				break;
			}
			else{
				s = sd;
			}
		}
	}

	for(i=0;i<num_s;i++){
		for(j=0;j<num_a;j++){
			printf("Q[%d][%d]=%lf ", i, j, Qtable[i][j]);
		}
		printf("\n");
	}

	for(x=0;x<x_size;x++){
		for(y=0;y<y_size;y++){
			s = xy2s(x, y, x_size);
			Qmax = max_Qval(s, num_a, Qtable);
			if(maze[x][y]!=0 || Qmax == 0 ){
				printf("%3d", maze[x][y]);
			}
			else{
				a = select_action(s, num_a, Qtable);
				if(a==0){
					printf("  >");
				}
				else if(a==1){
					printf("  v");
				}
				else if(a==3){
					printf("  <");
				}
				else{
					printf("  ^");
				}
			}
		}
		printf("\n");
	}

	printf("成功回数:%d\n", success_num);
	printf("失敗回数:%d\n", failure_num);

	for(i=0;i<num_s;i++){
		delete[]Qtable[i];
	}
	delete[]Qtable;

	for(i=0;i<x_size;i++){
		delete[]maze[i];
	}
	delete[]maze;

	return 0;
}
