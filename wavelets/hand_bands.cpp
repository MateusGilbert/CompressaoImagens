#include "header.hpp"
#include "subdefs2.h"

subbands split_bands(double *pSIMG[], int x, int y, int n_lv){
	subbands bands;
	int st_x = 0, st_y = 0;
	int wid_x = x/pow(2, n_lv);
	int wid_y = y/pow(2, n_lv);
	int n_bands = n_lv*3 + 1;

	for (int n=0; n < n_bands; n++){
		float *band_img = new float[wid_x*wid_y];

		for (int j=st_y; j<st_y+wid_y; j++)
			for (int i=st_x; i<st_x+wid_x; i++)
				band_img[(i - st_x)*wid_y + (j - st_y)] =
					(float) pSIMG[j][i];
		subband cur;
		cur.x = wid_x;
		cur.y = wid_y;
		cur.img = band_img;
		bands.push_back(cur);

		switch (n % 3){
			case 1:
				st_y = 0;
				st_x += wid_x;
				break;
			case 2:
				st_y += wid_y;
				break;
			case 0:
				st_y += wid_y;
				st_x = 0;
				if (n == 0)
					break;
			default:
				wid_x *= 2;
				wid_y *= 2;
		}
	}

	return bands;
}

void agg_bands(subbands bands, double *pSIMG[], int x, int y, int n_lv, bool init_p){
	int st_x = 0, st_y = 0;
	int wid_x = x/pow(2, n_lv);
	int wid_y = y/pow(2, n_lv);
	int n_bands = n_lv*3 + 1;

	//to initialize pSIMG  -- conferir, talvez tirar?
	if (init_p){
		int yimg = (int) (y + y/2);
		int ximg = (int) (x/(2*DCTSIZE))*2*DCTSIZE;
		for (int i=0; i<yimg; i++)
			if ((pSIMG[i] = (double *) malloc(ximg*sizeof(double))) == NULL){
				cout<<"Memory allocation for luminance transform failed at line"<<i<<endl;
				exit(1);
			}
	}

//	for (int n=0; n < n_bands; n++){
//		float *band_img = bands[n].img;
//		if (band_img == (float *) NULL)
//			cout<<endl<<"N = "<<n<<endl;
//	}

	for (int n=0; n < n_bands; n++){
		float *band_img = bands[n].img;

		for (int j=st_y; j<st_y+wid_y; j++)
			for (int i=st_x; i<st_x+wid_x; i++){
				pSIMG[j][i] = (double) band_img[(i - st_x)*wid_y + (j - st_y)];
			}

		switch (n % 3){
			case 1:
				st_y = 0;
				st_x += wid_x;
				break;
			case 2:
				st_y += wid_y;
				break;
			case 0:
				st_y += wid_y;
				st_x = 0;
				if (n == 0)
					break;
			default:
				wid_x *= 2;
				wid_y *= 2;
		}
	}
}

subband quantize_band(subband inp, vect_list centroids, int x_fr, int y_fr){
	subband out;

	out.x = inp.x;
	out.y = inp.y;
	out.img = v_encode(inp.img, centroids, inp.x, inp.y, x_fr, y_fr);

	return out;
}

cod_subb quantize_band2(subband inp, vect_list centroids, int x_fr, int y_fr){
	cod_subb out;

	out.x = inp.x;
	out.y = inp.y;
	out.x_fr = x_fr;
	out.y_fr = y_fr;
	out.vects = v_enc(inp.img, centroids, inp.x, inp.y, x_fr, y_fr);

	return out;
}

subband dec_vects(cod_subb vects, vect_list centroids){
	subband out;

	out.x = vects.x;
	out.y = vects.y;
	out.img = v_dec(vects.vects, centroids, vects.x, vects.y, vects.x_fr, vects.y_fr);

	return out;
}
