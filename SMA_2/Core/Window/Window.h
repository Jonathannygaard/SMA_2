//
// Created by jonat on 08/04/2024.
//

#ifndef WINDOW_H
#define WINDOW_H

struct GLFWwindow;


class Window {
public:
    static float Width;
    static float Height;

    static GLFWwindow* init();
};



#endif //WINDOW_H
