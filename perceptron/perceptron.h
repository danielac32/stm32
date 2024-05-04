#ifndef PERCEPTRON_H
#define PERCEPTRON_H

float* inputs;
float* weights;
int n;
float alpha;
void Perceptron(int ninputs);
void randomize();
void train(int T, float predicted);
int feed_forward();
int activate(float sum);

#endif