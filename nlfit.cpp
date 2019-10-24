#include <stdio.h>
#include <iostream>
#include <math.h>
#include <nlopt.h>

/* ----- ----- DATA ----- ----- */
float data_x[] = {
    16.05,
    18.15,
    20.2,
    22,
    24.15,
    25.05,
    26.1,
    27.05,
    28,
    29.21,
    30.2
};

float data_y[] = {
    3.78,
    3.87,
    3.97,
    4.09,
    4.2,
    4.27,
    4.36,
    4.43,
    4.52,
    4.63,
    4.77
};
/* ----- ----- /DATA ----- ----- */







const int ARR_SIZE = 11;

float avg(float *data, int length){
    float sum = 0;
    for (int i = 0; i < length; i++){
        sum += data[i];
    }
    return sum / length;
}

float res_sum(float A, float B, float C, float D){ // TO BE OPTIMIZED
    float asinh = 0;
    float sum = 0;
    for (int i = 0; i < ARR_SIZE; i++){ //This is using 11 data points
        asinh = A*log(B*(data_x[i] - C) + sqrt(1+pow(B*(data_x[i] - C),2)))+D;
        sum += pow(data_y[i] - asinh, 2);
    }
    return sum;
}

double opt_res_sum(unsigned n, const double *x, double *grad, void *my_func_data){
    return res_sum(x[0],x[1],x[2],x[3]);
}


float tot_sum(float *data_y, float avg){
    float sum = 0;
    for (int i = 0; i < ARR_SIZE; i++){
        sum += pow(data_y[i] - avg,2);
    }
    return sum;
}

int main(){
    float A = 0.5;
    float B = 100;
    float C = 33;
    float D = 8;
    float SSR = 0;
    float SST = 0;
    float R2 = 0;
    float average = avg(data_y, ARR_SIZE);
    
    double lowb[] = {0,95,30,5};
    double upb[] = {1,110,35,10};

    nlopt_opt obj = nlopt_create(NLOPT_GN_DIRECT, 4);
    nlopt_set_lower_bounds(obj, lowb);
    nlopt_set_upper_bounds(obj, upb);
    nlopt_set_min_objective(obj, opt_res_sum, NULL);

    double x[4] = { 0.5, 100, 33, 8};  /* `*`some` `initial` `guess`*` */
    double minf; /* `*`the` `minimum` `objective` `value,` `upon` `return`*` */
    int err = nlopt_optimize(obj, x, &minf);
    if (err < 0) {
        printf("nlopt failed!\n");
        printf("Error code: %d\n", err);
    }
    else {
        printf("found minimum at f(%g,%g,%g,%g) = %0.10g\n", x[0], x[1], x[2], x[3], minf);
    }


    SSR = res_sum(A,B,C,D);
    SST = tot_sum(data_y, average);
    R2 = (SST - SSR) / SST;
    
    std::cout << SSR << std::endl; // We want to minimize SSR!
    std::cout << "R^2 = " << R2 << std::endl;
    
    return 0;
}