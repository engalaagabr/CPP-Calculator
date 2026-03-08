#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

char display[128] = "0";
double value = 0.0;
char op = 0;
bool newNumber = true;

double Calculate(double a,double b,char oper)
{
    switch(oper)
    {
        case '+': return a+b;
        case '-': return a-b;
        case '*': return a*b;
        case '/': return (b!=0)?a/b:0;
    }
    return b;
}

void PressNumber(int num)
{
    if(newNumber)
    {
        snprintf(display,sizeof(display),"%d",num);
        newNumber=false;
    }
    else
    {
        if(strcmp(display,"0")==0)
            snprintf(display,sizeof(display),"%d",num);
        else
        {
            char temp[16];
            snprintf(temp,sizeof(temp),"%d",num);
            strcat(display,temp);
        }
    }
}

void PressOperator(char newOp)
{
    double current = atof(display);

    if(op!=0)
        value = Calculate(value,current,op);
    else
        value = current;

    op = newOp;

    snprintf(display,sizeof(display),"%.2f",value);

    newNumber=true;
}

void PressEqual()
{
    double current = atof(display);

    value = Calculate(value,current,op);

    snprintf(display,sizeof(display),"%.2f",value);

    op=0;
    newNumber=true;
}

void Reset()
{
    strcpy(display,"0");
    value=0;
    op=0;
    newNumber=true;
}

void CalculatorUI()
{
    ImGui::SetNextWindowSize(ImVec2(460,540),ImGuiCond_Always);
    ImGui::Begin("Calculator",NULL,ImGuiWindowFlags_NoResize);

    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##display",display,sizeof(display),
                     ImGuiInputTextFlags_ReadOnly);

    ImGui::Spacing();

    ImVec2 btn(100,80);

    auto Num=[&](const char* label,int v)
    {
        if(ImGui::Button(label,btn))
            PressNumber(v);
    };

    auto Op=[&](const char* label,char oper)
    {
        ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.80f,0.45f,0.10f,1));
        if(ImGui::Button(label,btn))
            PressOperator(oper);
        ImGui::PopStyleColor();
    };

    Num("7",7); ImGui::SameLine();
    Num("8",8); ImGui::SameLine();
    Num("9",9); ImGui::SameLine();
    Op("/",'/');

    Num("4",4); ImGui::SameLine();
    Num("5",5); ImGui::SameLine();
    Num("6",6); ImGui::SameLine();
    Op("*",'*');

    Num("1",1); ImGui::SameLine();
    Num("2",2); ImGui::SameLine();
    Num("3",3); ImGui::SameLine();
    Op("-",'-');

    Num("0",0); ImGui::SameLine();

    if(ImGui::Button("C",btn))
        Reset();

    ImGui::SameLine();

    if(ImGui::Button("=",btn))
        PressEqual();

    ImGui::SameLine();
    Op("+",'+');

    ImGui::End();
}

int main()
{
    if(!glfwInit())
        return 1;

    const char* glsl_version="#version 130";

    GLFWwindow* window=glfwCreateWindow(500,600,"Calculator",NULL,NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding=12;
    style.FrameRounding=10;
    style.FramePadding=ImVec2(10,8);
    style.ItemSpacing=ImVec2(10,10);

    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        CalculatorUI();

        ImGui::Render();

        int display_w,display_h;
        glfwGetFramebufferSize(window,&display_w,&display_h);

        glViewport(0,0,display_w,display_h);
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}