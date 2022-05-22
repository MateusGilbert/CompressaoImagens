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
		sq_dist = eq_dist2(vect, c, aux->size);
		if ((sq_dist < aux->sq_dist[idx]) or (aux->cent_ind[idx] == cur_id++)){
			aux->sq_dist[idx] = sq_dist;
			if (aux->cent_ind[idx] != cur_id-1)
				aux->cent_ind[idx] = cur_id - 1;
		}
	}

	return;
}

inline reg_register split_vects(lbg_aux aux){
	reg_register reg_reg[aux->n_cent];

	for (int i=0; i<aux->n_train; i++){
		int c_id = aux->cent_ind[i];
		reg_reg[i].push_back(i);
	}

	return reg_reg;
}

vect_list lbg(vect_list v_train, int size, int N, int eps=.1){
	vect_list centroids = new vect_list[N];
	int D_old, D=0;

	//initialize centroids

	//initialize quantization region's auxiliary
	lbg_aux aux;
	int n_train = v_train.length();
	aux->n_train = n_train;
	aux->n_cent = N;
	aux->cent_ind = new int[n_train];
	aux->q_regions = new reg_register[N];
	aux->sq_dist = new int[n_train];
	for (int i=0; i<n_train; i++)
		aux->sq_dist[i] = 3.4e+38;    //emulate inf dist
	aux->size=size;

	main_loop:
		D_old = D;		//5
		D = 0;
		for (int i=0; i<n_train; i++)				//2
			def_qregion(v_train[i], centroids, i, aux);
		reg_register reg_reg = split_vects(aux);
		for (vector< int > V_i : reg_reg){		//3
			for (int idx : V_i)
				D += aux->sq_dist[idx]
			D /= N;
		}

	if ((D - D_old)/D < eps)//4
		return centroids;

	//falta o 5 de atualizar os centroides
	goto main_loop;
}
