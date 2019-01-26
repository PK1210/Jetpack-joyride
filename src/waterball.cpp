#include "waterball.h"

#include "main.h"

Waterball::Waterball(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->speed_y = 0.15f;
    this->speed_x = 0.1f;

    // Our vertices. Three consecutive floats give a vertex; Three consecutive vertices give a triangle.
    // A rectangle has 2 triangles
    static const GLfloat vertex_buffer_data[] = {
        width/2.0f, height/2.0f, 0.0f, // triangle 1 : begin
       -width/2.0f, height/2.0f, 0.0f,
       -width/2.0f,-height/2.0f, 0.0f, // triangle 1 : end
       -width/2.0f, height/2.0f, 0.0f, // triangle 2 : begin
       -width/2.0f,-height/2.0f, 0.0f,
        width/2.0f,-height/2.0f, 0.0f, // triangle 2 : end
   };

    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, COLOR_BLUE, GL_FILL);
}

void Waterball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of  around which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -width, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Waterball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Waterball::tick() {
    // Y axis
    if(this->position.y == GRAVE)
        return;
    this->speed_y -= GRAVITY;
    if(this->position.y >= CEILING - height/2.0f)
        this->die();
    else if(this->position.y <= FLOOR + height/2.0f)
        this->die();
    else
        this->position.y += this->speed_y;

    // X Axis
    this->position.x += this->speed_x;
}

 void Waterball::die(){
     this->position.y = GRAVE;
}

 bounding_box_t Waterball::box() {
    bounding_box_t box = {this->position.x,this->position.y,width,height};
    return box;
}
