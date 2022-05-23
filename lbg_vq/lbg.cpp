#include "header.hpp"

typedef vector< vector < int > > reg_register;

typedef struct{
	/*reg_register q_regions;*/
	int *cent_ind, size, n_train, n_cent;
	float *sq_dist;
} lbg_aux;

inline float eq_dist2(int *x, int *y, int size){
	float cum=0;
	for (int i=0; i<size; i++)
		cum += pow(x[i] - y[i], 2);

	return cum;
}

inline void def_qregion(int *vect, vect_list centroids, int idx, lbg_aux aux){
	float sq_dist;
	int cur_id=0;
	for (int *c : centroids){
		sq_dist = eq_dist2(vect, c, aux.size);
		if ((sq_dist < aux.sq_dist[idx]) or (aux.cent_ind[idx] == cur_id++)){
			aux.sq_dist[idx] = sq_dist;
			if (aux.cent_ind[idx] != cur_id-1)
				aux.cent_ind[idx] = cur_id - 1;
		}
	}

	return;
}

inline reg_register split_vects(lbg_aux aux){
	reg_register reg_reg;//[aux.n_cent];
	for (int i=0; i<aux.n_cent; i++)
		reg_reg.push_back(vector< int >());

	for (int i=0; i<aux.n_train; i++){
		int c_id = aux.cent_ind[i];
		reg_reg[c_id].push_back(i);
	}

	return reg_reg;
}

inline vect_list up_centroids(vect_list v_train, reg_register reg_reg, int v_size){
	vect_list centroids;//[reg_reg.size()];

	for (vector< int > V_i : reg_reg){
		int N = V_i.size();
		int *cent = new int[v_size];
		for (int i=0; i<v_size; i++)
			cent[i] = 0;

		for (int idx : V_i){
			for (int i=0; i<v_size; i++)
				cent[i] += v_train[idx][i]/N;
			centroids.push_back(cent);
		}
	}

	return centroids;
}

inline vect_list init_cent(vect_list vects, int n_cent, int dim){
	vect_list centroids;
	vector< int > pos;

	for (int i=0; i<dim; i++)
		pos.push_back(i);

	sample(vects.begin(), vects.end(),
			back_inserter(centroids), n_cent,
			mt19937{random_device{}()});

	for (int *y_i : centroids){
		int pert = rand() % 2;
		int pos = rand() % dim;						//selects a position
		if ((pert == 0) or (y_i[pos] == 255))
			pert = -1;
		else if (y_i[pos] == 0)
			pert = 1;
		y_i[pos] += pert;
	}

	return centroids;
}

inline void print_centroids(vect_list centroids, int size){
	for (int *y_i : centroids){
		cout<<"y_i = [ ";
		for (int i=0; i<size; i++)
			cout<<y_i[i]<<" ";
		cout<<"]"<<endl;
	}
}

/* Using Introduction to Data Compression, by K. Sayood
as reference. Pseudo-code can be found at pg. 309 (5th
edition)*/
vect_list lbg(vect_list v_train, int size, int N, float eps){
	vect_list centroids;
	int D_old, D=0;

	//initialize centroids
	centroids = init_cent(v_train, N, size);
	print_centroids(centroids,size);

	//initialize quantization region's auxiliary
	lbg_aux aux;
	int n_train = v_train.size();
	aux.n_train = n_train;
	aux.n_cent = N;
	aux.cent_ind = new int[n_train];
	/*aux.q_regions = new reg_register[N];*/
	aux.sq_dist = new float[n_train];
	for (int i=0; i<n_train; i++)
		aux.sq_dist[i] = 3.4e+38;    //emulate inf dist
	aux.size=size;

	main_loop:
		D_old = D;		//5
		D = 0;
		for (int i=0; i<n_train; i++)				//2
			def_qregion(v_train[i], centroids, i, aux);
		reg_register reg_reg = split_vects(aux);
		for (vector< int > V_i : reg_reg){		//3
			for (int idx : V_i)
				D += aux.sq_dist[idx];
			D /= N;
		}

	if ((D - D_old)/D < eps)//4
		return centroids;

	//5
	/*delete centroids;*/
	centroids = up_centroids(v_train, reg_reg, size);
	goto main_loop;
}


int main(void){
	vect_list vectors = {//ajeitar isso!!!
		{1,2}, {1,1}, {0,1}, {2,2}, {2,1}, {3,4}, {4,2}, {5,2}, {2,4}
	};
	int n_cent = 3, size=2;
	vect_list centr = lbg(vectors, n_cent, size);
	print_centroids(centr, size);
	return 0;
}
