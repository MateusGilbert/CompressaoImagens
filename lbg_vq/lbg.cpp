#include "header.hpp"

typedef vector< vector < int > > reg_register;

typedef struct{
	int *cent_ind, size, n_train, n_cent;
	float *sq_dist;
} lbg_aux;

inline float eq_dist2(int *x, int *y, int size){
	float cum=0;
	for (int i=0; i<size; i++)
		cum += (float) pow(x[i] - y[i], 2);

	return cum;
}

inline void def_qregion(int *vect, vect_list centroids, int idx, lbg_aux aux){
	float sq_dist;
	int cur_id=0;

	for (int *c : centroids){
		sq_dist = eq_dist2(vect, c, aux.size);
		if ((sq_dist < aux.sq_dist[idx]) or (aux.cent_ind[idx] == cur_id)){
			aux.sq_dist[idx] = sq_dist;
			if (aux.cent_ind[idx] != cur_id)
				aux.cent_ind[idx] = cur_id;
		}
		cur_id++;
	}

	return;
}

inline reg_register split_vects(lbg_aux aux){
	reg_register reg_reg;
	for (int i=0; i<aux.n_cent; i++)
		reg_reg.push_back(vector< int >());

	for (int i=0; i<aux.n_train; i++){
		int c_id = aux.cent_ind[i];
		reg_reg[c_id].push_back(i);
	}

	return reg_reg;
}

inline vect_list up_centroids(vect_list v_train, reg_register reg_reg, int v_size){
	vect_list centroids;

	for (vector< int > V_i : reg_reg){
		int N = V_i.size();
		int *cent = new int[v_size];
		for (int i=0; i<v_size; i++)
			cent[i] = 0;

		for (int idx : V_i){
			for (int i=0; i<v_size; i++)
				cent[i] += v_train[idx][i];
		}
		for (int i=0; i<v_size; i++)
			cent[i] /= N;
		centroids.push_back(cent);
	}

	return centroids;
}

void print_centroids(vect_list centroids, int size){
	for (int *y_i : centroids){
		cout<<"y_i = [ ";
		for (int i=0; i<size; i++)
			cout<<y_i[i]<<" ";
		cout<<"]"<<endl;
	}
	cout<<endl;
}

inline vect_list init_cent(vect_list vects, int n_cent, int dim){
	vect_list centroids;
	vector< int > pos;

	for (int i=0; i<dim; i++)
		pos.push_back(i);

	sample(vects.begin(), vects.end(),
			back_inserter(centroids), n_cent,
			mt19937{random_device{}()});
//	print_centroids(centroids,dim);
//	cout<<endl;

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

/* Using Introduction to Data Compression, by K. Sayood
as reference. Pseudo-code can be found at pg. 309 (5th
edition)*/
vect_list lbg(vect_list v_train, int N, int size, float eps){
	vect_list centroids;
	float D_old, D=3.4e+38;

	//initialize centroids
	centroids = init_cent(v_train, N, size);
	/*print_centroids(centroids,size);*/
	/*cout<<endl;*/

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
			/*D /= N;*/
		}

	if ((D_old - D)/D < eps)//4 -- dif. do livro
		return centroids;

	//5
	centroids = up_centroids(v_train, reg_reg, size);
//	print_centroids(centroids,size);
//	cout<<endl;
//	char a;
//	cin>>a;
	goto main_loop;
}

inline vect_list init_vects(int *vals, int n, int size){
	vect_list vects;

	for (int i=0; i<n; i+=size){
		vects.push_back(new int[size]);
		for (int j=i; j-i<size; j++)
			vects[i / size][j-i] = vals[j];
	}

	return vects;
}


//int main(void){
//	int vals[] = {15,2, 20,13, 2,2, 0,15, 30,4, 14,4, 2,2, 10,10, 11,12, 5,42, 24,44, 42,24, 10,1};
//	vect_list vectors = init_vects(vals, 22, 2);
//	int n_cent = 5, size=2;
//
//	for (int *x_i : vectors){
//		cout<<"x_i = [ ";
//		for (int i=0; i<size; i++)
//			cout<<x_i[i]<<" ";
//		cout<<"]"<<endl;
//	}
//	cout<<endl;
//
//	vect_list centr = lbg(vectors, n_cent, size, 1e-6);
//	print_centroids(centr, size);
//	return 0;
//}
