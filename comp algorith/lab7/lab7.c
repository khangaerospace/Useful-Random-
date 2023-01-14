#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

struct rgb_img{
    uint8_t *raster;
    size_t height;
    size_t width;
};

void create_img(struct rgb_img **im, size_t height, size_t width){
    *im = (struct rgb_img *)malloc(sizeof(struct rgb_img));
    (*im)->height = height;
    (*im)->width = width;

    (*im)->raster = (uint8_t *)malloc(3 * height * width);
}


int read_2bytes(FILE *fp){
    uint8_t bytes[2];
    fread(bytes, sizeof(uint8_t), 1, fp);
    fread(bytes+1, sizeof(uint8_t), 1, fp);
    return (  ((int)bytes[0]) << 8)  + (int)bytes[1];
}

void write_2bytes(FILE *fp, int num){
    uint8_t bytes[2];
    bytes[0] = (uint8_t)((num & 0XFFFF) >> 8);
    bytes[1] = (uint8_t)(num & 0XFF);
    fwrite(bytes, 1, 1, fp);
    fwrite(bytes+1, 1, 1, fp);
}

void read_in_img(struct rgb_img **im, char *filename){
    FILE *fp = fopen(filename, "rb");
    size_t height = read_2bytes(fp);
    size_t width = read_2bytes(fp);
    create_img(im, height, width);
    fread((*im)->raster, 1, 3*width*height, fp);
    fclose(fp);
}

void write_img(struct rgb_img *im, char *filename){
    FILE *fp = fopen(filename, "wb");
    write_2bytes(fp, im->height);
    write_2bytes(fp, im->width);
    fwrite(im->raster, 1, im->height * im->width * 3, fp);
    fclose(fp);
}

uint8_t get_pixel(struct rgb_img *im, int y, int x, int col){
    return im->raster[3 * (y*(im->width) + x) + col];
}

void set_pixel(struct rgb_img *im, int y, int x, int r, int g, int b){
    im->raster[3 * (y*(im->width) + x) + 0] = r;
    im->raster[3 * (y*(im->width) + x) + 1] = g;
    im->raster[3 * (y*(im->width) + x) + 2] = b;
}

void destroy_image(struct rgb_img *im)
{
    free(im->raster);
    free(im);
}


void print_grad(struct rgb_img *grad){
    int height = grad->height;
    int width = grad->width;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            printf("%d\t", get_pixel(grad, i, j, 0));
        }
    printf("\n");    
    }
} 

void brighter(struct rgb_img *im, double level){
    for(int y =0 ; y < im->height; y++){
        for(int x=0; x< im->width; x++ ){
            int r = (int) get_pixel(im,y,x, 0)*level;
            int g = (int) get_pixel(im, y,x,1)*level;
            int b = (int) get_pixel(im, y,x,2)*level;
            set_pixel(im,y,x,r,g,b);
        }
    }   
}

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>


void calc_energy(struct rgb_img *im, struct rgb_img **grad) {
    double sum = 0;
    int number = (im->height) * (im->width);
    create_img(grad, (im->height), (im->width));
    int size = 0;
    int i;
//    struct rgb_img *total = malloc(number*sizeof(struct rgb_img));
    for (int y = 0; y < im->height; y++) {
        for (int x = 0; x < im->width; x++) {
            double ry1;
            double gy1;
            double by1;
            double ry_1;
            double gy1_1;
            double by_1;
            //on y direction
            double rx1;
            double gx1;
            double bx1;
            double rx_1;
            double gx_1;
            double bx_1;
            if (y == 0 && x == 0) {
                //delta x
                ry1 = (double) get_pixel(im, y + 1, x, 0);
                gy1 = (double) get_pixel(im, y + 1, x, 1);
                by1 = (double) get_pixel(im, y + 1, x, 2);
                ry_1 = (double) get_pixel(im, im->height - 1, x, 0);
                gy1_1 = (double) get_pixel(im, im->height - 1, x, 1);
                by_1 = (double) get_pixel(im, im->height - 1, x, 2);
                //delta y
                rx1 = (double) get_pixel(im, y, x + 1, 0);
                gx1 = (double) get_pixel(im, y, x + 1, 1);
                bx1 = (double) get_pixel(im, y, x + 1, 2);
                rx_1 = (double) get_pixel(im, y, im->width - 1, 0);
                gx_1 = (double) get_pixel(im, y, im->width - 1, 1);
                bx_1 = (double) get_pixel(im, y, im->width - 1, 2);
            } else if (y == 0 && x == im->width - 1) {
                //delta x
                ry1 = (double) get_pixel(im, y + 1, x, 0);
                gy1 = (double) get_pixel(im, y + 1, x, 1);
                by1 = (double) get_pixel(im, y + 1, x, 2);
                ry_1 = (double) get_pixel(im, im->height - 1, x, 0);
                gy1_1 = (double) get_pixel(im, im->height - 1, x, 1);
                by_1 = (double) get_pixel(im, im->height - 1, x, 2);
                //delta y
                rx1 = (double) get_pixel(im, y, 0, 0);
                gx1 = (double) get_pixel(im, y, 0, 1);
                bx1 = (double) get_pixel(im, y, 0, 2);
                rx_1 = (double) get_pixel(im, y, x - 1, 0);
                gx_1 = (double) get_pixel(im, y, x - 1, 1);
                bx_1 = (double) get_pixel(im, y, x - 1, 2);
            } else if (y == im->height - 1 && x == 0) {
                //delta x
                ry1 = (double) get_pixel(im, 0, x, 0);
                gy1 = (double) get_pixel(im, 0, x, 1);
                by1 = (double) get_pixel(im, 0, x, 2);
                ry_1 = (double) get_pixel(im, y - 1, x, 0);
                gy1_1 = (double) get_pixel(im, y - 1, x, 1);
                by_1 = (double) get_pixel(im, y - 1, x, 2);
                //delta y
                rx1 = (double) get_pixel(im, y, x + 1, 0);
                gx1 = (double) get_pixel(im, y, x + 1, 1);
                bx1 = (double) get_pixel(im, y, x + 1, 2);
                rx_1 = (double) get_pixel(im, y, im->width - 1, 0);
                gx_1 = (double) get_pixel(im, y, im->width - 1, 1);
                bx_1 = (double) get_pixel(im, y, im->width - 1, 2);
            } else if (y == im->height - 1 && x == im->width - 1) {
                ry1 = (double) get_pixel(im, 0, x, 0);
                gy1 = (double) get_pixel(im, 0, x, 1);
                by1 = (double) get_pixel(im, 0, x, 2);
                ry_1 = (double) get_pixel(im, y - 1, x, 0);
                gy1_1 = (double) get_pixel(im, y - 1, x, 1);
                by_1 = (double) get_pixel(im, y - 1, x, 2);
                //delta y
                rx1 = (double) get_pixel(im, y, 0, 0);
                gx1 = (double) get_pixel(im, y, 0, 1);
                bx1 = (double) get_pixel(im, y, 0, 2);
                rx_1 = (double) get_pixel(im, y, x - 1, 0);
                gx_1 = (double) get_pixel(im, y, x - 1, 1);
                bx_1 = (double) get_pixel(im, y, x - 1, 2);

            } else if (y == 0) {
                //delta x
                ry1 = (double) get_pixel(im, y + 1, x, 0);
                gy1 = (double) get_pixel(im, y + 1, x, 1);
                by1 = (double) get_pixel(im, y + 1, x, 2);
                ry_1 = (double) get_pixel(im, im->height - 1, x, 0);
                gy1_1 = (double) get_pixel(im, im->height - 1, x, 1);
                by_1 = (double) get_pixel(im, im->height - 1, x, 2);
                //delta y
                rx1 = (double) get_pixel(im, y, x + 1, 0);
                gx1 = (double) get_pixel(im, y, x + 1, 1);
                bx1 = (double) get_pixel(im, y, x + 1, 2);
                rx_1 = (double) get_pixel(im, y, x - 1, 0);
                gx_1 = (double) get_pixel(im, y, x - 1, 1);
                bx_1 = (double) get_pixel(im, y, x - 1, 2);

            } else if (x == 0) {
                //on x direction
                ry1 = (double) get_pixel(im, y + 1, x, 0);
                gy1 = (double) get_pixel(im, y + 1, x, 1);
                by1 = (double) get_pixel(im, y + 1, x, 2);
                ry_1 = (double) get_pixel(im, y - 1, x, 0);
                gy1_1 = (double) get_pixel(im, y - 1, x, 1);
                by_1 = (double) get_pixel(im, y - 1, x, 2);
                //on y direction
                rx1 = (double) get_pixel(im, y, x + 1, 0);
                gx1 = (double) get_pixel(im, y, x + 1, 1);
                bx1 = (double) get_pixel(im, y, x + 1, 2);
                rx_1 = (double) get_pixel(im, y, im->width - 1, 0);
                gx_1 = (double) get_pixel(im, y, im->width - 1, 1);
                bx_1 = (double) get_pixel(im, y, im->width - 1, 2);

            } else if (y == im->height - 1) {
                ry1 = (double) get_pixel(im, 0, x, 0);
                gy1 = (double) get_pixel(im, 0, x, 1);
                by1 = (double) get_pixel(im, 0, x, 2);
                ry_1 = (double) get_pixel(im, y - 1, x, 0);
                gy1_1 = (double) get_pixel(im, y - 1, x, 1);
                by_1 = (double) get_pixel(im, y - 1, x, 2);
                //delta y
                rx1 = (double) get_pixel(im, y, x + 1, 0);
                gx1 = (double) get_pixel(im, y, x + 1, 1);
                bx1 = (double) get_pixel(im, y, x + 1, 2);
                rx_1 = (double) get_pixel(im, y, x - 1, 0);
                gx_1 = (double) get_pixel(im, y, x - 1, 1);
                bx_1 = (double) get_pixel(im, y, x - 1, 2);
            } else if (x == im->width - 1) {
                //on x direction
                ry1 = (double) get_pixel(im, y + 1, x, 0);
                gy1 = (double) get_pixel(im, y + 1, x, 1);
                by1 = (double) get_pixel(im, y + 1, x, 2);
                ry_1 = (double) get_pixel(im, y - 1, x, 0);
                gy1_1 = (double) get_pixel(im, y - 1, x, 1);
                by_1 = (double) get_pixel(im, y - 1, x, 2);
                //on y direction
                rx1 = (double) get_pixel(im, y, 0, 0);
                gx1 = (double) get_pixel(im, y, 0, 1);
                bx1 = (double) get_pixel(im, y, 0, 2);
                rx_1 = (double) get_pixel(im, y, x - 1, 0);
                gx_1 = (double) get_pixel(im, y, x - 1, 1);
                bx_1 = (double) get_pixel(im, y, x - 1, 2);
            } else {
                ry1 = (double) get_pixel(im, y + 1, x, 0);
                gy1 = (double) get_pixel(im, y + 1, x, 1);
                by1 = (double) get_pixel(im, y + 1, x, 2);
                ry_1 = (double) get_pixel(im, y - 1, x, 0);
                gy1_1 = (double) get_pixel(im, y - 1, x, 1);
                by_1 = (double) get_pixel(im, y - 1, x, 2);
                //on y direction
                rx1 = (double) get_pixel(im, y, x + 1, 0);
                gx1 = (double) get_pixel(im, y, x + 1, 1);
                bx1 = (double) get_pixel(im, y, x + 1, 2);
                rx_1 = (double) get_pixel(im, y, x - 1, 0);
                gx_1 = (double) get_pixel(im, y, x - 1, 1);
                bx_1 = (double) get_pixel(im, y, x - 1, 2);
            }
            double sumry = ry1 - ry_1;
            double sumgy = gy1 - gy1_1;
            double sumby = by1 - by_1;

            double sumrx = rx1 - rx_1;
            double sumgx = gx1 - gx_1;
            double sumbx = bx1 - bx_1;
            sum = (sqrt(sumry*sumry + sumgy*sumgy + sumby*sumby + sumrx*sumrx +sumgx*sumgx + sumbx*sumbx)) / 10;
            uint8_t energy; // integer ranging from 0 to 255
            energy = (uint8_t) sum;
//            printf("Unrounded energy: %f\n", sum);
//            printf("Rounded energy: %d\n", energy);
            set_pixel(*grad, y, x, energy, energy, energy);
        }
    }
//    printf("end of function\n");
}


double find_lowest_cost(double *arr, int width, int index_row, int index_col){
    double lowest = arr[(index_row-1)*width+index_col-1];
    double x = arr[(index_row-1)*width+index_col-1];
    double y = arr[(index_row-1)*width+index_col];
    double z = arr[(index_row-1)*width+index_col+1];
    if(index_col == 0)
        return fmin(y,z);
    if(index_col == width-1)
        return fmin(x,y);
    if(x>y){
        lowest = y;
        if(y>z) {
            lowest = z;
        }
    }
    if(x<y){
        lowest = x;
        if(x>z){
            lowest = z;
        }
    }
    return lowest;
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){
    *best_arr = (double *)malloc(sizeof(double)*(grad->height*grad->width));
    double *arr = (double *)malloc(sizeof(double)*(grad->height*grad->width));
    for(int i = 0; i < grad->height; i++){
        for(int j = 0; j < grad->width; j++) {
            arr[i*grad->width+j] = get_pixel(grad, i, j, 0);
//            printf("%lf\n", arr[i*grad->width+j]);
        }
    }
    for(int i = 0; i < grad->height; i++){
        for(int j = 0; j < grad->width; j++){
            if(i==0) {
                double v = get_pixel(grad, 0, j, 0);
                (*best_arr)[i * grad->width + j] = get_pixel(grad, 0, j, 0);
            }
            if(i!=0){
                double w = arr[i*grad->width + j];
                double z = find_lowest_cost(arr,grad->width,i,j);
                (*best_arr)[i * grad->width + j] = (w+z);
                arr[i*grad->width+j] = w+z;
//                printf("%lf %lf\n", w, z);
            }
        }
    }
}

int find_lowest_above(double *best_array, int width, int index_row, int index_col){
    int lowest = index_col-1;
    double x = best_array[(index_row-1)*width+index_col-1];
    double y = best_array[(index_row-1)*width+index_col];
    double z = best_array[(index_row-1)*width+index_col+1];
    if(index_col == 0)
        if(y>z)
            return 1;
        else
            return 0;
    if(index_col == width-1)
        if(y>x)
            return index_col-1;
        else
            return index_col;
    if(x>y){
        lowest = index_col;
        if(y>z){
            lowest = index_col+1;
        }
    }
    if(x<y){
        lowest = index_col-1;
        if(x>z){
            lowest = index_col+1;
        }
    }
    return lowest;
}

void recover_path(double *best, int height, int width, int **path){
    *path = (int *)malloc(sizeof(int)*(height));
    int max = best[(height-1)*width];
    int index = 0;
    int j = (height-1)*width+1;
    while(j<(height*width)){
        if(max>best[j]){
            max = best[j];
            index++;
        }
        j++;
    }
    int i = height-1;
    while(i>0){
        if(i==height-1)
            (*path)[i] = index;
        if(i!=height-1){
            (*path)[i] = find_lowest_above(best, width, i+1, index);
            index = find_lowest_above(best, width, i+1, index);
        }
        i--;
    }
    (*path)[0] = find_lowest_above(best, width, 1, index);
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    create_img(dest, (src->height), (src->width) - 1);
    for (int i = 0; i < src->height; i++) {
        int flag = 0;
        for (int j = 0; j < src->width; j++) {
            if (j == path[i]) {
                flag = 1;
                continue;
            }
            if(flag == 0){
                set_pixel(*dest, i, j, get_pixel(src, i, j, 0), get_pixel(src, i, j, 1), get_pixel(src, i, j, 2));
            }
            if(flag == 1){
                set_pixel(*dest, i, j-1, get_pixel(src, i, j, 0), get_pixel(src, i, j, 1), get_pixel(src, i, j, 2));
            }
        }
    }
}


int main(void)
{
    struct rgb_img *im;
    struct rgb_img *cur_im;
    struct rgb_img *grad;
    double *best;
    int *path;

    read_in_img(&im, "C:\\Users\\khang\\OneDrive\\Music\\Documents\\school UofT textbook\\year 1 semester 2\\esc190 comp algorith\\lab7\\6x5.bin");
    /*calc_energy(im,&grad);
    //printf("%ld     %ld\n",grad->height,grad->width);
    print_grad(grad);
    //printf("%ld     %ld\n",grad->height,grad->width);
    dynamic_seam(grad,&best);
    //printf("%ld     %ld",grad->height,grad->width);
    recover_path(best,grad->height,grad->width,&path);*/
    

    for(int i = 0; i < 5; i++){
        printf("i = %d\n", i);
        calc_energy(im,  &grad);
        dynamic_seam(grad, &best);
        recover_path(best, grad->height, grad->width, &path);
        remove_seam(im, &cur_im, path);

        char filename[200];
        sprintf(filename, "img%d.bin", i);
        write_img(cur_im, filename);


        destroy_image(im);
        destroy_image(grad);
        free(best);
        free(path);
        im = cur_im;
    }
    destroy_image(im);
}
