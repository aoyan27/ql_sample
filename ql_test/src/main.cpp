#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double vending_machine(int s, int a, int &sd){
	double reward;

	if(a==0){
		sd=!s;
		reward=0;
	}
	else{
		if(s==1){
			sd=s;
			reward=10;
		}
		else{
			sd=s;
			reward=0;
		}
	}

	return reward;
}

double max_Qval(int s, int num_a, double** Qtable){
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

int select_action(int s, int num_a, double** Qtable){
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

	// for(i=0;i<num_i_max;i++){
		// printf("i_max[%d]=%d\n",i,i_max[i]);
	// }

	a=i_max[rand()%num_i_max];
	return a;
}

int epsilon_greedy(int epsilon, int s, int num_a, double** Qtable){
	int a;
	if(epsilon>rand()%100){
		a=rand()%num_a;
		// printf("無作為に選択\n");
	}
	else{
		a=select_action(s, num_a, Qtable);
		// printf("最大値をとる行動を選択\n");
	}

	return a;
}

int main(){
	double **Qtable;
	double Q_max=0;
	double reward=0;
	double alpha = 0.5;
	double gamma = 0.9;
	int epsilon = 10;
	int trial_max = 100;

	int num_a = 2;
	int num_s = 2;
	int a = 0;
	int s = 0;
	int sd = 0;
	int i,j;

	srand((unsigned)time(NULL));

	Qtable = new double*[num_s];

	for(i=0;i<num_s;i++){
		Qtable[i] = new double[num_a];
	}

	for(i=0;i<num_s;i++){
		for(j=0;j<num_a;j++){
			Qtable[i][j] = 0;
			printf("Q[%d][%d]=%lf\n",i, j, Qtable[i][j]);
		}
	}

	for(i=0;i<trial_max;i++){
		a = epsilon_greedy(epsilon, s, num_a, Qtable);
		reward = vending_machine(s, a, sd);
		Q_max = max_Qval(sd, num_a, Qtable);
		Qtable[s][a] = (1-alpha) * Qtable[s][a] + alpha * (reward + gamma * Q_max);

		s = sd;

		printf("i=%d\n", i);
		if(reward>0){
			printf("成功(%d)\n", a);
		}
	}

	for(i=0;i<num_s;i++){
		for(j=0;j<num_a;j++){
			printf("%lf ", Qtable[i][j]);
		}
		printf("\n");
	}
	
	for(i=0;i<num_s;i++){
		delete[]Qtable[i];
	}
	delete[]Qtable;

	return 0;
}
