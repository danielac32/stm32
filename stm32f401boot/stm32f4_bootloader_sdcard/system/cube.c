#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <memory.h>
#include <kernel.h>
#include <delay.h>
#include <gpio.h>
#include <uart.h>
#include <math.h>
#include <usb.h>
#include "usb_cdc.h"
#include "usb-msc.h"
#include "sd-spi.h"
#include "disk.h"
#include "fat_filelib.h"
#include "st7735.h"
#include "gfx.h"
#include <buttons.h>
#include <cube.h>


/*************************************************************************/
#define SHAPE_SIZE 1000
#define ROTATION_SPEED 25 // Delay in ms between cube redraws

//float d = 3.0f;
float px[] = { -3,  3,  3, -3, -3,  3,  3, -3 };
float py[] = { -3, -3,  3,  3, -3, -3,  3,  3 };
float pz[] = { -3, -3, -3, -3,  3,  3,  3,  3 };
float p2x[] = {0,0,0,0,0,0,0,0};
float p2y[] = {0,0,0,0,0,0,0,0};

float r[3] = {0,0,0};
 

void drawCube() {
 
  r[0] = r[0] + FLOAT_PI / 180.0f; // Add a degree
  r[1] = r[1] + FLOAT_PI / 180.0f; // Add a degree
  r[2] = r[2] + FLOAT_PI / 180.0f; // Add a degree
  if (r[0] >= 360.0f * FLOAT_PI / 180.0f) r[0] = 0;
  if (r[1] >= 360.0f * FLOAT_PI / 180.0f) r[1] = 0;
  if (r[2] >= 360.0f * FLOAT_PI / 180.0f) r[2] = 0;

  for (int i = 0; i < 8; i++) {
    float px2 = px[i];
    float py2 = cos(r[0]) * py[i] - sin(r[0]) * pz[i];
    float pz2 = sin(r[0]) * py[i] + cos(r[0]) * pz[i];

    float px3 = cos(r[1]) * px2 + sin(r[1]) * pz2;
    float py3 = py2;
    float pz3 = -sin(r[1]) * px2 + cos(r[1]) * pz2;

    float ax = cos(r[2]) * px3 - sin(r[2]) * py3;
    float ay = sin(r[2]) * px3 + cos(r[2]) * py3;
    float az = pz3 - 150;

    p2x[i] = GFX_getWidth() / 2 + ax * SHAPE_SIZE / az;
    p2y[i] = GFX_getHeight() / 2 + ay * SHAPE_SIZE / az;
  }

  for (int i = 0; i < 3; i++) {
    GFX_drawLine(p2x[i],   p2y[i],   p2x[i+1], p2y[i+1], GFX_RGB565(255,0,0));
    GFX_drawLine(p2x[i+4], p2y[i+4], p2x[i+5], p2y[i+5], GFX_RGB565(255,0,0));
    GFX_drawLine(p2x[i],   p2y[i],   p2x[i+4], p2y[i+4], GFX_RGB565(255,0,0));
  }
  
  GFX_drawLine(p2x[3], p2y[3], p2x[0], p2y[0], GFX_RGB565(0,255,0));
  GFX_drawLine(p2x[7], p2y[7], p2x[4], p2y[4], GFX_RGB565(0,255,0));
  GFX_drawLine(p2x[3], p2y[3], p2x[7], p2y[7], GFX_RGB565(0,255,0));

  GFX_flush();
  GFX_clearScreen();
}
/************************************************************************************/

