#include "core/Application.h"
#include <iostream>
#include <vector>



using namespace std;

struct Particule
{
    vec4 position;
    vec4 velocity;
    Particule(vec3 pos, vec3 vel)
    {
        position = vec4(pos[0], pos[1], pos[2], 1.0f);
        velocity = vec4(vel[0], vel[1], vel[2], 1.0f);
    }
};

class Simulator: public Application
{
private:
    Program* program = NULL;
    Program* compute = NULL;
    Renderer* renderer = NULL;
    Computer* computer = NULL;
    Buffer* particules = NULL;
    Buffer* velocities = NULL;
    
public:
    Simulator(int argc, char* argv[]);
    void update(int time);
    void render();
    void setup();
    void teardown();
};

Simulator::Simulator(int argc, char* argv[]) : Application(argc, argv) {}

void Simulator::update(int elapsedTime)
{
    /*argu1: Nombre_particules/4*/
    computer->compute(50, 1, 1);
}

void Simulator::render()
{
     renderer->render(PRIMITIVE_POINTS, 200);
}

void Simulator::setup()
{   
    setClearColor(0.95f, 0.95f, 0.95f, 1.0f); 

    int i = 0;
    vector<Particule> ppv;

    for(i;i<200;i++){
        //-10+i*0.1,10,1
        ppv.push_back( Particule(vec3(-10*i*0.1,10,1), vec3(0,0,0)));
    }

    /*
    Particule pp[] = {
        Particule(vec3(1, 10, 1), vec3(0)),
        Particule(vec3(-1, 10, 1), vec3(0)),
        Particule(vec3(1, 10, -1), vec3(0)),
        Particule(vec3(-1, 10, -1), vec3(0)),

        Particule(vec3(1, 10, 1), vec3(0)),
        Particule(vec3(5, 10, 5), vec3(0)),
        Particule(vec3(1, 10, -1), vec3(0)),
        Particule(vec3(5, 10, -5), vec3(0))
    };
    */



    /*vec3 parti[] = {
        vec3(1, 10, 1),
        vec3(-1, 10, 1),
        vec3(1, 10, -1),
        vec3(-1, 10, -1)
    };*/

    /*size-of: Taille en byte d'une particule
    * => Multiple par nombre de particules pour avoir ce qu'il faut
    */
    particules = new Buffer(ppv.data(), 200*sizeof(Particule));
    //line(-10,5,10,5);
    float line_vertex[]=
    {
        -10,5, 10,5
    };
    glVertexPointer(2, GL_FLOAT, 0, line_vertex);
    glDrawArrays(GL_LINES, 0, 2);

    glBegin(GL_LINES);
        glVertex3f(-10,5,0);
        glVertex3f(10,5,0);
    glEnd();
    glDrawArrays(GL_LINES, 0, 2);
    

    /*vec3 veloc[] = {
        vec3(0),
        vec3(0),
        vec3(0),
        vec3(0)
    };

    velocities = new Buffer(4, 3, FLOAT, veloc);*/

    program = new Program();
    program->addShader(Shader::fromFile("shaders/perspective.vert"));
    program->addShader(Shader::fromFile("shaders/black.frag"));
    program->link();

    renderer = program->createRenderer();

    renderer->setVertexData("vertex", particules, TYPE_FLOAT, 0, 3, sizeof(Particule));

    mat4 projection = perspective(90.0f, 640.0/480.0, 0.1, 20);
    /*PARAMS
    *1: CAM position
    *2: Where the CAM look at
    *3: Reference Axis*/
    mat4 view = lookat(vec3(5, 5, -10), vec3(0, 5, 0), vec3(0, 1, 0));

    renderer->setMatrix("projectionMatrix", projection);
    renderer->setMatrix("modelViewMatrix", view);

    compute = new Program();
    compute->addShader(Shader::fromFile("shaders/gravity.comp"));
    compute->link();

    computer = compute->createComputer();
    computer->setData(1, particules);
    //computer->setData(2, velocities);
}

void Simulator::teardown()
{
   
}

int main(int argc, char** argv)
{
    try 
    {
        Simulator app = Simulator(argc, argv); 
	    return app.run();
    }
	catch(Exception e)
    {
        cout << e.getMessage() << endl;
        return 1;
    }
}