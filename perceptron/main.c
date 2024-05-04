//#include "perceptron.h"
#include <stdlib.h>
#include <stdio.h>
#include<time.h>



float *inputs;
float *weights;
int n;
float alpha=0.001;



void Perceptron(int ninputs);
void randomize();
void train(int T, float predicted);
int feed_forward();
int activate(float sum);

void Perceptron(int ninputs){
    // Set the number of inputs
    n = ninputs + 1;
    // Asking for memory
    inputs = (float*)malloc(n);
    weights = (float*)malloc(n);
    // Bias
    inputs[n - 1] = 1;

    // Start with random weights
    for (int i = 0; i < n; i++)
    {
      weights[i] = 0.00;
    }
    printf("%d\n",n );
    for (int i = 0; i < n; i++)
    {
       //printf("weights : %f inputs : %f\n",weights[i],inputs[i] );
    }
}

void train(int T, float predicted){
    float error = T - predicted;

    // Update weights
    for (int i = 0; i < n; i++)
    {
      weights[i] += alpha*error*inputs[i];
      //printf("%d %d %d %d \n", weights[i],alpha,error,inputs[i]);
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



void train_()
{
  int distance_step = 1;
  int count =0;
  FILE *fp;

  fp = fopen ("train.txt", "w");
  // Train for 160 epochs
  for (int epoch = 0; epoch < 340; epoch++)
  {
    for (int d = 0; d < 340; d += distance_step)
    {
      // Set the input in the model
      inputs[0] = d;

      // Make a prediction
      int predicted = feed_forward();
      printf("(%d) predicted %d input %f weight %f\n",count++,predicted ,inputs[0],weights[0]);
      fprintf(fp,"(%d) predicted %d input %f weight %f\n",count++,predicted ,inputs[0],weights[0]);
      if ((d <= 150 && predicted == -1) || (d > 150 && predicted == 1))
      {
        train(-predicted, predicted);
      }
    }
  }
  printf("listo\n");
  fclose(fp);
}



int main(int argc, char const *argv[])
{
	Perceptron(1);


    //int predicted = feed_forward();
    //printf("(%d) predicted %d input %f weight %f\n",count++,predicted ,inputs[0],weights[0]);




    train_();
	while(1){
		 
		scanf("%f",&inputs[0]);
        printf("weights : %f inputs : %f\n",weights[0],inputs[0] );
   
        int predicted = feed_forward();
        printf("Predicted: %d\n",predicted);
	}
 
    
       
	return 0;
}