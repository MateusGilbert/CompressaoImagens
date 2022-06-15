#include "header.hpp"

subbands split_bands(double *pSIMG[], int x, int y, int n_lv){
	subbands bands;
	int st_x = st_y = 0;
	int wid_x = x/pow(2, n_lv);
	int wid_y = y/pow(2, n_lv);
	int n_bands = n_lv*3 + 1;

	for (int n=0; n < n_bands; n++){
		int *band_img = new int[wid_x*wid_y];
		subband cur = new subband;

		for (int j=st_y; j<st_y+wid_y; j++)
			for (int i=st_x; i<st_x+wid_x; i++)
				band_img[(i - st_x)*wid_y + (j - st_y)] =
					(int) subd_round(pSIMG[j][i]);
		cur->x = wid_x;
		cur->y = wid_y;
		cur->img = band_img;
		bands.push_back(cur);

		switch (n % 3){
			case 1:
				st_y = 0;
				st_x += wid_x;
				break;
			case 2:
				st_y += wid_y
				break;
			case 0:
				st_y += w_y;
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
