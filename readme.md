## raytracing to render engine
---
i am trying to built my own render engine just to sharpen my skills  and learn more about computer graphics and how render machines actually work behind the scenes
### introduction
for learner who want to follow my steps of working please check from commits to commits cause each commit will be tackling a  specific challenge
- i like to use  ` #pragma once ` instead of  ` #ifndef  HEADER_H `

### initializing your own vector
---
first and foremost before diving in deep you should  have your own vector representation in my code it will be found in *header/vector.h* or click here [vector header](https://github.com/yorkings/refactored_graphical_ray_tracing/headers/vec3.h).

we have to notice we use vector because our data reuires x,y,z or red,blue,green  and the most efficient way to represent data is in vector.remember a vector falls in 2D. 
vector usecase ,will be  used for storing spatial coordinates, storing colors, and performing math operations.
 - **inline**: i use inline keyword  for easy function overidding to take place and that compiler not to be confused

 - Theres big difference between  `inline float get_b() const { return z; }` and `inline const float get_b()  { return z; }` so the first is correct usage based on my code because  it makes the function unable to  modify the object's members. It can be called on both const and non-const objects.


###  first chapter understanding a ray
ray  is found by this approach    **$ \text{ray} = \text{origin} + t \times \text{direction} $ **, where **t** is the steps at each point