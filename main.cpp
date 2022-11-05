#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <deps/linmath.h>
#include <stdlib.h>
#include <stdio.h>
#define M_PI   3.14159265358979323846264338327950288
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
static float fov = 60.f;
float min_deg = 10.f;
float max_deg = 120.f;

struct Camera {
    double Yaw;
    double Pitch;
    vec3 XDir;
    vec3 YDir;
    vec3 ZDir;
    vec3 Pos;
    double ZStep;
    double XStep;
} cam = {
    0,
    0,
    {1,0,0},
    {0,1,0},
    {0,0,1},
    {0,0,2},
    0,
    0
};

static const struct
{
    float x, y, z;
    float r, g, b;
} vertices[36] =
{
    { 0.2f, 0.0f, 0.2f, 1.0f, 1.0f, 0.0f},     //przednia œciana I pol
    { -0.2f, 0.0f, 0.2f, 1.0f, 1.0f, 0.0f},
    { -0.2f, 1.0f, 0.2f, 1.0f, 1.0f, 0.0f},
    //{ 0.2f, 1.0f, 0.2f, 1.0f, 1.0f, 0.0f},

    { -0.2f, 1.0f, 0.2f, 1.0f, 1.0f, 0.0f},  //przednia œciana II pol
    { 0.2f, 1.0f, 0.2f, 1.0f, 1.0f, 0.0f},
    { 0.2f, 0.0f, 0.2f, 1.0f, 1.0f, 0.0f},

    { -0.2f, 0.0f, 0.2f, 1.0f, 0.0f, 1.0f},     //lewa œciana I pol
    { -0.2f, 0.0f, -0.2f, 1.0f, 0.0f, 1.0f},
    { -0.2f, 1.0f, -0.2f, 1.0f, 0.0f, 1.0f},
    //{ -0.2f, 1.0f, 0.2f, 1.0f, 0.0f, 1.0f},

    { -0.2f, 1.0f, -0.2f, 1.0f, 0.0f, 1.0f}, //lewa œciana II pol
    { -0.2f, 1.0f, 0.2f, 1.0f, 0.0f, 1.0f},
    { -0.2f, 0.0f, 0.2f, 1.0f, 0.0f, 1.0f},

    { 0.2f, 0.0f, 0.2f, 1.0f, 0.5f, 1.0f},     //prawa œciana I pol
    { 0.2f, 0.0f, -0.2f, 1.0f, 0.5f, 1.0f},
    { 0.2f, 1.0f, -0.2f, 1.0f, 0.5f, 1.0f},
    //{ 0.2f, 1.0f, 0.2f, 1.0f, 0.5f, 1.0f},

    { 0.2f, 1.0f, -0.2f, 1.0f, 0.5f, 1.0f},  //prawa œciana II pol
    { 0.2f, 1.0f, 0.2f, 1.0f, 0.5f, 1.0f},
     { 0.2f, 0.0f, 0.2f, 1.0f, 0.5f, 1.0f},

    { 0.2f, 0.0f, -0.2f, 1.0f, 1.0f, 0.5f},     //tylnia œciana I pol
    { -0.2f, 0.0f, -0.2f, 1.0f, 1.0f, 0.5f},
    { -0.2f, 1.0f, -0.2f, 1.0f, 1.0f, 0.5f},
    //{ 0.2f, 1.0f, -0.2f, 1.0f, 1.0f, 0.5f},

    { -0.2f, 1.0f, -0.2f, 1.0f, 1.0f, 0.5f}, //tylnia œciana II pol
    { 0.2f, 1.0f, -0.2f, 1.0f, 1.0f, 0.5f},
    { 0.2f, 0.0f, -0.2f, 1.0f, 1.0f, 0.5f},

    { 0.2f, 1.0f, 0.2f, 0.5f, 1.0f, 0.0f},     //górna œciana I pol
    { -0.2f, 1.0f, 0.2f, 0.5f, 1.0f, 0.0f},
    { -0.2f, 1.0f, -0.2f, 0.5f, 1.0f, 0.0f},
    //{ 0.2f, 1.0f, -0.2f, 0.5f, 1.0f, 0.0f},

     { -0.2f, 1.0f, -0.2f, 0.5f, 1.0f, 0.0f}, //górna œciana II pol
    { 0.2f, 1.0f, -0.2f, 0.5f, 1.0f, 0.0f},
    { 0.2f, 1.0f, 0.2f, 0.5f, 1.0f, 0.0f},

    { 0.2f, 0.0f, 0.2f, 1.0f, 0.0f, 0.0f},     //dolna œciana I pol
    { -0.2f, 0.0f, 0.2f, 1.0f, 0.0f, 0.0f},
    { -0.2f, 0.0f, -0.2f, 1.0f, 0.0f, 0.0f},
    //{ 0.2f, 0.0f, -0.2f, 1.0f, 0.0f, 0.0f},

    { -0.2f, 0.0f, -0.2f, 1.0f, 0.0f, 0.0f}, //dolna œciana II pol
    { 0.2f, 0.0f, -0.2f, 1.0f, 0.0f, 0.0f},
    { 0.2f, 0.0f, 0.2f, 1.0f, 0.0f, 0.0f},
};

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec3 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
static void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    static double xpos_prev = 0;
    static double ypos_prev = 0;
    double xdelta = xpos - xpos_prev;
    double ydelta = ypos - ypos_prev;
    xpos_prev = xpos;
    ypos_prev = ypos;

    const float sensitivity = 0.001f;

    if (fabs(xdelta) > 0)
        cam.Yaw = cam.Yaw - xdelta * sensitivity;

    if (fabs(ydelta) > 0)
        cam.Pitch = cam.Pitch - ydelta * sensitivity;
};
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        cam.ZStep = -0.01f;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        cam.ZStep = 0.01f;
    if ((key == GLFW_KEY_S || key == GLFW_KEY_W) && action == GLFW_RELEASE)
        cam.ZStep = 0;

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        cam.XStep = -0.01f;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        cam.XStep = 0.01f;
    if ((key == GLFW_KEY_D || key == GLFW_KEY_A) && action == GLFW_RELEASE)
        cam.XStep = 0;

    float scroll_value = 10.f;
    if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS && fov > min_deg) {
        fov -= scroll_value;
    }
    if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS && fov < max_deg) {
        fov += scroll_value;
    }

}
void set_view_martix(mat4x4 V, struct Camera* cam) {
    mat4x4 Wc;
    mat4x4_identity(Wc);

    mat4x4_rotate_Y(Wc, Wc, cam->Yaw);
    mat4x4_rotate_X(Wc, Wc, cam->Pitch);

    mat4x4 T;
    float x = cam->Pos[0];
    float y = cam->Pos[1];
    float z = cam->Pos[2];
    mat4x4_translate(T, x, y, z);
    mat4x4_mul(Wc, T, Wc);
    mat4x4_invert(V, Wc);

    cam->XDir[0] = Wc[0][0];
    cam->XDir[1] = Wc[0][1];
    cam->XDir[2] = Wc[0][2];

    cam->YDir[0] = Wc[1][0];
    cam->YDir[1] = Wc[1][1];
    cam->YDir[2] = Wc[1][2];

    cam->ZDir[0] = Wc[2][0];
    cam->ZDir[1] = Wc[2][1];
    cam->ZDir[2] = Wc[2][2];
}
void scroll_action(int scroll) {
    if (scroll < 0 && fov < max_deg)
        fov -= scroll;

    if (scroll > 0 && fov > min_deg)
        fov -= scroll;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

    float scroll = (float)yoffset * 10.f;

    scroll_action(scroll);

}
void multiple_objects(mat4x4 m,mat4x4 mvp, GLint mvp_location)
{
    int number_in_line = 10;
    for (int i = 0; i < number_in_line; i++)
    {
        for (int j = 0; j < number_in_line; j++)
        {
            mat4x4_translate(m, 1.0, 0.0, 0.0);
            mat4x4_mul(mvp, mvp, m);
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        mat4x4_translate(m, -number_in_line, 0.0, 1.0);
        mat4x4_mul(mvp, mvp, m);
    }       
}
int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetCursorPos(window, 0, 0);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(vertices[0]), (void*)0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(vertices[0]), (void*)(sizeof(float) * 3));
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float ratio;
        int width, height;
        mat4x4 m, v, p, mvp;
        mat4x4 m1;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4x4_identity(m);
        /*float time = (float)glfwGetTime();
        mat4x4_translate(m, 0.0f, time*(- 0.02f), 0.0);
        mat4x4_rotate(m, m, 1, 1, 1, time);
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);*/
        mat4x4_perspective(p, degToRad(fov), ratio, 0.1f, 100.0f);

        vec3 d;
        vec3_scale(d, cam.XDir, cam.XStep);
        vec3_add(cam.Pos, cam.Pos, d);
        vec3_scale(d, cam.ZDir, cam.ZStep);
        vec3_add(cam.Pos, cam.Pos, d);

        set_view_martix(v, &cam);
        mat4x4_mul(mvp, p, v);
        //mat4x4_mul(mvp, mvp, m);

        glUseProgram(program);
        /*glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
        glDrawArrays(GL_TRIANGLES, 0, 36);*/
        multiple_objects(m,mvp,mvp_location);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
