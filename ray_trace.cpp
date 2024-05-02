//---------------------------------------
// Program: ray_trace.cpp
// Purpose: Demonstrate ray tracing.
// Author:  John Gauch
// Date:    Spring 2019
//---------------------------------------
#include <cmath>
#include <cstdio>
#include <cstdlib>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

// Include ray tracing and phong shading classes
#include "ray_classes.h"

// Global variables
#define XDIM 600
#define YDIM 600
#define ZDIM 600
unsigned char image[YDIM][XDIM][3];
float position = -5;
string mode = "phong";
float Bounce = -1;
const float RADIUS = 2.0;
const int SPHERES = 2;
Sphere3D sphere[SPHERES];
ColorRGB color[SPHERES];
int displayMode = 0;
float angle = 0.0f;
float radius;

//---------------------------------------
// Calculate random value between [min..max]
//---------------------------------------
float myrand(float min, float max)
{
   return rand() * (max - min) / RAND_MAX + min;
}

//---------------------------------------
// Check to see if point is in shadow
//---------------------------------------
bool in_shadow(Point3D pt, Vector3D dir, int current, Sphere3D sphere[], int count)
{
   // Define ray to light source
   Ray3D shadow_ray;
   shadow_ray.set(pt, dir);

   // Check to see ray intersects any sphere
   Point3D point;
   Vector3D normal;
   for (int index=0; index<count; index++)
      if ((index != current) && 
         (sphere[index].get_intersection(shadow_ray, point, normal)))
         return true;
   return false;
}

//---------------------------------------
// Perform ray tracing of scene
//---------------------------------------
void ray_trace()
{
   // Define camera point
   Point3D camera;
   camera.set(0,0,position);

   // Define light source
   ColorRGB lightColor;
   lightColor.set(250,250,250);
   Vector3D lightDir;
   lightDir.set(-1,-1,-1);
   lightDir.normalize();

   ColorRGB lightColor2;
   lightColor2.set(255,140,0);
   Vector3D lightDir2;
   lightDir2.set(1,1,1);
   lightDir2.normalize();

   ColorRGB lightColor3;
   lightColor3.set(0,191,255);
   Vector3D lightDir3;
   lightDir3.set(1,-1,-1);
   lightDir3.normalize();

   ColorRGB lightColor4;
   lightColor4.set(255,255,224);
   Vector3D lightDir4;
   lightDir4.set(-1,1,1);
   lightDir4.normalize();

   ColorRGB lightColor5;
   lightColor5.set(128,0,128);
   Vector3D lightDir5;
   lightDir5.set(1,1,-1);
   lightDir5.normalize();


   // Define shader
   Phong shader;
   shader.SetCamera(camera);
   shader.SetLight(lightColor, lightDir);

   Phong shader2;
   shader2.SetCamera(camera);
   shader2.SetLight(lightColor2, lightDir2);

   Phong shader3;
   shader3.SetCamera(camera);
   shader3.SetLight(lightColor3, lightDir3);

   Phong shader4;
   shader4.SetCamera(camera);
   shader4.SetLight(lightColor4, lightDir4);

   Phong shader5;
   shader5.SetCamera(camera);
   shader5.SetLight(lightColor5, lightDir5);

   // Perform ray tracing
   for (int y = 0; y < YDIM; y++)
   for (int x = 0; x < XDIM; x++)
   {
      if(displayMode == 0) {
         // Clear image
         image[y][x][0] = 0;
         image[y][x][1] = 0;
         image[y][x][2] = 0;

         // Define sample point on image plane
         float xpos = (x - XDIM/2) * 2.0 / XDIM;
         float ypos = (y - YDIM/2) * 2.0 / YDIM;
         Point3D point;
         point.set(xpos, ypos, 0);
      
         // Define ray from camera through image
         Ray3D ray;
         ray.set(camera, point);

         // Perform sphere intersection
         int closest = -1;
         Point3D p, closest_p;
         Vector3D n, closest_n;
         closest_p.set(0,0,ZDIM);
         for (int s=0; s<SPHERES; s++)
         {
            if ((sphere[s].get_intersection(ray, p, n)) && (p.pz < closest_p.pz))
            {
               closest = s;
               closest_p = p;
               closest_n = n;
            }
         }
         // Calculate pixel color
         if (closest >= 0)
         {
            // Display surface normal
            if (mode == "normal")
            {
               image[y][x][0] = 127 + closest_n.vx * 127;
               image[y][x][1] = 127 + closest_n.vy * 127;
               image[y][x][2] = 127 + closest_n.vz * 127;
            }

            // Calculate Phong shade
            if (mode == "phong")
            {

               // Check to see if in shadow 
               if (in_shadow(closest_p, lightDir, closest, sphere, SPHERES))
                  shader.SetObject(color[closest], 0.4, 0.0, 0.0, 1);
               else
                  shader.SetObject(color[closest], 0.4, 0.4, 0.4, 10);

         
               // Calculate pixel color
               ColorRGB pixel;
               shader.GetShade(closest_p, closest_n, pixel);
               image[y][x][0] = pixel.R;
               image[y][x][1] = pixel.G;
               image[y][x][2] = pixel.B;
            }
         }
      } else if(displayMode == 1) {
                  // Clear image
         image[y][x][0] = 0;
         image[y][x][1] = 0;
         image[y][x][2] = 0;

         // Define sample point on image plane
         float xpos = (x - XDIM/2) * 2.0 / XDIM;
         float ypos = (y - YDIM/2) * 2.0 / YDIM;
         Point3D point;
         point.set(xpos, ypos, 0);
      
         // Define ray from camera through image
         Ray3D ray;
         ray.set(camera, point);

         // Perform sphere intersection
         int closest = -1;
         Point3D p, closest_p;
         Vector3D n, closest_n;
         closest_p.set(0,0,ZDIM);
         for (int s=0; s<SPHERES; s++)
         {
            if ((sphere[s].get_intersection(ray, p, n)) && (p.pz < closest_p.pz))
            {
               closest = s;
               closest_p = p;
               closest_n = n;
            }
         }
         // Calculate pixel color
         if (closest >= 0)
         {
            // Display surface normal
            if (mode == "normal")
            {
               image[y][x][0] = 127 + closest_n.vx * 127;
               image[y][x][1] = 127 + closest_n.vy * 127;
               image[y][x][2] = 127 + closest_n.vz * 127;
            }

            // Calculate Phong shade
            if (mode == "phong")
            {
               // Check to see if in shadow 
               if (in_shadow(closest_p, lightDir, closest, sphere, SPHERES) || 
                   in_shadow(closest_p, lightDir2, closest, sphere, SPHERES) ||
                   in_shadow(closest_p, lightDir3, closest, sphere, SPHERES) ||
                   in_shadow(closest_p, lightDir4, closest, sphere, SPHERES) ||
                   in_shadow(closest_p, lightDir5, closest, sphere, SPHERES)) {
                  shader.SetObject(color[closest], 0.4, 0.0, 0.0, 1);
                  shader2.SetObject(color[closest], 0.4, 0.0, 0.0, 1);
                  shader3.SetObject(color[closest], 0.4, 0.0, 0.0, 1);
                  shader4.SetObject(color[closest], 0.4, 0.0, 0.0, 1);
                  shader5.SetObject(color[closest], 0.4, 0.0, 0.0, 1);
               } else {
                  shader.SetObject(color[closest], 0.4, 0.4, 0.4, 10);
                  shader2.SetObject(color[closest], 0.4, 0.4, 0.4, 10);
                  shader3.SetObject(color[closest], 0.4, 0.4, 0.4, 10);
                  shader4.SetObject(color[closest], 0.4, 0.4, 0.4, 10);
                  shader5.SetObject(color[closest], 0.4, 0.4, 0.4, 10);
               }

               // Calculate pixel color
               ColorRGB pixel;
               shader.GetShade(closest_p, closest_n, pixel);
               ColorRGB pixel2;
               shader2.GetShade(closest_p, closest_n, pixel2);
               ColorRGB pixel3;
               shader3.GetShade(closest_p, closest_n, pixel3);
               ColorRGB pixel4;
               shader4.GetShade(closest_p, closest_n, pixel4);
               ColorRGB pixel5;
               shader5.GetShade(closest_p, closest_n, pixel5);

            }
         }
      }

   }
}
 
//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);

   // Print command menu
   cout << "Program commands:\n"
        << "   'm' - toggle display mode\n"
        << "   '+' - increase camera distance\n"
        << "   '-' - decrease camera distance\n"
        << "   'p' - show Phong shading\n"
        << "   'n' - show surface normals\n"
        << "   'q' - quit program\n";

   //Define center sphere
   float cz1 = myrand(0, RADIUS/2);
   Point3D center1;
   center1.set(0,0,cz1);
   Vector3D motion1;
   motion1.set(0,0,0);
   float radius1 = myrand(RADIUS/20, RADIUS/10);
   sphere[0].set(center1, motion1, radius1);
   int R1 = rand() % 255;
   int G1 = rand() % 255;
   int B1 = rand() % 255;
   color[0].set(R1,G1,B1);

   //Define orbiting sphere
   float cx = myrand(-RADIUS/2, RADIUS/2);
   float cy = myrand(-RADIUS/2, RADIUS/2);
   float cz = myrand(0, RADIUS/2);
   Point3D center;
   center.set(cx,cy,cz);
   float mx = myrand(-RADIUS/100, RADIUS/200);
   float my = myrand(-RADIUS/100, RADIUS/200);
   float mz = myrand(-RADIUS/100, RADIUS/200);
   Vector3D motion;
   motion.set(mx,my,mz);
   float radius = myrand(RADIUS/20, RADIUS/10);
   sphere[1].set(center, motion, radius);
   int R = rand() % 255;
   int G = rand() % 255;
   int B = rand() % 255;
   color[1].set(R,G,B);

   // Perform ray tracing
   cout << "camera: 0,0," << position << endl;
   ray_trace();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Display image
   glClear(GL_COLOR_BUFFER_BIT);
   glDrawPixels(XDIM, YDIM, GL_RGB, GL_UNSIGNED_BYTE, image);
   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // End program
   if (key == 'q')
      exit(0);
   if (key == 'm')
      if(displayMode == 0) {
         cout << "Displaying Multiple Light Sources" << endl;
         displayMode = 1;
      } else {
         cout << "Displaying Single Light Source" << endl;
         displayMode = 0;
      }

   // Move camera position
   else if (key == '+' && position > -10)
   {
      position = position - 0.5;
      cout << "camera: 0,0," << position << endl;
   }
   else if (key == '-' && position < -5)
   {
      position = position + 0.5;
      cout << "camera: 0,0," << position << endl;
   }

   // Change display mode
   else if (key == 'n')
      mode = "normal";
   else if (key == 'p')
      mode = "phong";

   // Perform ray tracing
   ray_trace();
   glutPostRedisplay();
}

//---------------------------------------
// Timer callback for OpenGL
//---------------------------------------
void timer(int value)
{
   // Increment angle
   angle += 0.01f;

   // Calculate new position
   sphere[1].center.px = sphere[0].center.px + radius * cos(angle);
   sphere[1].center.py = sphere[0].center.py + radius * sin(angle);

   // Calculate and display image
   ray_trace();
   glutPostRedisplay();
   glutTimerFunc(10, timer, 0);
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   // Create OpenGL window
   glutInit(&argc, argv);
   glutInitWindowSize(XDIM, YDIM);
   glutInitWindowPosition(0, 0);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Ray Trace");
   init();

   // Calculate radius after initializing sphere[0] and sphere[1]
   radius = sqrt(pow(sphere[1].center.px - sphere[0].center.px, 2) + 
                 pow(sphere[1].center.py - sphere[0].center.py, 2) + 
                 pow(sphere[1].center.pz - sphere[0].center.pz, 2));

   // Specify callback function
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutTimerFunc(10, timer, 0);
   glutMainLoop();
   return 0;
}