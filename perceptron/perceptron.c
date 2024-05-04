#include "perceptron.h"
#include <stdlib.h>
#include <stdio.h>
#include<time.h>



void Perceptron(int ninputs){
    alpha = 0.001;
    // Set the number of inputs
    n = ninputs + 1;
    // Asking for memory
    inputs = (float*)malloc(n);
    weights = (float*)malloc(n);
    // Bias
    inputs[n - 1] = 1;
    // Start with random weights
    //randomize();
    //for (int i = 0; i < n; i++)
    //{
      // printf("weights : %d inputs : %d\n",weights[i],inputs[i] );
    //}
}
/*void randomize(){
    for (int i = 0; i < n; i++)
    {
      weights[i] = ((float)rand()) / 1000;
    }
}*/

void train(int T, float predicted){
    float error = T - predicted;

    // Update weights
    for (int i = 0; i < n; i++)
    {
      weights[i] += alpha*error*inputs[i];
    }
}
int feed_forward(){
float sum = 0.0;

    for (int i = 0; i < n; i++)
    {
      sum += inputs[i]*weights[i];
    }

    return activate(sum);
}

int activate(float sum){
return (sum >= 0) ? 1:-1;
}




